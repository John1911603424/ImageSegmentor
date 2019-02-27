// MyColorSpace.h: interface for the MyColorSpace class.
// 04.05;
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCOLORSPACE_H__88292E4A_088A_45EB_8DCF_38694FB6190F__INCLUDED_)
#define AFX_MYCOLORSPACE_H__88292E4A_088A_45EB_8DCF_38694FB6190F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mymath.h"

#define  PI 3.14159265359

typedef struct tagMyRGB{
	FLOAT r;
	FLOAT g;
	FLOAT b;
}MyRGB;	

typedef struct tagMyHSV{
	FLOAT h;
	FLOAT s;
	FLOAT v;
}MyHSV;	

typedef struct tagMyLUV{
	FLOAT l;
	FLOAT u;
	FLOAT v;
}MyLUV;	

//һάֱ��ͼ��Ԫ��
typedef struct tagD1ColorBinUnit{
	MyHSV  hsv;
	FLOAT count;
}D1ColorBinUnit;

//һάֱ��ͼ��
typedef struct tagD1ColorBin{
	D1ColorBinUnit*  binunits;
	INT bincount;//ֱ��ͼ��BIN����
}D1ColorBin;


typedef struct tagHsvBelongTo{
	MyHSV hsv;
	INT belongto;
}HsvBelongTo;

//ֱ��ͼBIN����
#define CBDIM 72

class MyColorSpace  
{
public:
	MyColorSpace();
	virtual ~MyColorSpace();

	BOOL Rgb2Hsi(FLOAT r, FLOAT g, FLOAT b, FLOAT& h, FLOAT& s, FLOAT& i);//������˹������ͼ�����ڶ��棬p235-p237��
	BOOL Hsi2Rgb(FLOAT h, FLOAT s, FLOAT i, FLOAT& r, FLOAT& g, FLOAT& b);//������˹������ͼ�����ڶ��棬p237��
	BOOL Rgb2Ycbcr(FLOAT r, FLOAT g, FLOAT b, FLOAT& y, FLOAT& cb, FLOAT& cr);//�ݡ�Color and Texture Descriptors��by B.S.Manjunath,eg.
	BOOL Ycbcr2Rgb(FLOAT y, FLOAT cb, FLOAT cr, FLOAT& r, FLOAT& g, FLOAT& b);//Rec 601-1 YCbCr to RGB, ����David Bourgin, Color space FAQ
	BOOL Rgb2Hsv(FLOAT r, FLOAT g, FLOAT b, FLOAT& h, FLOAT& s, FLOAT& v);//�ݡ�Color and Texture Descriptors��by B.S.Manjunath,eg.
	BOOL Rgb2Hsv2(FLOAT r, FLOAT g, FLOAT b, FLOAT& h, FLOAT& s, FLOAT& v);//Ҧ�ӳ����е�ת�������������еĵĲ�һ��������
	DOUBLE GetHsvDistance(MyHSV x, MyHSV y);
	//BOOL Luv2Rgb(FLOAT r, FLOAT g, FLOAT b, FLOAT& l, FLOAT& u, FLOAT& v);
	BOOL Rgb2Xyz(FLOAT r, FLOAT g, FLOAT b, FLOAT& x, FLOAT& y, FLOAT& z);//RGB to CIE XYZitu(D65),����David Bourgin, Color space FAQ
	BOOL Xyz2Rgb(FLOAT x, FLOAT y, FLOAT z, FLOAT& r, FLOAT& g, FLOAT& b);//CIE XYZitu(D65) to RGB,����David Bourgin, Color space FAQ
	BOOL Xyz2Luv(FLOAT x, FLOAT y, FLOAT z, FLOAT& l, FLOAT& u, FLOAT& v);//CIE XYZitu(D65) to CIE Luv,����David Bourgin, Color space FAQ
	BOOL Xyz2Lab(FLOAT x, FLOAT y, FLOAT z, FLOAT& l, FLOAT& a, FLOAT& b);//CIE XYZitu(D65) to CIE Lab, ����David Bourgin, Color space FAQ
	BOOL Lab2Lch(FLOAT li, FLOAT a, FLOAT b, FLOAT& lo, FLOAT& c, FLOAT& h);//CIE Lab to CIE LCH, ����David Bourgin, Color space FAQ

	BOOL RgbtoHsv(BYTE* inDatas, int width, int height, MyHSV* hsvbuff);//��λͼͼ������ת��ΪHSV���ݣ�
	BOOL RgbtoXyz(BYTE* inDatas, int width, int height, FLOAT* xyzbuff);//��λͼͼ������ת��ΪHSI���ݣ�
	BOOL XyztoRgb(FLOAT* xyzDatas, int width, int height, BYTE* rgbDatas);
	BOOL RgbtoLuv(BYTE* inDatas, int width, int height, MyLUV* luvbuff);//��λͼͼ������ת��ΪLUV���ݣ�

	BOOL RgbtoLuvPcm(BYTE* inDatas, int width, int height, MyLUV* luvbuff);//���ڱ�ת������λͼͼ������ת��ΪLUV���ݣ�

	BOOL Luv2Rgb(FLOAT l, FLOAT u, FLOAT v, INT& r, INT& g, INT& b);//LUV to RGB;
	BOOL LuvToRgb(FLOAT* luvData, INT width, INT height, BYTE* rgbData);

private:
	MyMath myMath;
	FLOAT Labf(FLOAT input, FLOAT Y_Yn);//���ڼ���Labɫ�ʣ�
};

#endif // !defined(AFX_MYCOLORSPACE_H__88292E4A_088A_45EB_8DCF_38694FB6190F__INCLUDED_)
