#pragma once
#include <iostream>
#include <cmath>
#include "DataPoint.h"

using namespace std;

//聚类分析类型
class ClusterAnalysis
{
private:
	vector<DataPoint> dadaSets;			//数据集合
	unsigned int dimNum;				//维度
	double radius;						//半径
	unsigned int dataNum;				//数据数量
	unsigned int minPTs;				//邻域最小数据个数

	double GetDistance(DataPoint& dp1, DataPoint& dp2);						//距离函数
	void SetArrivalPoints(DataPoint& dp);									//设置数据点的领域点列表
	void KeyPointCluster(unsigned long i, unsigned long clusterId);			//对数据点领域内的点执行聚类操作
public:

	ClusterAnalysis() {}                    //默认构造函数
	bool Init(const char* fileName, double radius, int minPTs);		//初始化操作
	bool DoDBSCANRecursive();										//DBSCAN递归算法
	bool WriteToFile(const char* fileName);							//将聚类结果写入文件
};

