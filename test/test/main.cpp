/*线性校正
功能：利用边缘检测与霍夫变换找轴线，根据轴线进行线性校正
日期：5.15
备注：旋转方式2暂时没写
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

//-----------------------------------【部分参数声明部分】---------------------------------------
//		描述：包含程序所使用的部分参数
//-----------------------------------------------------------------------------------------------
#define targetHight 800	//压缩后的图像高度
#define figureName "t4.jpg"//处理图像的名称

//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------------
Mat resizeImage,grayImage,edgeImage,edgeImage2,midImage,finalImage,tmp,dst,img;//临时变量和目标图的定义
vector<Point> mPoint;
vector<int> xpos;

void on_mouse(int event,int x,int y,int flags,void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号  
{  
	static Point pre_pt = (-1,-1);//初始坐标  
	static Point cur_pt = (-1,-1);//实时坐标  
	char temp[16];  
	grayImage.copyTo(img);  
	grayImage.copyTo(tmp);
	if (event == CV_EVENT_LBUTTONDOWN)//左键按下，读取初始坐标，并在图像上该点处划圆  
	{  
		cout<<"("<<x<<","<<y<<")"<<endl;
		grayImage.copyTo(img);//将原始图片复制到img中  
		sprintf(temp,"(%d,%d)",x,y);  
		pre_pt = Point(x,y);  
		mPoint.push_back(pre_pt);
		putText(img,temp,pre_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),10,8);//在窗口上显示坐标  
		//circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);//划圆  
		imshow("img",img);  
	}  
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数  
	{  
		img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标  
		sprintf(temp,"(%d,%d)",x,y);  
		cur_pt = Point(x,y);  
		putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));//只是实时显示鼠标移动的坐标  
		imshow("img",tmp);  
	}  
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))//左键按下时，鼠标移动，则在图像上划矩形  
	{  
		img.copyTo(tmp);  
		sprintf(temp,"(%d,%d)",x,y);  
		cur_pt = Point(x,y);  
		putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));  
		rectangle(tmp,pre_pt,cur_pt,Scalar(0,255,0,0),1,8,0);//在临时图像上实时显示鼠标拖动时形成的矩形  
		imshow("img",tmp);  
	}  
	else if (event == CV_EVENT_LBUTTONUP)//左键松开，将在图像上划矩形  
	{  
		grayImage.copyTo(img);  
		sprintf(temp,"(%d,%d)",x,y);  
		cur_pt = Point(x,y);  
		putText(img,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));  
		circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);  
		rectangle(img,pre_pt,cur_pt,Scalar(0,255,0,0),1,8,0);//根据初始点和结束点，将矩形画到img上  
		imshow("img",img);  
		img.copyTo(tmp);  
		//截取矩形包围的图像，并保存到dst中  
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
	//【1】载入原始图和Mat变量定义
	Mat origImage = imread(figureName);  //载入图像
	if(!origImage.data ) { cout<<"读取图片错误，请确定目录下是否有imread函数指定的图片存在~"<<endl;getchar(); return false; }
	

	//【2】改变图像大小
	double proportion = targetHight*1.0/origImage.rows;//图像缩放比例
	resize(origImage, resizeImage, Size(), proportion, proportion);//将图像高度归一为指定高度
		//cout<<"缩放后图像高度："<<resizeImage.rows<<endl;

	//【3】转化为灰度图、均衡化、进行边缘检测与边缘扩展
	cvtColor( resizeImage, grayImage, CV_BGR2GRAY );//转换为灰度图
	equalizeHist( grayImage, grayImage );//灰度图均衡化
	Canny(grayImage, edgeImage, 100, 300, 3);//进行canny边缘检测
		/*	参数3、4改善边缘检测效果
			Ⅰ.如果某一像素位置的幅值超过 高 阈值, 该像素被保留为边缘像素。
			Ⅱ.如果某一像素位置的幅值小于 低 阈值, 该像素被排除。
			Ⅲ.如果某一像素位置的幅值在两个阈值之间,该像素仅仅在连接到一个高于 高 阈值的像素时被保留。*/
	edgeImage2 = MyMethod::expandLine(edgeImage);//对边缘检测后的二值图像进行边缘扩展

	//【4】进行霍夫线变换
	vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合(列，行，列，行)
	HoughLinesP(edgeImage2, lines, 1, CV_PI/360, targetHight*0.1, targetHight*0.6, targetHight*0.005 );//进行霍夫变换
		/*	第五个参数，Accumulator threshold parameter. Only those lines are returned that get enough votes (  ).
			第六个参数，double类型的minLineLength，有默认值0，表示最低线段的长度，比这个设定参数短的线段就不能被显现出来。
			第七个参数，double类型的maxLineGap，有默认值0，允许将同一行点与点之间连接起来的最大的距离*/

	//【5】依次在图中绘制出每条线段
	resizeImage.copyTo(midImage);//将缩放图复制给效果图
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line( midImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,255), 2, CV_AA);//point:(列，行)
	}
	//【6】找出最长线段
	MaxLine maxl = MaxLine(lines,midImage);//初始化寻找最长线段的类
	maxl.drawOnImage();//在图中画出最长线段


	//【7】对*原图*进行仿射变换
	Vec4i l = maxl.maxSegment();//找到压缩图片的最长线段
	Vec4i l2;//定义原图中的最长线段
	for (int i = 0;i<4;i++)//将压缩图片中的最长线段转换成原图中最长线段
	{
		l2[i]=int(l[i]/proportion);
	}

	TransImage transIm = TransImage(origImage,l2);//定义旋转类
	finalImage = transIm.trans1();//作旋转变换

	cvtColor( finalImage, grayImage, CV_BGR2GRAY );//转换为灰度图

	cout<<grayImage.rows<<"  "<<grayImage.cols<<endl;
	equalizeHist( grayImage, grayImage );//灰度图均衡化

	//【3】进行局部二值化
	int blockSize = (grayImage.cols+grayImage.rows)*0.5*0.05;//阈值计算窗口边长，必须为奇数
	if (blockSize%2==0){blockSize++;}
		//cout<<"blockSize:"<<blockSize<<endl;
	adaptiveThreshold(grayImage,grayImage,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY, blockSize, 20);
	/*
	参数5
	For the method ADAPTIVE_THRESH_MEAN_C , the threshold value T(x,y) is a mean of the neighborhood of (x, y) minus C .
	For the method ADAPTIVE_THRESH_GAUSSIAN_C , the threshold value T(x, y) is a weighted sum (cross-correlation with a 
	Gaussian window) of the neighborhood of (x, y) minus C . The default sigma (standard deviation) is used for the specified blockSize . 
	*/
	//【4】进行中值滤波（去除椒盐噪声）
	int ksize = (grayImage.cols+grayImage.rows)*0.5/800;//滤波窗口边长，必须为奇数
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

			
	
	//【8】显示图像
	//namedWindow("【原始图】",WINDOW_NORMAL);//显示原始图像
	//imshow("【原始图】", origImage);  

	//namedWindow("【缩放图】",WINDOW_NORMAL);//显示改变大小后的图像
	//imshow("【缩放图】", resizeImage);

	//namedWindow("【灰度图】",WINDOW_NORMAL);//显示直方图均衡化后的灰度图像
	//imshow("【灰度图】", grayImage);

	//namedWindow("【边缘检测后的图】",WINDOW_NORMAL);//显示边缘检测并扩展后的图像
	//imshow("【边缘检测后的图】", edgeImage2);  

	//namedWindow("【旋转前效果图】",WINDOW_NORMAL);//显示最终效果图
	//imshow("【旋转前效果图】", midImage);  

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));  
	Mat element2 = getStructuringElement(MORPH_RECT, Size(15, 15));  
	

	namedWindow("img",WINDOW_NORMAL);//显示最终效果图
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
	////首先在图像中鼠标两点选择区域，再按任意键开始计算。
	//VerProject verPro=VerProject(mPoint[0],mPoint[1],grayImage);	//文字投影类
	//xpos=verPro.findPosition();		//提取文字位置
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
