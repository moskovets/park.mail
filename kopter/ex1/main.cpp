#include <iostream>

#include <GL/glew.h>        // Include OpenGL headers
#include <GL/glfw.h>        // Include OpenGL Framework headers

#include "opencv/cv.h"      // Include OpenCV
#include "opencv/highgui.h" // Include OpenCV highgui

using namespace std;

// Vars for the size of our OpenGL window
float windowWidth  = 640.0f;
float windowHeight = 480.0f;

int sliderPosition = 9;    // Initial slider position
int divisions = 32;        // Initial number of divisions

uint blockXSize;           // Horizontal block size
uint blockYSize;           // Vertical block size

uint pixelCount;           // The number of pixels in a block (blockXSize multiplied by blockYSize)

uint camWidth;             // The width  of the webcam input stream
uint camHeight;            // The height of the webcam input stream

bool quit = false;         // Enough already!

bool useLines     = false;       // If true use lines, otherwise use points
bool smoothPoints = false;       // Whether to smooth the points or not. Unsmoothed points gives squares/rectangles - which looks quite cool!
bool sizeFromBrightness = false; // Whether to use the average brightness of a block to alter the point size

// Template function to convert things to strings
template<class T>
std::string toString(const T& t)
{
    std::ostringstream stream;
    stream << t;
    return stream.str();
}

// Structure to hold a colour as RGB values in the range 0 to 255
struct Colour3ub
{
    uint8_t redComponent;
    uint8_t greenComponent;
    uint8_t blueComponent;
};


// Function to handle screen resizing
void resize(int theWidth, int theHeight)
{
    // Resize the window
    glfwSetWindowSize(theWidth, theHeight);

    // Set our global window size variables to their new value
    windowWidth = theWidth;
    windowHeight = theHeight;

    // Setup our viewport to be the entire size of the window
    glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);

    // Change to the projection matrix and set orthagonal projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, 0, 1); // Paramters: left, right, bottom, top, near, far

    // Make sure we're changing the model view and not the projection then reset the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


// Handle keypresses and respond accordingly
void handleKeypress(int theKey, int theAction)
{
    // If a key was pressed...
    if (theAction == GLFW_PRESS)
    {
        // ...react accordingly!
        switch (theKey)
        {
            // Use L to switch to drawing lines
            case 'L':
                useLines = true;
                break;

                // Use P to switch to drawing points
            case 'P':
                useLines = false;
                break;

                // Use S to switch whether to generate the point size from the overall block brightness or not
            case 'S':
                sizeFromBrightness = !sizeFromBrightness;
                break;

                // Use Space to toggle point smoothing (on gives you circular points, off gives you square points)
            case GLFW_KEY_SPACE:
                if (smoothPoints == false)
                {
                    glEnable(GL_POINT_SMOOTH);
                    smoothPoints = true;
                }
                else
                {
                    glDisable(GL_POINT_SMOOTH);
                    smoothPoints = false;
                }
                break;

                // Use Escape to quit
            case GLFW_KEY_ESC:
                quit = true;
                break;

            default:
                break;

        } // End of switch statement

    } // End of if keyaction was GLFW_PRESS condition

} // End of handleKeypress function


void initGL(int theWidth, int theHeight, string theWindowTitle)
{
    //  ----- GLFW Setup -----

    glfwInit();

    // Frame counter and window settings variables
    int	redBits    = 8, greenBits = 8,   blueBits   = 8;
    int alphaBits  = 8, depthBits = 0,  stencilBits = 0;

    // Create a window or die trying...
    if(!glfwOpenWindow(windowWidth, windowHeight, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, GLFW_WINDOW))
    {
        cout << "Failed to open window!" << endl;
        glfwTerminate();
        exit(-1);
    }

    // Register the callback to change the window size
    glfwSetWindowSizeCallback(&resize);

    // Register the callback to handle keypresses
    glfwSetKeyCallback(handleKeypress);


    //  ----- GLEW Setup -----

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "GLEW initialisation error: " << glewGetErrorString(err) << endl;
        exit(-1);
    }
    cout << "GLEW intialised successfully. Using GLEW version: " << glewGetString(GLEW_VERSION) << endl;

    // ----- Viewport and Projection Setup -----

    // Setup our viewport to be the entire size of the window
    glViewport(0, 0, (GLsizei)theWidth, (GLsizei)theHeight);

    // Change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, 0, 1); // Paramters: left, right, bottom, top, near, far

    // Make sure we're changing the model view and not the projection then reset the matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // ----- OpenGL Setup -----

    glfwSwapInterval(1); 		// Lock to vertical sync of monitor (normally 60Hz, so 60fps)

    glDisable(GL_SMOOTH);		// Disable (gouraud) shading

    glDisable(GL_DEPTH_TEST); 	// Disable depth testing

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // // Set our clear colour to black

    glEnable(GL_LINE_SMOOTH);	// Enable anti-aliasing on lines

}

void calcFPS(uint & frame, double & t0, double & time)
{
    //get the current time
    time = glfwGetTime();

    // Calculate and display the FPS
    if((time-t0) > 1.0 || frame == 0)
    {
        double fps = (double)frame / (time - t0);

        string windowTitle = "Webcam OpenGL Manipulation | FPS: ";

        string tempFPS = toString(fps);

        windowTitle += tempFPS;

        const char* pWindowTitle = windowTitle.c_str();

        glfwSetWindowTitle(pWindowTitle);

        t0 = time;
        frame = 0;
    }

    frame ++; // Increment our frame count

}


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
    blockXSize = camWidth  / divisions;
    blockYSize = camHeight / divisions;

    pixelCount = blockXSize * blockYSize; // Calculate our new pixelCount
}


// Function to draw a point at a specified location with a specified colour
void glDrawPoint (const GLfloat & xLoc, const GLfloat & yLoc, const Colour3ub & theColour)
{
    // Set the colour
    glColor3ub(theColour.redComponent, theColour.greenComponent, theColour.blueComponent);

    // Set the point size based on the brightness of the colour divided by a fudge-factor
    float averageColour = (theColour.redComponent + theColour.greenComponent + theColour.blueComponent) / 3.0f;

    if (sizeFromBrightness == true)
    {
        glPointSize(averageColour / 15.0f); // Divide by lower numbers for wacky 3D-esque effects!
    }
    else
    {
        glPointSize(blockYSize);
    }

    // Draw the point
    glBegin(GL_POINTS);
    glVertex2f(xLoc, yLoc);
    glEnd();

}


// Function to draw a line between two specified points with a specified colour
void glDrawLine (const GLfloat & xLoc, const GLfloat & yLoc, const GLfloat & xLoc2, const GLfloat & yLoc2, const Colour3ub & theColour)
{
    // Set the colour
    glColor3ub(theColour.redComponent, theColour.greenComponent, theColour.blueComponent);

    // Set the line width based on the brightness of the colour divided by a fudge-factor
    float averageColour = (theColour.redComponent + theColour.greenComponent + theColour.blueComponent) / 3.0f;

    glLineWidth(averageColour / 15.0f);

    // Draw a line between the top-left and bottom-right points of the block
    glBegin(GL_LINES);
    glVertex2f(xLoc, yLoc);
    glVertex2f(xLoc2, yLoc2);
    glEnd();
}


// Function to calculate the average colour of a block of pixels
Colour3ub getBlockColour(IplImage * pFrame, const uint & xLoop, const uint & yLoop, const uint & pixelsPerBlock)
{
    // Create and initialise colour component counters
    uint redSum     = 0;
    uint greenSum   = 0;
    uint blueSum    = 0;

    uchar * ptr; // Create our pointer to the pixel beforehand to avoid running the constructor each time through the loops

    // Read every pixel in the block and calculate the average colour
    for (uint pixXLoop = 0; pixXLoop < blockXSize; pixXLoop++)
    {
        for (uint pixYLoop = 0; pixYLoop < blockYSize; pixYLoop++)
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
    Colour3ub colour;
    colour.redComponent   = redSum   / pixelsPerBlock;
    colour.greenComponent = greenSum / pixelsPerBlock;
    colour.blueComponent  = blueSum  / pixelsPerBlock;

    return colour;
}


int main()
{
    // Frame-rate calculator var declaration and initialisation
    double time, t0;
    uint frame = 0;
    t0 = glfwGetTime();

    // Call our initGL function to set up our OpenGL options
    string windowTitle = "Low Rez Stream Circles | r3dux.org\tFPS: --";
    initGL(windowWidth, windowHeight, windowTitle);

    // Create two OpenCV windows
    cvNamedWindow("WebCam", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Low Rez Stream", CV_WINDOW_AUTOSIZE);

    int maxSliderValue = 11;

    // Create the divisions slider lider
    cvCreateTrackbar("Division Setting", "Low Rez Stream", &sliderPosition, maxSliderValue, onDivisionSlide);

    // Start capturing data from the web cam
    CvCapture* pCapture = cvCaptureFromCAM(CV_CAP_V4L2);

    // Get an initial frame so we know the size of things (cvQueryFrame is a combination of cvGrabFrame and cvRetrieveFrame)
    IplImage* pFrame = NULL;
    pFrame = cvQueryFrame(pCapture);

    // Create an image the same size and colour-depth as our input stream
    IplImage* pLowRezFrame = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U, 3);

    // Get the width and height of our webcam input stream
    camWidth  = pFrame->width;
    camHeight = pFrame->height;

    // Keep track of how many pixels in a block (so we can divide by it)
    uint pixelsPerBlock = 0;

    // Loop controling vars
    char keypress;

    while (quit == false)
    {
        // Clear the screen and reset the modelview matrix
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Grab a frame from the webcam
        pFrame = cvQueryFrame(pCapture);

        // Draw the original frame and low resolution version
        cvShowImage("WebCam", pFrame);
        cvShowImage("Low Rez Stream", pLowRezFrame);

        // Calculate our blocksize per frame to cater for slider
        blockXSize = camWidth  / divisions;
        blockYSize = camHeight / divisions;

        // Translate across and down a little to put our drawn geometry in the centre of the blocks
        glTranslatef( (float)(blockXSize / 2), (float)(blockYSize / 2), 0.0f);

        pixelsPerBlock = blockXSize * blockYSize; // How many pixels we'll read per block - used to find the average colour

        // Uncomment to get your block-stat fix...
        //cout << "At " << divisions << " divisions (Block size " << blockXSize << "x" << blockYSize << ", so " << pixelCount << " pixels per block)" << endl;

        // Loop through each block horizontally
        for (uint xLoop = 0; xLoop < camWidth; xLoop += blockXSize)
        {

            // Loop through each block vertically
            for (uint yLoop = 0; yLoop < camHeight; yLoop += blockYSize)
            {
                // Get the colour of the block
                Colour3ub colour = getBlockColour(pFrame, xLoop, yLoop, pixelsPerBlock);

                // Draw a rectangle of the average colour
                cvRectangle(
                        pLowRezFrame,
                        cvPoint(xLoop, yLoop),
                        cvPoint(xLoop + blockXSize, yLoop + blockYSize),
                        CV_RGB(colour.redComponent, colour.greenComponent, colour.blueComponent),
                        CV_FILLED,
                        8,
                        0
                );

                if (useLines == true)
                {
                    glDrawLine(xLoop, yLoop, xLoop + blockXSize, yLoop + blockYSize, colour);
                }
                else
                {
                    glDrawPoint(xLoop, yLoop, colour);
                }

            } // End of inner y loop

        } // End of outer x loop

        // Swap the buffers
        glfwSwapBuffers();

        // Calculate the FPS
        calcFPS(frame, t0, time);

        // Wait 2 milliseconds - if we don't wait at least 2ms then we're polling faster than the webcam and although
        // the stream will still be captured and shown in the OpenGL window, it will not be displayed in any OpenCV windows.
        keypress = cvWaitKey(2);

    } // End of while loop

    // Release our stream capture object
    cvReleaseCapture(&pCapture);

    // Release our images and destroy all windows
    cvReleaseImage(&pFrame);
    cvReleaseImage(&pLowRezFrame);
    cvDestroyAllWindows();
}
/*#include <iostream>
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
}*/