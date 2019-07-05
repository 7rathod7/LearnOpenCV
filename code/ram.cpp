#include <iostream>
#include <sys/sysinfo.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

/*!
    int sysinfo( struct sysinfo *info );
*/

/*!
struct sysinfo {
    long upTime;    // seconds since boot
    unsigned long loads[3]; // 1, 5, and 15 minute load averages
    unsigned long totalram; // Total usable main memory size
    unsigned long freeram;  // Available memory size
    unsigned long sharedram;    // Amount of shared memory
    unsigned long bufferram;    // Memory used by buffers
    unsigned long totalswap;    // total swap space size
    unsigned long freeswap; // swap space still available
    unsigned short procs;    // number of current processes
    unsigned long totalhigh;    // total high memory size
    unsigned long freehigh; // Available high memory size
    unsigned int memunit;   // Memory unit size in bytes
    char _f[20 - 2 * sizeof( long ) - sizeof( int ) ];  // Padding for libc5
};
*/

using namespace std;

int main( int argc, char **argv ) {

    IplImage *window = cvCreateImage( CvSize( 200, 100 ), IPL_DEPTH_8U, 3 );

    if( window == NULL ) {
        fprintf( stderr, "Error : failed to create image\n");
        exit( 1 );
    }

    cvNamedWindow( "ram", CV_WINDOW_AUTOSIZE );

    struct sysinfo sys_info;
    if( sysinfo( &sys_info ) != 0 ) perror("sysinfo");

    /*! Total and free ram */
    double totalRam;
    double freeRam;

    CvFont a;
    cvInitFont( &a, CV_FONT_HERSHEY_SIMPLEX, 1, 1);

    char input_text[100]="";

    int t = 0;
    while( t != 27 ) {
        cvZero( window );
        struct sysinfo sys_info;
        if( sysinfo( &sys_info ) != 0 ) perror("sysinfo");

        totalRam = sys_info.totalram / ( 1024.0 * 1024.0 * 1024.0 );
        freeRam = sys_info.  freeram / ( 1024.0 * 1024.0 * 1024.0 );
        printf( "Total Ram: %lfG\tFree: %lfG\n", totalRam, totalRam - freeRam );
        //bzero( input_text, 100 * sizeof( char ) );
        //memcpy( input_text, &freeRam, sizeof( freeRam ) );
        sprintf(input_text, "%lf", freeRam);

        cvPutText( window, input_text, cvPoint( 0, 30 ), &a , cvScalar( 0, 0, 255 ) );

        cvShowImage( "ram", window );
        t = cvWaitKey( 15 );
    }

    cvReleaseImage( &window );
    cvDestroyWindow( "ram" );

    return 0;
}
