#include "tinytest.h"
#include "util.h"
#include "data.h"
#include "ordered_list.h"
#include "catalog.h"
#include "dictionary.h"
#include <stdio.h>


// These tests were written as a part of the code's development and now are essentially safeguards against regression.
// They are not meant to be complete unit-tests.

void testLexiographicCompare() {
    ASSERT("\"\" <= \"a\"", lexiographicCompare("", "a"));
    ASSERT("\"a\" <= \"a\"", lexiographicCompare("a", "a"));
    ASSERT("\"a\" <= \"ab\"", lexiographicCompare("a", "ab"));
    ASSERT("\"ab\" <= \"ac\"", lexiographicCompare("ab", "ac"));
    ASSERT("\"ab\" <= \"ba\"", lexiographicCompare("ab", "ba"));
}

void testOrderedList() {
    OrderedList * list = newOrderedList(&lexiographicCompare);
    OLNode * nodes[4];

    ASSERT_EQUALS(olSupremum(list, "hello world"), NULL);

    // Populate list.
    nodes[0] = olAdd(list, "abc", NULL);
    nodes[1] = olAdd(list, "", NULL);
    nodes[2] = olAdd(list, "abba", NULL);
    nodes[3] = olAdd(list, "abb", (void *) 42);

    // Things are linked and in right order.
    ASSERT_EQUALS(olIndex(nodes[1]), 0);
    ASSERT_EQUALS(olIndex(nodes[3]), 1);
    ASSERT_EQUALS(olIndex(nodes[2]), 2);
    ASSERT_EQUALS(olIndex(nodes[0]), 3);
    ASSERT_EQUALS(olNext(nodes[0]), NULL);
    ASSERT_EQUALS(olPrev(nodes[1]), NULL);
    ASSERT_STRING_EQUALS(olKey(olSupremum(list, "")), "");

    // Seeking works.
    ASSERT_EQUALS(olSeekBy(nodes[2], -2), nodes[1]);
    ASSERT_EQUALS(olSeekBy(nodes[1], 2), nodes[2]);
    ASSERT_EQUALS(olSeekBy(nodes[1], 42), NULL);

    // Reindex an element.
    ASSERT_EQUALS(olSupremum(list, "oops"), NULL);
    olReindex(list, nodes[1], "oops");

    // Things remain in the right order and linked correctly.
    ASSERT_EQUALS(olLast(list), nodes[1]);
    ASSERT_EQUALS(olIndex(nodes[3]), 0);
    ASSERT_EQUALS(olIndex(nodes[2]), 1);
    ASSERT_EQUALS(olIndex(nodes[0]), 2);
    ASSERT_EQUALS(olPrev(nodes[1]), nodes[0]);
    ASSERT_EQUALS(olNext(nodes[0]), nodes[1]);
    ASSERT_EQUALS(olIndex(nodes[1]), 3);

    // Finding supremums works as expected.
    ASSERT_STRING_EQUALS(olKey(olSupremum(list, "ab")), "abb");
    ASSERT_STRING_EQUALS(olKey(olSupremum(list, "abb")), "abb");

    // Removing stuff works.
    olRemove(list, nodes[1]);
    ASSERT_EQUALS(olFirst(list), nodes[3]);
    ASSERT_EQUALS(olPrev(nodes[3]), NULL);

    freeOrderedList(list);
}

void testDictionary() {
    Dictionary * dictionary = newDictionary();
    DictEntry * entries[3];
    entries[0] = dictAdd(dictionary, "hello", "world");
    entries[1] = dictAdd(dictionary, "goodbye", "world");
    entries[2] = dictAdd(dictionary, "", "oops");

    ASSERT_STRING_EQUALS(dictValue(entries[0]), "world");
    ASSERT_STRING_EQUALS(dictKey(entries[0]), "hello");

    ASSERT_EQUALS(dictLookup(dictionary, "hello"), entries[0]);
    ASSERT_EQUALS(dictLookup(dictionary, ""), entries[2]);
    ASSERT_EQUALS(dictLookup(dictionary, "oops"), NULL);

    ASSERT_EQUALS(dictAdd(dictionary, "hello", "not world"), NULL);
    dictRemove(dictionary, entries[0]);
    ASSERT_EQUALS(dictLookup(dictionary, "hello"), NULL);

    freeDictionary(dictionary);
}

void testWriteCatalog() {
    Catalog * catalog;
    newCatalogRandom(&catalog, 10, 100);
    ASSERT_EQUALS(catCategoryCount(catalog), 10);

    writeCatalog(catalog, "data/random.txt");
    freeCatalog(catalog);
}

void testLoadingCatalog() {
    Catalog * catalog;
    ASSERT_EQUALS(newCatalogFromFile(&catalog, "data/random.txt"), true);
    freeCatalog(catalog);
}

int main() {
    initRandomSeed();
    RUN(testLexiographicCompare);
    RUN(testOrderedList);
    RUN(testDictionary);
    RUN(testWriteCatalog);
    RUN(testLoadingCatalog);
    TEST_REPORT();
}
