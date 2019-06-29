/*! smoothing program */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

/*!
	open morphology smoothing to find indepedent seperate component
*/

int main( int argc, char **argv ) {

	if ( argc != 3 ) {
		fprintf( stderr, "Error : invalid number of arguments passed to program\n" );
		exit( -1 );
	}

	/*! Load an interesting image	*/
	IplImage *src1 = cvLoadImage( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	IplImage *src2 = cvLoadImage( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
	IplImage *diff12 = cvCreateImage(
		cvGetSize( src1 ),
		src1->depth,
		src1->nChannels
	);
	
	cvNamedWindow( "source 1", CV_WINDOW_NORMAL );
	cvNamedWindow( "source 2", CV_WINDOW_NORMAL );
	cvNamedWindow( "diff12", CV_WINDOW_NORMAL );
	cvNamedWindow( "result", CV_WINDOW_NORMAL );
	cvNamedWindow( "mop", CV_WINDOW_NORMAL );

	cvShowImage( "source 1", src1 );
	cvShowImage( "source 2", src2 );

	// cvSub( src1, src2, diff12 );
	cvAbsDiff( src1, src2, diff12 );
	cvShowImage( "diff12", diff12 );
	// cvWaitKey( 0 );

	/*! applying threshold */
	IplImage *result = cvCreateImage(
		cvGetSize( diff12 ),
		diff12->depth,
		diff12->nChannels
	);
	cvThreshold( diff12, result, 40, 255, CV_THRESH_BINARY );
	cvShowImage( "result", result );
	// cvWaitKey( 0 );

	/*! doing cv_mop_open on result to further clean up */
	IplImage *mopResult = cvCreateImage(
		cvGetSize( diff12 ),
		diff12->depth,
		diff12->nChannels
	);
	cvMorphologyEx( result, mopResult, NULL, NULL, CV_MOP_OPEN, 1 );
	cvShowImage( "mop", mopResult );
	cvWaitKey( 0 );

	cvReleaseImage( &src1 );
	cvReleaseImage( &src2 );
	cvReleaseImage( &diff12 );
	cvReleaseImage( &result );
	cvReleaseImage( &mopResult );
	cvDestroyWindow( "source 1" );
	cvDestroyWindow( "source 2" );
	cvDestroyWindow( "diff12" );
	cvDestroyWindow( "result" );
	cvDestroyWindow( "mop" );

	return 0;
}