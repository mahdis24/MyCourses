#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            int gray_value = round((image[row][column].rgbtRed + image[row][column].rgbtGreen + image[row][column].rgbtBlue) / 3.0);

            image[row][column].rgbtRed = gray_value;
            image[row][column].rgbtGreen = gray_value;
            image[row][column].rgbtBlue = gray_value;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int originalRed, originalGreen, originalBlue, sepiaRed, sepiaGreen, sepiaBlue;

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            originalRed = image[row][column].rgbtRed;
            originalGreen = image[row][column].rgbtGreen;
            originalBlue = image[row][column].rgbtBlue;

            sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[row][column].rgbtRed = sepiaRed;
            image[row][column].rgbtGreen = sepiaGreen;
            image[row][column].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE row_copy[width];

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            row_copy[column] = image[row][column];
        }

        for (int column = 0; column < width; column++)
        {
            image[row][column].rgbtRed = row_copy[width - column - 1].rgbtRed;
            image[row][column].rgbtGreen = row_copy[width - column - 1].rgbtGreen;
            image[row][column].rgbtBlue = row_copy[width - column - 1].rgbtBlue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            copy[row][column] = image[row][column];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            int counter = 0;
            float sum_red = 0, sum_green = 0, sum_blue = 0;

            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (!(row + i < 0 || row + i >= height || column + j < 0 || column + j >= width))
                    {
                        sum_red += copy[row + i][column + j].rgbtRed;
                        sum_green += copy[row + i][column + j].rgbtGreen;
                        sum_blue += copy[row + i][column + j].rgbtBlue;

                        counter++;
                    }
                    else
                    {
                        continue;
                    }
                }
            }

            image[row][column].rgbtRed = round(sum_red / counter);
            image[row][column].rgbtGreen = round(sum_green / counter);
            image[row][column].rgbtBlue = round(sum_blue / counter);
        }
    }

    return;
}
