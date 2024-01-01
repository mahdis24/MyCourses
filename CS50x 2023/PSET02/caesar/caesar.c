#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int argv_len = strlen(argv[1]);
    for (int i = 0; i < argv_len; i++)
    {
        if (isalpha(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }

    int key = atoi(argv[1]);

    string plain = get_string("plaintext:  ");

    char cipher[strlen(plain) - 2];

    for (int i = 0; plain[i] != '\0'; i++)
    {
        char c = plain[i];

        if (isalpha(c))
        {
            c += key % 26;

            if (!(islower(c) || isupper(c)))
            {
                c -= 26;
            }
        }

        cipher[i] = c;
    }

    printf("ciphertext: %s\n", cipher);
    return 0;
}
