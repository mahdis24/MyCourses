#include <cs50.h>
#include <stdio.h>
#include <string.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    string message = get_string("Message: ");

    for (int i = 0; message[i] != '\0'; i++)
    {
        int decimal = message[i];
        int byte[BITS_IN_BYTE];
        int position = BITS_IN_BYTE - 1;

        while (position > -1)
        {
            byte[position] = decimal % 2;
            position--;
            decimal /= 2;
        }

        for (int j = 0; j < BITS_IN_BYTE; j++)
        {
            print_bulb(byte[j]);
        }

        printf("\n");
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
