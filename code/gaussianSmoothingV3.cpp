/*! smoothing program */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

/*! 
	ref : https://github.com/drichardson/examples/blob/master/OpenCV/BookExercises/Exercise5.3.c

*/

int main( int argc, char **argv ) {

	if ( argc != 2 ) {
		fprintf( stderr, "Error : invalid number of arguments passed to program\n" );
		exit( -1 );
	}

	/*! Load an interesting image	*/
	IplImage *image = cvLoadImage( argv[1] );
	IplImage *output = cvCreateImage(
		cvGetSize( image ),
		image->depth,
		image->nChannels
	);

	cvNamedWindow( "final11", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "final12", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "final13", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "final21", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "final22", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "final23", CV_WINDOW_AUTOSIZE );
	cvMoveWindow ( "final12", image->width, 80);
	cvMoveWindow ( "final13", 2*image->width, 80);
	cvMoveWindow ( "final21", 0, 80 + image->height );
	cvMoveWindow ( "final22", image->width, 80 + image->height );
	cvMoveWindow ( "final23", 2*image->width, 80 + image->height );

	/*! param1=param2=9 */
	/*! param3=1 */
	cvSmooth( image, output, CV_GAUSSIAN, 9, 9, 1 );
	cvShowImage( "final11", output );
	// cvWaitKey( 0 );
	/*! param3=4 */
	cvSmooth( image, output, CV_GAUSSIAN, 9, 9, 4 );
	cvShowImage( "final12", output );
	// cvWaitKey( 0 );
	/*! param3=6 */
	cvSmooth( image, output, CV_GAUSSIAN, 9, 9, 6 );
	cvShowImage( "final13", output );
	// cvWaitKey( 0 );

	/*! param1=param2=0 */
	/*! param3=1 */
	cvSmooth( image, output, CV_GAUSSIAN, 0, 0, 1 );
	cvShowImage( "final21", output );
	// cvWaitKey( 0 );
	/*! param3=4 */
	cvSmooth( image, output, CV_GAUSSIAN, 0, 0, 4 );
	cvShowImage( "final22", output );
	// cvWaitKey( 0 );
	/*! param3=6 */
	cvSmooth( image, output, CV_GAUSSIAN, 0, 0, 6 );
	cvShowImage( "final23", output );
	cvWaitKey( 0 );

	/*!
		after performing above operation, output results are different
		because kernel size are different and when param1=param2=0, kernel
		size is calculated from param3=sigma of gaussian distribution
	 */

	/*! param3=1 param4=9 */
	cvSmooth( image, output, CV_GAUSSIAN, 0, 0, 1, 9 );
	cvShowImage( "final22", output );
	// cvWaitKey( 0 );
	cvSmooth( image, output, CV_GAUSSIAN, 0, 0, 9, 1 );
	cvShowImage( "final23", output );
	cvWaitKey( 0 );
	/*!
		param3 is sigma along x axis while param4 is sigam along y axis
		first operation is horizontal smoothing while second operation 
		is vertical smoothing
	 */

	cvSmooth( image, output, CV_GAUSSIAN, 0, 0, 1, 9 );
	cvSmooth( output, output, CV_GAUSSIAN, 0, 0, 9, 1 );
	cvShowImage( "final23", output );
	cvWaitKey( 0 );
	/*!
		smoothing is done along both horizontal and vertical direction resulting
		in uniform smoothing .
	 */
	
	cvReleaseImage( &image );
	cvReleaseImage( &output );
	cvDestroyWindow( "final11" );
	cvDestroyWindow( "final12" );
	cvDestroyWindow( "final13" );
	cvDestroyWindow( "final21" );
	cvDestroyWindow( "final22" );
	cvDestroyWindow( "final23" );

	return 0;
}