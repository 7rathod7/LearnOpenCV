/*
 * header
 */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

/*
 * globals
 */
IplImage *img2;
char *window_name;

/*
 * define our callback which we will install for mouse events
 */
void my_mouse_callback( int event, int x, int y, int flags, void* param ) {
    IplImage *image = (IplImage*) param;
    CvFont a;
    uchar *pinput;
    int r, g, b;
    char co_ord_x[15], co_ord_y[5];
    switch ( event ) {
        case CV_EVENT_MOUSEMOVE:
            break;
        
        case CV_EVENT_LBUTTONDOWN:
            pinput = (uchar*)image->imageData;
            b = pinput[ x * image->widthStep + y * image->nChannels + 0];
            g = pinput[ x * image->widthStep + y * image->nChannels + 1];
            r = pinput[ x * image->widthStep + y * image->nChannels + 2];
            cvCopy( image, img2 );
            sprintf( co_ord_x, "%d", r );
            sprintf( co_ord_y, "%d", g );
            strcat( co_ord_x, "," );
            strcat( co_ord_x, co_ord_y );
            sprintf( co_ord_y, "%d", b );
            strcat( co_ord_x, "," );
            strcat( co_ord_x, co_ord_y );
            // char te[10];
            // sprintf( te, "%d", h[0] );
            cvInitFont( &a, CV_FONT_HERSHEY_SIMPLEX, 0.6, 0.6);
            cvPutText( img2, co_ord_x, cvPoint( x, y ), &a , cvScalar( 255 ) );
            cvShowImage( window_name, img2 );
            // co_ord_x = x + '0';     // not working
            // cout << co_ord_x << "\n";
            break;
        
        case CV_EVENT_LBUTTONUP:
            break;

        default:
            break;
    }
}

/*
 * main function
 */
int main( int argc, char** argv ) {

    window_name = argv[1];
    

    IplImage* img = cvLoadImage( argv[1], CV_LOAD_IMAGE_ANYCOLOR );
    img2 = cvCreateImage(
        cvGetSize( img ),
        img->depth,
        img->nChannels
    );
    cvCopy( img, img2 );
    
    // passing 0 in second argument, will make the window resize
    cvNamedWindow( argv[1], CV_WINDOW_AUTOSIZE );
    cvSetMouseCallback(
        argv[1],
        my_mouse_callback,
        ( void * ) img
    );
    // for ( int i = 0; i < 10; i++) {
    //     // cvResizeWindow(
    //     //     argv[1],
    //     //     cvGetWindowProperty(
    //     //         argv[1],
    //     //         CV_CAP_PROP_FRAME_WIDTH
    //     //     )+i,
    //     //     cvGetWindowProperty(
    //     //         argv[1],
    //     //         CV_CAP_PROP_FRAME_HEIGHT
    //     //     )+i
    //     // );
    //     cvMoveWindow( argv[1], 10*i, 0 );
    // }

    // show text on image
    // CvFont a;
    // cvInitFont( &a, CV_FONT_HERSHEY_SIMPLEX, 1, 1);
    // cvPutText( img2, "jennifer", cvPoint( 10, 30 ), &a , cvScalar(255) );
    
    cvShowImage( argv[1], img2 );
    // cout << cvGetWindowProperty( argv[1], CV_CAP_PROP_FRAME_WIDTH );
    cvWaitKey( 0 );

    // specifying file format is must
    // cvSaveImage( "saved.jpg", img );
    
    cvReleaseImage(&img);
    cvReleaseImage(&img2);
    cvDestroyWindow( argv[1] );
    cvDestroyAllWindows();
    return 0;
}