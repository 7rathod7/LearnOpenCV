#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "fontcolor.h"

using namespace std;

/*!
	program on usage of cvPyrMeanShiftFiltering
*/

int main( int argc, char **argv ) {

	if ( argc != 2 ) {
		fprintf( stderr, "Error : invalid number of arguments passed\n");
		exit( -1 );
	}

    /*! Load a scene */
    IplImage *scene = cvLoadImage( argv[1], CV_LOAD_IMAGE_UNCHANGED );

    if( scene == NULL ) {
        fprintf( stderr, "Error : unable to load scene\n");
        exit( 1 );
    }

    cvNamedWindow( "scene", CV_WINDOW_NORMAL );
    cvShowImage( "scene", scene );
    /*! cvWaitKey( 0 ); */

    /*! use cvpyrmeanshiftfiltering */
    IplImage *dst = cvCreateImage( cvGetSize( scene ), scene->depth, scene->nChannels );

    if( dst == NULL ) {
        fprintf( stderr, "Error : failed to create image\n");
        exit( 1 );
    }

    cvPyrMeanShiftFiltering( scene, dst, 10, 10, 8 );
    cvNamedWindow( "segmentation", CV_WINDOW_NORMAL );
    cvShowImage( "segmentation", dst );
    cvWaitKey( 0 );

    /*! clear the memory */
    cvReleaseImage( &scene );
    cvReleaseImage( &dst );
    cvDestroyWindow( "scene" );
    cvDestroyWindow( "segmentation" );
	return 0;
}
