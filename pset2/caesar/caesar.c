#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, string argv[])
{

    // check for correct # of arguments
    if (argc!= 2)
    {
     printf("Usage: ./caesar k\n");
     return 1;
    }

    //convert string to number
    int conv = atoi(argv[1]);
    int k = conv % 26;

    //change input into numbers
    printf("plaintext: ");
    string p = get_string();

    //shift numbers according to key
    printf("ciphertext: ");
    for (int i = 0; i < strlen(p); i++)
    {   // check if key is alphabetic
        if (isalpha(p[i]))
        {
          //check if key is uppercase
          if (isupper(p[i]))

          {                  // Subtract by 65 ("value of 'A') to form alphabetic index, add key, modulo 26 to remain within index, add 65 to got back to ASCII
            printf("%c", ((((p[i] - 65) + k) % 26) + 65));

          }
                             // Subtract by 97 ("value of 'a"), same as above
          else if (islower(p[i]))
          {
            printf("%c", ((((p[i] - 97) + k) % 26) + 97));
          }
        }
        else
          {
            // print non alpha characters
               printf("%c", p[i]);
          }



    }
    printf("\n");
    return 0;
}


