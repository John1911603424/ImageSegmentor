// MyGabor.h: interface for the CMyGabor class.
/*

�޸���Ҧ��Gabor������룬
�ο����ף�
    Texture Features for Browsing and Retrieval of Image Data, by B.S.Manjunath and W.Y.Ma
�Լ���Ҧ������-��������������ң��ͼ���������

               04.06.03
*/
/////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYGABOR_H__6E9D2397_C60E_4079_8277_D7912494E533__INCLUDED_)
#define AFX_MYGABOR_H__6E9D2397_C60E_4079_8277_D7912494E533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyGabor  
{
public:
	CMyGabor();
	virtual ~CMyGabor();
    void GetBlockGaborCoeff(FLOAT* inData, INT m, INT n, FLOAT& outCoeff);//�õ��������ݿ���ָ������ͳ߶���Gabor�ı任ϵ��, ������Ҫ��֤�������ݿ�Ĵ�С����Ҫ��
    void GetImageGaborCoeff(FLOAT* imageData, INT imagWidth, INT imageHeight, INT m, INT n, FLOAT** outData);//�õ�����ͼ����ָ������ͳ߶��µ�Gabor�任, ����ڴ��ɵ����߷���;

private:
	void InitPara(INT m, INT n);//��ʼ���������, mΪ�߶ȣ�nΪ����
	void GetCoeffAt(INT m, INT n, INT x, INT y, DOUBLE& coeffr, DOUBLE& coeffi);//�õ�һ���˲���ϵ����
	void CalcuCoeffWhen(INT m, INT n);//����˳߶Ⱥͷ���ʱ���˲���ϵ��, mΪ�߶ȣ�nΪ����

private:
	CString curMN;//��ǰ�Ѽ���߶Ⱥͷ�����˲�����
	INT myM;//�߶ȸ�����
	INT myN;//���������
	INT myL;//�˲���ϵ��������
    DOUBLE curWmn;//����Ƶ�ʣ�
	DOUBLE myUl;//upper center frequency;
	DOUBLE myUh;//lower center frequency;
	DOUBLE curAlpha;
	LONG gaborSize;

	DOUBLE* coeffR;//ϵ��ʵ����
	DOUBLE* coeffI;//ϵ���鲿��
	DOUBLE  deltaXmn;//����Xmn��
	DOUBLE  deltaYmn;//����Ymn��
	DOUBLE  sita;//�Ƕȣ�
};

#endif // !defined(AFX_MYGABOR_H__6E9D2397_C60E_4079_8277_D7912494E533__INCLUDED_)
