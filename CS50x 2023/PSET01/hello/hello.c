#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("\n************************ Hello ************************\n\n");

    // Print Hello, World!
    printf("Hello, World!\n\n");

    // Get User's Name
    string name = get_string("What's Your Name? ");

    // Hello To User, By His/Her Name
    printf("\nHello, %s!\n\n", name);
}
