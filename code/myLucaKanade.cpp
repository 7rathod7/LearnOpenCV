#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <iostream>

using namespace std;

#define MAX_CORNERS 500
#define WINDOW_SIZE 10

IplImage *query_frame(CvCapture *video, IplImage **frame_buffer, IplImage *current_frame) {
  IplImage *frame = cvQueryFrame(video);

  if (frame) {
    cvConvertImage(frame, current_frame, 0);

    IplImage *temp = frame_buffer[0];
    frame_buffer[0] = frame_buffer[1];
    frame_buffer[1] = temp;
    cvConvertImage(frame, frame_buffer[0], 0);
  }

  return frame;
}

CvPoint2D32f point;
bool addRemovePt = false;

void my_mouse_callback( int event, int x, int y, int flags, void* param ) {
    IplImage *image = (IplImage*) param;
    switch ( event ) {
        case CV_EVENT_MOUSEMOVE:
            break;
        
        case CV_EVENT_LBUTTONDOWN:
            point = cvPoint2D32f( ( float ) x, ( float ) y );
            addRemovePt = true;
            break;
        
        case CV_EVENT_LBUTTONUP:
            break;
        
        default:
            break;
    }
}

int main( int argc, char **argv ) {
    CvCapture *capture = cvCreateCameraCapture( 0 );
    if ( capture == NULL ) {
        fprintf( stderr, "Error : couldnt create camera capture\n");
        exit(-1 );
    }

      // Extract video parameters
    CvSize video_frame_size;
    video_frame_size.width = cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH);
    video_frame_size.height = cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT);

    // Initialize variables for optical flow calculation
    IplImage *current_frame = cvCreateImage(video_frame_size, IPL_DEPTH_8U, 3);
    IplImage *eigen_image = cvCreateImage(video_frame_size, IPL_DEPTH_32F, 1);
    IplImage *temp_image = cvCreateImage(video_frame_size, IPL_DEPTH_32F, 1);

    int corner_count = MAX_CORNERS;
    CvPoint2D32f corners[2][MAX_CORNERS];
    char features_found[MAX_CORNERS];
    float feature_errors[MAX_CORNERS];

    IplImage *frame_buffer[2];
    IplImage *pyramid_images[2];
    CvSize pyramid_size = cvSize(video_frame_size.width + 8, video_frame_size.height / 3);

    int i;
    for (i = 0; i < 2; i++) {
        frame_buffer[i] = cvCreateImage(video_frame_size, IPL_DEPTH_8U, 1);
        pyramid_images[i] = cvCreateImage(pyramid_size, IPL_DEPTH_32F, 1);
    }

    cvNamedWindow( "Lucas Kanade", CV_WINDOW_AUTOSIZE );
    bool needToInit = false;
    bool nightMode = false;
    cvSetMouseCallback(
        "Lucas Kanade",
        my_mouse_callback,
        NULL
    );

    // Process video
    while (query_frame( capture, frame_buffer, current_frame)) {

        // Corner finding with Shi and Thomasi
        cvGoodFeaturesToTrack( frame_buffer[0], eigen_image, temp_image, corners[0],
        &corner_count, 0.01, 5.0, 0, 3, 0, 0.4);

        cvFindCornerSubPix( frame_buffer[0], corners[0], corner_count, cvSize(WINDOW_SIZE, WINDOW_SIZE), cvSize(-1, -1),
         cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3 ) );

        // Pyramid Lucas-Kanade
        cvCalcOpticalFlowPyrLK( frame_buffer[0], frame_buffer[1], pyramid_images[0], pyramid_images[1],
        corners[0], corners[1], corner_count, cvSize( WINDOW_SIZE, WINDOW_SIZE ), 5, features_found,
        feature_errors, cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3 ), 0);

        // Draw optical flow vectors
        int i;
        for (i = 0; i < corner_count; i++) {
            if (features_found[i] == 0 || feature_errors[i] > 550) {
                continue;
            }

            CvPoint points[2];
            points[0] = cvPoint(cvRound(corners[0][i].x), cvRound(corners[0][i].y));
            points[1] = cvPoint(cvRound(corners[1][i].x), cvRound(corners[1][i].y));

            cvLine( current_frame, points[0], points[1], CV_RGB(255, 0, 0), 1, 8, 0 );
        }

        needToInit = false;
        cvShowImage( "Lucas Kanade", current_frame );
        char c = (char)cvWaitKey(10);
        if( c == 27 ) break;
        // switch( c ) {
        //     case 'r':
        //         needToInit = true;
        //         break;
        //     case 'c':
        //         // points[0].clear();
        //         // points[1].clear();
        //         break;
        //     case 'n':
        //         nightMode = !nightMode;
        //         break;
        // }
    }

    // Clean up
    cvReleaseImage(&current_frame);
    cvReleaseImage(&eigen_image);
    cvReleaseImage(&temp_image);

    for (i = 0; i < 2; i++) {
        cvReleaseImage(&frame_buffer[0]);
        cvReleaseImage(&pyramid_images[0]);
    }
    cvDestroyWindow( "Lucas Kanade" );
    cvReleaseCapture( &capture );
    return 0;
}