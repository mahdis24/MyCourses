// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 60360;

// Hash table
node *table[N];

int dictionary_size = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_value = hash(word);
    node *n = table[hash_value];

    while (n != NULL)
    {
        if (strcasecmp(word, n->word) == 0)
        {
            return true;
        }
        n = n->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // I implemented this hash function method from the guidance of the course instructor I had in Iran
    long sum = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }

    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dictionary_pointer = fopen(dictionary, "r");

    if (dictionary_pointer == NULL)
    {
        printf("Unable to Open %s\n", dictionary);
        return false;
    }

    char next_word[LENGTH + 1];

    while (fscanf(dictionary_pointer, "%s", next_word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, next_word);

        int hash_value = hash(next_word);

        n->next = table[hash_value];
        table[hash_value] = n;
        dictionary_size++;
    }

    fclose(dictionary_pointer);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dictionary_size;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *n = table[i];

        while (n != NULL)
        {
            node *temp = n;
            n = n->next;
            free(temp);
        }

        if (n == NULL && i == N - 1)
        {
            return true;
        }
    }

    return false;
}
