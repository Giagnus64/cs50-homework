#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int h;
    do
    {
      printf("Height: ");
      h = get_int();
    }
    while (h < 0 || h >= 24);

    for(int j = 0; j < h; j++)
    {
        for(int s = 1; s < h - j; s++)
        {
          printf(" ");
        }

        for(int p = 0; p < 1 + j; p++)
        {
            printf("#");
        }
        printf("  ");
        for(int r = 0; r < 1 + j; r++)
        {
            printf("#");

        }
        printf("\n");
    }
}

