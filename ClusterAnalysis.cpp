#include "ClusterAnalysis.h"
#include <fstream>
#include <iosfwd>
#include <math.h>

/*
�����������ʼ������
˵�����������ļ������뾶��������С���ݸ�����Ϣд������㷨�࣬��ȡ�ļ�����������Ϣ����д���㷨�����ݼ�����
������
char* fileName;    //�ļ���
double radius;    //�뾶
int minPTs;        //������С���ݸ���
����ֵ�� true;    */
bool ClusterAnalysis::Init(const char* fileName, double radius, int minPTs)
{
	this->radius = radius;        //���ð뾶
	this->minPTs = minPTs;        //����������С���ݸ���
	this->dimNum = DIME_NUM;    //��������ά��
	ifstream ifs(fileName);        //���ļ�
	if (!ifs.is_open())                //���ļ��Ѿ����򿪣���������Ϣ
	{
		cout << "Error opening file";    //���������Ϣ
		exit(-1);                        //�����˳�
	}

	unsigned long i = 0;			//���ݸ���ͳ��
	while (!ifs.eof())                //���ļ��ж�ȡPOI��Ϣ����POI��Ϣд��POI�б���
	{
		DataPoint tempDP;                //��ʱ���ݵ����
		double tempDimData[DIME_NUM];    //��ʱ���ݵ�ά����Ϣ
		for (int j = 0; j < DIME_NUM; j++)    //���ļ�����ȡÿһά����
		{
			ifs >> tempDimData[j];
		}
		tempDP.SetDimension(tempDimData);		//��ά����Ϣ�������ݵ������

		tempDP.SetDpId(i);						//�����ݵ����ID����Ϊi
		tempDP.SetVisited(false);				//���ݵ����isVisited����Ϊfalse
		tempDP.SetClusterId(-1);				//����Ĭ�ϴ�IDΪ-1
		dadaSets.push_back(tempDP);				//������ѹ�����ݼ�������
		i++;									//����+1
	}
	ifs.close();        //�ر��ļ���
	dataNum = i;            //�������ݶ��󼯺ϴ�СΪi

	cout << "**************SetArrivalPoints**************" << endl;
	for (unsigned long i = 0; i < dataNum; i++)
	{
		cout << "SetArrivalPoints" << i << "th point" << endl;

		SetArrivalPoints(dadaSets[i]);            //�������ݵ������ڶ���
	}
	return true;    //����
}

/*
���������Ѿ��������㷨��������ݼ���д���ļ�
˵�������Ѿ���������д���ļ�
������
char* fileName;    //Ҫд����ļ���
����ֵ�� true    */
bool ClusterAnalysis::WriteToFile(const char* fileName)
{
	/*ofstream of1(fileName);*/                                //��ʼ���ļ������
	FILE * fout;
	fopen_s(&fout, fileName, "wt");

	for (unsigned long i = 0; i < dataNum; i++)                //�Դ������ÿ�����ݵ�д���ļ�
	{
		//for (int d = 0; d < DIME_NUM; d++)                    //��ά����Ϣд���ļ�
		//of1 << dadaSets[i].GetDimension()[d] << '\t';
		//of1 << dadaSets[i].GetClusterId() << endl;        //��������IDд���ļ�
		fprintf(fout, "%lf\t%lf\t%lf\t%ld\n", dadaSets[i].GetDimension()[0],
				dadaSets[i].GetDimension()[1], dadaSets[i].GetDimension()[2],
			dadaSets[i].GetClusterId());
	}
	//of1.close();    //�ر�����ļ���
	fclose(fout);
	return true;    //����
}

/*
�������������ݵ��������б�
˵�����������ݵ��������б�
������
����ֵ�� true;    */
void ClusterAnalysis::SetArrivalPoints(DataPoint& dp)
{
	for (unsigned long i = 0; i < dataNum; i++)             //��ÿ�����ݵ�ִ��
	{
		double distance = GetDistance(dadaSets[i], dp);    //��ȡ���ض���֮��ľ���
		if (distance <= radius && i != dp.GetDpId())        //������С�ڰ뾶�������ض����id��dp��id��ִͬ��
			dp.GetArrivalPoints().push_back(i);            //���ض���idѹ��dp�������б���
	}
	if (dp.GetArrivalPoints().size() >= minPTs)            //��dp���������ݵ�������> minPTsִ��
	{
		dp.SetKey(true);    //��dp���Ķ����־λ��Ϊtrue
		return;                //����
	}
	dp.SetKey(false);    //���Ǻ��Ķ�����dp���Ķ����־λ��Ϊfalse
}


/*
������ִ�о������
˵����ִ�о������
������
����ֵ�� true;    */
bool ClusterAnalysis::DoDBSCANRecursive()
{
	unsigned long clusterId = 0;                        //����id��������ʼ��Ϊ0
	for (unsigned long i = 0; i < dataNum; i++)            //��ÿһ�����ݵ�ִ��
	{
		DataPoint& dp = dadaSets[i];                    //ȡ����i�����ݵ����
		if (!dp.isVisited() && dp.IsKey())            //������û�����ʹ��������Ǻ��Ķ���ִ��
		{
			dp.SetClusterId(clusterId);                //���øö���������IDΪclusterId
			dp.SetVisited(true);                    //���øö����ѱ����ʹ�
			KeyPointCluster(i, clusterId);            //�Ըö��������ڵ���о���
			clusterId++;                            //clusterId����1
		}
		//cout << "������\T" << i << endl;
	}

	cout << "������" << clusterId << "��" << endl;        //�㷨��ɺ�����������
	return true;    //����
}

/*
�����������ݵ������ڵĵ�ִ�о������
˵�������õݹ�ķ�����������Ⱦ�������
������
unsigned long dpID;            //���ݵ�id
unsigned long clusterId;       //���ݵ�������id
����ֵ�� void;    */
void ClusterAnalysis::KeyPointCluster(unsigned long dpID, unsigned long clusterId)
{
	DataPoint& srcDp = dadaSets[dpID];        //��ȡ���ݵ����
	if (!srcDp.IsKey())    return;
	vector<unsigned long>& arrvalPoints = srcDp.GetArrivalPoints();        //��ȡ���������ڵ�ID�б�
	for (unsigned long i = 0; i < arrvalPoints.size(); i++)
	{
		DataPoint& desDp = dadaSets[arrvalPoints[i]];    //��ȡ�����ڵ����ݵ�
		if (!desDp.isVisited())                            //���ö���û�б����ʹ�ִ��
		{
			//cout << "���ݵ�\t"<< desDp.GetDpId()<<"����IDΪ\t" <<clusterId << endl;
			desDp.SetClusterId(clusterId);        //���øö��������ص�IDΪclusterId�������ö����������
			desDp.SetVisited(true);                //���øö����ѱ�����
			if (desDp.IsKey())                    //���ö����Ǻ��Ķ���
			{
				KeyPointCluster(desDp.GetDpId(), clusterId);    //�ݹ�ضԸ���������ݵ������ڵĵ�ִ�о������������������ȷ���
			}
		}
	}
}

//�����ݵ�֮�����
/*
��������ȡ�����ݵ�֮�����
˵������ȡ�����ݵ�֮���ŷʽ����
������
DataPoint& dp1;        //���ݵ�1
DataPoint& dp2;        //���ݵ�2
����ֵ�� double;    //����֮��ľ���        */
double ClusterAnalysis::GetDistance(DataPoint& dp1, DataPoint& dp2)
{
	double distance = 0;        //��ʼ������Ϊ0
	for (int i = 0; i < DIME_NUM; i++)    //������ÿһά����ִ��
	{
		distance += pow(dp1.GetDimension()[i] - dp2.GetDimension()[i], 2);    //����+ÿһά���ƽ��
	}
	return pow(distance, 0.5);        //���������ؾ���
}
