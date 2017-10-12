#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("Minutes: ");
    int t = get_int();
    printf("Bottles: %i\n", t * 12);
}
