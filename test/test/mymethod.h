#ifndef __MYMETHOD_HPP__
#define __MYMETHOD_HPP__

#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

class MyMethod
{
public:
	//-----------------------------------【expandLine( )函数】----------------------------------  
	//      描述：将二值化图像的白色部分进行扩大
	//---------------------------------------------------------------------------------------------- 
	static Mat expandLine(Mat image)
	{
		Mat expImage = Mat::zeros( image.rows, image.cols, image.type() );//初始化一张与image同类型的空白（全黑）图像
		int sum=0;//统计目标方格（如2×2）内是否有白色点

		for (int i=1;i<image.rows-1;i++)
		{
			for (int j=1;j<image.cols-1;j++,sum=0)//遍历图像中的点
			{
				if (image.at<uchar>(i,j)==255)//如果点(i,j)为白色，则将(i+1,j)与(i,j+1)也涂成白色
				{
					expImage.at<uchar>(i+1,j)=255;
					expImage.at<uchar>(i,j+1)=255;
				}
			}

		}
		return expImage;
	}
};
#endif