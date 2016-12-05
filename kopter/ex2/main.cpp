#include <iostream>
#include "opencv/cv.h"
#include "opencv/highgui.h"
using namespace std;

int sliderPosition = 9; // Initial slider position
int divisions = 32;	// Initial number of divisions

int blockXSize;         // Horizontal block size
int blockYSize;         // Vertical block size

int pixelCount;         // The number of pixels in a block (blockXSize multiplied by blockYSize)

int width;              // The width  of the input stream
int height;             // The height of the input stream

// Callback function to adjust number of divisions when we move the slider
void onDivisionSlide(int theSliderValue)
{
    // Put a lower limit of 1 on our slider position
    if (sliderPosition < 0)
    {
        sliderPosition = 1;
    }

    // Set the number of divisions depending on the slider location
    // Factors of both 640 and 480: 1, 2, 4, 5, 8, 10, 16, 20, 32, 40, 160
    switch (theSliderValue)
    {
        case 1:
            divisions = 1;
            break;

        case 2:
            divisions = 2;
            break;

        case 3:
            divisions = 4;
            break;

        case 4:
            divisions = 5;
            break;

        case 5:
            divisions = 8;
            break;

        case 6:
            divisions = 10;
            break;

        case 7:
            divisions = 16;
            break;

        case 8:
            divisions = 20;
            break;

        case 9:
            divisions = 32;
            break;

        case 10:
            divisions = 40;
            break;

        case 11:
            divisions = 160;
            break;

        default:
            break;
    }

    // Recalculate our block sizes and pixelCount for the new number of divisions
    blockXSize = width  / divisions;
    blockYSize = height / divisions;

    pixelCount = blockXSize * blockYSize;
}

int main()
{
    // Create two windows
    cvNamedWindow("WebCam", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Low Rez Stream", CV_WINDOW_AUTOSIZE);

    int maxSliderValue = 11;

    // Create the divisions slider lider
    cvCreateTrackbar("Divisions", "Low Rez Stream", &sliderPosition, maxSliderValue, onDivisionSlide);

    // Start capturing data from the web cam
    CvCapture* pCapture = cvCaptureFromCAM(CV_CAP_V4L2);

    // Get an initial frame so we know the size of things (cvQueryFrame is a combination of cvGrabFrame and cvRetrieveFrame)
    IplImage* pFrame = NULL;
    pFrame = cvQueryFrame(pCapture);

    // Create an image the same size and colour-depth as our input stream
    IplImage* pLowRezFrame = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);

    uchar *ptr; // Pointer to our pixel

    int red, green, blue; // Integers to hold our pixel values

    // Get the width and height of our webcam input stream
    int width  = pFrame->width;
    int height = pFrame->height;

    // Integers to hold our total colour values (used to find the average)
    int redSum     = 0;
    int greenSum   = 0;
    int blueSum    = 0;

    // Loop controling vars
    char keypress;
    bool quit = false;

    while (quit == false)
    {
        // Grab a frame from the webcam
        pFrame = cvQueryFrame(pCapture);

        // Draw the original frame and low resolution version
        cvShowImage("WebCam", pFrame);
        cvShowImage("Low Rez Stream", pLowRezFrame);

        // Calculate our blocksize per frame to cater for slider
        blockXSize = width  / divisions;
        blockYSize = height / divisions;

        pixelCount = blockXSize * blockYSize; // How many pixels we'll read per block - used to find the average colour

        cout << "At " << divisions << " divisions (Block size " << blockXSize << "x" << blockYSize << ", so " << pixelCount << " pixels per block)" << endl;

        // Loop through each block horizontally
        for (int xLoop = 0; xLoop < width; xLoop += blockXSize)
        {

            // Loop through each block vertically
            for (int yLoop = 0; yLoop < height; yLoop += blockYSize)
            {

                // Reset our colour counters for each block
                redSum     = 0;
                greenSum   = 0;
                blueSum    = 0;

                // Read every pixel in the block and calculate the average colour
                for (int pixXLoop = 0; pixXLoop < blockXSize; pixXLoop++)
                {

                    for (int pixYLoop = 0; pixYLoop < blockYSize; pixYLoop++)
                    {

                        // Get the pixel colour from the webcam stream
                        ptr = cvPtr2D(pFrame, yLoop + pixYLoop, xLoop + pixXLoop, NULL);

                        // Add each component to its sum
                        redSum   += ptr[2];
                        greenSum += ptr[1];
                        blueSum  += ptr[0];

                    } // End of inner y pixel counting loop

                } // End of outer x pixel countier loop

                // Calculate the average colour of the block
                red   = redSum   / pixelCount;
                green = greenSum / pixelCount;
                blue  = blueSum  / pixelCount;

                // Draw a rectangle of the average colour
                cvRectangle(
                        pLowRezFrame,
                        cvPoint(xLoop, yLoop),
                        cvPoint(xLoop + blockXSize, yLoop + blockYSize),
                        CV_RGB(red, green, blue),
                        CV_FILLED,
                        8,
                        0
                );

            } // End of inner y loop

        } // End of outer x loop

        // Wait 20 milliseconds
        keypress = cvWaitKey(20);

        // Set the flag to quit if the key pressed was escape
        if (keypress == 27)
        {
            quit = true;
        }

    } // End of while loop

    // Release our stream capture object
    cvReleaseCapture(&pCapture);

    // Release our images & destroy all windows
    cvReleaseImage(&pFrame);
    cvReleaseImage(&pLowRezFrame);
    cvDestroyAllWindows();
}