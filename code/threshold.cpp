#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "fontcolor.h"

using namespace std;

/*!
	program on usage of cvthreshold
*/

int main( int argc, char **argv ) {

	if ( argc != 2 ) {
		fprintf( stderr, "Error : invalid number of arguments passed\n");
		exit( -1 );
	}

    /*! Load a scene */
    IplImage *scene = cvLoadImage( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
    if( scene == NULL ) {
        fprintf( stderr, "Error : unable to load scene\n");
        exit( 1 );
    }
    cvNamedWindow( "scene", CV_WINDOW_NORMAL );
    cvShowImage( "scene", scene );
    /*! cvWaitKey( 0 ); */

    /*! set the threshold to 128 by using all setting type */
    IplImage *dst = cvCreateImage( cvGetSize( scene ), scene->depth, scene->nChannels );
    /*! binary */
    cvThreshold( scene, dst, 128, 255, CV_THRESH_BINARY );
    cvNamedWindow( "binary", CV_WINDOW_NORMAL );
    cvShowImage( "binary", dst );
    /*! binary inverse */
    cvThreshold( scene, dst, 128, 255, CV_THRESH_BINARY_INV );
    cvNamedWindow( "binaryinv", CV_WINDOW_NORMAL );
    cvShowImage( "binaryinv", dst );
    /*! truncate */
    cvThreshold( scene, dst, 128, 255, CV_THRESH_TRUNC );
    cvNamedWindow( "truncate", CV_WINDOW_NORMAL );
    cvShowImage( "truncate", dst );
    /*! to zero inverse */
    cvThreshold( scene, dst, 128, 255, CV_THRESH_TOZERO_INV );
    cvNamedWindow( "tozeroinv", CV_WINDOW_NORMAL );
    cvShowImage( "tozeroinv", dst );
    /*! to zero */
    cvThreshold( scene, dst, 128, 255, CV_THRESH_TOZERO );
    cvNamedWindow( "tozero", CV_WINDOW_NORMAL );
    cvShowImage( "tozero", dst );

    /*! use adaptive threshold and repeat above stuff param1 = 5*/
    /*! binary */
    cvAdaptiveThreshold( scene, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY );
    cvNamedWindow( "binary1", CV_WINDOW_NORMAL );
    cvShowImage( "binary1", dst );
    /*! binary inverse */
    cvAdaptiveThreshold( scene, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV );
    cvNamedWindow( "binaryinv1", CV_WINDOW_NORMAL );
    cvShowImage( "binaryinv1", dst );
    /*! truncate -> doesnt work here */
    /* cvAdaptiveThreshold( scene, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_TRUNC );
    cvNamedWindow( "truncate1", CV_WINDOW_NORMAL );
    cvShowImage( "truncate1", dst ); */
    /*! to zero inverse -> doesnt work here */
    /* cvAdaptiveThreshold( scene, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_TOZERO_INV );
    cvNamedWindow( "tozeroinv1", CV_WINDOW_NORMAL );
    cvShowImage( "tozeroinv1", dst ); */
    /*! to zero -> doesnt work here */
    /* cvAdaptiveThreshold( scene, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_TOZERO );
    cvNamedWindow( "tozero1", CV_WINDOW_NORMAL );
    cvShowImage( "tozero1", dst ); */
    /*! use adaptive threshold and repeat above stuff param1 = 0 and param1 = 5 */
    cvWaitKey( 0 );
    /*! binary */
    cvAdaptiveThreshold( scene, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 0 );
    cvAdaptiveThreshold( dst, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 5 );
    cvNamedWindow( "binary2", CV_WINDOW_NORMAL );
    cvShowImage( "binary2", dst );
    /*! binary inverse */
    cvAdaptiveThreshold( scene, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 3, 0 );
    cvAdaptiveThreshold( dst, dst, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 3, 5 );
    cvNamedWindow( "binaryinv2", CV_WINDOW_NORMAL );
    cvShowImage( "binaryinv2", dst );
    cvWaitKey( 0 );
    /*! clear the memory */
    cvReleaseImage( &scene );
    cvReleaseImage( &dst );
    cvDestroyWindow( "scene" );
    cvDestroyWindow( "binary" );
    cvDestroyWindow( "binaryinv" );
    cvDestroyWindow( "truncate" );
    cvDestroyWindow( "tozeroinv" );
    cvDestroyWindow( "tozero" );
    cvDestroyWindow( "binary1" );
    cvDestroyWindow( "binaryinv1" );
    cvDestroyWindow( "binary2" );
    cvDestroyWindow( "binaryinv2" );
    /* cvDestroyWindow( "truncate1" );
    cvDestroyWindow( "tozeroinv1" );
    cvDestroyWindow( "tozero1" ); */
	return 0;
}
