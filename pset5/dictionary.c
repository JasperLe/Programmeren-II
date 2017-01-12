/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define BUCKETS 3200000


// create the hashtable
typedef struct node
{
    char word[LENGTH+1];
    struct node* next;
}   node;
    
struct node *hashtable[BUCKETS] = {NULL};

/**
 * Hashfunction based on first 3 chars and string length
 * returns index
 */
int hash_function(const char* word)
{
    unsigned int index = tolower(word[0]) - 97;
    for (int i = 3; i--; )
    {
        if (isalpha(word[i]) && strlen(word) > 1)
        {
            index += abs(tolower(word[i]) - 97);
            index *= strlen(word);
        }
    }
    return index;
}

/**
 * Returns true if word is in dictionary else false.
 * Copies word_in to temp and compares temp to compare -> word
 */
bool check(const char* word_in)
{
    unsigned int n = hash_function(word_in);
    char temp[LENGTH+1];
    for (int i = 0; i < strlen(word_in); i++)
    {
        temp[i] = tolower(word_in[i]);
    }
    temp[strlen(word_in)] = '\0';
    
    node* compare = hashtable[n];
    while (compare != NULL)
    {
        if (strcmp(temp, compare -> word) == 0)
        {
            return true;
        }
        compare = compare -> next;
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 * 
 * Made it work thanks to http://stackoverflow.com/questions/31930046/what-is-a-hash-table-and-how-do-you-make-it-in-c
 * Update**
 * new words are inserted at the head
 */
bool load(const char* dictionary)
{
    // open specified dictionary
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // create new nodes for each word
    char input[LENGTH+1];
    // creating and appending nodes
    while (fscanf(dict, "%s", input) != EOF)
    {
        unsigned int n = hash_function(input);

        // make new node and insert at head of list
        node* temp = malloc(sizeof(node));
        // CHECK FOR OUT OF MEMORY VERGETEN!!!!!!!!!!!!<<<<<<<<<<<<<<<<<<<<<<<<<<
        
        strcpy(temp -> word, input);
        temp -> next = hashtable[n];
        hashtable[n] = temp;
        
        //call size to keep track of size
        size();
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // starts at -1 to compensate for the call in main (not pretty)
    static int number = -1;
    number++;
    return number;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int n = 0; n < BUCKETS; n++)
    {
        node* cursor = hashtable[n];
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    return true;
}
