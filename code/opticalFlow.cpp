//! Optical Flow Demo Program
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <math.h>

#define numPoints 400

static const double pi = 3.14159265358979323846;

inline static double square( int a ) {
    return a * a;
}

/*!
    This is just an inline that allocates images. I did this to reduce clutter in the
    actual computer vision algorithmic code. basically it allocates the requested image
    unless that image is already non-NULL. It always leaves a non-NULL image as-is even
    if that image's size, depth, and/or channels are different than the request.
 */
inline static void allocateOnDemand( IplImage **img, CvSize size, int depth, int channels ) {
    if ( *img != NULL ) return;
    *img = cvCreateImage( size, depth, channels );
    if ( *img == NULL ) {
        fprintf( stderr, "Error : Couldn't allocates image.Out of memory\n");
        exit(-1);
    }
}

int main( int argc, char **argv ) {
    CvCapture *input_video = cvCaptureFromFile( argv[1] );
    if ( input_video == NULL ) {
        fprintf( stderr, "Error : Couldn't open Video\n");
        exit(-1);
    }
    // cvQueryFrame( input_video );
    CvSize frameSize;
    frameSize.height = (int) cvGetCaptureProperty( input_video, CV_CAP_PROP_FRAME_HEIGHT);
    frameSize.width = (int) cvGetCaptureProperty( input_video, CV_CAP_PROP_FRAME_WIDTH);
    
    //! determine the number of frames in the avi
    long numberOfFrames;
    cvSetCaptureProperty( input_video, CV_CAP_PROP_POS_AVI_RATIO, 1.);
    //! Now that we are at the end, read avi position in frames
    numberOfFrames = (int) cvGetCaptureProperty( input_video, CV_CAP_PROP_POS_FRAMES );
    //! return to the beginning
    cvSetCaptureProperty( input_video, CV_CAP_PROP_POS_FRAMES, 0.);

    /*!
        Create three windows called frames N, frames N+1 and optical flow
        for visualizing the output, have those windows automatically change their
        size to match the output
     */
    cvNamedWindow("Optical Flow", CV_WINDOW_AUTOSIZE );
    
    long currentFrame = 0;
    while( 1 ) {
        static IplImage *frame = NULL, *frame1 = NULL, *frame1_1C = NULL, *frame2_1C = NULL,
            *eig_image = NULL, *temp_image = NULL, *pyramid1 = NULL, *pyramid2 = NULL;
        
        /*!
            Go to the frame we want. Important if multiple frames are queried in 
            the loop which they of course are for optical flow. Note that the very
            first call to this is actually not needed. (Because the correct position
            is set outside the for() loop.)
         */
        cvSetCaptureProperty( input_video, CV_CAP_PROP_POS_FRAMES, currentFrame );

        /*!
            get the next frame of the video
            Important! cvqueryframe() always returns pointer to the same memory location.
            So successive calls:
            frame1 = cvQueryFrame();
            frame2 = cvQueryFrame();
            frame3 = cvQueryFrame();
            Will result in (frame1 == frame2 && frame2 == frame3) being true
            the solution is to make a copy of the cvqueryframe() output
         */
        frame = cvQueryFrame( input_video );
        if ( frame == NULL ) {
            //! Why did we get a NULL frame ? We shouldnt be at the end
            fprintf( stderr, "Error : Hmm, the end came sooner than we thought.\n" );
            return -1;
        }
        /*!
            Allocate another image if not already allocated.
            Image has One challenge of color (ie monochrome) with 8-bit color depth
            This is the image format opencv algorithms actually operate on (mostly)
         */
        allocateOnDemand( &frame1_1C, frameSize, IPL_DEPTH_8U, 1 );
        /*!
            convert whatever the avi image format is into Opencv preferred  format.
            And flip the image vertically. Flip is a shameless hack. Opencv reads
            in avis upside-down by default.
         */
        cvConvertImage( frame, frame1_1C, CV_CVTIMG_FLIP );

        /*!
            We'll make a full color backup of this frame so that we can draw on it
            It's not the best idea to draw on the static memory space of cvqueryframe()
         */
        allocateOnDemand( &frame1, frameSize, IPL_DEPTH_8U, 3 );
        cvConvertImage( frame, frame1, CV_CVTIMG_FLIP );

        /*!
            get the second frame of video Sample principles as the first
         */
        frame = cvQueryFrame( input_video );
        if ( frame == NULL ) {
            fprintf( stderr, "Error : Hmm. the end came sooner than we thought.\n" );
            return -1;
        }
        allocateOnDemand( &frame2_1C, frameSize, IPL_DEPTH_8U, 1 );
        cvConvertImage( frame, frame2_1C, CV_CVTIMG_FLIP );

        /*!
            preparation : allocate the necessary storage
         */
        allocateOnDemand( &eig_image, frameSize, IPL_DEPTH_32F, 1 );
        allocateOnDemand( &temp_image, frameSize, IPL_DEPTH_32F, 1 );

        /*!
            preparation : this array will contain the features found in frame 1
         */
        CvPoint2D32f frame1_features[numPoints];

        /*!
            preparation : before the function call this variable is the array size
            ( or the maximum number of features to find ). After the function call
            this variable is the number of features actually found
         */
        int numberOfFeatures;
        /*!
            I'm hard coding this at 400. But you should make this a #deifne so that you can change
            the number of features you use for an accuracy/speed tradeoff analysis.
         */ 
        numberOfFeatures = numPoints;

        /*!
            Actually run the SHi and Tomasi algorithm!!
            frame1_1c is the input image
            eig_image and temp_image are the workspace for the algorithm
            the first "0.01" specifies the minimum quality of the features(based on eigen values)
            the second "0.01" specifies the minimum Euclidean distance between features.
            "NULL" means use the entire input image. you could point to a part of the image
            When the Algorithms returns:
            frame1_features will contain the feature points.
            numberOfFeatures will be set to a value <= numPoints indicating the number of feature points
         */
        cvGoodFeaturesToTrack( frame1_1C, eig_image, temp_image, frame1_features, &numberOfFeatures, 0.01, 0.01, NULL );

        /*! Pyramidal Lucas Kanade Optical Flow! */

        /*!
            This array will contain the locations of the point from frame 1 in frame 2
         */
        CvPoint2D32f frame2_features[numPoints];

        /*!
            The ith element of this array will be non-zero if and only if the ith feature
            of frame 1 was found in frame 2
         */
        char opticalFlowFoundFeature[numPoints];

        /*!
            The ith element of this array is the error in the optical flow or the ith
            feature of frame1 as found in frame2. if the ith feature was not found (see the
            array above) I think the ith entry in this array is undefined.
         */
        float opticalFlowFeatureError[numPoints];

        /*!
            This is the window size to use to avoid the aperture problem
         */
        CvSize opticalFlowWindow = cvSize( 3, 3 );

        /*!
            This termination criteria tells the algorithm to stop when it has either 
            done 20 iteration or when epsilon is better than .3. You can play with
            these paramaters for speed vs. accuracy but these values work pretty well
            in many situations.
         */
        CvTermCriteria opticalFlowTerminationCriteria = cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3 );

        /*!
            This is some workspace for the algorithm
            The algorithm actually carves the image into pyramids of different resolutions
         */
        allocateOnDemand( &pyramid1, frameSize, IPL_DEPTH_8U, 1 );
        allocateOnDemand( &pyramid2, frameSize, IPL_DEPTH_8U, 1 );

        /*!
            Actually run Pyramidal Lucas Kanade Optical Flow!!
            frame1_1c is the first frame with the known features
            frame2_1c is the second frame where we want to find the first frame's features
            pyramid1 and pyramid2 are workspace for the algorithm
            frame1_features are the features from the first frame
            frame2_features is the (outputted) locations of those features in the second frame
            numberOfFeatures is the number of features in the frame1_features array.
            opticalFlowWindow is the size of the window to use to avoid the aperture problem
            5 is the maximum number of pyramids to use. 0 would be just one level
            opticalFlowFoundFeature is as described above ( non-zero iff feature found by the flow )
            opticalFlowFeatureError is as described above ( error in the flow for this feature )
            opticalFlowTerminationCriteria is as described above ( how long the algorithm should look )
            0 means disable enhancements. ( For example, the second array isnt preinitialized with guesses )
         */
        cvCalcOpticalFlowPyrLK( frame1_1C, frame2_1C, pyramid1, pyramid2, frame1_features,
        frame2_features, numberOfFeatures, opticalFlowWindow, 5, opticalFlowFoundFeature, opticalFlowFeatureError,
        opticalFlowTerminationCriteria, 0 );

        /*! for fun ( and debugging :), let's draw the flow field */
        for( int i = 0; i < numberOfFeatures; i++ ) {
            /*! Pyramidal Lucas Kanade didnt really find the feature, skip it */
            if ( opticalFlowFoundFeature[i] == 0 ) continue;

            int lineThickness = 1;
            /*! cv_rgb is the red , green , blue component */
            CvScalar lineColor = CV_RGB( 255, 0, 0 );

            /*! Lets make the flow field look nice with arrows */
            /*!
                the arrows will be a bit too short for a nice visualization because of the high framerate
                (ie there's not much motion between frames). so lets lengthen them by factor of 3
             */
            CvPoint p,q;
            p.x = (int) frame1_features[i].x;
            p.y = (int) frame1_features[i].y;
            q.x = (int) frame2_features[i].x;
            q.y = (int) frame2_features[i].y;

            double angle = atan2( (double) p.y-q.y, (double) p.x-q.x );
            double hypotenuse = sqrt( square(p.y-q.y) + square(p.x-q.x) );

            /*!Here we lengthen the arrow by a factor of three */
            q.x = (int) (p.x - 3 * hypotenuse * cos( angle ));
            q.y = (int) (p.y - 3 * hypotenuse * sin( angle ));

            /*! 
                Now we draw the main line of the arrow
                frame1 is the frame to draw on
                p is point where the line begins
                q is point where the line stops
                cv_AA  means antialiasing drawing
                0 means no fractional bits in the center coordinates or radius 
            */
           cvLine( frame1, p, q, lineColor, lineThickness, CV_AA, 0 );
           /*!
            Now draw the tips of the arrow. i do some scaling so that the tips
            look proportional to main line of the arrow
            */
           p.x = (int) (q.x + 9 * cos(angle + pi / 4 ));
           p.y = (int) (q.y + 9 * sin(angle + pi / 4 ));
           cvLine( frame1, p, q, lineColor, lineThickness, CV_AA, 0 );
           p.x = (int) (q.x + 9 * cos(angle - pi / 4 ));
           p.y = (int) (q.y + 9 * sin(angle - pi / 4 ));
           cvLine( frame1, p, q, lineColor, lineThickness, CV_AA, 0 );
        }
        /*!
            Now display the image we drew on. Recall that optical flow is the name of
            window we created above
         */
        cvConvertImage(frame1, frame1, CV_CVTIMG_FLIP);
        cvShowImage( "Optical Flow", frame1 );
        /*!
            And wait for the user to press a key (so the user has time to look at the image)
            if the argument is 0 then it waits forever otherwise it waits that number of milliseconds
            The return value is the key the user pressed
         */
        int keyPressed = cvWaitKey( 15 );
        /*!
            If the user pushes "b" "B" go back one frame
            otherwise go forward one frame
         */
        if ( keyPressed == 'b' || keyPressed == 'B' ) currentFrame--;
        else if ( keyPressed == 27 ) break;
        else currentFrame++;
        /*!
            Dont run past the front/end of the avi
         */
        if ( currentFrame < 0 ) currentFrame = 0;
        if ( currentFrame >= numberOfFrames - 1 ) currentFrame = numberOfFrames - 2;
    }

    cvDestroyWindow( "Optical Flow");
    
    return 0;
}