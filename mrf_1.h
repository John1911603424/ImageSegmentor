#ifndef MRF_1_H
#define MRF_1_H

#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "cv.h"
#include "highgui.h"

using namespace std;

// ��Ҫ��
class ImageOperations
{
public:
	IplImage *in_image, *out_image; 
	// ͼ��Ŀ�Ⱥ͸߶�
	int width, height;
public:
  ImageOperations();
  //bool IsOutput();			// TRUE if  out_image <> NULL
  // ��������������Ϊ��ֵ�ͷ��means and variances�������ڴ档
  void SetNoRegions(int n);
  int GetNoRegions() { return no_regions; }
  void SetBeta(double b) { beta = b; }
  void SetT(double x) { t = x; }
  void SetT0(double t) { T0 = t; }
  void SetC(double x) { c = x; }
  void SetAlpha(double x) { alpha = x; }
  int GetK() { return K; }
  double GetT() { return T; }
  double GetE() { return E; }
  //double GetTimer() { return (timer_valid? timer.GetElapsedTimeMs() : 0.0); }
  // �����������ľ�ֵ�ͷ���
  void CalculateMeanAndVariance(int region,int x, int y, int w, int h);
  void SetMeanAndVariance(int region,double avg, double var);

  // ����ȫ�����������ڵ�ǰ�ı�ǩ����
  double CalculateEnergy();
  // ����site(i,j)�ľֲ��������������ȱ�ǩ�Ѿ������site
  double LocalEnergy(int i, int j, int label);

  void Metropolis(bool mmd=false);  // executes Metropolis or MMD (if mmd=true)
  void ICM();			    // executes ICM
  void Gibbs();			    // executes Gibbs sampler

  virtual void OnIterationOver(void);
  // ��ʼ����ǩͼ��
  void InitOutImage();
  // ���ݵ�ǰ��ǩͼ�񣬻�������Ҷ�ͼ��
  void CreateOutput();

protected:
  // ���������Ҳ���ǽ�ͼ�����ط�Ϊ�����ࣿ
  int no_regions;
  // 2���ŵ�potentialǿ��
  double beta;

  // ��ֹ��������ֵ�������(deltaE < t)����ֹ��
  double t;
  // ��ʼ�¶ȣ�ICM��ʹ������������
  double T0;
  // �¶ȱ仯ϵ��T(n+1)=c*T(n).
  double c;
  double alpha;		            // alpha value for MMD

  // ÿ������ľ�ֵ
  double *mean;
  // ÿ������ķ���
  double *variance;
  double *work_buf1;
  // ��ǰȫ������
  double E;
  // �ϴε�����ȫ������
  double E_old;
  // ��ǰ�¶�
  double T;
  // ��ǰ��������
  int K;

  // ��ǩͼ��
  int **classes;
  // �Ҷ�ͼ��
  int **in_image_data;
  // ͼ��ĵ�����Ȼ�ȷֲ���evidence��
  float ** local_evidence;
  int use_local_evidence;

  // ����site(i,j)�������ǩk���singleton potential
  double Singleton(int i, int j, int label);
  double _Singleton(int i, int j, int label);
  // ����site(i,j)�������ǩk���doubleton potential
  double Doubleton(int i, int j, int label);
};
class mrf1: public ImageOperations
{
public:
	void OnIterationOver(void)
	{
		if(out_image)
		{
			CreateOutput();
		}
	}
};

#endif