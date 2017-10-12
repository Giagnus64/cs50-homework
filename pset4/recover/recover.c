#include <cs50.h>
#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main (int argc, char *argv[])
{
    //check for user input error
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover filename\n");
        return 1;
    }
    //rename file entered for organizational purposes
    char *recofile = argv[1];

    //check for file open error && create file POINTER //open card file
    FILE *recoptr = fopen(recofile, "r");
    if (recoptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", recofile);
        return 2;
    }
    // temporary storage
    BYTE *block = malloc(512 * sizeof(BYTE));
    int picnum = 0;
    FILE* picptr = NULL;
    char *filename = malloc(8 * sizeof(char));
    filename[7] = '\0';




    // repeat until end of card //read 512 bytes into a buffer
    while (fread(block, 512, 1, recoptr) == 1)
    {
      // start of a new JPEG?
      if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)

      {
          //if a jpg has already been found
          if (picnum > 0)
          {

            fclose(picptr);
            //start writing a new jpg
            sprintf(filename, "%03i.jpg", picnum);
            picptr = fopen(filename, "w");
            picnum++;
            fwrite(block, 512, 1, picptr);
          }

          // if this is the first jpg found, create first jpg
          else if (picnum == 0)
          {
          sprintf(filename, "%03i.jpg", picnum);
          picptr = fopen(filename, "w");
          picnum++;
          fwrite(block, 512, 1, picptr);
          }
      }
      else // if not the start of a jpeg, continue writing to the file
      {
          if (picnum > 0)
          {
              fwrite(block, 512, 1, picptr);
          }
      }
    }
    fclose(picptr);
    fclose(recoptr);
    free(filename);
    free(block);
    return 0;

}