//! Transparent Overlay
//! http://www.aishack.in/tutorials/transparent-image-overlays-opencv/
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <iostream>

using namespace std;


// A function to draw the histogram
IplImage* DrawHistogram(CvHistogram *hist, float scaleX=1, float scaleY=1)
{
    // Find the maximum value of the histogram to scale
    // other values accordingly
    float histMax = 0;

    cvGetMinMaxHistValue(hist, 0, &histMax, 0, 0);

    // Create a new blank image based on scaleX and scaleY
    IplImage* imgHist = cvCreateImage(cvSize(256*scaleX, 64*scaleY), 8 ,1);

    cvZero(imgHist);

    // Go through each bin
    for(int i=0;i<255;i++)
    {
        float histValue = cvGetReal1D(hist->bins, i);        // Get value for the current bin...

        float nextValue = cvGetReal1D(hist->bins, i+1);    // ... and the next bin

        // Calculate the four points of the polygon that these two
        // bins enclose
        CvPoint pt1 = cvPoint(i*scaleX, 64*scaleY);
        CvPoint pt2 = cvPoint(i*scaleX+scaleX, 64*scaleY);
        CvPoint pt3 = cvPoint(i*scaleX+scaleX, (64-nextValue*64/histMax)*scaleY);

        CvPoint pt4 = cvPoint(i*scaleX, (64-histValue*64/histMax)*scaleY);

        // A close convex polygon
        int numPts = 5;

        CvPoint pts[] = {pt1, pt2, pt3, pt4, pt1};

        // Draw it to the image
        cvFillConvexPoly(imgHist, pts, numPts, cvScalar(255));

    }

    // Return it... make sure you delete it once you're done!
    return imgHist;
}

void OverlayImage(IplImage* src, IplImage* overlay, CvPoint location, CvScalar S, CvScalar D) {
    for( int x = 0; x < overlay->width; x++ ) {
        if( x + location.x >= src->width ) continue;
        for( int y = 0; y < overlay->height; y++ ) {
            if( y + location.y >= src->height ) continue;
            CvScalar source = cvGet2D(src, y+location.y, x+location.x);
            CvScalar over = cvGet2D(overlay, y, x);
            CvScalar merged;
            for( int i = 0; i < 4; i++ )
                merged.val[i] = (S.val[i]*source.val[i]+D.val[i]*over.val[i]);
            cvSet2D(src, y+location.y, x+location.x, merged);
        }
    }
}

//! main function
int main( int argc, char **argv ) {

    CvCapture *capture = cvCaptureFromCAM( -1 );    // camera instance
    if ( !capture ) {
        cout << "\n";
        return -1;
    }

    IplImage *image;
    cvNamedWindow( "window" );

    // Variables to be used for the histgoram
    int numBins = 256;
    float range[] = {0, 255};

    float *ranges[] = { range };

    // Initialize a histogram
    CvHistogram *hist = cvCreateHist( 1, &numBins, CV_HIST_ARRAY, ranges, 1 );
    cvClearHist( hist );

    IplImage* imgBlack = cvCreateImage( cvSize( 128, 32 ), IPL_DEPTH_8U, 1 );
    cvZero( imgBlack );

    IplImage* imgRedHist = cvCreateImage( cvSize( 128, 32 ), 8, 3 );
    IplImage* imgGreenHist = cvCreateImage( cvSize( 128, 32 ), 8, 3 );
    IplImage* imgBlueHist = cvCreateImage( cvSize( 128, 32 ), 8, 3 );
    // uchar *yourBuffer;

    while( 1 ) {
        image = cvQueryFrame( capture );
        IplImage* imgRed = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 1 );
        IplImage* imgGreen = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 1 );
        IplImage* imgBlue = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 1 );
        // yourBuffer = (uchar*)image->imageData;
        // cvConvertImage(image, image, CV_CVTIMG_FLIP);
        cvSplit(image, imgBlue, imgGreen, imgRed, NULL);
        cvCalcHist(&imgRed, hist, 0, 0);              // Calculate the histogram for red
        // cout << "yo0\n";
        IplImage* imgHistRedOnly = DrawHistogram(hist, 0.5, 0.5);    // Draw it
        // // cout << "yo1";
        cvClearHist(hist);                            // Clear the histgoram

        cvCalcHist(&imgGreen, hist, 0, 0);            // Reuse it to calculate histogram for green
        IplImage* imgHistGreenOnly = DrawHistogram(hist, 0.5, 0.5);

        cvClearHist(hist);

        cvCalcHist(&imgBlue, hist, 0, 0);            // And again for blue
        IplImage* imgHistBlueOnly = DrawHistogram(hist, 0.5, 0.5);
        cvClearHist(hist);
        cvZero(imgRedHist);
        cvZero(imgGreenHist);
        cvZero(imgBlueHist);
        cvMerge(imgHistBlueOnly, imgBlack, imgBlack, NULL, imgBlueHist);
        cvMerge(imgBlack, imgHistGreenOnly, imgBlack, NULL, imgGreenHist);
        cvMerge(imgBlack, imgBlack, imgHistRedOnly, NULL, imgRedHist);
        OverlayImage(image, imgRedHist, cvPoint(485, 24), cvScalar(0.5,0.5,0.5,0.5), cvScalar(0.5,0.5,0.5,0.5));
        OverlayImage(image, imgGreenHist, cvPoint(485, 76), cvScalar(0.5,0.5,0.5,0.5), cvScalar(0.5,0.5,0.5,0.5));
        OverlayImage(image, imgBlueHist, cvPoint(485, 128), cvScalar(0.5,0.5,0.5,0.5), cvScalar(0.5,0.5,0.5,0.5));
        char c = cvWaitKey( 15 );
        if ( c == 27 ) break;
        cvShowImage( "window", image );
        cvReleaseImage( &imgRed );
        cvReleaseImage( &imgBlue );
        cvReleaseImage( &imgGreen );
        cvReleaseImage( &imgHistRedOnly );
        cvReleaseImage( &imgHistBlueOnly );
        cvReleaseImage( &imgHistGreenOnly );
    }

    cvReleaseImage( &image );
    cvReleaseImage( &imgBlack );
    cvReleaseImage( &imgRedHist );
    cvReleaseImage( &imgBlueHist );
    cvReleaseImage( &imgGreenHist );
    cvReleaseImage( &image );
    cvDestroyWindow( "window" );

    return 0;
}