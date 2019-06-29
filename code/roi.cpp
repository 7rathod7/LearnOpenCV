/*
 * headers
 */
#include <opencv/cv.h>
#include <opencv/highgui.h>

/*
 * main function
 */
int main( int argc, char** argv ) {
    
    // ROI example
    // IplImage* src;
    // if ( argc == 7 && ((src = cvLoadImage( argv[1], 1 )) != 0 )) {
    //     int x = atoi( argv[2] );
    //     int y = atoi( argv[3] );
    //     int width = atoi( argv[4] );
    //     int height = atoi( argv[5] );
        
    //     int add = atoi( argv[6] );
    //     cvSetImageROI( src, cvRect( x, y, width, height ));
    //     // cvScalar ( B, G, R )
    //     cvAddS( src,  cvScalar( 0, 0, add ), src );
    //     cvResetImageROI( src );
    //     cvNamedWindow( "Roi_Add", 1 );
    //     cvShowImage( "Roi_Add", src );
    //     cvWaitKey( 0 );
    //     cvDestroyWindow( "Roi_Add" );
    // }

    // IplImage* interest_img = cvLoadImage( argv[1] );
    // CvRect interest_rect( 300, 300, 400, 400);

    // // Use widthstep to get a region of interest
    // IplImage* sub_img = cvCreateImageHeader(
    //     cvSize(
    //         interest_rect.width*3,
    //         interest_rect.height
    //     ),
    //     interest_img->depth,
    //     interest_img->nChannels
    // );

    // sub_img->origin = interest_img->origin;
    // sub_img->widthStep = 3*interest_img->widthStep;
    // sub_img->imageData = interest_img->imageData +
    //     interest_rect.y * interest_img->widthStep +
    //     interest_rect.x * interest_img->nChannels;

    // // cvAddS( sub_img, cvScalar( 1 ), sub_img );
    // cvNamedWindow( "Roi_Add", 1 );
    // cvShowImage( "Roi_Add", sub_img );
    // cvWaitKey( 0 );
    // cvDestroyWindow( "Roi_Add" );
    // cvReleaseImageHeader( &sub_img );

    // alpha blending
    IplImage *src1, *src2;
    if ( argc == 8 && ((src1 = cvLoadImage( argv[1], 1 )) != 0 )
            && ((src2 = cvLoadImage( argv[2], 1 )) != 0 )) {
        int x = atoi( argv[3] );
        int y = atoi( argv[4] );
        int width = atoi( argv[5] );
        int height = atoi( argv[6] );
        
        double alpha = atof( argv[7] );
        cvSetImageROI( src1, cvRect( x, y, width, height ));
        cvSetImageROI( src2, cvRect( 0, 0, width, height ));
        cvAddWeighted( src1, alpha, src2, 1.0 - alpha, 0.0, src1 );
        // cvScalar ( B, G, R )
        // cvAddS( src,  cvScalar( 0, 0, add ), src );
        cvResetImageROI( src1 );
        cvNamedWindow( "Alpha_blend", 1 );
        cvShowImage( "Alpha_blend", src1 );
        cvWaitKey( 0 );
        cvDestroyWindow( "Alpha_blend" );
        cvReleaseImage( &src1 );
        cvReleaseImage( &src2 );
    }

    // BGR split
    // IplImage *im1_bgr = cvLoadImage( "res/skull.jpg" );
    // IplImage *im1_b = cvCreateImage(cvGetSize(im1_bgr), 8, 1);
    // IplImage *im1_g = cvCreateImage(cvGetSize(im1_bgr), 8, 1);
    // IplImage *im1_r = cvCreateImage(cvGetSize(im1_bgr), 8, 1);
    // cvSplit(im1_bgr, im1_b, im1_g, im1_r, NULL);

    // // Alpha channel creation (transparency)
    // IplImage *im1_a = cvCreateImage(cvGetSize(im1_bgr), 8, 1);
    // // Set the alpha value
    // cvSet(im1_a, cvScalar(128), NULL);

    // // Merge the 4 channel to an BGRA image
    // IplImage *im1_bgra = cvCreateImage(cvGetSize(im1_bgr), 8, 4);
    // cvMerge(im1_b, im1_g, im1_r, im1_a, im1_bgra);
    // cvNamedWindow ( "window" );
    // cvShowImage( "window", im1_bgra );
    // cvWaitKey( 0 );
    // cvDestroyWindow( "window" );

    return 0;
}