/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */


bool search(int value, int values[], int n)
{
   //establish integers defining the bounds of binary search
   int start = 0;
   int end = n - 1;

   // returns false immediately if n is negative
    if (n < 0)
    {
       return false;
    }

    // returns true if midpoint holds value via binary search
    while (start <= end)
    {
        //establish definition of midpoint
        int mp = (start + end) / 2;
        //value will eventually end up in search
        if (values[mp] == value)
        {
            return true;
        }
        // get rid of half that is lower than origianl midpoint
        else if (values[mp] < value)
        {
             start = mp + 1;
        }
        //get rid of half that is higher than original midpoint
        else if (values[mp] > value)
        {
            end = mp - 1;
        }
        //if number is not found return false
        else
        {
            return false;
        }
    // if number is not found return false
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // initialize counting array and stray variable
    int base[65536] = { 0 };
    int y = 0;
    // assign values to counting array according to given random array
    for (int i = 0; i < n ; i++)
    {
        base[values[i]] = base[values[i]] + 1;
    }
    // assign values into original array in order
    for (int m = 0; m < 65536; m++)
    {
        if (base[m] != 0)
        {
            values[y] = m;
            y++;
            base[m] = base[m] - 1;
            m--;
        }
    }
     //print array for testing purposes
    for (int p = 0; p < n; p++)
    {
        printf("%i\n", values[p]);
    }

}
