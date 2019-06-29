#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

int g_slider_position = 2;
CvCapture* g_capture = NULL;

IplImage* doPyrDown( IplImage* in ) {
    assert( in->width%2 == 0 && in->height%2 ==0 );
    IplImage* out = cvCreateImage(
        cvSize( in->width/2, in->height/2 ),
        in->depth,
        in->nChannels
    );
    cvPyrDown( in, out );
    return out;
}

/*
 * this function outputs edges 
 */
IplImage* doCanny( IplImage* in, double lowThresh, double highThresh, double aperture ) {
    if ( in->nChannels != 1 ) {
        cout << "canny only handle gray scale images\n";
        return(0);  // canny only handles gray scale images
    }
    
    IplImage* out = cvCreateImage(
        cvSize( cvGetSize( in ).width, cvGetSize( in ).height ),
        IPL_DEPTH_8U,
        1
    );

    cvCanny( in, out, lowThresh, highThresh, aperture );
    return out;
}

void example2_4( IplImage* image ) {
    cvNamedWindow( "Example4-in" );
    cvNamedWindow( "Example4-out" );
    cvShowImage( "Example4-in", image );
    
    IplImage* out = cvCreateImage(
        cvGetSize(image),
        IPL_DEPTH_8U,
        3
    );

    cvSmooth( image, out, CV_GAUSSIAN, 7, 7 );
    cvShowImage( "Example4-out", out );
    cvReleaseImage( &out );

    cvWaitKey( 0 );
    cvReleaseImage( &out );
    cvReleaseImage( &image );
    cvDestroyWindow( "Example4-in" );
    cvDestroyWindow( "Example4-out" );
}

int main( int argc, char** argv ) {
    
    CvCapture* capture;

    if ( argc == 1 ) {
        capture = cvCreateCameraCapture(0);
        // cout << "invalid number of arguments\n";
        // return 0;
    }
    else {
        capture = cvCreateFileCapture( argv[1] );
    }
    assert( capture != NULL );

    int i;
    IplImage* in = 0;
    // in = cvQueryFrame( capture );

    // // converting rgb image to gray
    // IplImage* gray = cvCreateImage(
    //     cvSize( cvGetSize( in ).width, cvGetSize( in ).height ),
    //     IPL_DEPTH_8U,
    //     1
    // );
    // cvCvtColor( in, gray, CV_RGB2GRAY );

    // example2_4( cvLoadImage( argv[1] ) );
    // cvNamedWindow( "PyrDown" );
    // cvShowImage( "PyrDown", doPyrDown( in ) );
    // cvSaveImage( "sampleimage.jpg", doPyrDown( in ) );
    // cvWaitKey( 0 );
    // cvDestroyWindow( "PyDown" );

    // cannying
    // cvNamedWindow( "canny window" );
    // for( i = 0; i < 10; i++ ) {
    //     cvShowImage( "canny window", doCanny( gray, 10, 100, 3 ) );
    //     cvWaitKey( 0 );
    // }
    // cvDestroyWindow( "canny window" );

    // combining two pyramid down operator and canny the subroutine
    // cvNamedWindow( "jennie" );
    // IplImage* img1 = doPyrDown( gray );
    // img1 = doPyrDown( img1 );
    // img1 = doCanny( img1, 10, 100, 3 );
    // cvShowImage( "jennie", img1 );
    // cvWaitKey( 0 );
    // cvReleaseImage( &img1 );
    // cvDestroyWindow( "jennie" );

    // camera
    cvNamedWindow( "camera" );
    cvNamedWindow( "camera1" );
    cvCreateTrackbar(
            "trackbar slider",
            "camera",
            &g_slider_position,
            8,
            NULL
        );
    IplImage* downpyr;
    while ( (in = cvQueryFrame( capture )) != NULL ) {
        cvShowImage( "camera", in );
        downpyr = doPyrDown( in );
        if ( g_slider_position > 2 ) {
            for ( i = 2; i <= g_slider_position; i++) {
                downpyr = doPyrDown( downpyr );
            }
        }
        cvShowImage( "camera1", downpyr );
        char c = cvWaitKey(10);
        if ( c == 27 ) break;
    }
    cvWaitKey( 0 );
    cvDestroyWindow( "camera" );
    cvDestroyWindow( "camera1" );

    // cvReleaseImage( &gray );
    cvReleaseImage( &downpyr );
    cvReleaseImage( &in );
    cvReleaseCapture ( &capture );
    // cvReleaseImage( &in );
    return 0;
}