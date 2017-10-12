/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy sizefactor infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    int factor = atoi(argv[1]);

    // check integer
    if (factor > 100 || factor <= 0)
    {
       fprintf(stderr, "Factor must be between 1 and 100\n");
       return 5;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    //determine padding of inputfile
    int paddingIn = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //determine changes in fileheader and/or infoheader to match new scaled bmp
     LONG newbiW = (bi.biWidth * factor);
     LONG newbiH = (bi.biHeight * factor);
     int paddingOut = (4 - (newbiW * sizeof(RGBTRIPLE)) % 4) % 4;
     DWORD newbiSi = ((sizeof(RGBTRIPLE) * newbiW) + paddingOut) * abs(newbiH);
     DWORD newbfSz = newbiSi + (sizeof(BITMAPFILEHEADER)) + (sizeof(BITMAPINFOHEADER));

    // make copies of new fileheader and infoheader
    BITMAPFILEHEADER newbf = bf;
    BITMAPINFOHEADER newbi = bi;

    // overwrite new Fileheader && Infoheader
    newbi.biWidth = newbiW;
    newbi.biHeight = newbiH;
    newbi.biSizeImage = newbiSi;
    newbf.bfSize = newbfSz;

    // write outfile's BITMAPFILEHEADER
    fwrite(&newbf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&newbi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
       //recopy method to establish width scaling
       for (int y = 0; y < (factor - 1); y++)
       {

         //HEIGHT SCALE iterate over pixels in scanline
         for (int j = 0; j < bi.biWidth; j++)
         {
             // temporary storage
             RGBTRIPLE triple;

             // read RGB triple from infile
             fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

             //Scale horizontally by multiplying pixels by factor
             for (int k = 0; k < factor; k++)
             {

               // write RGB triple to outfile
               fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

             }
         }
            // Add New Padding
          for (int p = 0; p < paddingOut; p++)
          {
                fputc(0x00, outptr);
          }

        // offset line to work with recopy method
        long offset = (bi.biWidth * sizeof(RGBTRIPLE));
         fseek(inptr, -offset, SEEK_CUR);

       }
         //WIDTH SCALE iterate over pixels in scanline
        for (int c = 0; c < bi.biWidth; c++)
        {
            //temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

          //write RBG triple n times
          for (int z = 0; z < factor; z++)
            {
                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

        }
            // Add New Padding
            for (int d = 0; d < paddingOut; d++)
            {
                fputc(0x00, outptr);
            }

      // skip over INPUT padding, if any
      fseek(inptr, paddingIn, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
