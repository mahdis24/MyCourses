#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int calculate_letters(string text, int len);
int calculate_words(string text, int len);
int calculate_sentences(string text, int len);

int main(void)
{
    string text = get_string("Text: ");

    int str_len = strlen(text);

    float l = calculate_letters(text, str_len);
    // printf("\n%i Letter(s)\n", (int)l);

    float w = calculate_words(text, str_len);
    // printf("\n%i Word(s)\n", (int)w);

    float s = calculate_sentences(text, str_len);
    // printf("\n%i Sentence(s)\n", (int)s);

    float L = 100 * (l / w);
    float S = 100 * (s / w);

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int calculate_letters(string text, int len)
{
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (isalpha(text[i]) != 0)
        {
            count++;
        }
    }

    return count;
}

int calculate_words(string text, int len)
{
    int count = 1;

    for (int i = 0; i < len; i++)
    {
        if (text[i] == ' ')
        {
            count++;
        }
    }

    return count;
}

int calculate_sentences(string text, int len)
{
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }

    return count;
}
