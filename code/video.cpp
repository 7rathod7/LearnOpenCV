/*
 * headers
 */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

IplImage *bigimage;

bool A1 = false, A2 = false, A3 = false;
CvPoint a1, a2, a3;

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
            if ( x >= 0 && x < image->width/3 ) {
                A1 = true;
                a1 = cvPoint( x, y );
            } else if ( x >= image->width / 3 && x < 2 * image->width / 3) {
                A2 = true;
                a2 = cvPoint( x, y );
            } else {
                A3 = true;
                a3 = cvPoint( x, y );
            }
            break;
        
        case CV_EVENT_LBUTTONUP:
            break;

        default:
            break;
    }
}

/*
 * canny edge detection
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

/*
 * main function
 */
int main( int argc, char** argv ) {
    cvNamedWindow( "Video", CV_WINDOW_AUTOSIZE );
    // cvNamedWindow( "grayVideo", CV_WINDOW_AUTOSIZE );
    // cvNamedWindow( "cannyEdgeVideo", CV_WINDOW_AUTOSIZE );
    CvCapture *capture = cvCreateFileCapture( argv[1] );
    IplImage *frame, *temp, *temp1;

    // get capture property
    // double f = cvGetCaptureProperty(
    //     capture,
    //     CV_CAP_PROP_FOURCC
    // );
    // char *fourcc = ( char* )&f;

    // cout << fourcc[0] << fourcc[1] << endl;

    while(1) {
        // cout << A1 << A2 << A3 << endl;
        frame = cvQueryFrame( capture );
        if ( !frame ) break;
        temp = cvCreateImage(
            cvSize( frame->width, frame->height ),
            IPL_DEPTH_8U,
            1
        );
        temp1 = cvCreateImage(
            cvSize( frame->width, frame->height ),
            IPL_DEPTH_8U,
            3
        );
        bigimage = cvCreateImage(
            cvSize( 3*frame->width, frame->height ),
            IPL_DEPTH_8U,
            3
        );
        cvConvertImage( frame, temp, 0);
        cvConvertImage( temp, temp1, 0);
        cvSetMouseCallback(
            "Video",
            my_mouse_callback,
            ( void * ) bigimage
        );
        // cvShowImage( "Video", frame );
        // cvShowImage( "grayVideo", temp );
        // cvShowImage( "cannyEdgeVideo", doCanny( temp, 10, 100, 3 ) );

        // CvArr* k ;

        // copying original frames first
        cvSetImageROI( bigimage, cvRect( 0, 0, frame->width, frame->height ));
        // cvScalar ( B, G, R )
        // cvAddS( src,  cvScalar( 0, 0, add ), src );
        cvCopy( frame, bigimage );
        cvResetImageROI( bigimage );
        // copying gray frames second
        cvSetImageROI( bigimage, cvRect( frame->width, 0, frame->width, frame->height ));
        cvCopy( temp1, bigimage );
        cvResetImageROI( bigimage );
        // copying canny edges lastly
        cvSetImageROI( bigimage, cvRect( 2 * frame->width, 0, frame->width, frame->height ));
        cvConvertImage( doCanny( temp, 10, 100, 3 ), temp1, 0);
        cvCopy( temp1, bigimage );
        cvResetImageROI( bigimage );

        CvFont a;
        cvInitFont( &a, CV_FONT_HERSHEY_SIMPLEX, 1, 1);
        cvPutText( bigimage, "original", cvPoint( 10, 30 ), &a , cvScalar( 0, 0, 255 ) );
        cvPutText( bigimage, "gray scale", cvPoint( frame->width+10, 30 ), &a , cvScalar( 0, 0, 255 ) );
        cvPutText( bigimage, "canny edge", cvPoint( 2 * frame->width+10, 30 ), &a , cvScalar( 0, 0, 255 ) );

        char co_ord_x[10], co_ord_y[5];
        if ( A1 ) {
            sprintf( co_ord_x, "%d", a1.x );
            strcat( co_ord_x, "," );
            sprintf( co_ord_y, "%d", a1.y );
            strcat( co_ord_x, co_ord_y );
            cvPutText( bigimage, co_ord_x, a1, &a , cvScalar( 0, 0, 255 ) );
        }
        if ( A2 ) {
            sprintf( co_ord_x, "%d", a2.x );
            strcat( co_ord_x, "," );
            sprintf( co_ord_y, "%d", a2.y );
            strcat( co_ord_x, co_ord_y );
            cvPutText( bigimage, co_ord_x, a2, &a , cvScalar( 0, 0, 255 ) );
        }
        if ( A3 ) {
            sprintf( co_ord_x, "%d", a3.x );
            strcat( co_ord_x, "," );
            sprintf( co_ord_y, "%d", a3.y );
            strcat( co_ord_x, co_ord_y );
            cvPutText( bigimage, co_ord_x, a3, &a , cvScalar( 0, 0, 255 ) );
        }
        cvShowImage( "Video", bigimage );

        char c = cvWaitKey( 33 );
        if ( c == 27 ) break;
        cvReleaseImage( &temp );
    }
    cvReleaseCapture( &capture );
    cvDestroyWindow ( "Video" );
    // cvDestroyWindow ( "grayVideo" );
    // cvDestroyWindow ( "cannyEdgeVideo" );
    return 0;
}