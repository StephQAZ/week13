﻿// week13_1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

float* Cal_cellHist(Mat src)
{
	//计算角度及梯度
	Mat gx, gy;
	Mat mag, angle;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);
	cartToPolar(gx, gy, mag, angle, true);

	float hist[8] = { 0 };
	for (int j = 0; j < 16; j++)
	{
		for (int i = 0; i < 16; i++)
		{
			if (angle.at<float>(j, i) >= 0 && angle.at<float>(j, i) < 45)
			{
				hist[0] += mag.at<float>(j, i);
			}
			else if (angle.at<float>(j, i) >= 45 && angle.at<float>(j, i) < 90)
			{
				hist[1] += mag.at<float>(j, i);
			}
			else if (angle.at<float>(j, i) >= 90 && angle.at<float>(j, i) < 135)
			{
				hist[2] += mag.at<float>(j, i);
			}
			else if (angle.at<float>(j, i) >= 135 && angle.at<float>(j, i) < 180)
			{
				hist[3] += mag.at<float>(j, i);
			}
			else if (angle.at<float>(j, i) >= 180 && angle.at<float>(j, i) < 225)
			{
				hist[4] += mag.at<float>(j, i);
			}
			else if (angle.at<float>(j, i) >= 225 && angle.at<float>(j, i) < 270)
			{
				hist[5] += mag.at<float>(j, i);
			}
			else if (angle.at<float>(j, i) >= 270 && angle.at<float>(j, i) < 315)
			{
				hist[6] += mag.at<float>(j, i);
			}
			else if (angle.at<float>(j, i) >= 315 && angle.at<float>(j, i) < 360)
			{
				hist[7] += mag.at<float>(j, i);
			}
		}
	}
	return hist;
}


int main()
{
	Mat img1 = imread("C:\\Users\\27318\\Desktop\\大二下网络课程\\数字图像\\十四周\\template.png", 0);
	Mat srcimg2 = imread("C:\\Users\\27318\\Desktop\\大二下网络课程\\数字图像\\十四周\\img.png", 0);
	int cellSize = 16;
	int nX = img1.cols / cellSize;
	int nY = img1.rows / cellSize;
	Mat img2 = Mat::zeros(img1.rows, img1.cols, CV_8UC1);
	int num = nX * nY * 8;

	float* srcHist = new float[num];
	memset(srcHist, 0, sizeof(float) * num);

	float* img1Hist = new float[num];
	memset(img1Hist, 0, sizeof(float) * num);

	float* img2Hist = new float[num];
	memset(img2Hist, 0, sizeof(float) * num);

	int cnt1 = 0;
	int cnt2 = 0;
	float distance = 0;
	float distance_min = 10e100;
	int x = 0;
	int y = 0;
	for (int q = 80; q < /*srcimg2.rows- img1.rows*/110; q++) {
		for (int p = 80; p < /*srcimg2.cols - img1.cols*/110; p++) {
			for (int m = 0; m < img1.rows; m++) {
				for (int n = 0; n < img1.cols; n++) {
					img2.at<uchar>(m, n) = srcimg2.at<uchar>(q + m, p + n);
				}
			}
			//img1
			cnt1 = 0;
			for (int j = 0; j < nY; j++)
			{
				for (int i = 0; i < nX; i++)
				{
					Rect rectL(i * cellSize, j * cellSize, cellSize, cellSize);
					Mat nowMat = img1(rectL);
					for (int k = 0; k < 8; k++)
					{
						img1Hist[k + cnt1 * 8] = Cal_cellHist(nowMat)[k];
					}
					cnt1++;
				}
			}
			//img2
			cnt2 = 0;
			for (int j = 0; j < nY; j++)
			{
				for (int i = 0; i < nX; i++)
				{
					Rect rectL(i * cellSize, j * cellSize, cellSize, cellSize);
					Mat nowMat = img2(rectL);
					for (int k = 0; k < 8; k++)
					{
						img2Hist[k + cnt2 * 8] = Cal_cellHist(nowMat)[k];
					}
					cnt2++;
				}
			}
			distance = 0;
			for (int i = 0; i < num; i++)
			{
				distance += (img1Hist[i] - img2Hist[i]) * (img1Hist[i] - img2Hist[i]);
				if (distance < distance_min) {
					distance_min = distance;
					x = q;
					y = p;
				}
			}
		}
	}
	cout << "最小值" << distance_min << endl;
	cout << "x" << x << endl;
	cout << "y" << y << endl;
	Rect rect(y, x, img1.cols, img1.rows);//左上坐标（x,y）和矩形的长(x)宽(y)
	cv::rectangle(srcimg2, rect, Scalar(255, 0, 0), 1, LINE_8, 0);
	imshow("dstMat", srcimg2);
	waitKey(0);
	delete[] img1Hist;
	delete[] img2Hist;
	return 0;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
