/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"


//define the structure of the node to form a trie
typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;
//establish the pointer to the root node of trie
node *root;
//integer to count words while forming dictionary for size function later on
unsigned int wordcount = 0;

void freedom(node* a);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{

  //establish a traversal pointer
  node *ctrav = root;
  //establish a pointer to the next node
  node *newnode;
    //iterate through characters in given strings
    for(int b = 0; b < strlen(word); b++)
    {
         //make all characters lowercase as in dictionary
      char c = tolower(word[b]);
         //establish cases is character is apostrophe
        if (c == '\'')
        {
           /*//move 2nd traversal pointer to new node
           newnode = ctrav->children[26];
           //if traversal pointer points to nothing, word is misspelled
           if(newnode == NULL)
           {
            return false;
           }
           //if traversal pointer points to a new node, move the original pointer to the newnode
           else
           {
               ctrav = ctrav->children[26];
           }*/ c = 123;
        }
           //move 2nd traversal to second node

            newnode = ctrav->children[c - 'a'];
            // if 2nd node doesn't exist, word is misspelled
            if (newnode == NULL)
            {
              return false;
            }
            //if 2nd node does exist, move the 1st traversal pointer to the new node
            else
            {
              ctrav = newnode;
            }


    }
  //if the traversal pointer is stopped, and the bool is true, word is spelled correctly
  if (ctrav->is_word == true)
  {
      return true;
  }
  return false;

}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
   root = calloc(1, sizeof(node));
   //establish traversal pointer
   node *trav = root;
   node *newnode;
   //call for the first node to be formed


  // open dictionary file and establish error is unable to
   FILE *dp = fopen(dictionary, "r");
    if (dp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }

   // iterate through characters in dictionary
   for (char i = fgetc(dp); i != EOF; i = fgetc(dp))
   {
      //if there is a new line, mark true at the traversal pointer (which is at the previous node)
      if (i == '\n')
      {
          trav -> is_word = true;
          trav = root; // move traversal back to start
          wordcount++; // add to wordcount for size function later
      }
      //establish case for apostrophe
     else if (i == '\'')
      {

          if (trav->children[26] == NULL)
          {
              newnode = calloc(1, sizeof(node));
              trav ->children[26] = newnode;
              trav = newnode;
          }
          //if node already exists, move traversal pointer there
          else
          {
              trav = trav->children[26];
          }
      }

      else
      {
          //if no node at letter space, calloc one and move traversal pointer there
         if (trav->children[i - 'a'] == NULL)
         {
          newnode = calloc(1, sizeof(node));
          trav->children[i - 'a'] = newnode;
          trav = newnode;
         }
         // if node exists, move traversal pointer there
         else
         {
           trav = trav->children[i - 'a'];
         }
      }

   }
    fclose(dp); // close file
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{

return wordcount;// return counted words from when dictionary was formed

}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
freedom(root);// recursive function with root pointer passed in
return true;
}


void freedom(node* a)
{
    for (int f = 0; f < 27; f++)// iterate through possible node spaces
    {
        if(a ->children[f] != NULL)// if node space holds a pointer
        {

          freedom(a->children[f]); //run through function until getting to a pointer with no nodes

        }

    }
    free(a);
}
