在深度学习时，制作样本数据集时，需要产生和读取一些二进制图像的数据集，如[MNIST](http://yann.lecun.com/exdb/mnist/)，[CIFAR-10](http://www.cs.toronto.edu/~kriz/cifar.html)等都提供了适合C语言的二进制版本。

以CIFAR-10的数据集为例，官网上有两段关键的介绍：

二进制版本数据集格式为（图像大小为`32x32`）：

```
<1 x label><3072 x pixel>
...
<1 x label><3072 x pixel>
```
> In other words, the first byte is the label of the first image, which is a number in the range 0-9. The next 3072 bytes are the values of the pixels of the image. The first 1024 bytes are the red channel values, the next 1024 the green, and the final 1024 the blue. The values are stored in row-major order, so the first 32 bytes are the red channel values of the first row of the image. 

由此，绘制一个简图：

![memory](http://img.blog.csdn.net/20160302203858639)

根据图像大小`32x32 = 1024`，不难知道，每个颜色值存储为`1 byte`，因此，对于单个图像的二进制存储与读取（先不管RGB颜色存储顺序），找了一张`32x32`的彩色lena图像，如下实现：

```c
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
```

运行结果如下：

  ![results](http://img.blog.csdn.net/20160302205757017)

再看图片属性：

![attribute](http://img.blog.csdn.net/20160302210020863)

与官网上的大小`3073`一致，那么这么存取应该没问题。

严格按照官网的RGB通道分别存储，略作修改就可以实现：

```c
/*	for ( int i=0; i<rows*cols; i++ )
    {
		fwrite(&dp[i*3],   sizeof(char), 1, fpw);
		fwrite(&dp[i*3+1], sizeof(char), 1, fpw);
		fwrite(&dp[i*3+2], sizeof(char), 1, fpw);
	}
*/

    for ( int i=0; i<rows*cols; i++ )
		fwrite(&dp[i*3+2],   sizeof(char), 1, fpw); // R
	
    for ( int i=0; i<rows*cols; i++ )
		fwrite(&dp[i*3+1],   sizeof(char), 1, fpw); // G
	
	for ( int i=0; i<rows*cols; i++ )
		fwrite(&dp[i*3],   sizeof(char), 1, fpw);  // B
```

存储和读取多张图片方法类似，这里就不做介绍。
