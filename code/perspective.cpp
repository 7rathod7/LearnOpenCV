//! perspective transformation
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <iostream>

using namespace std;

//! main function
int main( int argc, char **argv ) {
    
    double incr = 0.5;

    //! read an image
    IplImage *image = cvLoadImage( argv[1] );
    IplImage *dstimage = cvCreateImage(
        cvGetSize( image ),
        image->depth,
        image->nChannels
    );

    CvPoint2D32f src_p[4];
    CvPoint2D32f dst_p[4];

    float w = image->width;
    float h = image->height;
    float hw = w / 2.0f;
    float hh = h / 2.0f;

    //! from points
    src_p[0] = cvPoint2D32f( 0.0f, 0.0f );
    src_p[1] = cvPoint2D32f( w, 0.0f );
    src_p[2] = cvPoint2D32f( w, h );
    src_p[3] = cvPoint2D32f( 0.0f, h );

    dst_p[0] = cvPoint2D32f( 0.0f, 0.0f );
    dst_p[1] = cvPoint2D32f( w, 0.0f );
    dst_p[2] = cvPoint2D32f( w, h );
    dst_p[3] = cvPoint2D32f( 0.0f, h );
    CvMat *wrap_matrix = cvCreateMat( 3, 3, CV_32FC1 );
    CvMat *mat = cvGetPerspectiveTransform( src_p, dst_p, wrap_matrix );
    // uchar *pinput;
    // pinput = (uchar*)wrap_matrix->data;

    // code for incrementing/decrementing the cell value of wrap matrix
    
    // cvGetPerspectiveTransform()
    // cvWaitKey( 0 );

    //! working on keyboard inputs
    //! 9th cell work on zoom in/out
    int i = 1;
    int j;
    double angle;
    while( i = cvWaitKey( 15 ) ) {
        if ( i == 27 ) break;
        if ( i == 114 || i == 1048690 ) {
            angle += 0.01;

            //! to points
            // for ( int g = 0; g < 4; g++) {
            //     dst_p[g] = cvPoint2D32f( src_p[g].x * cos( angle ) - src_p[g].y * sin( angle ), src_p[g].x * sin( angle ) + src_p[g].y * cos( angle ) );
            // }
            dst_p[0] = cvPoint2D32f( -hw, -hh );
            dst_p[1] = cvPoint2D32f( w-hw, -hh );
            dst_p[2] = cvPoint2D32f( w-hw, h-hh );
            dst_p[3] = cvPoint2D32f( -hw, h-hh );

            CvMat *wrap_matrix3 = cvCreateMat( 3, 3, CV_32FC1 );
            CvMat *mat3 = cvGetPerspectiveTransform( src_p, dst_p, wrap_matrix3 );
            cvMatMul( mat3, mat, mat);
            for ( int g = 0; g < 4; g++) {
                dst_p[g] = cvPoint2D32f( src_p[g].x * cos( angle ) - src_p[g].y * sin( angle ), src_p[g].x * sin( angle ) + src_p[g].y * cos( angle ) );
            }
            CvMat *wrap_matrix1 = cvCreateMat( 3, 3, CV_32FC1 );
            CvMat *mat1 = cvGetPerspectiveTransform( src_p, dst_p, wrap_matrix1 );
            cvMatMul( mat1, mat, mat);
            dst_p[0] = cvPoint2D32f( hw, hh );
            dst_p[1] = cvPoint2D32f( w+hw, hh );
            dst_p[2] = cvPoint2D32f( w+hw, h+hh );
            dst_p[3] = cvPoint2D32f( hw, h+hh );

            CvMat *wrap_matrix2 = cvCreateMat( 3, 3, CV_32FC1 );
            CvMat *mat2 = cvGetPerspectiveTransform( src_p, dst_p, wrap_matrix2 );
            cvMatMul( mat2, mat, mat);
        }
        // if ( i != -1 ) cout << i << "\n";
        // i = cvWaitKey( 15 );
        if ( i != -1 && i < 1114042 && i > 1114032) {
            j = i-1114033;
            cvmSet( mat, ( i-1114033 ) / 3, ( i-1114033 ) % 3, cvmGet( mat, ( i-1114033 ) / 3, ( i-1114033 ) % 3 ) + incr );
            int i, j;
            for ( i = 0; i < 3; i++ )
                for ( j = 0; j < 3; j++ ) cout << cvmGet( mat, i, j ) << "\t";
            cout << "\n";
        }
        if ( i == 1114081) {
            if ( cvmGet( mat, j / 3, j % 3 ) > 0 ) cvmSet( mat, j / 3, j % 3, cvmGet( mat, j / 3, j % 3 ) - incr );
            int i, j;
            for ( i = 0; i < 3; i++ )
                for ( j = 0; j < 3; j++ ) cout << cvmGet( mat, i, j ) << "\t";
            cout << "\n";
        }
        if ( i == 105 || i == 1048681 ) {
            cvmSet( mat, ( 8 ) / 3, ( 8 ) % 3, cvmGet( mat, ( 8 ) / 3, ( 8 ) % 3 ) - 0.1 );
            // cvmSet( mat, ( 4 ) / 3, ( 4 ) % 3, cvmGet( mat, ( 4 ) / 3, ( 4 ) % 3 ) + 0.1 );
        }
        if ( i == 111 || i == 1048687 ) {
            cvmSet( mat, ( 8 ) / 3, ( 8 ) % 3, cvmGet( mat, ( 8 ) / 3, ( 8 ) % 3 ) + 0.1 );
            // cvmSet( mat, ( 4 ) / 3, ( 4 ) % 3, cvmGet( mat, ( 4 ) / 3, ( 4 ) % 3 ) - 0.1 );
        }
        cvWarpPerspective( image , dstimage, mat );

        cvNamedWindow( "perspective" );
        cvShowImage( "perspective", dstimage );
        // if ( i != -1 ) {
        //     cout << i << "\n";
        //     cout << dstimage->height << "\t" << dstimage->width << "\n";
        // }
    }

    // to do b. zoom in/out c.rotate image

    cvReleaseImage( &image );
    cvDestroyWindow( "perspective" );
    
    return 0;
}