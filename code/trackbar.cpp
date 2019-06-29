/*
 * header
 */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

/*
 * global variables
 */
int g_slider_position = 0;
int g_switch_value = 0;
bool gray_mode = false;
CvCapture* g_capture = NULL;

/*
 * slider callback function
 */
void onTrackbarSlide( int pos ) {
    cvSetCaptureProperty(
        g_capture,
        CV_CAP_PROP_POS_FRAMES,
        pos
    );
}

/*
 * switch callback function
 */
void switch_off_func() {
    gray_mode = false;
}
void switch_on_func() {
    gray_mode = true;
}
void switch_callback( int pos ) {
    if ( pos == 0 ) {
        switch_off_func();
    }
    else {
        switch_on_func();
    }
}

/*
 * main fucntion
 */
int main( int argc, char** argv ) {
    if ( argc != 2 ) {
        cout << "invalid number of arguments\n";
        return 0;
    }
    cvNamedWindow( "trackbar slider", CV_WINDOW_AUTOSIZE );
    g_capture = cvCreateFileCapture( argv[1] );
    int frames = (int) cvGetCaptureProperty(
        g_capture,
        CV_CAP_PROP_FRAME_COUNT
    );
    if ( frames != 0 ) {
        cvCreateTrackbar(
            "Position",
            "trackbar slider",
            &g_slider_position,
            frames,
            onTrackbarSlide
        );

        cvCreateTrackbar(
            "Switch",
            "trackbar slider",
            &g_switch_value,
            1,
            switch_callback
        );
    }
    IplImage* frame;
    while( 1 ) {
        if ( !cvGetTrackbarPos(
                "Switch",
                "trackbar slider"
              ) ) {
                  frame = cvQueryFrame( g_capture );
        }
        if ( !frame ) break;
        IplImage *temp = cvCreateImage(
            cvSize( frame->width, frame->height ),
            IPL_DEPTH_8U,
            1
        );
        if ( gray_mode ) cvCvtColor( frame, temp, CV_RGB2GRAY );
        if ( gray_mode ) cvShowImage( "trackbar slider", temp );
        else cvShowImage( "trackbar slider", frame );
        char c = cvWaitKey(33);
        if ( c == 27 ) break;
        cvReleaseImage( &temp );
        cvSetTrackbarPos(
            "Position",
            "trackbar slider",
            cvGetCaptureProperty(
                g_capture,
                CV_CAP_PROP_POS_FRAMES
            )
        );
    }
    cvReleaseCapture( &g_capture );
    cvDestroyWindow( "trackbar slider" );
    return 0;
}