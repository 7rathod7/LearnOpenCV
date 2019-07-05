#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

CvRect box;
bool drawing_box = false;
IplImage *orig;

/*! A little subroutine to draw a box onto a image  */
void draw_box( IplImage *img ) {
    cvRectangle(
        img,
        cvPoint( box.x, box.y ),
        cvPoint( box.x + box.width, box.y + box.height ),
        cvScalar( 0x00, 0x00, 0xff )    /*! red */
    );
    if ( box.width > 0 && box.height > 0 ) {
        cvSetImageROI( img, cvRect( box.x, box.y, box.width, box.height ));
        /*! cvScalar ( B, G, R ) */
        cvAddS( img,  cvScalar( 0, 0, 200 ), img );
        cvResetImageROI( img );
    }
}

/*! get stats about RGB values */
void RGB_hist(IplImage *img) {
    IplImage *image = cvCreateImage(
            cvGetSize( img ),
            img->depth,
            img->nChannels
    );
    IplImage *temp = cvCreateImage(
            cvSize( 800, 400 ),
            image->depth,
            3
    );
    int r_values[8], i, j;
    long int sum;
    /*! working on histogram, k = 0(r), k = 1(g), k = 2(b) */
    for (int k = 0; k < 3; k++) {
        /*! create empty black window    */
        cvZero( image );
        cvZero( temp );
        cvCopy( img, image);

        /*! create 8 rectangular region  */
        // box = cvRect( 0, 0, 100, 100 );
        sum = 0;
        for ( i = 0; i < 8; i++ ) r_values[i] = 0;
        uchar *pinput;
        int r, g, b;
        pinput = (uchar*)image->imageData;
        // b = pinput[ x * image->widthStep + y * image->nChannels + 0];
        // g = pinput[ x * image->widthStep + y * image->nChannels + 1];
        for ( i = box.x; i < box.x + box.width; i++ ) {
            for ( j = box.y; j < box.y + box.height; j++ ) {
                r = pinput[ i * image->widthStep + j * image->nChannels + k ];
                r_values[r / 32]++;
                // cout << r << "\t";
            }
        }
        for ( i = 0; i < 8; i++ ) sum += r_values[i];
        for ( i = 0; i < 8; i++ ) {
            if ( k == 0 ) cvRectangle(
                temp,
                cvPoint( 100 * i, 0 ),
                cvPoint( 100 * i + 95, 0 + 395 ),
                cvScalar( 0xff, 0x00, 0x00 )    /* blue */
            );
            if ( k == 1 ) cvRectangle(
                temp,
                cvPoint( 100 * i, 0 ),
                cvPoint( 100 * i + 95, 0 + 395 ),
                cvScalar( 0x00, 0xff, 0x00 )    /* green */
            );
            if ( k == 2 ) cvRectangle(
                temp,
                cvPoint( 100 * i, 0 ),
                cvPoint( 100 * i + 95, 0 + 395 ),
                cvScalar( 0x00, 0x00, 0xff )    /* red */
            );
            // int u = ceil( 395 * (r_values[i] * 1.0 / sum ) );
            // cout <<  ceil( 395 * (r_values[i] * 1.0 / sum ) ) << "\n";
            cvSetImageROI( temp, cvRect( 100 * i, 395 - ceil( 395 * (r_values[i] * 1.0 / sum ) ), 95, ceil( 395 * (r_values[i] * 1.0 / sum ) ) ));
            /*! cvScalar ( B, G, R ) */
            if ( k == 2 ) cvAddS( temp,  cvScalar( 0, 0, 200 ), temp );
            if ( k == 1 ) cvAddS( temp,  cvScalar( 0, 200, 0 ), temp );
            if ( k == 0 ) cvAddS( temp,  cvScalar( 200, 0, 0 ), temp );
            cvResetImageROI( temp );
        }

        cvNamedWindow( "Stat", CV_WINDOW_AUTOSIZE );
        cvShowImage( "Stat", temp );
        cvWaitKey( 5000 );
        cvDestroyWindow( "Stat" );
    }
    cvReleaseImage( &image );
    cvReleaseImage( &temp );
}

/*! define our callback which we will install for mouse events  */
void my_mouse_callback( int event, int x, int y, int flags, void* param ) {
    IplImage *image = (IplImage*) param;
    switch ( event ) {
        case CV_EVENT_MOUSEMOVE:
            if ( drawing_box ) {
                box.width = x - box.x;
                box.height = y - box.y;
            }
            break;

        case CV_EVENT_LBUTTONDOWN:
            // cout << "left mouse button is down\n";
            drawing_box = true;
            box = cvRect( x, y, 0, 0 );
            cvCopy( orig, image );
            break;

        case CV_EVENT_LBUTTONUP:
            drawing_box = false;
            if ( box.width < 0 ) {
                box.x += box.width;
                box.width *= -1;
            }
            if ( box.height < 0 ) {
                box.y += box.height;
                box.height *= -1;
            }
            draw_box( image );
            if( box.width > 0 && box.height > 0) RGB_hist( image );

        default:
            break;
    }
}

/*! main function   */
int main( int argc, char** argv ) {

    /*! return if number of command line arguments passed are incorrect */
    if ( argc != 2 ) {
        fprintf( stderr, "pass 2 command line arguments\n" );
        exit( -1 );
    }
    box = cvRect( -1, -1, 0, 0 );
    IplImage *image = cvLoadImage( argv[1] );
    IplImage *temp = cvCloneImage( image );
    orig = cvCloneImage( image );

    cvNamedWindow( "Box Example" );

    /*! here is the crucial moment that we actually install the callback
        Note that we set the value 'param' to be the image we are working with so
        that the callback will have the image to edit    */
    cvSetMouseCallback(
        "Box Example",
        my_mouse_callback,
        ( void * ) image
    );

    /*! the main program loop. here we copy the working image
        to the temp image and if the user is drawing, then put
        the currently contemplated box onto that temp image
        display the temp image, and wait 15ms for a keystroke the repeat */
    while( 1 ) {
        cvCopy( image, temp );
        if ( drawing_box ) draw_box( temp );
        cvShowImage( "Box Example", temp );

        if ( cvWaitKey( 15 ) == 27 ) break;
    }

    // Be tidy
    cvReleaseImage ( &image );
    cvReleaseImage ( &temp );
    cvDestroyWindow ( "Box Example" );
    return 0;
}
