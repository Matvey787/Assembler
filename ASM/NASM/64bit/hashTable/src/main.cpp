#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hash_utils.h"
#include "handText.h"

int main()
{
    //cleanText("book.txt", "cleanedBook.txt");
    HashTable* table = createHT();
    parseText("cleanedBook.txt", table);
    // insertHT(table, "hello");
    // insertHT(table, "world");
    // insertHT(table, "abc");
    // printf("%d\n", searchHT(table, "hello"));
    // deleteHT(table, "hello");
    visualizeHT(table, "table.svg");
    freeHT(table);
}
