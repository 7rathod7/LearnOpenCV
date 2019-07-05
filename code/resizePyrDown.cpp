#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "fontcolor.h"

using namespace std;

/*!
	program on resizing image and
	pyramid images
	Answer : after seeing resultant images, we get more clear image by performing
	cvresize than cvpyrdown. Because in later method, we perform downsampling the
	input image whereas in first method, we do different interpolation.
*/

int main( int argc, char **argv ) {

	if ( argc != 2 ) {
		fprintf( stderr, "Error : invalid number of arguments passed\n");
		exit( -1 );
	}

	IplImage *image = cvLoadImage( argv[1], CV_LOAD_IMAGE_UNCHANGED );
	cvNamedWindow( "source", CV_WINDOW_NORMAL );
	cvShowImage( "source", image );
	/* cvWaitKey( 0 ); */

	/*!
        code for resizing the image by 2 times in both direction
        for 3 times
	*/
	IplImage *dest1;
	dest1 = cvCreateImage( cvSize( image->width / 2, image->height / 2 ), image->depth, image->nChannels );
	cvResize( image, dest1 );
	IplImage *dest2;
	dest2 = cvCreateImage( cvSize( dest1->width / 2, dest1->height / 2 ), dest1->depth, dest1->nChannels );
	cvResize( dest1, dest2 );
	IplImage *dest3;
	dest3 = cvCreateImage( cvSize( dest2->width / 2, dest2->height / 2 ), dest2->depth, dest2->nChannels );
	cvResize( dest2, dest3 );
    cvNamedWindow( "resize", CV_WINDOW_NORMAL );
    cvShowImage( "resize", dest3 );
	/* cvWaitKey( 0 ); */

	/*! performing cvpyrdown operation 3 times on input image */
    IplImage *dest11;
	dest11 = cvCreateImage( cvSize( image->width / 2, image->height / 2 ), image->depth, image->nChannels );
	cvPyrDown( image, dest11 );
	IplImage *dest12;
	dest12 = cvCreateImage( cvSize( dest11->width / 2, dest11->height / 2 ), dest11->depth, dest11->nChannels );
	cvPyrDown( dest11, dest12 );
	IplImage *dest13;
	dest13 = cvCreateImage( cvSize( dest12->width / 2, dest12->height / 2 ), dest12->depth, dest12->nChannels );
	cvPyrDown( dest12, dest13 );
    cvNamedWindow( "cvpyrdown", CV_WINDOW_NORMAL );
    cvShowImage( "cvpyrdown", dest13 );
	cvWaitKey( 0 );

    cvReleaseImage( &image );
    cvReleaseImage( &dest1 );
    cvReleaseImage( &dest2 );
    cvReleaseImage( &dest3 );
    cvReleaseImage( &dest11 );
    cvReleaseImage( &dest12 );
    cvReleaseImage( &dest13 );
    cvDestroyWindow( "source" );
    cvDestroyWindow( "resize" );
    cvDestroyWindow( "cvpyrdown" );

	return 0;
}
