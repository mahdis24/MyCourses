#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("\n************************ Mario More ************************\n\n");

    // Get Height From User, as a Integer, Between 1 and 8
    int height = 0;

    printf("Enter Height (From 1 to 8):\n");
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    printf("\n");

    // Show Mario Blocks
    for (int i = 1; i <= height; i++)
    {
        // Show Empty Blocks
        for (int j = 0; j < height - i; j++)
        {
            printf(" ");
        }

        // Show Hashtag Blocks
        for (int z = 0; z < 2; z++)
        {
            for (int k = 0; k < i; k++)
            {
                printf("#");
            }

            if (z == 0)
            {
                printf("  ");
            }
        }

        // Go To Next Row
        printf("\n");
    }
}