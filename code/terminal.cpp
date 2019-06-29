/*
 * headers
 */
#include <iostream>
#include "fontcolor.h"

using namespace std;

/*
 * main function
 */
int main( int argc, char **argv ) {
    cout << "\033[6m" << "12hello World\n" << RESET;
    return 0;
}