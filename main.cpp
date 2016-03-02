#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

void main()
{
    FILE *fpw = fopen( "E:\\patch.bin", "wb" );
    if ( fpw == NULL )
    {
        cout << "Open error!" << endl;
        fclose(fpw);
        return;
    }

    Mat image = imread("E:\\lena32.jpg");
    if ( !image.data || image.channels() != 3 )
    {
        cout << "Image read failed or image channels isn't equal to 3."
            << endl;
        return;
    }

    // write image to binary format file
    int labelw = 1;
    int rows = image.rows;
    int cols = image.cols;

    fwrite( &labelw, sizeof(char), 1, fpw );

    char* dp = (char*)image.data;
    for ( int i=0; i<rows*cols; i++ )
    {
        fwrite( &dp[i*3],   sizeof(char), 1, fpw );
        fwrite( &dp[i*3+1], sizeof(char), 1, fpw );
        fwrite( &dp[i*3+2], sizeof(char), 1, fpw );
    }
    fclose(fpw);

    // read image from binary format file
    FILE *fpr = fopen( "E:\\patch.bin", "rb" );
    if ( fpr == NULL )
    {
        cout << "Open error!" << endl;
        fclose(fpr);
        return;
    }

    int labelr(0);
    fread( &labelr, sizeof(char), 1, fpr );

    cout << "label: " << labelr << endl;

    Mat image2( rows, cols, CV_8UC3, Scalar::all(0) );

    char* pData = (char*)image2.data;
    for ( int i=0; i<rows*cols; i++ )
    {
        fread( &pData[i*3],   sizeof(char), 1, fpr );
        fread( &pData[i*3+1], sizeof(char), 1, fpr );
        fread( &pData[i*3+2], sizeof(char), 1, fpr );
    }
    fclose(fpr);

    imshow("1", image2);
    waitKey(0); 
}
