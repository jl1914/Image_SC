/*����У��
���ܣ����ñ�Ե��������任�����ߣ��������߽�������У��
���ڣ�5.15
��ע����ת��ʽ2��ʱûд
*/
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "myclass.h"
#include "mymethod.h"

using namespace cv;
using namespace std;

//-----------------------------------�����ֲ����������֡�---------------------------------------
//		����������������ʹ�õĲ��ֲ���
//-----------------------------------------------------------------------------------------------
#define targetHight 800	//ѹ�����ͼ��߶�
#define figureName "t4.jpg"//����ͼ�������

//-----------------------------------��main( )������--------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-----------------------------------------------------------------------------------------------
Mat resizeImage,grayImage,edgeImage,edgeImage2,midImage,finalImage,tmp,dst,img;//��ʱ������Ŀ��ͼ�Ķ���
vector<Point> mPoint;
vector<int> xpos;

void on_mouse(int event,int x,int y,int flags,void *ustc)//event����¼����ţ�x,y������꣬flags��ק�ͼ��̲����Ĵ���  
{  
	static Point pre_pt = (-1,-1);//��ʼ����  
	static Point cur_pt = (-1,-1);//ʵʱ����  
	char temp[16];  
	grayImage.copyTo(img);  
	grayImage.copyTo(tmp);
	if (event == CV_EVENT_LBUTTONDOWN)//������£���ȡ��ʼ���꣬����ͼ���ϸõ㴦��Բ  
	{  
		cout<<"("<<x<<","<<y<<")"<<endl;
		grayImage.copyTo(img);//��ԭʼͼƬ���Ƶ�img��  
		sprintf(temp,"(%d,%d)",x,y);  
		pre_pt = Point(x,y);  
		mPoint.push_back(pre_pt);
		putText(img,temp,pre_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),10,8);//�ڴ�������ʾ����  
		//circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);//��Բ  
		imshow("img",img);  
	}  
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//���û�а��µ����������ƶ��Ĵ�����  
	{  
		img.copyTo(tmp);//��img���Ƶ���ʱͼ��tmp�ϣ�������ʾʵʱ����  
		sprintf(temp,"(%d,%d)",x,y);  
		cur_pt = Point(x,y);  
		putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));//ֻ��ʵʱ��ʾ����ƶ�������  
		imshow("img",tmp);  
	}  
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//�������ʱ������ƶ�������ͼ���ϻ�����  
	{  
		img.copyTo(tmp);  
		sprintf(temp,"(%d,%d)",x,y);  
		cur_pt = Point(x,y);  
		putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));  
		rectangle(tmp,pre_pt,cur_pt,Scalar(0,255,0,0),1,8,0);//����ʱͼ����ʵʱ��ʾ����϶�ʱ�γɵľ���  
		imshow("img",tmp);  
	}  
	else if (event == CV_EVENT_LBUTTONUP)//����ɿ�������ͼ���ϻ�����  
	{  
		grayImage.copyTo(img);  
		sprintf(temp,"(%d,%d)",x,y);  
		cur_pt = Point(x,y);  
		putText(img,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));  
		circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);  
		rectangle(img,pre_pt,cur_pt,Scalar(0,255,0,0),1,8,0);//���ݳ�ʼ��ͽ����㣬�����λ���img��  
		imshow("img",img);  
		img.copyTo(tmp);  
		//��ȡ���ΰ�Χ��ͼ�񣬲����浽dst��  
		int width = abs(pre_pt.x - cur_pt.x);  
		int height = abs(pre_pt.y - cur_pt.y);  
		if (width == 0 || height == 0)  
		{  
			//printf("width == 0 || height == 0");  
			return;  
		}  
		dst = grayImage(Rect(min(cur_pt.x,pre_pt.x),min(cur_pt.y,pre_pt.y),width,height));  
		namedWindow("dst");  
		imshow("dst",dst);  
		waitKey(0);  
	}  
}  


int main( )
{
	//��1������ԭʼͼ��Mat��������
	Mat origImage = imread(figureName);  //����ͼ��
	if(!origImage.data ) { cout<<"��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread����ָ����ͼƬ����~"<<endl;getchar(); return false; }
	

	//��2���ı�ͼ���С
	double proportion = targetHight*1.0/origImage.rows;//ͼ�����ű���
	resize(origImage, resizeImage, Size(), proportion, proportion);//��ͼ��߶ȹ�һΪָ���߶�
		//cout<<"���ź�ͼ��߶ȣ�"<<resizeImage.rows<<endl;

	//��3��ת��Ϊ�Ҷ�ͼ�����⻯�����б�Ե������Ե��չ
	cvtColor( resizeImage, grayImage, CV_BGR2GRAY );//ת��Ϊ�Ҷ�ͼ
	equalizeHist( grayImage, grayImage );//�Ҷ�ͼ���⻯
	Canny(grayImage, edgeImage, 100, 300, 3);//����canny��Ե���
		/*	����3��4���Ʊ�Ե���Ч��
			��.���ĳһ����λ�õķ�ֵ���� �� ��ֵ, �����ر�����Ϊ��Ե���ء�
			��.���ĳһ����λ�õķ�ֵС�� �� ��ֵ, �����ر��ų���
			��.���ĳһ����λ�õķ�ֵ��������ֵ֮��,�����ؽ��������ӵ�һ������ �� ��ֵ������ʱ��������*/
	edgeImage2 = MyMethod::expandLine(edgeImage);//�Ա�Ե����Ķ�ֵͼ����б�Ե��չ

	//��4�����л����߱任
	vector<Vec4i> lines;//����һ��ʸ���ṹlines���ڴ�ŵõ����߶�ʸ������(�У��У��У���)
	HoughLinesP(edgeImage2, lines, 1, CV_PI/360, targetHight*0.1, targetHight*0.6, targetHight*0.005 );//���л���任
		/*	�����������Accumulator threshold parameter. Only those lines are returned that get enough votes (  ).
			������������double���͵�minLineLength����Ĭ��ֵ0����ʾ����߶εĳ��ȣ�������趨�����̵��߶ξͲ��ܱ����ֳ�����
			���߸�������double���͵�maxLineGap����Ĭ��ֵ0������ͬһ�е����֮���������������ľ���*/

	//��5��������ͼ�л��Ƴ�ÿ���߶�
	resizeImage.copyTo(midImage);//������ͼ���Ƹ�Ч��ͼ
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line( midImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,255), 2, CV_AA);//point:(�У���)
	}
	//��6���ҳ���߶�
	MaxLine maxl = MaxLine(lines,midImage);//��ʼ��Ѱ����߶ε���
	maxl.drawOnImage();//��ͼ�л�����߶�


	//��7����*ԭͼ*���з���任
	Vec4i l = maxl.maxSegment();//�ҵ�ѹ��ͼƬ����߶�
	Vec4i l2;//����ԭͼ�е���߶�
	for (int i = 0;i<4;i++)//��ѹ��ͼƬ�е���߶�ת����ԭͼ����߶�
	{
		l2[i]=int(l[i]/proportion);
	}

	TransImage transIm = TransImage(origImage,l2);//������ת��
	finalImage = transIm.trans1();//����ת�任

	cvtColor( finalImage, grayImage, CV_BGR2GRAY );//ת��Ϊ�Ҷ�ͼ

	cout<<grayImage.rows<<"  "<<grayImage.cols<<endl;
	equalizeHist( grayImage, grayImage );//�Ҷ�ͼ���⻯

	//��3�����оֲ���ֵ��
	int blockSize = (grayImage.cols+grayImage.rows)*0.5*0.05;//��ֵ���㴰�ڱ߳�������Ϊ����
	if (blockSize%2==0){blockSize++;}
		//cout<<"blockSize:"<<blockSize<<endl;
	adaptiveThreshold(grayImage,grayImage,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY, blockSize, 20);
	/*
	����5
	For the method ADAPTIVE_THRESH_MEAN_C , the threshold value T(x,y) is a mean of the neighborhood of (x, y) minus C .
	For the method ADAPTIVE_THRESH_GAUSSIAN_C , the threshold value T(x, y) is a weighted sum (cross-correlation with a 
	Gaussian window) of the neighborhood of (x, y) minus C . The default sigma (standard deviation) is used for the specified blockSize . 
	*/
	//��4��������ֵ�˲���ȥ������������
	int ksize = (grayImage.cols+grayImage.rows)*0.5/800;//�˲����ڱ߳�������Ϊ����
	if (ksize%2==0){ksize++;}
	if (ksize<3){ksize = 3;}
	if (ksize>7){ksize = 7;}
		//cout<<"ksize:"<<ksize<<endl;
	medianBlur(grayImage,grayImage,5);
	//for(int i=0;i<finalImage.cols;i++)
	//	cout<<(int)finalImage.at<cv::Vec3b>(2335,i)[1]<<" ";
	//cout<<endl<<"END";
	

	//for(int i=0;i<grayImage.rows;i++) {
	//	double sump=0;
	//	for(int j=0;j<grayImage.cols;j++) {
	//		sump+=grayImage.at<uchar>(i,j);
	//	}
	//	sump/=grayImage.cols;
	//	//cout<<sump<<"  ";
	//}

			
	
	//��8����ʾͼ��
	//namedWindow("��ԭʼͼ��",WINDOW_NORMAL);//��ʾԭʼͼ��
	//imshow("��ԭʼͼ��", origImage);  

	//namedWindow("������ͼ��",WINDOW_NORMAL);//��ʾ�ı��С���ͼ��
	//imshow("������ͼ��", resizeImage);

	//namedWindow("���Ҷ�ͼ��",WINDOW_NORMAL);//��ʾֱ��ͼ���⻯��ĻҶ�ͼ��
	//imshow("���Ҷ�ͼ��", grayImage);

	//namedWindow("����Ե�����ͼ��",WINDOW_NORMAL);//��ʾ��Ե��Ⲣ��չ���ͼ��
	//imshow("����Ե�����ͼ��", edgeImage2);  

	//namedWindow("����תǰЧ��ͼ��",WINDOW_NORMAL);//��ʾ����Ч��ͼ
	//imshow("����תǰЧ��ͼ��", midImage);  

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));  
	Mat element2 = getStructuringElement(MORPH_RECT, Size(15, 15));  
	

	namedWindow("img",WINDOW_NORMAL);//��ʾ����Ч��ͼ
	//setMouseCallback("img",on_mouse,0); 
	imshow("img", grayImage);  

	waitKey(0);  
	erode(grayImage,grayImage, element ); 
	dilate(grayImage,grayImage, element2 );
	erode(grayImage,grayImage, element ); 


	imshow("img", grayImage);  
	waitKey(0); 
	////mPoint.push_back(Point(2045,1439)) ;
	////mPoint.push_back(Point(3206,1492));
	////������ͼ�����������ѡ�������ٰ��������ʼ���㡣
	//VerProject verPro=VerProject(mPoint[0],mPoint[1],grayImage);	//����ͶӰ��
	//xpos=verPro.findPosition();		//��ȡ����λ��
	//for (int i=0;i<xpos.size();i++)
	//{
	//	line( grayImage , Point( xpos[i],mPoint[0].y ) , Point( xpos[i], mPoint[1].y ) , cvScalar( 255 , 0 , 0 ,0 ) , 3 , 8  ,0 );  
	//	 
	//}

	///*for(int i=mPoint[0].y;i<mPoint[1].y;i++) {
	//	double sump=0;
	//	int numj=0;
	//	for(int j=mPoint[0].x;j<mPoint[1].x;j++) {
	//		sump+=grayImage.at<uchar>(i,j);
	//		numj++;
	//	}
	//	sump/=numj;
	//	cout<<sump<<"		";
	//}*/
	//imshow("img", grayImage); 
	//waitKey(0); 
	return 0;  
}
