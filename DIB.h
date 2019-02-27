/*/////////////////////////////////////////////////////////////
����һ�� DIBλͼ�����࣬�����漸���ڲ����������� DIBλͼ��Ϣ��
	BYTE *m_lpBits; ---------- ����
	BYTE *m_lpPalette; ------- ��ɫ�壻����24λλͼ����ָ����NULL
	int m_nWidth; ------------ λͼ���
	int m_nHeight; ----------- λͼ�߶�
	int m_nBitCount; --------- ÿ�������õ�λ��

����λͼ�ĺ����У�
	BOOL LoadFromFile(LPCTSTR filename);
	        ------ �� BMP�ļ��ж���ͼ��
	BOOL ImportPcx(char *fn);
	        ------ �� PCX�ļ��ж���ͼ�񣻼��� pcx��ʽ����

��DIB λͼ����ĺ����У�
	BOOL Grey();			//ת���ɻҶ�ͼ��
	BOOL HFlip();			//ͼ�����ת
	BOOL VFlip();			//ͼ������ת
	BOOL Rotate();			//˳ʱ����ת90��
	BOOL Negative();		//ת�ɸ���
/*////////////////////////////////////////////////////////////*/

#ifndef __DIB_H
#define __DIB_H
//#include <windows.h>
#include <afx.h>
//#include <stdlib.h>


#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
// ����ͼ��ÿ��������ռ���ֽ���Ŀ 
#define BYTE_PER_LINE(w, c) ((((w)*(c)+31)/32)*4)
// ���ݱ�ʾ��ɫ��λ����ȷ���ܱ�ʾ����ɫ��
// �����24λ��ɫ��Ϊ0�������8λ��ɫ��Ϊ256�������4λ��ɫ��Ϊ16
#define PALETTESIZE(b) (((b)==8)?256:(((b)==4)?16:0))

class CDIB
{
//	friend class CHC;
public:	
	BYTE *m_lpBits;		// ͼ������ָ��
	BYTE *bpBits;//back up image data
	BYTE *m_lpPalette;	// ��ɫ��ָ��
	int m_nBitCount;	// ͼ���б�ʾÿ�������õ�λ��
	int m_nWidth;		// ͼ��Ŀ�ȣ���λ������
	int m_nHeight;		// ͼ��ĸ߶ȣ���λ������
	int m_nColors;//number of colors in palette 
	int m_nPlanes; //
public:	
	void SetPixelColor2(int x,int y,BYTE color);
	void SetPixelIndex(int x,int y,BYTE index);
	BYTE GetNearestIndex(RGBQUAD color);
	void SetPixelColor(int x,int y,RGBQUAD color);
	bool CreateDIBFromBits(int nWidth,int nHeight,BYTE * buf,int=24);
	RGBQUAD GetPaletteColor(BYTE idx);
	BYTE GetPixelIndex(int x,int y);
	BYTE GetPixelColor2(int x,int y);
	RGBQUAD GetPixelColor(int x,int y);
	int CreateDIBFromIPL(int, int ,char*,int bits);
	int LoadDIBToIPL(char*imageData, int bits);
	int LoadDIBToBuf(BYTE* buf);
	int LoadDIBToBuf(float *buf);
	bool SaveToFile(const CString&,CString&);
	int lookregion(int label,int*tag, CRect rect,int area);
	void ShowReg(int*tag=0,int step=32);
	// ��Dibλͼ������䵽��CDib������ݽṹ
	BOOL CreateDIB(int r, int g, int b,const CString&);
	BOOL LoadDib(BYTE* lpDib);
	// ��CDib������ݽṹ�е�����ת��ΪDibλͼ��ʽ����
	BOOL SaveToDib(BYTE *lpDib);
	//void PlotRegions();
	BOOL SaveEdge(LPCTSTR fn,BYTE*EM);
	void LookRegions(int*tag,int option=0);
	void Origin();

//	void MapSegImage();
	void MakeGrayPalette(int BitCount);
	void histovector(float*hive);
	BOOL InteEqualize();
	CDIB();		// CDib��Ĺ��캯��

	CDIB( const char *pszFileName);
	~CDIB();	// CDib�����������

	// ���CDib�����й�λͼ������
	void Clear();
	// ���λͼ�����Ƿ�գ���ͼ��
	// ��ȡͼ����
	int GetWidth()	{return m_nWidth;}
	// ��ȡͼ��߶�
	int GetHeight()	{return m_nHeight;}
	int GetLength()	{return m_nHeight*m_nWidth;}
	// ��ȡͼ���б�ʾÿ�������õ�λ��
	int GetBitCount()	{return m_nBitCount;}
	int GetWidthInBytes( int nBits, int nWidth );

	// ����λͼ���ݻ���λͼ
	int Stretch(HDC,int,int,int,int,UINT,DWORD);
	// ��λͼ�ļ��ж�ȡλͼ���ݣ����ݴ򿪵��ļ�ָ�룩
	BOOL LoadFromFile(CFile *pf);
	// ��λͼ�ļ��ж�ȡλͼ���ݣ������ļ�����

	BOOL LoadFromFile(LPCTSTR filename);
	// ��λͼ���ݱ��浽λͼ�ļ������ݴ򿪵��ļ�ָ�룩
	BOOL SaveToFile(CFile *pf);
	// ��λͼ���ݱ��浽λͼ�ļ��������ļ�����
	BOOL SaveToFile(LPCTSTR fn);
	// ����ָ���ļ�������PCX��ʽ��ͼ���ļ�������

	// ��ͼ�����ݸ��Ƶ�������
	BOOL Copy();
	// ���Ƽ������е�����
	BOOL Paste();

	// ��ͼ��ת��Ϊ�Ҷ�ͼ��
	BOOL Grey();
	// ����תͼ��
	BOOL HFlip();
	// ����תͼ��	
	BOOL VFlip();
	// ��ͼ��˳ʱ����ת90��
	BOOL Rotate();
	// ��ͼ��ת��Ϊ����
	BOOL Negative();
private:
	CDIB(const CDIB&);
	CDIB& operator=(const CDIB&);
};

void ImageProps(int&, CString&,const CString&fn);
//input:allocated EM 
//output: EM initiated with data for edge detection
BYTE* GetEM(const CString&fn, BYTE *EM,int eB);
#endif //dib.h
