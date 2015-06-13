#ifndef __MYCLASS_HPP__
#define __MYCLASS_HPP__

#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>


using namespace cv;
using namespace std;
//-----------------------------------��Maxline�ࡿ---------------------------------------------  
//      ������Ѱ�������߶��е���߶�  
//---------------------------------------------------------------------------------------------- 

class MaxLine
{
public:
	MaxLine(vector<Vec4i> lines)//��ʼ�����������߶μ���
	{
		maxLiN = -1;//��߶α��
		maxL = -1;//��߶γ���
		this->lines = lines;//������߶μ���

		for( size_t i = 0; i < lines.size(); i++ )//Ѱ����߶�
		{
			Vec4i l = lines[i];
			double thisLine = sqrt(pow((double)(l[0]-l[2]),2)+pow((double)(l[1]-l[3]),2));
			if (thisLine > maxL)
			{
				maxL = thisLine;
				maxLiN = i;
				maxSeg = l;
			}
		}
	}
	MaxLine(vector<Vec4i> lines,Mat image)//��ʼ���������߶μ�����ͼ��
	{
		maxLiN = -1;//��߶α��
		maxL = -1;//��߶γ���
		this->lines = lines;//������߶μ���
		this->image = image;//��ʼ��ͼ��

		for( size_t i = 0; i < lines.size(); i++ )//Ѱ����߶�
		{
			Vec4i l = lines[i];
			double thisLine = sqrt(pow((double)(l[0]-l[2]),2)+pow((double)(l[1]-l[3]),2));
			if (thisLine > maxL)
			{
				maxL = thisLine;
				maxLiN = i;
				maxSeg = l;
			}
		}
	}
	int maxLineNumber()
	{
		if (maxLiN != -1)
		{
			return maxLiN;
		} 
		else
		{
			cout<<"û���ҵ�����"<<endl;getchar(); return -1;
		}
	}
	int maxLength()
	{
		if (maxL != -1)
		{
			return maxL;
		} 
		else
		{
			cout<<"û���ҵ�����"<<endl;getchar(); return -1;
		}
	}
	Vec4i maxSegment()
	{
		if (maxLiN != -1)
		{
			return maxSeg;
		} 
		else
		{
			cout<<"û���ҵ�����"<<endl;getchar(); return -1;
		}
	}
	void drawOnImage()
	{
		if (!image.empty())
		{
			line( image, Point(maxSeg[0], maxSeg[1]), Point(maxSeg[2], maxSeg[3]), Scalar(0,0,255), 2, CV_AA);
		} 
		else
		{
			cout<<"��������ͼ��"<<endl;getchar();
		}

	}
private:
	int maxLiN;//��߶α��
	double maxL;//��߶γ���
	Vec4i maxSeg;//��߶�����
	Mat image;//�����ͼ��
	vector<Vec4i> lines;//������߶μ���
};

//-----------------------------------��TransImage�ࡿ------------------------------------------  
//      ��������ͼƬ��תָ���Ƕȶ����
//---------------------------------------------------------------------------------------------- 
class TransImage
{
public:
	Mat inputImage;//����ͼ��
	Vec4i inputLine;//������߶�
	Mat outputImage;//���ͼ��
	int outpulLineCol;//�任����߶�������
	double angle;//��ת�Ƕ�

	TransImage(Mat inputImage, Vec4i inputLine)//��ʼ����
	{
		this->inputImage = inputImage;
		this->inputLine = inputLine;
	}

	Mat trans1()//��ת��ʽ1
	{
		Mat rotMat( 2, 3, CV_32FC1 );//��ʼ����ת����

		//��ʼ����ת�Ƕ�
		angle = -atan((double)(inputLine[2]-inputLine[0])/(double)(inputLine[3]-inputLine[1]))/CV_PI*180;

		// ��ʼ��Ŀ��ͼ��Ĵ�С
		outputImage = Mat::zeros( inputImage.rows, inputImage.cols, inputImage.type() );

		//��6����ͼ�������ת
		int x = (inputLine[2]-inputLine[0])*(inputImage.rows/2-inputLine[1])/(inputLine[3]-inputLine[1])+inputLine[0];
		int y = inputImage.rows/2;
		Point center = Point( x, y );
		outpulLineCol = x;
		double scale = 1;
		// ͨ���������תϸ����Ϣ�����ת����
		rotMat = getRotationMatrix2D( center, angle, scale );
		// ��ת�����ź��ͼ��
		warpAffine( inputImage, outputImage, rotMat, outputImage.size() ,1,0,Scalar(255,255,255));

		return outputImage;
	}
};

/*
	//-----------------------------------��transImage( )������----------------------------------  
	//      ��������ͼƬ��תָ���Ƕȶ����
	//---------------------------------------------------------------------------------------------- 
	static Mat transImage(Mat inputImage, Vec4i line)
	{
		//��1������׼��
		//��������㣬��������������
		Point2f firstTriangle[3];
		Point2f nextTriangle[3];
		//����һЩMat����
		Mat rotMat( 2, 3, CV_32FC1 );
		Mat warpMat( 2, 3, CV_32FC1 );
		Mat outputImage;

		double angle = atan((double)(l[2]-l[0])/(double)(l[3]-l[1]))/CV_PI*180;

		// ��ʼ��Ŀ��ͼ��Ĵ�С
		int height = cos(angle)*inputImage.rows+sin(angle)*inputImage.cols;
		int width = cos(angle)*inputImage.cols+sin(angle)*inputImage.rows;
		outputImage = Mat::zeros( height, width, inputImage.type() );

		//��3������Դͼ���Ŀ��ͼ���ϵ�������Լ������任
		firstTriangle[0] = Point2f( 0,0 );
		firstTriangle[1] = Point2f( static_cast<float>(inputImage.cols) , 0 );
		firstTriangle[2] = Point2f( 0, static_cast<float>(inputImage.rows) );
		nextTriangle[0] = Point2f( 0,0 );
		nextTriangle[1] = Point2f( static_cast<float>(outputImage.cols) , 0);
		nextTriangle[2] = Point2f( 0, static_cast<float>(outputImage.rows) );
/*		//��4����÷���任
		warpMat = getAffineTransform( firstTriangle, nextTriangle );

		//��5����Դͼ��Ӧ�øո���õķ���任
		warpAffine( inputImage, outputImage, warpMat, outputImage.size() );

		//��6����ͼ��������ź�����ת
		// ������ͼ���е�˳ʱ����ת50����������Ϊ0.6����ת����
		Point center = Point( outputImage.cols/2, outputImage.rows/2 );
		double scale = 1;
		// ͨ���������תϸ����Ϣ�����ת����
		rotMat = getRotationMatrix2D( center, angle, scale );
		// ��ת�����ź��ͼ��
		warpAffine( inputImage, outputImage, rotMat, outputImage.size() );

		return outputImage;
	}



};
*/


//-----------------------------------��VerProject�ࡿ---------------------------------------------  
//      ��������ֱͶӰѰ���ּ��  
//---------------------------------------------------------------------------------------------- 

class VerProject
{
public:

	VerProject(Point p1, Point p2, Mat garyImage)//��ʼ��
	{
		image=garyImage;
		this->p1=p1;
		this->p2=p2;
	}
	
	vector<int> findPosition()
	{
		//find local peaks, key and value stored in vector wPoints, reSumps
		int pos;
		int Pd=20;
		int k=0;
		double sump=0;
		int i;
		for(i=p1.x;i<p2.x;i++) {
			
			int numj=0;
			for(int j=p1.y;j<p2.y;j++) {
				sump+=image.at<uchar>(j,i);
				numj++;
			}
			sump/=numj;
			sumps.push_back(sump);
			//cout<<sump<<"	";
			
			if (k>1 && sumps[k-1]>sumps[k-2] && sumps[k-1]>sumps[k]) {
				reSumps.push_back(sump);
				wPoints.push_back(i);
				//cout<<sump<<","<<i<<"	";
				
			}
			k++;
					
		}
		
		// delete peaks too closed between each other. which mean fliter to noise.
		// Pd : the min neighbor distance. it should be auto-changed with different pictures. 
		while(wPoints.size() >1)
		{
			int kk=1;
			pos = (int) ( max_element(reSumps.begin(),reSumps.end()) - reSumps.begin() );
			//cout<<reSumps[pos]<<"	"<<wPoints[pos]<<endl;
			delPoints.push_back(wPoints[pos]);
			delSumps.push_back(reSumps[pos]);
			//cout<<"pos:"<<pos<<endl;
			//if (pos>0)
			//cout<< wPoints[pos]-wPoints[pos-1]<<endl;

			while(pos-1>=0 && wPoints[pos]-wPoints[pos-1]<Pd)	//forward delete
			{
				
				wPoints.erase(wPoints.begin()+(--pos));

				reSumps.erase(reSumps.begin()+pos);
				
			}
			while(pos+kk<wPoints.size() && wPoints[pos+kk]-wPoints[pos]<Pd)	//backward delete
			{
				
				wPoints.erase(wPoints.begin()+pos+kk);
				
				reSumps.erase(reSumps.begin()+pos+kk);
				
			}

			
			wPoints.erase(wPoints.begin()+pos);
		
			reSumps.erase(reSumps.begin()+pos);

		}
		
		//delPoints.push_back(wPoints[0]);
		
		for (int i=0;i<delPoints.size();i++)
			cout<<delPoints[i]<<"	";
		return delPoints;

	}
	
private:

	Point p1,p2;//region of text
	double maxL;//��߶γ���
	Vec4i maxSeg;//��߶�����
	Mat image;//�����ͼ��
	vector<int> wPoints;	//vertical position of text
	vector<int> resPoints;
	vector<int> delPoints;	//position after delete
	vector<double> sumps;	//vertical value of text
	vector<double> reSumps;
	vector<double> delSumps;//value after delete
};

#endif