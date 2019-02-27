// MyTexture.cpp: implementation of the CMyTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myimagedb.h"
#include "MyTexture.h"
#include <MATH.H>
#include "mymath.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//#define NEIRADIUS 16//����뾶��������DOC�е�ͬ������һ�£�
#define DHISNORM ( sqrt( 4. * (pow(((pow(((NEIRADIUS*2+1)-1), 2. ) )/4.), 2.)) ) )//���ܵ������룬���ڹ�һ������ֱ��ͼ���룻

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMyTexture::CMyTexture()
{

}

CMyTexture::~CMyTexture()
{

}

FLOAT CMyTexture::CalcuEntropy(BYTE* inMatrix, INT width, INT height)
//��������ͼ�����Ϣ�أ�
{
	//��������ͼ���ܵĻҶ�ֵ��
	INT ptcount = width * height;
	DOUBLE maxen = -1;
	DOUBLE allitensity = 0;
	for (INT i=0; i<ptcount; i++)
	{
		allitensity += (DOUBLE) ( inMatrix[i] );
	}

	if (allitensity<1)
	{
		return 0;
	}

	//������Ϣ�أ�
	FLOAT entropy = 0;
	for (i=0; i<ptcount; i++)
	{
		DOUBLE fe = inMatrix[i] / allitensity;
		DOUBLE templog = log(fe);
		entropy -= (FLOAT) ( fe * templog );
	}
	
	return entropy;
}

FLOAT CMyTexture::CalcuDisBetweenDHis(DirectionHis* his1, DirectionHis* his2, INT binCounts)
//������������ֱ��ͼ���ŷ�Ͼ��룬��NEIRADIUS��һ����[0, 1]֮�䣻
{
	FLOAT added = 0;
	for (INT i=0; i<binCounts; i++)
	{
		INT tempd = (his1[i].count - his1[i].count);
		FLOAT tempf = (FLOAT) ( pow((DOUBLE)tempd, 2.) );
		added += tempf;
	}

	FLOAT dis = (FLOAT) sqrt(added);
	dis = (FLOAT) ( dis / DHISNORM );//��һ��

	return dis;
}

BOOL CMyTexture::GetDirectionReal(BYTE* inMatrix, INT width, INT height, FLOAT& direction, FLOAT& itensity, FLOAT& err)
//���������ķ���, direction����itensityǿ��, err���
{
	//���ȼ�������ص�deltaX��deltaY��
	//Ȼ���������ص㴦���ݶȷ����Լ�ģ��
	//�����ݶ�ƽ��������·��򣬼���ģ��ƽ����ǿ��
	//�������Ϊ���򷽲

	//��������������ˮƽ�ʹ�ֱ�����ϵ��ݶ�,��Ե�㲻���㣻
	FLOAT* deltasita;
	FLOAT* deltar;
	INT deltacount = width * height;
	deltasita = new FLOAT[deltacount];
	deltar = new FLOAT[deltacount];
    GetGradient(inMatrix, width, height, deltar, deltasita);

	//�����ֵ�뷽��Լ�ģ��ֵ�뷽�
	FLOAT dmean, derr, rmean, rerr;
	dmean = derr = rmean = rerr = 0;

	myMath.ClacuMeanPositive(deltasita, deltacount
		, dmean, derr);//�����ֵ�뷽�
	myMath.ClacuMeanPositive(deltar, deltacount
		, rmean, rerr);//ģ��ֵ�뷽�

	direction = dmean;//�Ƕȣ�
	itensity = rmean;//����ǿ�ȣ�
	err = derr;//���򷽲

	delete [] deltar; deltar = NULL;
	delete [] deltasita; deltasita = NULL;

	return TRUE;	
}

BOOL CMyTexture::GetDirection(BYTE* inMatrix, INT width, INT height, INT& direction, FLOAT& itensity)
//���������ķ���, ���direction��ʾ�ĸ�����֮һ��itensity��ʾǿ�ȣ�
{
	DirectionHis* dhisarr = NULL;
	INT binsinhis = -1;
	//���������ķ���ֱ��ͼ��
	BOOL tempb = CalcuDirectionHis(inMatrix, width, height, &dhisarr, binsinhis);
	if (binsinhis<0)
	{
		return FALSE;
	}

	if (tempb)
	{
		//����������
		direction = -1;
		itensity = -1;
	}

	//ȡ���count��ӦBINΪ�ÿ鷽��
	DOUBLE* countarr = new DOUBLE[binsinhis];//��ʱ�����Ÿ�BIN counts;
	INT tempmax = -1;
	for (INT i=0; i<binsinhis; i++)
	{
		countarr[i] = dhisarr[i].count;
		if (dhisarr[i].count>tempmax)
		{
			tempmax = dhisarr[i].count;
			direction = i;
		}		
	}

	delete [] dhisarr; dhisarr = NULL;

	//���¼����ֵ�ͷ���Է���ĵ�����Ϊǿ�ȣ�
	FLOAT tempmean, temperr;
	myMath.ClacuMean(countarr, binsinhis, tempmean, temperr);
	itensity = (FLOAT) ( 1. / temperr );

	delete [] countarr; countarr = NULL;

	return TRUE;	
}

BOOL CMyTexture::CalcuDirectionHis(BYTE* inMatrix, INT width, INT height, DirectionHis** outHis, INT& binsInHis)
//���������ķ���ֱ��ͼ,������ȷ�����򷵻�TRUE�����򷵻�FALSE��
{
	//���ȼ�������ص�deltaX��deltaY��
	//Ȼ���������ص㴦���ݶȷ����Լ�ģ��
	//�����Ǿ��ൽָ�����ĸ�������ȥ�õ�����ֱ��ͼ��
	//���¼����ݶȷ�����ģ��
	FLOAT* deltasita;
	FLOAT* deltar;
	INT deltacount = width * height;
	deltasita = new FLOAT[deltacount];
	deltar = new FLOAT[deltacount];
    GetGradient(inMatrix, width, height, deltar, deltasita);

	DirectionHis* dhis = new DirectionHis[5];//����ֱ��ͼ,���һ��Ϊ��Ч����
	//����ֵ��
	for (INT i=0; i<4; i++)
	{
		dhis[i].count = 0;
		dhis[i].sita = (FLOAT) ( PI/4 * i );//ÿ��45��ȡһ������
	}
	dhis[4].sita = -1;
	dhis[4].count = 0;

	//���¼��㷽��ֱ��ͼ��
	FLOAT bias = (FLOAT) (0.1);//ģ��ֵ��
	for (INT y=0; y<height; y++)
	{
		for (INT x=0; x<width; x++)
		{
			INT temppos = y*(width) + x;
			//�����������protype;
			INT j = (INT) ( deltasita[temppos] / (PI/4.) );
			if (j<0 || j>3 || deltar[temppos]==0)
			{
				j = 4;
			}
			dhis[j].count++;
		}
	}

	delete [] deltar; deltar = NULL;
	delete [] deltasita; deltasita = NULL;

	*outHis = dhis;
	binsInHis = 4;

    if ( dhis[4].count>(deltacount - width) )
	{
		return FALSE;
	}
	return TRUE;
}

void CMyTexture::GetGradient(BYTE* image, INT width, INT height
		, FLOAT* deltar, FLOAT* deltasita)
//�õ�����ͼ����ݶȣ�
{
	//��������������ˮƽ�ʹ�ֱ�����ϵ��ݶ�,��Ե���ݶȼ�Ϊ0��
	INT* deltaxarr;
	INT* deltayarr;
	INT grawidth = width;
	INT graheight = height;
	INT deltacount = grawidth * graheight;
	deltaxarr = new INT[deltacount];
	deltayarr = new INT[deltacount];
	for (INT y=0; y<graheight; y++)
	{
		INT x1 = 0;
		INT pos1 = y*grawidth + x1;
		deltaxarr[pos1] = 0;
		deltayarr[pos1] = 0;
		INT x2 = grawidth-1;
		INT pos2 = y*grawidth + x2;
		deltaxarr[pos2] = 0;
		deltayarr[pos2] = 0;
	}
	for (INT x=0; x<grawidth; x++)
	{
		INT y1 = 0;
		INT pos1 = y1*grawidth + x;
		deltaxarr[pos1] = 0;
		deltayarr[pos1] = 0;
		INT y2 = graheight-1;
		INT pos2 = y2*grawidth + x;
		deltaxarr[pos2] = 0;
		deltayarr[pos2] = 0;
	}

	for (y=1; y<graheight-1; y++)
	{
		for (x=1; x<grawidth-1; x++)
		{
			INT inarrpos = ((y)*width + (x)) + 1;//��������е�λ�ã�
			INT deltaarrpos = y*grawidth + x;//���ݶ������е�λ�ã�
			//������㣻
			deltaxarr[deltaarrpos] = (INT) ( (
				image[((y-1)*width + (x+1))] //����
				+ image[((y)*width + (x+1))] //��
				+ image[((y+1)*width + (x+1))] //����
				- image[((y-1)*width + (x-1))] //����
				- image[((y)*width + (x-1))] //��
				- image[((y+1)*width + (x-1))] ) / 3 );//����
			deltayarr[deltaarrpos] = (INT) ( ( 
				image[((y-1)*width + (x+1))] //����
				+ image[((y-1)*width + (x))] //��
				+ image[((y-1)*width + (x-1))] //����
				- image[((y+1)*width + (x-1))] //����
				- image[((y+1)*width + (x))] //��
				- image[((y+1)*width + (x+1))]) / 3 );//����
		}
	}

	for (y=0; y<graheight; y++)
	{
		for (x=0; x<grawidth; x++)
		{
			INT temppos = y*grawidth + x;
			if ( (deltaxarr[temppos])==0 )
			{
				if (deltayarr[temppos]!=0)
				{
					deltasita[temppos] = 0;//ˮƽ����;
					deltar[temppos] = (FLOAT) abs(deltayarr[temppos]);
				}else
				{
					deltasita[temppos] = -1;//��ȷ������;
					deltar[temppos] = (FLOAT) abs(deltayarr[temppos]);
				}
				continue;
			}
			
			FLOAT tempf = (FLOAT) atan( (FLOAT)deltayarr[temppos]
				/ (FLOAT)deltaxarr[temppos] );
			deltasita[temppos] = (FLOAT) ( tempf + PI/2. );
			deltar[temppos] = (FLOAT) sqrt((DOUBLE) 
				( deltayarr[temppos]*deltayarr[temppos]
				+ deltaxarr[temppos]*deltaxarr[temppos] ) );
		}
	}

	delete [] deltaxarr; deltaxarr = NULL; //ɾ��ˮƽ�ʹ�ֱ�ݶ����飻
	delete [] deltayarr; deltayarr = NULL;
}
