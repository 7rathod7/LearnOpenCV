#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "fontcolor.h"

using namespace std;

/*!
	program on morphological Top Hat operation
	final result is image with removed boundary curves
*/

int main( int argc, char **argv ) {

	if ( argc != 2 ) {
		fprintf( stderr, "Error : invalid number of arguments passed\n");
		exit( -1 );
	}

	IplImage *image = cvLoadImage( argv[1], CV_LOAD_IMAGE_UNCHANGED );

	if( image == NULL ) {
        fprintf( stderr, "Error : failed to load image\n");
        exit( 1 );
	}

	cvNamedWindow( "source", CV_WINDOW_NORMAL );
	cvShowImage( "source", image );
	/*! cvWaitKey( 0 ); */

	/*! code for performing morphological top operation */
	IplImage *dest;
	dest = cvCreateImage( cvGetSize( image ), image->depth, image->nChannels );

	if( dest == NULL ) {
        fprintf( stderr, "Error : failed to create image\n");
        exit( 1 );
	}

	cvMorphologyEx( image, dest, NULL, NULL, CV_MOP_TOPHAT, 2 );
    cvNamedWindow( "MorTopHat", CV_WINDOW_NORMAL );
    cvShowImage( "MorTopHat", dest );
	/*! cvWaitKey( 0 ); */

	/*! convert the above result to 8 bit mask */
	IplImage *mask;
	mask = cvCreateImage( cvGetSize( image ), image->depth, 1 );
	if( mask == NULL ) {
        fprintf( stderr, "Error : failed to create image\n");
        exit( 1 );
	}
    cvConvertImage( dest, mask , 0 );
    cvNamedWindow( "mask", CV_WINDOW_NORMAL );
    cvShowImage( "mask", mask );
	/*! cvWaitKey( 0 ); */

	/*! copy grayscale value into top hat pieces and display the result */
    cvSet( dest, cvScalarAll( 80 ), mask );
    cvNamedWindow( "tophatmask", CV_WINDOW_NORMAL );
    cvShowImage( "tophatmask", dest );
	cvWaitKey( 0 );

	/*! cleaning allotted memory */
    cvReleaseImage( &image );
    cvReleaseImage( &dest );
    cvReleaseImage( &mask );
    cvDestroyWindow( "source" );
    cvDestroyWindow( "MorTopHat" );
    cvDestroyWindow( "mask" );
    cvDestroyWindow( "tophatmask" );

	return 0;
}
