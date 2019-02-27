#ifndef _GLOBAL_API
#define _GLOBAL_API

//#include "stdafx.h"
#include <math.h>
#include "mymath.h"
#include "MyColorSpace.h"
#include "Thinner.h"
#include "MMOFUNC.h"

//#include "ColorTable.h"
//#include <complex>
//using namespace std;
typedef struct tagMyFeatureVector{
	CString classname;
	DOUBLE colorl;
	DOUBLE colorlw;
	DOUBLE coloru;
    DOUBLE coloruw;
	DOUBLE colorv;
	DOUBLE colorvw;
	DOUBLE minmaxtexl;
	DOUBLE minmaxtexlw;
	DOUBLE minmaxtexu;
	DOUBLE minmaxtexuw;
	DOUBLE minmaxtexv;
	DOUBLE minmaxtexvw;
}MyFeatureVector;	

typedef struct tagMyPtNei{
	BYTE* ptneiarr;
}MyPtNei;

typedef struct tagRgnInfo{
	BOOL  isflag;//���ñ�ǣ�
	INT   ptcount;//���������ĵ�����
	FLOAT   l;//����l��ֵ
	FLOAT   u;//����u��ֵ
	FLOAT   v;//����v��ֵ
}MyRgnInfo;//�ָ���������һЩͳ����Ϣ,ͼ���и��������������Ϣ�����flag�����У�

//void RoadThin(BYTE*roadseed, int Width,int Height,vector<int>&terminal);

	BYTE* EdgeSusan(BYTE*,int w,int h,int=20,bool=false);
	void Morph(BYTE*,int wid, int heg,int opt);

void GaussianFilter(BYTE*,int Width, int Height,float sigma);

void EdgeMag(BYTE*,int w,int h);	
void EdgePoints(BYTE* ,int w,int h,float sig=0.8f, float low=0.4f, float high=0.79f);
MyLUV*  GetNearPixelsLUV(int xPos, int yPos
	  , MyLUV* inLUVs, int picWidth, int picHeight
	  , int inScale, int& outWidth, int& outHeight);
//�õ�LUV����


void  GetNearPixelsGreenExt(int xPos, int yPos
	    , BYTE* inPixels, int picWidth, int picHeight
	    , int radius, BYTE** outArr);
//�õ���������ֵ(������,Gͨ��),����λ�ô�0��ʼ����, ��Ե���Գ����أ�



void  GetNearPixelsExt(int xPos, int yPos
	, BYTE* inPixels, int picWidth, int picHeight
	, int radius, BYTE** outArr);
//�õ���������ֵ(������),����λ�ô�0��ʼ����, ��Ե�����أ�



BYTE*  GetNearPixels(int xPos, int yPos, 
      BYTE* inPixels, int picWidth, int picHeight, int inScale, 
	  int& outWidth, int& outHeight);
//�õ���������ֵ, ����λ�ô�0��ʼ������
void GetGradient(BYTE* image, int width, int height
		, FLOAT* deltar, FLOAT* deltasita);
void  AddBNeiToANei(int curid, int nearid, CString* neiarr, int* mergearr);
//��nearid������ӵ�curid��������ȥ��



int  FindNearestNei(int curid, CString neistr, MyRgnInfo* rginfoarr, int* mergearr);
//Ѱ��neistr����curid��ӽ����������ظ���id�ţ�


int  FindMergedRgnMaxbias(int idint, int* mergearr, int bias);
//����ֵ��ֹ���Һϲ���������coarse watershed, 
//�����߱��뱣֤idint��Ч������mergearr[idint]>0��
//�Լ�mergearr��Ч������mergearr[idint]<idint;


int  FindMergedRgn(int idint, int* mergearr);
//�ҵ�idint�������ϲ��������ţ�


void  AddNeiRgn(int curid, int neiid, CString* neiarr);
//����neiidΪcurid��������


void  AddNeiOfCur(int curid, int left, int right, int up, int down, int* flag, CString* neiarr);
//ˢ�µ�ǰ���������������

//////////////////////////////////////////////////////////////////////////
// Luc Vincent and Pierre Soille�ķ�ˮ��ָ�flood�����ʵ�ִ��룬 
// �޸�����Ӧα����, α���������������ġ�Watersheds in Digital Spaces:
// An Efficient Algorithm Based on Immersion Simulations��
// IEEE TRANSACTIONS ON PATTERN ANALYSIS AND MACHINE INTELLIGENCE.
// VOL.13, NO.6, JUNE 1991;
// by dzj, 2004.06.28 
// MyImageGraPt* imiarr - ��������������
// int* graddarr -------- ����ĸ��ݶ����飬�ɴ�ֱ�Ӵ�ȡ��H���ص�
// int minh��int maxh == ��С����ݶ�
// int* flagarr --------- ����������
// ע�⣺Ŀǰ�����ˮ���ǣ�ֻ��ÿ��������������
//////////////////////////////////////////////////////////////////////////
void FloodVincent(MyImageGraPt* imiarr, int imageWidth,int imageHeight,int* graddarr, int minh, int maxh, int* flagarr, int& outrgnumber);

//�ҵ�idint�������ϲ��������ţ�
int FindMergedRgnMaxbias(int idint, int* mergearr, int bias);
#define NearMeasureBias 200.0//�ж�������ɫ���Ƶ���ֵ��
void  MergeRgs(MyRgnInfo* rginfoarr, int rgnumber, int* flag, int width, int height, int* outmerge, int& rgnum);
//�ϲ���������


void  MergeNearest(int curid, MyRgnInfo* rginfoarr, CString* neiarr, int* mergearr);
//�ϲ���������


void  MergeTwoRgn(int curid, int nearid
	, CString* neiarr, MyRgnInfo* rginfoarr, int* mergearr);
//��nearid�ϲ���curid��ȥ�����ºϲ��������Ϣ������¼�úϲ���



//----------------------------------------------------------------------
/* DIB������
BOOL DIBShow(CFile* pFile);
BOOL ConvertDDBToDIB(HBITMAP hBitmap, CDIB* pDibDest, HPALETTE hPal);
BOOL CopyDIB(CDIB* pDibSrc, CDIB* pDibDst);
BOOL CopyScreenToDIB(LPRECT lpRect, CDIB* pDibDest);*/
HPALETTE GetSystemPalette() ;
HPALETTE CopyPalette(HPALETTE hPalSrc);
BOOL ReadDIB(CFile* pFile, LPBITMAPINFOHEADER lpBMIH, LPBYTE lpImage);
HPALETTE MakeDIBPalette(LPVOID lpvColorTable,  LPBITMAPINFOHEADER lpBMIH);

//----------------------------------------------------------------------
// �˶����
BOOL GetBackground(CString strFilePath, int nTotalFrameNum, int nImageWidth, 
		   int nImageHeight, unsigned char* pUnchBackGround);
//BOOL LoadDibSeq(CString strFilePath, int nCurFrameNum, int nTotalFrameNum, CDIB* pDib);
CString GetFileName(CString strFilePathName, int nCurFrameNum);
void BinaFrameDiff(unsigned char *pUnchImg1, unsigned char *pUnchImg2, 
		  int nWidth, int nHeight, unsigned char * pUnchResult,
		  int nThreshold=10);
void ErodeFrameDiff(unsigned char *pUnchImg, int nWidth, int nHeight, int nErodeHalfWin, 
	   int nErodeThreshold, unsigned char *pUnchResult);

//----------------------------------------------------------------------
/* ͼ��ָ�
void RegionSegFixThreshold(CDIB * pDib, int nThreshold=100);
void RobertsOperator(CDIB * pDib, double * pdGrad);
void LaplacianOperator(CDIB * pDib, double * pdGrad);
void EdgeTrack(CDIB * pDib, unsigned char * pUnEdgeTrack);
void RegionGrow(CDIB * pDib, unsigned char * pUnRegion, int nThreshold = 10);
void RegionSegAdaptive(CDIB * pDib) ;
void SobelOperator(CDIB * pDib, double * pdGrad);
void PrewittOperator(CDIB * pDib, double * pdGrad);*/
	// canny ����
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, double sigma, unsigned char * pUnchSmthdImg);
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize) ; 
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
						 int *pnGradX , int *pnGradY);
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag) ; 
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, int nHeight,	unsigned char *pUnchRst);
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 unsigned char * pUnchEdge, double dRatioHigh, double dRationLow) ;
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge) ;
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, unsigned char *pUnchEdge) ;
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) ;
void Gradient(unsigned char*pImage, int nWidth, int nHeight,float* pEdge);

// С���任����ԭ��
// Daubechies��������С����
// ��֧ͬ�����䳤���µ��˲���ϵ������
const double hCoef[10][20] =
{
	{ .707106781187,  .707106781187},

	{ .482962913145,  .836516303738,  .224143868042, -.129409522551 },

	{ .332670552950,  .806891509311,  .459877502118, -.135011020010, -.085441273882,  .035226291882 },

	{ .230377813309,  .714846570553,  .630880767930, -.027983769417,
	 -.187034811719,  .030841381836,  .032883011667, -.010597401785 },

	{ .160102397974,  .603829269797,  .724308528438,  .138428145901, -.242294887066,
	 -.032244869585,  .077571493840, -.006241490213, -.012580751999,  .003335725285 },

	{ .111540743350,  .494623890398,  .751133908021,  .315250351709, -.226264693965,
	 -.129766867567,  .097501605587,  .027522865530, -.031582039318,  .000553842201,
	  .004777257511, -.001077301085 },

	{ .077852054085,  .396539319482,  .729132090846,  .469782287405, -.143906003929,
	 -.224036184994,  .071309219267,  .080612609151, -.038029936935, -.016574541631,
	  .012550998556,  .000429577973, -.001801640704,  .000353713800 },

	{ .054415842243,  .312871590914,  .675630736297,  .585354683654, -.015829105256,
	 -.284015542962,  .000472484574,  .128747426620, -.017369301002, -.044088253931,
	  .013981027917,  .008746094047, -.004870352993, -.000391740373,  .000675449406,
	 -.000117476784 },

	{ .038077947364,  .243834674613,  .604823123690,  .657288078051,  .133197385825,
	 -.293273783279, -.096840783223,  .148540749338,  .030725681479, -.067632829061,
	  .000250947115,  .022361662124, -.004723204758, -.004281503682,  .001847646883,
	  .000230385764, -.000251963189,  .000039347320 },

	{ .026670057901,  .188176800078,  .527201188932,  .688459039454,  .281172343661,
	 -.249846424327, -.195946274377,  .127369340336,  .093057364604, -.071394147166,
	 -.029457536822,  .033212674059,  .003606553567, -.010733175483,  .001395351747,
	  .001992405295, -.000685856695, -.000116466855,  .000093588670, -.000013264203 }
};

BOOL DWT_1D(double* pDbSrc, int nMaxLevel,int nDWTSteps, int nInv, int nStep, int nSupp);
BOOL DWTStep_1D(double* pDbSrc, int nCurLevel,int nInv, int nStep, int nSupp);
BOOL DWT_2D(double* pDbSrc, int nMaxWLevel, int nMaxHLevel, int nDWTSteps, int nInv, int nStep, int nSupp);
BOOL DWTStep_2D(double* pDbSrc, int nCurWLevel, int nCurHLevel,	int nMaxWLevel, int nMaxHLevel, int nInv, int nStep, int nSupp);	
BOOL ImageDWT(LPBYTE lpImage, int nMaxWLevel, int nMaxHLevel,int nDWTSteps, int nInv, int nStep, int nSupp);
int Log2(int n);
BYTE FloatToByte(double f);
char FloatToChar(double f);


//----------------------------------------------------------------------
/* ͼ����ǿ����
BOOL GraySegLinTrans(CDIB* pDib, int nX1, int nY1, 
					 int nX2, int nY2);					// �ֶ����Ա任
BOOL HistogramEqualize(CDIB* pDib);						// ֱ��ͼ���⻯
BOOL  GeneralTemplate(CDIB* pDib,  int nTempWidth, int nTempHeight,	
					 int nTempCenX, int nTempCenY,
					 double* pdbTemp, double dbCoef);	// ͨ��ģ�����
BOOL MedianFilter(CDIB* pDib,  int nTempWidth, int nTempHeight, 
				 int nTempCenX, int nTempCenY);			// ��ֵ�˲�
unsigned char  GetMedianValue(unsigned char * pUnchFltValue, int iFilterLen);	// ��ȡ��ֵ
BOOL  ReplaceDIBColorTable(CDIB* pDib, LPBYTE  pColorsTable);			// �滻DIB��ɫ��*/

void LowPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius) ;// ��ͨ�˲�
void HighPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius);// ��ͨ�˲�
void ButterWorthLowPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius);	//  ButterWorth��ͨ�˲�
void ButterWorthHighPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius);	//  ButterWorth��ͨ�˲�
void LinearSharpen (LPBYTE lpImage, int nWidth, int nHeight);			//  ��������ǿ

//-----------------------------------------------------------------------
// imageview.cpp����ԭ��
//BOOL LimbPatternBayer(CDIB *pDib);
//BOOL DitherFloydSteinberg(CDIB *pDib);

#endif