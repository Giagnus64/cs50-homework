#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    string k = argv[1];

    // check for correct # of arguments
    if (argc != 2)
    {
     printf("Usage: ./vigenere k\n");
     return 1;
    }

    //Check for alphabetical characters
    for (int i = 0; i < strlen(k); i++)
    {
        if (isalpha(k[i]) == false)
        {
            printf("Usage: ./vigenere k\n");
            return 1;
        }

    }
    // Get user input and print guidelines
    printf("plaintext: ");
    string p = get_string();
    printf("ciphertext: ");
    //Establish counter for alphabetical characters ONLY
    int v = 0;

    // Identify alphabetical characters, print non alpha chars, convert to alpha index, loop through key word using v % strlen(k)
    // Increase v by 1 to keep cipher within alphabetical characters
    for (int j = 0; j < strlen(p); j++)
    {
       // check if pt (plaintext) is alphabetic
         if (isalpha(p[j]))
         {
           //check if pt is uppercase
           if (isupper(p[j]))

           {                  // Subtract by 65 ("value of 'A') to form alphabetic index, add key, modulo 26 to remain within index, add 65 to got back to ASCII
            printf("%c", (((((p[j] - 65) + toupper(k[v % strlen(k)]) - 65) % 26) + 65)));
           v++;
           }
                             // Subtract by 97 ("value of 'a"), same as above
           if (islower(p[j]))
           {
             printf("%c", (((((p[j] - 97) + toupper(k[v % strlen(k)]) - 65) % 26) + 97)));
           v++;
           }
          }
          else
           {
             // print non alpha characters
                printf("%c", p[j]);
           }

    }
    printf("\n");
    return 0;

}