/*! smoothing program */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

/*!
	Differnce between cleandiff and dirtydiff images is :
	In dirtydiff image, the negligible features are highlighted whereas
	in cleandiff those are not.
*/

int main( int argc, char **argv ) {

	if ( argc != 3 ) {
		fprintf( stderr, "Error : invalid number of arguments passed to program( Photo1-ex5.4.jpg, Photo2-ex5.4.jpg )\n" );
		exit( 1 );
	}

	/*! Load an interesting image	*/
	IplImage *src1 = cvLoadImage( argv[1], CV_LOAD_IMAGE_UNCHANGED );
	IplImage *src2 = cvLoadImage( argv[2], CV_LOAD_IMAGE_UNCHANGED );

	if( src1 == NULL || src2 == NULL) {
        fprintf( stderr, "Error : couldnt load input images\n" );
		exit( 1 );
	}

	IplImage *diff12 = cvCreateImage(
		cvGetSize( src1 ),
		src1->depth,
		src1->nChannels
	);

	if( diff12 == NULL ) {
        fprintf( stderr, "Error : could not create image\n" );
		exit( 1 );
	}

	cvNamedWindow( "source 1", CV_WINDOW_NORMAL );
	cvNamedWindow( "source 2", CV_WINDOW_NORMAL );
	cvNamedWindow( "diff12", CV_WINDOW_NORMAL );
	cvNamedWindow( "cleandiff", CV_WINDOW_NORMAL );
	cvNamedWindow( "dirtydiff", CV_WINDOW_NORMAL );

	cvShowImage( "source 1", src1 );
	cvShowImage( "source 2", src2 );

	cvAbsDiff( src1, src2, diff12 );
	cvShowImage( "diff12", diff12 );
	// cvWaitKey( 0 );

	/*! creating clean diff */
	IplImage *cleandiff = cvCreateImage(
		cvGetSize( diff12 ),
		diff12->depth,
		diff12->nChannels
	);

	if( cleandiff == NULL ) {
        fprintf( stderr, "Error : could not create image\n" );
		exit( 1 );
	}

	cvErode( diff12, cleandiff );
	cvDilate( cleandiff, cleandiff );
	cvShowImage( "cleandiff", cleandiff );
	// cvWaitKey( 0 );

	/*! creating dirty diff */
	IplImage *dirtydiff = cvCreateImage(
		cvGetSize( diff12 ),
		diff12->depth,
		diff12->nChannels
	);

	if( dirtydiff == NULL ) {
        fprintf( stderr, "Error : could not create image\n" );
		exit( 1 );
	}

	cvDilate( diff12, dirtydiff );
	cvErode( dirtydiff, dirtydiff );
	cvShowImage( "dirtydiff", dirtydiff );
	cvWaitKey( 0 );


    /*! releasing alloted memory */
	cvReleaseImage( &src1 );
	cvReleaseImage( &src2 );
	cvReleaseImage( &diff12 );
	cvReleaseImage( &cleandiff );
	cvReleaseImage( &dirtydiff );
	cvDestroyWindow( "source 1" );
	cvDestroyWindow( "source 2" );
	cvDestroyWindow( "diff12" );
	cvDestroyWindow( "cleandiff" );
	cvDestroyWindow( "dirtydiff" );

	return 0;
}
