#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main (void)
{
   //prompt user for name
    string n = get_string();

   // check if user typed name without spaces, print 1st initial as uppercase
   if (isalpha (n[0]))
   {
      printf("%c",toupper(n[0]));
   }
   //print uppercase initials if user typed name with spaces via iteration
   for (int i = 0; i < strlen(n); i++)
   {
     if (n[i] == 32 && isalpha(n[i + 1]))
     {
        printf("%c", toupper (n [i + 1]));

     }
   }
     printf("\n");
}