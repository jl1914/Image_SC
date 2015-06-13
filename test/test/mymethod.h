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
	//-----------------------------------��expandLine( )������----------------------------------  
	//      ����������ֵ��ͼ��İ�ɫ���ֽ�������
	//---------------------------------------------------------------------------------------------- 
	static Mat expandLine(Mat image)
	{
		Mat expImage = Mat::zeros( image.rows, image.cols, image.type() );//��ʼ��һ����imageͬ���͵Ŀհף�ȫ�ڣ�ͼ��
		int sum=0;//ͳ��Ŀ�귽����2��2�����Ƿ��а�ɫ��

		for (int i=1;i<image.rows-1;i++)
		{
			for (int j=1;j<image.cols-1;j++,sum=0)//����ͼ���еĵ�
			{
				if (image.at<uchar>(i,j)==255)//�����(i,j)Ϊ��ɫ����(i+1,j)��(i,j+1)ҲͿ�ɰ�ɫ
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