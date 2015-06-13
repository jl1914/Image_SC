#ifndef __ZDCLASS_HPP__
#define __ZDCLASS_HPP__

#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

//-----------------------------------°æhorizon( ) class°ø----------------------------------  
//      description: horizontal integration to get the spacing.
//-----------------------------------------------------------------------------------------

class ylocation{
public:

	ylocation(Mat Image)//≥ı ºªØ
	{
		src = Image;
	}

	//Mat horizon(Mat &src1)
	vector<int> horizon()
	{
		//cvtColor(src, src, CV_BGR2GRAY); //thresholding to get the binary image.
		//Mat painty(src.rows, src.cols, CV_8U, Scalar(0,0,0));
		int* v = new int[src.cols];
		int* h = new int[src.rows];
		memset(h, 0, src.rows * 4);
		int x, y;
		uchar * data;
		//CvScalar s, t;   //array with four double numbers. 

		for (y = 0; y<src.rows; y++)  
		{
			data = src.ptr<uchar>(y);
			for (x = 0; x<src.cols; x++)
			{
				if (data[x] == 0)   //larger h means higher possibility of the existence of the words.
					h[y]++;
			}
		}

		for (y = 0; y<src.rows; y++)
		{
			data = painty.ptr<uchar>(y);
			for (x = 0; x<h[y]; x++)
			{
				data[x] = 255;  //paint the integration image.
			}
		}

		int flag0 = 0;
		int m = 0;
		int count = 0;

		for (y = 0; y < src.rows; y++){  //to get the position of each line.
			if (h[y] !=0){
				flag0 = 1;  //it means that the first line occurs.
			}
			if (flag0 == 1 && h[y] ==0){      //achieve the spacing.
				for (int m = y; m<src.rows; m++){
					if (h[m] !=0){   //achieve the next line.
						line(src, Point(0, (y + m) / 2), Point(50, (y + m) / 2), Scalar(0, 0, 0)); 
						yPosition.push_back((y+m)/2);
						flag0 = 0;
						break;
					}
				}
			}
			//line((Mat)(painty), Point(0, y), Point(50, y), Scalar(0, 0, 0));  //threshold changes with the peak
		}
		//return painty;
		//return src;
		for (int i = 0; i<yPosition.size(); i++)
			cout << yPosition[i] << "	";
		return yPosition;
	}
public:
	vector<int> yPosition;	//horizontal position of text
	Mat src;
};
#endif