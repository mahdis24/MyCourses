#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("\n************************ Population ************************\n\n");

    // Get Start Size From User, as a Integer
    int start_size = 0;

    do
    {
        start_size = get_int("Enter Start Size: ");
    }
    while (start_size < 9);

    printf("\n");

    // Get End Size From User, as a Integer
    int end_size = 0;

    do
    {
        end_size = get_int("Enter End Size: ");
    }
    while (end_size < start_size);

    printf("\n");

    // Calculate Number of Years
    int years = 0;

    while (start_size < end_size)
    {
        int birth_size = start_size / 3;
        int death_size = start_size / 4;
        start_size += birth_size - death_size;
        years++;
    }

    // Show Years To User

    printf("Years: %i\n\n", years);
}
