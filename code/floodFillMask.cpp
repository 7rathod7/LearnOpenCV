/*! smoothing program */
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

/*!
	open morphology smoothing to find indepedent seperate component
*/

int main( int argc, char **argv ) {

	if ( argc != 4 ) {
		fprintf( stderr, "Error : invalid number of arguments passed to program( TableWithMug.jpg, TableWithoutMug.jpg, OutdoorScene.jpg )\n" );
		exit( -1 );
	}

	/*! Load an interesting image	*/
	IplImage *src1 = cvLoadImage( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	IplImage *src2 = cvLoadImage( argv[2], CV_LOAD_IMAGE_GRAYSCALE );

	if( src1 == NULL || src2 == NULL ) {
        fprintf( stderr, "Error : failed to load images\n");
        exit( 1 );
	}

	IplImage *diff12 = cvCreateImage(
		cvGetSize( src1 ),
		src1->depth,
		src1->nChannels
	);

	if( diff12 == NULL ) {
        fprintf( stderr, "Error : failed to create image\n");
        exit( 1 );
	}

    cvNamedWindow( "source 1", CV_WINDOW_NORMAL );
	cvShowImage( "source 1", src1 );
	cvNamedWindow( "source 2", CV_WINDOW_NORMAL );
	cvShowImage( "source 2", src2 );

	// cvSub( src1, src2, diff12 );
	cvAbsDiff( src1, src2, diff12 );
	cvNamedWindow( "diff12", CV_WINDOW_NORMAL );
	cvShowImage( "diff12", diff12 );
	// cvWaitKey( 0 );

	/*! applying threshold */
	IplImage *result = cvCreateImage(
		cvGetSize( diff12 ),
		diff12->depth,
		diff12->nChannels
	);

	if( result == NULL ) {
        fprintf( stderr, "Error : failed to create image\n");
        exit( 1 );
	}

	cvThreshold( diff12, result, 40, 255, CV_THRESH_BINARY );
	cvNamedWindow( "result", CV_WINDOW_NORMAL );
	cvShowImage( "result", result );
	// cvWaitKey( 0 );

	/*! doing cv_mop_open on result to further clean up */
	IplImage *mopResult = cvCreateImage(
		cvGetSize( diff12 ),
		diff12->depth,
		diff12->nChannels
	);

	if( mopResult == NULL ) {
        fprintf( stderr, "Error : failed to create image\n");
        exit( 1 );
	}

	cvMorphologyEx( result, mopResult, NULL, NULL, CV_MOP_OPEN, 1 );
	cvNamedWindow( "mop", CV_WINDOW_NORMAL );
	cvShowImage( "mop", mopResult );
	cvWaitKey( 0 );

	/*! connected component */
	CvConnectedComp largestComp;
	bzero( &largestComp, sizeof( largestComp ) );
	CvPoint largestRegionPoint = cvPoint( -1, -1 );

	int x, y;
	for( y = 0; y < mopResult->height; y++) {
		uchar *row = (uchar*)( mopResult->imageData + mopResult->widthStep * y );

		for( x = 0; x < mopResult->width; x++ ) {
			if ( row[x] == 255 ) {
				CvConnectedComp region;
				cvFloodFill( mopResult, cvPoint( x, y ), cvScalarAll( 100 ), cvScalarAll( 0 ), cvScalarAll( 0 ), &region, 4, NULL );

				if ( largestRegionPoint.x == -1 ) {
					/*! first region found */
					largestRegionPoint = cvPoint( x, y );
					largestComp = region;
				}
				else if ( largestComp.area < region.area ) {
					/*! clear the last area */
					cvFloodFill( mopResult, largestRegionPoint, cvScalarAll( 0 ), cvScalarAll( 0 ), cvScalarAll( 0 ), NULL, 4, NULL );
					largestRegionPoint = cvPoint( x, y );
					largestComp = region;
				}
				else {
					cvFloodFill( mopResult, cvPoint( x, y ), cvScalarAll( 0 ), cvScalarAll( 0 ), cvScalarAll( 0 ), NULL, 4, NULL );
				}
			}
		}
	}

	if ( largestRegionPoint.x != -1 ) {
		cvFloodFill( mopResult, largestRegionPoint, cvScalarAll( 255 ), cvScalarAll( 0 ), cvScalarAll( 0 ), NULL, 4, NULL );
	}

	cvShowImage( "mop", mopResult );
	cvWaitKey( 0 );

	// loading outdoor scene
	IplImage *outdoorScene = cvLoadImage( argv[3], CV_LOAD_IMAGE_UNCHANGED );

	if ( outdoorScene == NULL ) {
		fprintf( stderr, "Error : falied to load outdoor scene\n");
		exit( -1 );
	}

	IplImage *tempSrc2 = cvLoadImage( argv[1], CV_LOAD_IMAGE_UNCHANGED );

	cvCopy( tempSrc2, outdoorScene, mopResult );
	cvShowImage( "mop", outdoorScene );
	cvWaitKey( 0 );

	/*! cleaning the allotted memory    */
	cvReleaseImage( &src1 );
	cvReleaseImage( &src2 );
	cvReleaseImage( &diff12 );
	cvReleaseImage( &result );
	cvReleaseImage( &mopResult );
	cvReleaseImage( &outdoorScene );
	cvReleaseImage( &tempSrc2 );
	cvDestroyWindow( "source 1" );
	cvDestroyWindow( "source 2" );
	cvDestroyWindow( "diff12" );
	cvDestroyWindow( "result" );
	cvDestroyWindow( "mop" );

	return 0;
}
