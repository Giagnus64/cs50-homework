#include <cs50.h>
#include <stdio.h>
#include <math.h>



int main(void)
{
  float c;
  do
 {
  printf("O hai! How much change is owed?\n");
  c = get_float();
 }
  while (c < 0);

//multiply by 100 and round

int s = round (c * 100);

int quarters = s / 25 ;
int dimes = (s % 25) / 10 ;
int nickels = ((s % 25) % 10) / 5 ;
int pennies = (((s % 25) % 10) % 5);

int coins = quarters + dimes + nickels + pennies;
 printf ("%i\n", coins);
}