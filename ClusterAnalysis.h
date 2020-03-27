#pragma once
#include <iostream>
#include <cmath>
#include "DataPoint.h"

using namespace std;

//�����������
class ClusterAnalysis
{
private:
	vector<DataPoint> dadaSets;			//���ݼ���
	unsigned int dimNum;				//ά��
	double radius;						//�뾶
	unsigned int dataNum;				//��������
	unsigned int minPTs;				//������С���ݸ���

	double GetDistance(DataPoint& dp1, DataPoint& dp2);						//���뺯��
	void SetArrivalPoints(DataPoint& dp);									//�������ݵ��������б�
	void KeyPointCluster(unsigned long i, unsigned long clusterId);			//�����ݵ������ڵĵ�ִ�о������
public:

	ClusterAnalysis() {}                    //Ĭ�Ϲ��캯��
	bool Init(const char* fileName, double radius, int minPTs);		//��ʼ������
	bool DoDBSCANRecursive();										//DBSCAN�ݹ��㷨
	bool WriteToFile(const char* fileName);							//��������д���ļ�
};

