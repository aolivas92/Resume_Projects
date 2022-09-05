#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through every pixel in the image
    for (int i  = 0; i < height; i++) // iterate through every row
    {
        for (int j  = 0; j < width; j++) // iterate through every column
        {
            // get the avrage value of the red, green and blue for every pixel
            double average = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;

            // set the rgb to the average to apply the grayscale
            image[i][j].rgbtBlue = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtRed = round(average);
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through every row in the image
    for (int i  = 0; i < height; i++)
    {
        RGBTRIPLE temp_row[width];

        // copy every pixel in this row to the temp
        for (int j = 0; j < width; j++)
        {
            temp_row[j] = image[i][j];
        }

        // copy every pixel in temp back to the image but flipped
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp_row[width - 1 - j];
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];

    // iterate through every pixel in the image
    for (int i  = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double average_Blue = 0;
            double average_Green = 0;
            double average_Red = 0;
            int pixels_added = 0;

            // add up every pixel above, below, and on the same row as our current pixel
            for (int k = -1; k < 2; k++)
            {
                int cur_height = i + k; // the current row that we are going to add

                if ((cur_height < height) && (cur_height >= 0)) // verify we are not above or below the image
                {
                    // add up every pixel at each column of our current row
                    for (int l = -1; l < 2; l++)
                    {
                        int cur_width =  j + l;// the current column that we are going to add

                        if ((cur_width < width) && (cur_width >= 0)) // verify we are not too far right or left
                        {
                            average_Blue += image[cur_height][cur_width].rgbtBlue;
                            average_Green += image[cur_height][cur_width].rgbtGreen;
                            average_Red += image[cur_height][cur_width].rgbtRed;
                            pixels_added++;
                        }
                    }

                }
            }

            // get the average of all the pixels added
            int final_Blue = round(average_Blue / pixels_added);
            int final_Green = round(average_Green / pixels_added);
            int final_Red = round(average_Red / pixels_added);

            // set our current pixel to the average of each color
            temp_image[i][j].rgbtBlue = final_Blue;
            temp_image[i][j].rgbtGreen = final_Green;
            temp_image[i][j].rgbtRed = final_Red;

        }
    }

    // iterate through the image and add the new values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp_image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp_image[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp_image[i][j].rgbtRed;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];

    double Gx[][3] = {{-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    double Gy[][3] = {{1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
    }; // flip the top and bottom of Gy so that the correct values get multiplyed by the correct variable

    // iterate through every pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double Blue_Gx = 0;
            double Green_Gx = 0;
            double Red_Gx = 0;

            double Blue_Gy = 0;
            double Green_Gy = 0;
            double Red_Gy = 0;

            // go to every row above, below, and the same row as our current pixel
            for (int k = -1; k < 2; k++)
            {
                int cur_height = i + k;

                if ((cur_height < height) && (cur_height >= 0)) // verify the current height is in range
                {
                    // go to every column on the left, right, and the same as our current pixel
                    for (int l = -1; l < 2; l++)
                    {
                        int cur_width = j + l;

                        if ((cur_width < width) && (cur_width >= 0))
                        {
                            Blue_Gx += image[cur_height][cur_width].rgbtBlue * Gx[k + 1][l + 1];
                            Green_Gx += image[cur_height][cur_width].rgbtGreen * Gx[k + 1][l + 1];
                            Red_Gx += image[cur_height][cur_width].rgbtRed * Gx[k + 1][l + 1];

                            Blue_Gy += image[cur_height][cur_width].rgbtBlue * Gy[k + 1][l + 1];
                            Green_Gy += image[cur_height][cur_width].rgbtGreen * Gy[k + 1][l + 1];
                            Red_Gy += image[cur_height][cur_width].rgbtRed * Gy[k + 1][l + 1];
                        }

                    }
                }

            }

            // square the average of the total Gx/Gy then get the squrt
            double finalBlue_Double = sqrt(pow(Blue_Gx, 2) + pow(Blue_Gy, 2));
            double finalGreen_Double = sqrt(pow(Green_Gx, 2) + pow(Green_Gy, 2));
            double finalRed_Double = sqrt(pow(Red_Gx, 2) + pow(Red_Gy, 2));

            // add up Gx and Gy
            int final_Blue = round(finalBlue_Double);
            int final_Green = round(finalGreen_Double);
            int final_Red = round(finalRed_Double);

            // change the values of each pixel
            if (final_Blue > 255)
            {
                temp_image[i][j].rgbtBlue = 255;
            }
            else
            {
                temp_image[i][j].rgbtBlue = final_Blue;
            }

            if (final_Green > 255)
            {
                temp_image[i][j].rgbtGreen = 255;
            }
            else
            {
                temp_image[i][j].rgbtGreen = final_Green;
            }

            if (final_Red > 255)
            {
                temp_image[i][j].rgbtRed = 255;
            }
            else
            {
                temp_image[i][j].rgbtRed = final_Red;
            }
        }
    }

    // iterate through the image and add the new values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp_image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp_image[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp_image[i][j].rgbtRed;
        }
    }
}
