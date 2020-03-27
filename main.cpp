#include "ClusterAnalysis.h"
#include <cstdio>

using namespace std;

int main()
{
	//string dataPath = "F:\\KITTIData\\data_odometry_velodyne\\dataset\\sequences\\00\\velodyne_pcd\\";
	string dataPath = "F://KITTIData//rawData//2011_09_26//2011_09_26_drive_0009_sync//velodyne_pcd//";
	string filename = dataPath + "0000000082DS.txt";
	string fileresult = dataPath + "0000000082DS_result.txt";

	ClusterAnalysis myClusterAnalysis;                        //�����㷨��������
	cout << "**************Init**************" << endl;
	myClusterAnalysis.Init(filename.c_str(), 0.5, 15);

	cout << "**************DoDBSCANRecursive**************" << endl;
	myClusterAnalysis.DoDBSCANRecursive();                    //ִ�о����㷨

	myClusterAnalysis.WriteToFile(fileresult.c_str());//дִ�к�Ľ��д���ļ�

	system("pause");    //��ʾ���
	return 0;            //����
}