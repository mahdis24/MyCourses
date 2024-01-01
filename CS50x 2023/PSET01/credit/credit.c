#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("\n************************ Credit ************************\n\n");

    // Get Number Card From User
    long number = get_long("Number: ");

    // Create This Temp Variable for Do Something on Card Number
    long temp_number = number;

    // Calculate Number Length (Digits)
    int number_length = 0;
    while (temp_number > 0)
    {
        temp_number /= 10;
        number_length++;
    }

    // Check Length Conditions
    if (number_length != 13 && number_length != 15 && number_length != 16)
    {
        printf("\nINVALID\n");
        return 0;
    }

    // Luhn’s Algorithm
    int sum = 0;
    temp_number = number;
    for (int i = 0; i < number_length; i++)
    {
        int digit = temp_number % 10;

        if (i % 2 != 0)
        {
            digit *= 2;

            if (digit > 9)
            {
                digit = (digit % 10) + (digit / 10);
            }
        }

        sum += digit;

        temp_number /= 10;
    }

    // Check Result of Luhn’s Algorithm
    if (sum % 10 == 0)
    {
        // Calculate First Two Digits of Number
        long first_two_digit = number;
        while (first_two_digit > 100)
        {
            first_two_digit /= 10;
        }

        // Detect Card Type
        if (number_length == 13 && (first_two_digit / 10) == 4)
        {
            printf("\nVISA\n");
        }
        else if (number_length == 15 && (first_two_digit == 34 || first_two_digit == 37))
        {
            printf("\nAMEX\n");
        }
        else if (number_length == 16)
        {
            if ((first_two_digit / 10) == 4)
            {
                printf("\nVISA\n");
            }
            else if (first_two_digit == 51 || first_two_digit == 52 || first_two_digit == 53 || first_two_digit == 54 ||
                     first_two_digit == 55)
            {
                printf("\nMASTERCARD\n");
            }
            else
            {
                printf("\nINVALID\n");
                return 0;
            }
        }
        else
        {
            printf("\nINVALID\n");
            return 0;
        }
    }
    else
    {
        printf("\nINVALID\n");
        return 0;
    }
}