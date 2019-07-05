/*! smoothing program */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>
#include "fontcolor.h"

using namespace std;

/*!
	creating random low variance filter/ applying
	bilateral filter on input image
*/

int main( int argc, char **argv ) {

	if ( argc != 2 ) {
		fprintf( stderr, "Error : invalid number of arguments passed\n");
		exit( 1 );
	}

	cout << RED << "\nPress 1 to generate or 2 to apply filter\n" << RESET;

	IplImage *image;

	int input;
	cin >> input;

	if( input == 1 ) {
		/*! code for generate low variance filter */
		image = cvCreateImage(
				cvSize( 500, 500 ),
				IPL_DEPTH_8U,
				3
			);
//        cout << rand() << endl;
        CvRNG rng = cvRNG( rand() );

        cvRandArr( &rng, image, CV_RAND_UNI, cvScalarAll( 100 ), cvScalarAll( 3 ) );

        cvNamedWindow( "lowvar", CV_WINDOW_NORMAL );
        cvShowImage( "lowvar", image );
        cvWaitKey( 0 );

        /*! int res = cvSaveImage( argv[1], image );

        if( res == 0 ) {
            fprintf( stderr, "Error : saving file\n" );
            exit( 1 );
        } */
        cvReleaseImage( &image );
        cvDestroyWindow( "lowvar" );
	}
	else if ( input == 2 ) {
		/*! code for applying filter */
        image = cvLoadImage( argv[1], CV_LOAD_IMAGE_UNCHANGED );

        if ( image == NULL ) {
            fprintf( stderr, "Error : unable to load image\n" );
            exit( 1 );
        }

        IplImage *dest = cvCreateImage( cvGetSize( image ), image->depth, image->nChannels );
        cvSmooth( image, dest, CV_BILATERAL, 9, 9, 120, 3 );

        cvNamedWindow( "smoothing", CV_WINDOW_NORMAL );
        cvShowImage( "smoothing", image );
        cvWaitKey( 0 );
        cvShowImage( "smoothing", dest );
        cvWaitKey( 0 );
        cvReleaseImage( &dest );
        cvDestroyWindow( "smoothing" );
        cvReleaseImage( &image );
	}
	else {
		/*! input passes is not correct */
		cout << YELLOW << "\ninput passed is not of context" << RESET;
	}

	return 0;
}
