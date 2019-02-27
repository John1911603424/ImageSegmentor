#include "stdafx.h"
#include "DIB.h"
#include "..\\include\\gdal.h"
#include "..\\include\\gdal_priv.h"
#include "..\\include\\ogrsf_frmts.h"
#include "..\\include\\cpl_string.h"
#include <assert.h>
//retrieve bandcount and datatype of a gdaldataset in file named fn
int ImageProps(int&count, CString&dataType, const CString&fn){
	GDALDataset*pDataset=(GDALDataset*)GDALOpen(fn,GA_ReadOnly);
	if(pDataset==0){
		 AfxMessageBox("Failed to open image!");
		 return 0;
	}
	count=pDataset->GetRasterCount();
	GDALRasterBand  *m_pBand= pDataset->GetRasterBand(1);
	//	CPLAssert( m_pBand->GetRasterDataType() == GDT_Byte );
	dataType=GDALGetDataTypeName(m_pBand->GetRasterDataType());//��������
	GDALClose((GDALDatasetH)pDataset);
	return 1;
}
//input:allocated EM 
//output: EM initiated with data for edge detection
BYTE* GetEM(const CString&fn, BYTE *EM,int eB)
{
	GDALDataset*pDataset=(GDALDataset*)GDALOpen(fn, GA_ReadOnly);
	int m_CurrentBand=1;
	if (eB<= 0 || eB>pDataset->GetRasterCount())
		eB = m_CurrentBand;
	int w=pDataset->GetRasterXSize(); //Ӱ��ĸ߶ȣ����
	int	h=pDataset->GetRasterYSize();

	BYTE* buf =	EM;
	GDALRasterBand  *m_pBand=NULL;
	m_pBand = pDataset->GetRasterBand(eB);
	if (m_pBand)
	{
		if (CE_None!=m_pBand->RasterIO( GF_Read, 0,0, w, h, buf, w,h, GDT_Byte, 0, 0 ))
		{
			AfxMessageBox("Error getting edge data!");
		}
	}
	GDALClose((GDALDatasetH)pDataset);
	return EM;
}
// ���캯������ʼ��CDIB���������
CDIB::CDIB():m_lpBits(NULL),m_lpPalette(NULL),m_nBitCount(0),
m_nWidth(0),m_nHeight(0),m_nColors(0),m_nPlanes(0)
{

}
CDIB::CDIB( const char *pszFileName):m_lpBits(NULL),m_lpPalette(NULL),m_nBitCount(0),m_nPlanes(0)
{
	m_nWidth = m_nHeight = m_nBitCount = m_nColors = 0;

	LoadFromFile( pszFileName);
}
/*CDIB::CDIB(const CDIB&cpy){
m_lpBits(cpy.m_lpBits),m_lpPalette(NULL),m_nBitCount(0),m_nPlanes(cpy)

	m_nWidth = m_nHeight = m_nBitCount = m_nColors = 0;
}*/

// ��������
CDIB::~CDIB()
{
	Clear();
}

// �����ǰ��ͼ�����ݣ����ͷ��ڴ�
void CDIB::Clear()
{

	// �ͷ�λͼ����
	if (m_lpBits) 
		delete[] m_lpBits;
	m_lpBits=NULL;
		// �Ƿ��ɫ������
	if (m_lpPalette) 
		delete[] m_lpPalette;
	m_lpPalette = NULL;
	// ���λͼ��Ϣ
	m_nWidth = m_nHeight = 0;
	m_nBitCount = 0;
}

// 	��λͼ���ݱ��浽λͼ�ļ��������ļ�����
BOOL CDIB::SaveToFile(LPCTSTR fn /*�ļ���*/)
{
	CFile f;
	
	// ������ܴ�ָ�����Ƶ��ļ�������
	if (!f.Open(fn, CFile::modeCreate|CFile::modeWrite)) 
		return FALSE;
	
	// ���򣬱���λͼ���ݵ��ļ�
	BOOL r = SaveToFile(&f);

	// �ر��ļ�
	f.Close();
	return r;
}

// ��λͼ�ļ��ж�ȡλͼ���ݣ������ļ�����
BOOL CDIB::LoadFromFile(LPCTSTR filename /*�ļ���*/)
{
	CFile f;

	// ������ܴ�ָ�����Ƶ��ļ�������
	if (!f.Open(filename, CFile::modeRead)) 
		return FALSE;
	
	// ���򣬱���λͼ���ݵ��ļ�
	BOOL r = LoadFromFile(&f);

	// �ر��ļ�
	f.Close();
	return r;
}

// ��λͼ���ݱ��浽λͼ�ļ������ݴ򿪵��ļ�ָ�룩
BOOL CDIB::SaveToFile(CFile *pf)
{
	// ���ͼ������Ϊ�գ�����
	if (m_lpBits == NULL) 
		return FALSE;

	// ��ȡͼ���ļ���ÿ��ͼ����ռ�ֽ���
	int nByteWidth = BYTE_PER_LINE(m_nWidth, m_nBitCount);

	// ���λͼ�ļ�ͷ�ṹ��ָ���ļ������Ϣ
	BITMAPFILEHEADER bm;
	
	// ָ���ļ�����Ϊλͼ
	bm.bfType = 'M'*256+'B';
	// ָ��λͼ�ļ��Ĵ�С
	bm.bfSize = nByteWidth*m_nHeight;
	// �����ĽṹԪ�أ�����Ϊ0
	bm.bfReserved1 = 0;
	bm.bfReserved2 = 0;
	// ������ļ�ͷ��ʼ��ʵ�ʵ�ͼ������֮���ƫ�������ֽ�����
	bm.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	// �������24λ���ɫλͼ�����ϵ�ɫ����Ϣ�ĳ���
	if (m_nBitCount < 24)
		bm.bfOffBits += PALETTESIZE(m_nBitCount)*sizeof(RGBQUAD);

	// ���ļ���д��λͼ�ļ�ͷ��Ϣ
	pf->Write(&bm, sizeof(BITMAPFILEHEADER));

	// ���λͼ��Ϣͷ�ṹ��ָ��λͼ�Ĵ�С����ɫ��Ϣ
	BITMAPINFOHEADER bmi;
	// ָ��λͼ��Ϣͷ�ṹ�Ĵ�С
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	// ָ��λͼ�Ŀ��
	bmi.biWidth = m_nWidth;
	// ָ��λͼ�ĸ߶�
	bmi.biHeight = m_nHeight;
	// Ŀ���豸��λ��������ֵ����Ϊ1
	bmi.biPlanes = 1;
	// ָ����ʾ��ɫʱ�õ���λ����
	// ���õ�ֵΪ1(�ڰ׶�ɫͼ)��4(16ɫͼ)��8(256ɫͼ)��24(���ɫͼ)
	bmi.biBitCount = (m_nBitCount);
	// ˵��û��ѹ��ͼ������
	bmi.biCompression = BI_RGB;
	// ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ�����
	// ����BI_RGB��ʽʱ��������Ϊ0 
	bmi.biSizeImage = 0;
	// ָ��Ŀ���豸��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
	bmi.biXPelsPerMeter = 0;
	// ָ��Ŀ���豸�Ĵ�ֱ�ֱ��ʣ�������/�ױ�ʾ
	bmi.biYPelsPerMeter = 0;
	// ָ��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ���
	bmi.biClrUsed = 0;
	// ָ����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��
	bmi.biClrImportant = 0;
	// ��λͼ��Ϣͷд���ļ�
	pf->Write(&bmi,sizeof BITMAPINFOHEADER);
	
	// �������24λ���ɫλͼ������ɫ����Ϣд���ļ�
	if (m_nBitCount<24)
		pf->Write(m_lpPalette, PALETTESIZE(m_nBitCount)*sizeof(RGBQUAD));
	
	// ��λͼ����д���ļ�
	pf->Write(m_lpBits, nByteWidth*m_nHeight);

	return TRUE;
}

// ��λͼ�ļ��ж�ȡλͼ���ݣ����ݴ򿪵��ļ�ָ�룩
BOOL CDIB::LoadFromFile(CFile *pf)
{
	BYTE *lpBitmap,*lpDib;
	unsigned int nLen;

	// ��ȡλͼ�ļ��Ĵ�С
	nLen = (unsigned int)pf->GetLength();

	// ���䱣��λͼ���ݵ��ڴ�ռ�
	lpBitmap = new BYTE[nLen];

	// ���ļ��ж���λͼ����
	pf->Read(lpBitmap, nLen);

	// ����ļ�������
	// �������λͼ�ļ�����ʾ�����ͷ��ڴ�
	if (lpBitmap[0]!='B' && lpBitmap[1]!='M')
	{
		AfxMessageBox("��λͼ�ļ�");
		delete[] lpBitmap;
		return FALSE;
	}

	// �����ҵ�λͼ��Ϣͷ����ʼλ��
	lpDib = lpBitmap+sizeof(BITMAPFILEHEADER);
	
	// ��DIBλͼ������䵽��CDib������ݽṹ
	BOOL ret = LoadDib (lpDib);
	
	// �ͷ��ڴ�
	delete[] lpBitmap;
	
	return ret;
}

// ��Dibλͼ������䵽��CDib������ݽṹ
BOOL CDIB::LoadDib(BYTE *lpDib)
{
	// �����ǰ��ͼ�����ݣ����ͷ�����ڴ�
 	Clear();
	
	BYTE *lpBits;
	BITMAPINFOHEADER *pInfo;
	int nWidth,nHeight,nBitCount,nByteWidth;
	RGBQUAD *pPalette;
	unsigned int PaletteSize;

	// ��ȡλͼ��Ϣͷ��ָ��
	pInfo = (BITMAPINFOHEADER *)lpDib;
	// ��ȡλͼ���ݵ�ָ��
	lpBits = lpDib+sizeof(BITMAPINFOHEADER);

	// ��ȡλͼ�Ŀ��
	nWidth = pInfo->biWidth;
	// ��ȡλͼ�ĸ߶�
	nHeight = pInfo->biHeight;
	// ��ȡλͼ��ʾ��ɫ���õ�λ��
	nBitCount = pInfo->biBitCount;
	// ����λͼÿ��������ռ���ֽ���Ŀ 
	nByteWidth = BYTE_PER_LINE(nWidth, nBitCount);
	m_nPlanes = pInfo->biPlanes;

	m_nColors = 1 << nBitCount;

//	DWORD tmp = pBIH->biClrUsed;
//
//    LONG PIX = pBIH->biXPelsPerMeter;
//	LONG PIY = pBIH->biYPelsPerMeter;


	if( m_nPlanes > 1 )
		m_nColors <<= ( m_nPlanes - 1 );
	if( nBitCount >= 16 )
		m_nColors = 0;
	// ���λͼ����ɫ��
	switch (nBitCount)
	{
		// �����24λ��ɫ��λͼ��û�е�ɫ����Ϣ
	    case 32:
		case 24:
			// �ͷ���ǰ��λͼ���ݺ͵�ɫ������ռ�е��ڴ�
			if (m_lpBits) 
				delete[] m_lpBits;
			if (m_lpPalette) 
				delete[] m_lpPalette;
			m_lpPalette = NULL;

			// ����Ϊλͼ�������ݷ����ڴ�
			m_lpBits= new BYTE[nByteWidth*nHeight];
			// ��λͼ�������ݸ��Ƶ�m_lpBits
			memcpy(m_lpBits, lpBits, nByteWidth*nHeight);				
			break;

		// �����8λ����4λ��ɫ��λͼ���е�ɫ����Ϣ
		case 8:
		case 4:
			// �ͷ���ǰ��λͼ���ݺ͵�ɫ������ռ�е��ڴ�
			if (m_lpBits) 
				delete[] m_lpBits;
			if (m_lpPalette) 
				delete[] m_lpPalette;

			// �����ɫ����ʵ����ɫ����
			PaletteSize = (1<<pInfo->biBitCount);
			// ���pInfo->biClrUsed������0��
			// ʹ��pInfo->biClrUsedָ����λͼʵ��ʹ�õ���ɫ��
			if (pInfo->biClrUsed!=0 && pInfo->biClrUsed<PaletteSize) 
				PaletteSize = pInfo->biClrUsed;
			
			// ��ȡ��ɫ����Ϣ����
			pPalette = (RGBQUAD *)lpBits;
			// ��ָ���ƶ����������ݵĿ�ʼ��
			lpBits += sizeof(RGBQUAD)*PaletteSize;

			// Ϊ�����ɫ����Ϣ���ݵ�m_lpPalette����ռ�
			m_lpPalette = new BYTE[sizeof(RGBQUAD)*PaletteSize];
			// ����ɫ����Ϣ���ݸ��Ƶ�m_lpPalette
			memcpy(m_lpPalette, pPalette, sizeof(RGBQUAD)*PaletteSize);

			// Ϊ����λͼ�������ݵ�m_lpBits����ռ�
			m_lpBits = new BYTE[nByteWidth*nHeight];
			// ��λͼ���ظ��Ƶ�m_lpBits
			memcpy(m_lpBits, lpBits, nByteWidth*nHeight);
			break;

		// ������������账��
		default:
			return FALSE;
	}
	unsigned int spp=nBitCount/8;

/*	redptr=new float[nHeight*nWidth];
	greenptr=new float[nHeight*nWidth];
	blueptr=new float[nHeight*nWidth];
	alphaptr=new int[nHeight*nWidth];
	uint32 temp=0;
	uint32 socket=0;
	if(nBitCount>=24)
	{
		int sernum=0;
		BYTE *rowp=m_lpBits;
		BYTE *pBits=rowp;
		for (int y = 0; y < nHeight; y++) 
		{
			for (int x = 0; x < nWidth; x++) 
			{			
				blueptr[sernum]=*pBits++;
				greenptr[sernum]=*pBits++;
				redptr[sernum]=*pBits++;
				alphaptr[sernum]=sernum;
				pBits+=(spp-3);
				sernum++;
			}
			rowp+=nByteWidth;
			pBits=rowp;
		}
	}*/
	// ��¼λͼ�Ŀ��
	m_nBitCount=nBitCount;
	// ��¼λͼ�ĸ߶�
	m_nWidth=nWidth;
	// ��¼λͼ��ʾ��ɫ���õ�λ��
	m_nHeight=nHeight;
	return TRUE;
}

// ��CDib������ݽṹ�е�����ת��ΪDIBλͼ��ʽ����
BOOL CDIB::SaveToDib(BYTE *lpDib)
{
	// ���û��λͼ���ݣ�����
	if (m_lpBits == NULL) 
		return FALSE;

	// ��ȡλͼ����
	BYTE *p = lpDib;
	// ����λͼÿ��������ռ���ֽ���Ŀ
	int nByteWidth = BYTE_PER_LINE(m_nWidth, m_nBitCount);
	
	// ���λͼ��Ϣͷ�ṹ
	BITMAPINFOHEADER bmi;
	// ָ��λͼ��Ϣͷ�ṹ�Ĵ�С
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	// ָ��λͼ�Ŀ��
	bmi.biWidth = m_nWidth;
	// ָ��λͼ�ĸ߶�
	bmi.biHeight = m_nHeight;
	// Ŀ���豸��λ��������ֵ����Ϊ1
	bmi.biPlanes = 1;
	// ָ����ʾ��ɫʱ�õ���λ����
	// ���õ�ֵΪ1(�ڰ׶�ɫͼ)��4(16ɫͼ)��8(256ɫͼ)��24(���ɫͼ)
	bmi.biBitCount = m_nBitCount;
	// ˵��û��ѹ��ͼ������
	bmi.biCompression = BI_RGB;
	// ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ�����
	// ����BI_RGB��ʽʱ��������Ϊ0 
	bmi.biSizeImage = 0;
	// ָ��Ŀ���豸��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
	bmi.biXPelsPerMeter = 0;
	// ָ��Ŀ���豸�Ĵ�ֱ�ֱ��ʣ�������/�ױ�ʾ
	bmi.biYPelsPerMeter=0;
	// ָ��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ���
	bmi.biClrUsed=0;
	// ָ����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ 
	bmi.biClrImportant=0;
	// ��λͼ��Ϣͷ�ṹ�����ݸ��Ƶ�λͼ������
	memcpy(p, &bmi, sizeof(BITMAPINFOHEADER));
	
	// ��ָ���ƶ���λͼ��Ϣͷ�ṹ֮��
	p += sizeof(BITMAPINFOHEADER);
	// �������24λ���ɫλͼ
	if (m_nBitCount != 24)
	{
		// ����ɫ����Ϣ���ݸ��Ƶ�λͼ������
		memcpy(p, m_lpPalette, PALETTESIZE(m_nBitCount)*sizeof(RGBQUAD));
		// ��ָ���ƶ�����ɫ����Ϣ����֮��
		p += PALETTESIZE(m_nBitCount)*sizeof(RGBQUAD);
	}
	
	// ���������ݸ��Ƶ�λͼ������
	memcpy(p, m_lpBits, nByteWidth*m_nHeight);

	return TRUE;
}

// ��ͼ�����ݸ��Ƶ�������
BOOL CDIB::Copy()
{
	// ���û��λͼ�������ݣ�����
	if (m_lpBits == 0) 
		return FALSE;

	// ����޷��򿪼����壬����
	if (!OpenClipboard(NULL)) 
		return FALSE;
	
	// ��ռ����壬�ͷż����������ݵľ����
	// ʹ��ǰ�����öԼ�����Ŀ���
	EmptyClipboard();
	
	HGLOBAL hMem;
	BYTE *lpDib;
	// ����λͼ������ռ�е��ֽ�����λͼ��Ϣͷ�ṹ���������ݣ�
	int nLen = sizeof(BITMAPINFOHEADER)
				+ BYTE_PER_LINE(m_nWidth, m_nBitCount)*m_nHeight;
	// �������24λ���ɫλͼ����Ҫ���ϵ�ɫ����Ϣ���ݵĳ���
	if (m_nBitCount != 24)
		nLen += PALETTESIZE(m_nBitCount)*sizeof(RGBQUAD);

	// Ϊλͼ���ݷ����ڴ�ռ�
	hMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, nLen);
	// ������������ڴ�ռ�
	lpDib = (BYTE *)GlobalLock(hMem);
	// ��CDib������ݽṹ�е�����ת��ΪDIBλͼ��ʽ����
	SaveToDib(lpDib);
	// �⿪���ڴ�ռ������
	GlobalUnlock(hMem);
	// �����ݰ���DIBλͼ��ʽ������������
	SetClipboardData(CF_DIB, hMem);
	// �رռ�����
	CloseClipboard();
	
	return TRUE;
}

// ���Ƽ������е�����
BOOL CDIB::Paste()
{
	HGLOBAL hMem;
	BYTE *lpDib;

	// ����޷��򿪼����壬����
	if (!OpenClipboard(NULL)) 
		return FALSE;

	// ������ܴӼ������ϵõ�λͼ��ʽ�����ݣ�����
	if (!(hMem=GetClipboardData(CF_DIB))) 
		return FALSE;

	// �����������ݵ��ڴ�ռ�
	lpDib = (BYTE *)GlobalLock(hMem);
	// ��DIBλͼ������䵽��CDib������ݽṹ
	LoadDib(lpDib);

	// �رռ�����
	CloseClipboard();
	return TRUE;
}

// ����λͼ���ݻ���λͼ
int CDIB::Stretch(HDC hDC,
			int XDest,int YDest,int nDestWidth,int nDestHeight,
			UINT iUsage,DWORD dwRop)
{
	// λͼ��Ϣ�ṹ������λͼ��Ϣͷ�͵�ɫ����Ϣ��
	// ����λͼ�Ĵ�С����ɫ���
	BITMAPINFO *pbmi;
	int palsize;

	// ��ȡ��ɫ�����ɫ��
	palsize = PALETTESIZE(m_nBitCount);
	// ����λͼ������ռ�ڴ�ռ�
	pbmi=(BITMAPINFO *)new BYTE[sizeof(BITMAPINFO)+sizeof(RGBQUAD)*palsize];

	// ����ɫ����Ϣ���Ƶ�λͼ��Ϣ�ṹ��
	memcpy (pbmi->bmiColors, m_lpPalette, sizeof(RGBQUAD)*palsize);
	//  ָ��λͼ��Ϣͷ�ṹ�Ĵ�С
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	// ָ��λͼ�Ŀ��
	pbmi->bmiHeader.biWidth = m_nWidth;
	// ָ��λͼ�ĸ߶�
	pbmi->bmiHeader.biHeight = m_nHeight;
	// Ŀ���豸��λ��������ֵ����Ϊ1	
	pbmi->bmiHeader.biPlanes = 1;
	// ָ����ʾ��ɫʱ�õ���λ��
	pbmi->bmiHeader.biBitCount = m_nBitCount;
	// ˵��û��ѹ��ͼ������ 
	pbmi->bmiHeader.biCompression = BI_RGB;
	// ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ�����
	// ����BI_RGB��ʽʱ��������Ϊ0 
	pbmi->bmiHeader.biSizeImage = 0;
	// ָ��Ŀ���豸��ˮƽ�ֱ��ʣ�������/�ױ�ʾ 
	pbmi->bmiHeader.biXPelsPerMeter = 0;
	// ָ��Ŀ���豸�Ĵ�ֱ�ֱ��ʣ�������/�ױ�ʾ
	pbmi->bmiHeader.biYPelsPerMeter = 0;
	// ָ��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ���
	pbmi->bmiHeader.biClrUsed=0;
	// ָ����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ 
	pbmi->bmiHeader.biClrImportant=0;

	// ���û�λͼ��ģʽ
	SetStretchBltMode(hDC, COLORONCOLOR);
	
	// ��ָ����λ���ϰ���ָ���Ĵ�С����λͼ
	int ret = StretchDIBits(hDC, XDest, YDest, nDestWidth, nDestHeight,
				0, 0, m_nWidth, m_nHeight, m_lpBits,
				pbmi, iUsage, dwRop);
	
	// ɾ����������ڴ�ռ�
	delete[] pbmi;
	return ret;
}

// ��ͼ��ת��Ϊ�Ҷ�ͼ��
BOOL CDIB::Grey()
{
	// ���û��λͼ���ݣ�����
	if (!m_lpBits) 
		return FALSE;

	int y, x, nByteWidth, palsize, grey;
	BYTE *p;
	
	// �����24λ���ɫ��λͼ
	if (m_nBitCount==24)
	{
		// ����λͼÿ��������ռ���ֽ���Ŀ
		nByteWidth = BYTE_PER_LINE(m_nWidth, m_nBitCount);
		// ����λͼ�е�ÿһ��
		for (y=0; y<m_nHeight; y++)
			// ����ÿһ���е�ÿһ������
			for (x=0; x<m_nWidth; x++)
			{
				// �ҵ����������������������е�λ��
				// 24λ���ɫλͼ�У�ÿ������ռ3���ֽ�
				p = m_lpBits+nByteWidth*y+x*3;
				
				// �޸����ص���ɫ��ʹ��ҶȻ�
				grey = (BYTE)(0.299*p[2]+0.587*p[1]+0.114*p[0]);
				p[0] = grey;
				p[1] = grey;
				p[2] = grey;
			}
	}
	// �������24λ���ɫλͼ���޸ĵ�ɫ���е���ɫ
	else
	{
		// ��õ�ɫ�����ɫ��
		palsize = PALETTESIZE(m_nBitCount);
		// ���ڵ�ɫ���е�ÿһ����ɫ
		for (x=0; x<palsize; x ++)
		{
			// ���ÿһ����ɫ�������ڵ�λ��
			// ��ɫ����ÿ����ɫռ4���ֽ�
			p = m_lpPalette+x*4;
			
			// ����ɫ�ҶȻ�
			grey = (BYTE)(0.299*p[2]+0.587*p[1]+0.114*p[0]);
			p[0]=grey;
			p[1]=grey;
			p[2]=grey;
		}
	}
	return TRUE;
}

// ����תͼ��
BOOL CDIB::HFlip()
{
	// ���û��λͼ���ݣ�����
	if (!m_lpBits) 
		return FALSE;
	
	int y, x, nByteWidth, temp;
	BYTE *p1, *p2;
	
	// ����λͼÿ��������ռ���ֽ���Ŀ
	nByteWidth = BYTE_PER_LINE(m_nWidth, m_nBitCount);

	// ���λͼ����ɫλ������������i�к͵�����i�е�����
	switch (m_nBitCount)
	{
		// �����24λ���ɫλͼ��ÿ������ռ3���ֽ�
		case 24:
			for (y=0; y<m_nHeight; y++)
				for (x=0; x<m_nWidth/2; x++)
				{
					// ��y�еĵ�x�����ص�����ָ��
					p1 = m_lpBits+nByteWidth*y+x*3;
					// ��y�еĵ�����x�����ص�����ָ��
					p2 = m_lpBits+nByteWidth*y+(m_nWidth-x-1)*3;
					// ����temp����p1�ĵ�1���ֽ���p2�ĵ�1���ֽڽ���
					temp = p1[0];
					p1[0] = p2[0];
					p2[0] = temp;
					// ����temp����p1�ĵ�2���ֽ���p2�ĵ�2���ֽڽ���
					temp = p1[1];
					p1[1] = p2[1];
					p2[1] = temp;
					// ����temp����p1�ĵ�3���ֽ���p2�ĵ�3���ֽڽ���
					temp = p1[2];
					p1[2] = p2[2];
					p2[2] = temp;
				}
			break;
	
		// �����8λ��ɫλͼ��ÿ������ռһ���ֽ�
		case 8:
			for (y=0; y<m_nHeight; y++)
				for (x=0; x<m_nWidth/2; x++)
				{
					// ��y�еĵ�x�����ص�����ָ��
					p1 = m_lpBits+nByteWidth*y+x;
					// ��y�еĵ�����x�����ص�����ָ��
					p2 = m_lpBits+nByteWidth*y+(m_nWidth-x-1);
					// ����temp���������������ص�����
					temp = *p1;
					*p1 = *p2;
					*p2 = temp;
				}
			break;
		
		// �����4λ��ɫλͼ������
		case 4:
			return FALSE;
	}
	return TRUE;
}

// ����תͼ��
BOOL CDIB::VFlip()
{
	// ���û��λͼ���ݣ�����
	if (!m_lpBits) 
		return FALSE;
	
	int y, nByteWidth;
	BYTE *p1, *p2, *pm;
	
	// ����λͼÿ��������ռ���ֽ���Ŀ
	nByteWidth = BYTE_PER_LINE(m_nWidth, m_nBitCount);

	// �����ܴ洢һ�����ص��ڴ�ռ�
	pm = new BYTE[nByteWidth];

	// ����y�������뵹��y�����ؽ��н���
	for (y=0; y<m_nHeight/2; y++)
	{
		// ȡ�õ�y�����ص�����
		p1 = m_lpBits+y*nByteWidth;
		// ȡ�õ�����y�����ص�����
		p2 = m_lpBits+(m_nHeight-y-1)*nByteWidth;
		// ����pm��������y�������뵹��y�����ؽ��н���
		memcpy(pm, p1, nByteWidth);
		memcpy(p1, p2, nByteWidth);
		memcpy(p2, pm, nByteWidth);
	}
	
	// ɾ��������ڴ�ռ�
	delete[] pm;

	return TRUE;
}

// ��ͼ��˳ʱ����ת90��
BOOL CDIB::Rotate()
{
	// ���û��λͼ���ݣ�����
	if (m_lpBits == NULL) 
		return FALSE;

	BYTE *pbits, *p1, *p2, *pa, *pb, *pa1, *pb1;
	int w2, h2, bw2, bw1, x, y;

	// ��ȡ��תǰλͼ�ĸ߶�
	w2 = m_nHeight;	
	// ��ȡ��תǰλͼ�Ŀ��
	h2 = m_nWidth;
	
	// ������תǰλͼÿ��������ռ���ֽ���Ŀ
	bw1 = BYTE_PER_LINE(m_nWidth, m_nBitCount);
	// ������ת��λͼÿ��������ռ���ֽ���Ŀ
	bw2 = BYTE_PER_LINE(w2, m_nBitCount);
	
	// �����ڴ�ռ��Ա�����ת���λͼ
	pbits = new BYTE[bw2*h2];

	// ���λͼ����ɫλ��
	switch (m_nBitCount)
	{
		// �����24λ���ɫλͼ��ÿ������ռ�����ֽ�
		case 24:
			for (y=0; y<m_nHeight; y++)
				for (x=0; x<m_nWidth; x++)
				{
					// ȡ��y�еĵ�x������
					p1 = m_lpBits+bw1*y+x*3;
					// ȡ����ת������Ӧ������
					p2 = pbits+bw2*(h2-x-1)+y*3;
					// ����תǰ���������ݸ��Ƹ���ת�������
					p2[0] = p1[0];
					p2[1] = p1[1];
					p2[2] = p1[2];
				}
			break;

		// �����8λ��ɫ��λͼ��ÿ������ռһ���ֽ�
		case 8:
			for (y=0; y<m_nHeight; y++)
				for (x=0; x<m_nWidth; x++)
				{
					// ȡ��y�еĵ�x������
					p1 = m_lpBits+bw1*y+x;
					// ȡ��ת�������Ӧ������
					p2 = pbits+bw2*(h2-x-1)+y;
					// ����תǰ���������ݸ��Ƹ���ת�������
					p2[0] = p1[0];
				}
			break;

		//	�����4λ��ɫ��λͼ��ÿ������ռ����ֽ�
		case 4:
			for (y=0; y<m_nHeight; y+=2)
				for (x=0; x<m_nWidth; x+=2)
				{
					// ȡ��תǰ������
					pa = m_lpBits+bw1*y+x/2;
					pb = pa+bw1;
					// ȡ��ת�������Ӧ������
					pb1 = pbits+bw2*(h2-x-1)+y/2;
					pa1 = pb1-bw2;
					// ���pa1�Ƿ�Խ�磬
					// ������תǰ���������ݸ��Ƹ���ת�������
					if (pa1 >= pbits)
						*pa1 = ((*pa&0x0f)<<4)|(*pb&0x0f);
					*pb1 = (*pa&0xf0)|((*pb&0xf0)>>4);
			}
		break;
	}
	
	// ɾ����תǰ��λͼ��������
	delete[] m_lpBits;
	
	// ����ת���λͼ���ݸ�ֵ��m_lpBits
	m_lpBits = pbits;
	// ��m_nWidth����Ϊ��ת���λͼ���
	m_nWidth = w2;
	// ��m_nHeight����Ϊ��ת���λͼ�߶�
	m_nHeight = h2;
	
	return TRUE;
}

// ��ͼ��ת��Ϊ����
BOOL CDIB::Negative()
{
	// ���û��λͼ���ݣ�����
	if (!m_lpBits) 
		return FALSE;

	int y, x, nByteWidth, palsize;
	BYTE *p;

	// �����24λ���ɫλͼ��ÿ������ռ�����ֽ�
	if (m_nBitCount==24)
	{
		// ����λͼÿ��������ռ���ֽ���Ŀ
		nByteWidth = BYTE_PER_LINE(m_nWidth, m_nBitCount);
		
		for (y=0; y<m_nHeight; y++)
			for (x=0; x<m_nWidth; x++)
			{
				// ȡ�õ�y�еĵ�x����
				p = m_lpBits+nByteWidth*y+x*3;
				// ��ÿһ�����ص�����ȡ��
				p[0] = ~p[0];
				p[1] = ~p[1];
				p[2] = ~p[2];
			}
	}
	else
	{
		// ��õ�ɫ�����ɫ��
		palsize = PALETTESIZE(m_nBitCount);
		// ���ڵ�ɫ���е�ÿһ����ɫ
		for (x=0; x<palsize; x++)
		{
			// ���ÿһ����ɫ�������ڵ�λ��
			// ��ɫ����ÿ����ɫռ4���ֽ�			
			p = m_lpPalette+x*4;
			// ��ÿ����ɫ������ȡ��
			p[0] = ~p[0];
			p[1] = ~p[1];
			p[2] = ~p[2];
		}
	}
	
	return TRUE;
}

// ����ָ���ļ�������PCX��ʽ��ͼ���ļ�������


//this function only saves rgb tiff.


BOOL CDIB::InteEqualize()
{
	// ָ��DIB����ָ��
	 unsigned char*    lpDIBBits;	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = m_lpBits;
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ֱ��ͼ���⣬�����Ŀ������ƣ�
	if (m_nBitCount!=8)
	{
		// ��ʾ�û�
		AfxMessageBox("Ŀǰֻ֧��256ɫλͼ��ֱ��ͼ���⣡");		
		// ����
		return false;
	}
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ��ʱ����
	LONG	lTemp;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// �Ҷ�ӳ���
	BYTE	bMap[256];
	
	// �Ҷ�ӳ���
	LONG	lCount[256];
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	LONG lWidth=m_nWidth,lHeight=m_nHeight;
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ���ü���Ϊ0
	for (i = 0; i < 256; i ++)
	{
		// ����
		lCount[i] = 0;
	}
	
	// ��������Ҷ�ֵ�ļ���
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * i + j;
			
			// ������1
			lCount[*(lpSrc)]++;
		}
	}
	
	// ����Ҷ�ӳ���
	for (i = 0; i < 256; i++)
	{
		// ��ʼΪ0
		lTemp = 0;
		
		for (j = 0; j <= i ; j++)
		{
			lTemp += lCount[j];
		}
		
		// �����Ӧ���»Ҷ�ֵ
		bMap[i] = (BYTE) (lTemp * 255 / lHeight / lWidth);
	}
	
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �����µĻҶ�ֵ
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// ����
	return TRUE;
}

void CDIB::histovector(float *hive)
{ 
	unsigned int hv[256]={0};
	unsigned char *lpDIBBits=m_lpBits;
	unsigned char *lpSrc;
	int w=m_nWidth,h=m_nHeight;
	int i ,j;
	LONG	lLineBytes;	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(w * 8);
	for (i = 0; i < h; i ++)
	{
		for (j = 0; j < w; j ++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * i + j;
			
			// ������1
			hv[*(lpSrc)]++;
		}
	}
	for(i=0;i<256;i++)
	{	(*(hive+i))=(float)hv[i]/(w*h);
	float x=(*(hive+i));}
}

void CDIB::MakeGrayPalette(int BitCount)
{
	unsigned int	PaletteSize = (1<<BitCount);
	m_lpPalette = new BYTE[sizeof(RGBQUAD)*PaletteSize];
    unsigned int  magnifier=1<<(8-BitCount);
	register int i;
	RGBQUAD *pal;
	for(i=0;i<PaletteSize;i++)
	{
		pal=(RGBQUAD*)(m_lpPalette+sizeof(RGBQUAD)*i);
		pal->rgbRed=i*magnifier;
		pal->rgbGreen=i*magnifier;
		pal->rgbBlue=i*magnifier;
		pal->rgbReserved=0;
	}
}






void CDIB::Origin()
{
	int nByteWidth = BYTE_PER_LINE(m_nWidth,m_nBitCount);
	memcpy(m_lpBits,bpBits,sizeof(BYTE)*nByteWidth*m_nHeight);
/*	for (int y = 0; y < m_nHeight; y++) 
		{
			for (int x = 0; x < m_nWidth; x++) 
			{			
				*pBits++=blueptr[sernum];
				*pBits++=greenptr[sernum];
				*pBits++=redptr[sernum];
				sernum++;
			}
			rowp+=nByteWidth;
			pBits=rowp;
		}*/
}
//for t-criterion region merger
//tag records label for each segment according to indice of S
//option 0 just normal delineate contours of segments, 
//option 1 binarise to green the road seed segments
//option 2 produce segments of average color value in each band
void CDIB::LookRegions(int *tag,int option)
{	
	int width = m_nWidth;
	int height=m_nHeight;
	int sernum=0,label=0,temp;
	int x,y;
	int nByteWidth = BYTE_PER_LINE(m_nWidth,m_nBitCount);
	BYTE *rowp=m_lpBits;
	BYTE *pBits=rowp;
	sernum=0;
	unsigned int acronym;
	rowp=m_lpBits;
	switch(option)
	{
	case 0:
	default:
		for (y = 0; y < height; y++) 
		{
			for (x = 0; x < width; x++) 
			{			
				if(x>0&&y>0&&(x<width-1)&&(y<height-1))
				{
					temp=x+(height-1-y)*width;
					label=tag[temp];
					//two pixels width boundary
					//if(label!=tag[sernum+1]||label!=tag[sernum-1]||label!=tag[sernum-width]||label!=tag[sernum+width])
					if(label!=tag[temp+1]||label!=tag[temp-width])//one pixel width boundary
					{
						int z=0;
						int spp=(m_nBitCount>>3);
						acronym=x*spp;
						for(z=0;z<(spp-1);z++)
						{
							*(rowp+x*spp+z)=0;
						}
						*(rowp+x*spp+z)=255;
					}
				}
				++sernum;
			}
			rowp+=nByteWidth;
		}
		break;
	case 1: 
		for (y = 0; y < height; y++) 
		{
			for (x = 0; x < width; x++) 
			{			
				
					if(tag[sernum]!=-1)
					{
						int z=0;
						int spp=(m_nBitCount>>3);
						acronym=x*spp;					
						switch(spp)
						{
						case 1:*(rowp+acronym)=255; break;
						case 3:
							*(rowp+acronym)=0;
							*(rowp+acronym+1)=255;
							*(rowp+acronym+2)=0; break;
						default: break;
						}
					}
				
				++sernum;
			}
			rowp+=nByteWidth;
		}
		break;
	case 3:
		for (y = 0; y < height; y++) 
		{
			for (x = 0; x < width; x++) 
			{			
				if(x>0&&y>0&&(x<width-1)&&(y<height-1))
				{
					label=tag[sernum];
					//two pixels width boundary
					//if(label!=tag[sernum+1]||label!=tag[sernum-1]||label!=tag[sernum-width]||label!=tag[sernum+width])
					if(label!=tag[sernum+1]||label!=tag[sernum+width])//one pixel width boundary
					{
						int z=0;
							int spp=(m_nBitCount>>3);
						acronym=x*spp;
						for(z=0;z<(spp-1);z++)
						{
							*(rowp+acronym+z)=0;
						}
						*(rowp+acronym+z)=255;
					}
				}
				++sernum;
			}
			rowp+=nByteWidth;
		}
		break;
	}
}

BOOL CDIB::CreateDIB(int r, int g, int b,const CString&fn)
{
	int spp;
	GDALDataset* pDataset=(GDALDataset*)GDALOpen(fn,GA_ReadOnly);
	if (pDataset)
	{
		m_nWidth=pDataset->GetRasterXSize(); //Ӱ��ĸ߶ȣ����
		m_nHeight=pDataset->GetRasterYSize();
		spp=pDataset->GetRasterCount();//���ε���Ŀ
	}
	int m_CurrentBand=1;
	if (r <= 0 || r>spp)
		r = m_CurrentBand;
	if (g<= 0 || g>spp)
		g = m_CurrentBand;
	if (b<= 0 || b>spp)
		b = m_CurrentBand;
	int height=m_nHeight,width=m_nWidth,nBitCount=24,nByteWidth=0;

	if (m_lpBits) 
		delete[] m_lpBits;
	if (m_lpPalette) 
		delete[] m_lpPalette;
	BYTE *rowp=NULL;
	BYTE *pBits=NULL;
	int x,y,temp,i;
	if(spp==1)
	{
	
		GDALRasterBand  *m_pBand=NULL;
		GDALColorTable*m_pCT=NULL;
		m_pBand = pDataset->GetRasterBand(1);
		m_pCT=m_pBand->GetColorTable();
		if(m_pCT!=NULL)
		{			
			int paletteSize = m_pCT->GetColorEntryCount();
					
			// Ϊ�����ɫ����Ϣ���ݵ�m_lpPalette����ռ�
			m_lpPalette = new BYTE[sizeof(RGBQUAD)*paletteSize];
			RGBQUAD *pal;
			const GDALColorEntry *pg=NULL;
			for(i=0;i<paletteSize;i++)
			{
				pal=(RGBQUAD*)(m_lpPalette+sizeof(RGBQUAD)*i);
				pg=m_pCT->GetColorEntry(i);
				pal->rgbRed=pg->c1;
				pal->rgbGreen=pg->c2;
				pal->rgbBlue=pg->c3;
				pal->rgbReserved=pg->c4;
			}
		}
		else MakeGrayPalette(8);
		m_nBitCount=8;
		// ����λͼÿ��������ռ���ֽ���Ŀ 
		nByteWidth = BYTE_PER_LINE(width, 8);
		m_lpBits= new BYTE[nByteWidth*height];
		bpBits=new BYTE[nByteWidth*height];
		byte* buf =	new byte[width*height];
		if (CE_None==m_pBand->RasterIO( GF_Read, 0,0, width, height, buf, width,height, GDT_Byte, 0, 0 ))
		{
			rowp=m_lpBits;
			pBits=rowp;
			temp=height*width-width;
			for(y=0;y<height;++y)
			{
				for (x = 0; x < width; ++x) 
				{
					*pBits=buf[temp];						
					++pBits;
					++temp;
				}
				rowp+=nByteWidth;
				pBits=rowp;
				temp-=2*width;
			}
		}
		delete []buf;
		memcpy(bpBits,m_lpBits,sizeof(BYTE)*nByteWidth*height);
		return TRUE;
		
	}
	// ��ȡλͼ��ʾ��ɫ���õ�λ��
	nBitCount =24;//add alpha channel

	// ����λͼÿ��������ռ���ֽ���Ŀ 
	nByteWidth = BYTE_PER_LINE(width, nBitCount);
	
	m_lpPalette = NULL;
	
	// ����Ϊλͼ�������ݷ����ڴ�
	m_lpBits= new BYTE[nByteWidth*height];
	bpBits=new BYTE[nByteWidth*height];
	GDALRasterBand  *m_pBand=NULL;
	m_pBand = pDataset->GetRasterBand(r);
	//	CPLAssert( m_pBand->GetRasterDataType() == GDT_Byte );

	int dataType=0;
	
	CString m_DataType=GDALGetDataTypeName(m_pBand->GetRasterDataType());//��������
	if(m_DataType=="Byte")dataType=0;
	if(m_DataType=="UInt16")
		dataType=1;
	if(m_DataType=="Int16")
		dataType=2;
	if(m_DataType=="UInt32")
		dataType=3;
	if(m_DataType=="Int32")dataType=4;
	if(m_DataType=="Float32")dataType=5;
	if(m_DataType=="Float64")dataType=6;
	if(m_DataType=="CInt16")dataType=7;
	if(m_DataType=="CInt32")dataType=8;
	if(m_DataType=="CFloat32")dataType=9;
	if(m_DataType=="CFloat64")dataType=10;

	switch(dataType)//nuiance between case 0 and default to open float datatype
	{
	case 0://byte��byte	
		{
			byte* buf =	new byte[width*height];
			if (m_pBand)
			{
				if (CE_None==m_pBand->RasterIO( GF_Read, 0,0, width, height, buf, width,height, GDT_Byte, 0, 0 ))
				{
					rowp=m_lpBits;
					pBits=rowp+2;
					temp=height*width-width;
					for(y=0;y<height;++y)
					{
						for (x = 0; x < width; ++x) 
						{
							*pBits=buf[temp];						
							pBits+=3;
							++temp;
						}
						rowp+=nByteWidth;
						pBits=rowp+2;
						temp-=2*width;
					}
				}
			}	
			m_pBand = pDataset->GetRasterBand(g);
			if (m_pBand)
			{	
				if (CE_None==m_pBand->RasterIO( GF_Read,0,0, width, height, buf, width,height, GDT_Byte, 0, 0 ))
				{
					rowp=m_lpBits;
					pBits=rowp+1;
					temp=height*width-width;
					for(y=0;y<height;++y)
					{
						for (x = 0; x < width; ++x) 
						{
							*pBits=buf[temp];						
							pBits+=3;
							++temp;
						}
						rowp+=nByteWidth;
						pBits=rowp+1;
						temp-=2*width;
					}
				}
			}
			
			m_pBand = pDataset->GetRasterBand(b);
			if (m_pBand)
			{	
				if (CE_None==m_pBand->RasterIO( GF_Read,0,0,width,height, buf, width,height, GDT_Byte, 	0, 0 ))
				{
					rowp=m_lpBits;
					pBits=rowp;
					temp=height*width-width;
					for(y=0;y<height;++y)
					{
						for (x = 0; x < width; ++x) 
						{
							*pBits=buf[temp];						
							pBits+=3;
							++temp;
						}
						rowp+=nByteWidth;
						pBits=rowp;
						temp-=2*width;
					}
				}
			}	
			delete []buf;
		}
		break;
	default://�������͵Ķ���float
		{
	
			float*buffFloat = new float[width*height];
			if (m_pBand)
			{
				if (CE_None==m_pBand->RasterIO( GF_Read,0,0, width, height, buffFloat, width,height, GDT_Float32, 0, 0 ))
				{
					
					rowp=m_lpBits;
					pBits=rowp+2;
					temp=height*width-width;
					for(y=0;y<height;++y)
					{
						for (x = 0; x < width; ++x) 
						{
							*pBits=(BYTE)buffFloat[temp];
									
							pBits+=3;
							++temp;
						}
						rowp+=nByteWidth;
						pBits=rowp+2;
						temp-=2*width;
					}
				}
			}	
			m_pBand = pDataset->GetRasterBand(g);
		
			if (m_pBand)
			{	
				if (CE_None==m_pBand->RasterIO( GF_Read,0,0, width, height, buffFloat, width,height, GDT_Float32, 0, 0 ))
				{
					
					rowp=m_lpBits;
					pBits=rowp+1;
					temp=height*width-width;
					for(y=0;y<height;++y)
					{
						for (x = 0; x < width; ++x) 
						{
							*pBits=(BYTE)buffFloat[temp];
						
							pBits+=3;
							++temp;
						}
						rowp+=nByteWidth;
						pBits=rowp+1;
						temp-=2*width;
					}
				}
			}
			
			m_pBand = pDataset->GetRasterBand(b);
	
			if (m_pBand)
			{	
				if (CE_None==m_pBand->RasterIO( GF_Read,0,0, width, height, buffFloat, width,height, GDT_Float32, 0, 0 ))
				{			
					rowp=m_lpBits;
					pBits=rowp;
					temp=height*width-width;
					for(y=0;y<height;++y)
					{
						for (x = 0; x < width; ++x) 
						{
							*pBits=(BYTE)buffFloat[temp];
								
							pBits+=3;
							++temp;
						}
						rowp+=nByteWidth;
						pBits=rowp;
						temp-=2*width;
					}
				}
			}
			
			delete []buffFloat;
		}
		break;
	}
	m_nBitCount=nBitCount;
	memcpy(bpBits,m_lpBits,sizeof(BYTE)*nByteWidth*height);
	GDALClose((GDALDatasetH) pDataset);
	return TRUE;
}
//function: save edge raster file into bmp file 24bit
BOOL CDIB::SaveEdge(LPCTSTR fn,BYTE*EM)
{
	CFile f,*pf;
	
	// ������ܴ�ָ�����Ƶ��ļ�������
	if (!f.Open(fn, CFile::modeCreate|CFile::modeWrite)) 
		return FALSE;
	else
	pf=&f;	
	// ���edge����Ϊ�գ�����
	if (EM== NULL) 
		return FALSE;
	int x=0,y=0;
	// ��ȡͼ���ļ���ÿ��ͼ����ռ�ֽ���
	int nByteWidth = BYTE_PER_LINE(m_nWidth,24);

	// ���λͼ�ļ�ͷ�ṹ��ָ���ļ������Ϣ
	BITMAPFILEHEADER bm;
	
	// ָ���ļ�����Ϊλͼ
	bm.bfType = 'M'*256+'B';
	// ָ��λͼ�ļ��Ĵ�С
	bm.bfSize = nByteWidth*m_nHeight;
	// �����ĽṹԪ�أ�����Ϊ0
	bm.bfReserved1 = 0;
	bm.bfReserved2 = 0;
	// ������ļ�ͷ��ʼ��ʵ�ʵ�ͼ������֮���ƫ�������ֽ�����
	bm.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	// �������24λ���ɫλͼ�����ϵ�ɫ����Ϣ�ĳ���
//	bm.bfOffBits += 256*sizeof(RGBQUAD);

	// ���ļ���д��λͼ�ļ�ͷ��Ϣ
	pf->Write(&bm, sizeof(BITMAPFILEHEADER));

	// ���λͼ��Ϣͷ�ṹ��ָ��λͼ�Ĵ�С����ɫ��Ϣ
	BITMAPINFOHEADER bmi;
	// ָ��λͼ��Ϣͷ�ṹ�Ĵ�С
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	// ָ��λͼ�Ŀ��
	bmi.biWidth = m_nWidth;
	// ָ��λͼ�ĸ߶�
	bmi.biHeight = m_nHeight;
	// Ŀ���豸��λ��������ֵ����Ϊ1
	bmi.biPlanes = 1;
	// ָ����ʾ��ɫʱ�õ���λ����
	// ���õ�ֵΪ1(�ڰ׶�ɫͼ)��4(16ɫͼ)��8(256ɫͼ)��24(���ɫͼ)
	bmi.biBitCount =24;
	// ˵��û��ѹ��ͼ������
	bmi.biCompression = BI_RGB;
	// ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ�����
	// ����BI_RGB��ʽʱ��������Ϊ0 
	bmi.biSizeImage = 0;
	// ָ��Ŀ���豸��ˮƽ�ֱ��ʣ�������/�ױ�ʾ
	bmi.biXPelsPerMeter = 0;
	// ָ��Ŀ���豸�Ĵ�ֱ�ֱ��ʣ�������/�ױ�ʾ
	bmi.biYPelsPerMeter = 0;
	// ָ��λͼʵ��ʹ�õĲ�ɫ���е���ɫ����������Ϊ0�Ļ�����˵��ʹ�����е�ɫ���
	bmi.biClrUsed = 0;
	// ָ����ͼ����ʾ����ҪӰ�����ɫ��������Ŀ�������0����ʾ����Ҫ��
	bmi.biClrImportant = 0;
	// ��λͼ��Ϣͷд���ļ�
	pf->Write(&bmi,sizeof BITMAPINFOHEADER);
	//todo: construct palette for edge map and convert data type from float to char
	// �������24λ���ɫλͼ������ɫ����Ϣд���ļ�
/*	RGBQUAD *pPalette=new RGBQUAD[256];
	for(x=0;x<PALETTESIZE(8);++x)
	{
		pPalette[x].rgbBlue=0;
		pPalette[x].rgbGreen=x;
		pPalette[x].rgbRed=0;
		pPalette[x].rgbReserved=0;
	}
	pf->Write(pPalette, 256*sizeof(RGBQUAD));*/
	BYTE*edge=new BYTE[nByteWidth*m_nHeight];
	BYTE*ep=edge;
	for (y = 0; y < m_nHeight; ++y) 
	{
		for (x = 0; x < m_nWidth; ++x) 
		{			
			ep[x*3]=EM[(m_nHeight-1-y)*m_nWidth+x];
			ep[x*3+1]=ep[x*3];
			ep[x*3+2]=ep[x*3];
		}
		ep+=nByteWidth;
	}
	// ��λͼ����д���ļ�
	pf->Write(edge, nByteWidth*m_nHeight);
	f.Close();
//	delete []pPalette;
	delete []edge;
	return TRUE;

}


//show region with specific property
void CDIB::ShowReg(int *tag,int step)
{
	int width = m_nWidth;
	int height=m_nHeight;
	int sernum=0,label=0,temp;
	int x,y;
	int nByteWidth = BYTE_PER_LINE(m_nWidth,m_nBitCount);
	BYTE *rowp=m_lpBits;
	BYTE *pBits=rowp;
	sernum=0;
	unsigned int acronym;

	rowp=m_lpBits;
	for (y = 0; y < height; y++) 
	{
		for (x = 0; x < width; x++) 
		{			
			if(x%step==0||y%step==0)
			{
				
					int z=0;
					int spp=(m_nBitCount>>3);
					acronym=x*spp;
					*(rowp+acronym+z)=255;
					for(z=1;z<spp;z++)
					{
						*(rowp+acronym+z)=0;
					}
					//*(rowp+acronym+z)=255;
				
			}
		
			
				if(x>0&&y>0&&(x<width-1)&&(y<height-1))
				{
					temp=x+(height-1-y)*width;
					label=tag[temp];
					//two pixels width boundary
					//if(label!=tag[sernum+1]||label!=tag[sernum-1]||label!=tag[sernum-width]||label!=tag[sernum+width])
					if(label!=tag[temp+1]||label!=tag[temp+width])//one pixel width boundary
					{
						int z=0;
						int spp=(m_nBitCount>>3);
						acronym=x*spp;
						for(z=0;z<(spp-1);z++)
						{
							*(rowp+acronym+z)=0;
						}
						*(rowp+acronym+z)=255;
					}
				}
			
			++sernum;
		}
		rowp+=nByteWidth;
	}
}
//option 0 to show region in original image,

int CDIB::lookregion(int label,int*tag, CRect rect,int area)
{
	int mx,my,pos,total,i,j,acronym;
	int nByteWidth = BYTE_PER_LINE(m_nWidth,m_nBitCount);
	BYTE *rowp=m_lpBits;
	int mH=rect.bottom,mW=rect.right;
	mx=rect.left;
	my=rect.top;
	
	total=0;
	for(j=my;j<mH;++j)
	{
		rowp=m_lpBits+nByteWidth*(m_nHeight-1-j);		
		for(i=mx;i<mW;++i)
		{
			pos=i+j*m_nWidth;
			if(tag[pos]==label)
			{			
				int z=0;
					int spp=(m_nBitCount>>3);
				acronym=i*spp;
						for(z=0;z<(spp-1);z++)
						{
							*(rowp+acronym+z)=0;
						}
						*(rowp+acronym+z)=255;
				
				++total;
			}				
		}	
		rowp+=nByteWidth;
	}
	assert(total==area);
	return 1;
}

//save cdib m_lpbits to file pathname and the spatial info stored in pdataset from file named path
//when this function put into serialize of CDoc, it does not work out fine.
bool CDIB::SaveToFile(const CString&path,CString &pathName)
{
	GDALDataset*pDataset=(GDALDataset*)GDALOpen(path,GA_ReadOnly);
	int period = pathName.ReverseFind('.');
	int temp;
	CString suffix;
	GDALDriver * driver;
	
	if(period>0)		
	{			
		temp=pathName.GetLength()-1-period;
		suffix=pathName.Right(temp);
		suffix.MakeLower();
		if (suffix == "bmp")
			driver = GetGDALDriverManager()->GetDriverByName("BMP");
		else if (suffix == "jpg")
			driver = GetGDALDriverManager()->GetDriverByName("JPEG");
		else if (suffix == "tif")
			driver = GetGDALDriverManager()->GetDriverByName("GTiff");
		else if (suffix == "img")
			driver = GetGDALDriverManager()->GetDriverByName("HFA");
		else if (suffix == "bt")
			driver = GetGDALDriverManager()->GetDriverByName("BT");
		else if (suffix == "ecw")
			driver = GetGDALDriverManager()->GetDriverByName("ECW");
		else if (suffix == "fits")
			driver = GetGDALDriverManager()->GetDriverByName("FITS");
		else if (suffix == "gif")
			driver = GetGDALDriverManager()->GetDriverByName("GIF");
		else if (suffix == "hdf")
			driver = GetGDALDriverManager()->GetDriverByName("HDF4");
		else if (suffix == "hdr")
			driver = GetGDALDriverManager()->GetDriverByName("EHdr");
		else
		{
			AfxMessageBox("This format is not supported!");
			return false;	
		}
	}
	//	pathName.Delete(period+1,temp);	
	else
	{
		suffix = "tif";
		driver = GetGDALDriverManager()->GetDriverByName("GTIFF");
		pathName=pathName+"."+suffix;
	}
	
	
	if( driver == NULL)
	{		
		AfxMessageBox("This format's driver is not available!");
		return false;
	}
	
	//using create
	GDALDataset *poDstDS;   
    GDALRasterBand  *poBand=NULL;
    char **papszOptions = NULL;
    OGRSpatialReference oSRS;
	GDALDataType dataType=GDT_Byte;
	double adfGeoTransform[6],backup[6] = { 444720, 30, 0, 3751320, 0, -30 };
	BYTE* buf =new BYTE[m_nWidth*m_nHeight];
	int spp=m_nBitCount>>3;
	const char *quark=(const char*)pathName;
	poDstDS = driver->Create( quark,m_nWidth,m_nHeight,spp,dataType,papszOptions );
	if(poDstDS==NULL)
	{	
		AfxMessageBox("The dataset cannot be created!");	
		return false;
	}
	if(CE_None==pDataset->GetGeoTransform( adfGeoTransform ))	
		poDstDS->SetGeoTransform( adfGeoTransform );
	//else
	//	poDstDS->SetGeoTransform(backup); 
	/*
	oSRS.SetUTM( 11, TRUE );
	oSRS.SetWellKnownGeogCS( "NAD27" );
	oSRS.exportToWkt( &pszSRS_WKT );
	poDstDS->SetProjection( pszSRS_WKT );
	*/
	int nByteWidth = BYTE_PER_LINE(m_nWidth,m_nBitCount);
	const char*pszSRS_WKT=pDataset->GetProjectionRef();
	poDstDS->SetProjection( pszSRS_WKT );
	int d,x,y,sernum;
	//note RGB order and row order, all inversed
	for(d=1;d<=spp;++d)
	{
		
		poBand = poDstDS->GetRasterBand(d);	
		if (poBand)
		{	
			sernum=0;
			BYTE*pBits=m_lpBits;
			for(y=0;y<m_nHeight;++y)
			{
				for (x = 0; x < m_nWidth; ++x) 
				{
					buf[sernum]=pBits[(m_nHeight-y-1)*nByteWidth+x*spp+spp-d];
					++sernum;
				}				
			}
			if (CE_None!=poBand->RasterIO( GF_Write,0,0, m_nWidth, m_nHeight, buf, m_nWidth,m_nHeight,dataType, 0, 0 ))
			{
				AfxMessageBox("error writing pdataset!");
			}
		}
	}
	delete[]buf; 
	GDALClose( (GDALDatasetH) poDstDS );
	GDALClose((GDALDatasetH)pDataset);
	return true;
	
}


int CDIB::GetWidthInBytes( int nBits, int nWidth )
{
	int nWidthBytes;
	nWidthBytes = nWidth;
	if( nBits == 1 )
		nWidthBytes = ( nWidth + 7 ) / 8;
	else if( nBits == 4 )
		nWidthBytes = ( nWidth + 1 ) / 2;
	else if( nBits == 16 )
		nWidthBytes = nWidth * 2;
	else if( nBits == 24 )
		nWidthBytes = nWidth * 3;
	else if( nBits == 32 )
		nWidthBytes = nWidth * 4;
	while( ( nWidthBytes & 3 ) != 0 )
		nWidthBytes++;
	return( nWidthBytes );
}

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  �������ƣ�GetPaletteColor
* 
*  ����������BYTE idx -- ��ɫ���ж�Ӧ���±� ֵ
* 
*  ����ֵ��  RGBQUAD  -- ��(x,y)����Ӧ��RGBQUAD�ṹ������ֵ 
*
*  ˵����    �ú������ڵ�ɫ�����±� idx ��Ӧ��RGBQUAD,�� GetPixelColor()����        
************************************************************************/

RGBQUAD CDIB::GetPaletteColor(BYTE idx)
{
	RGBQUAD rgb = {0,0,0,0};
	
	if (m_lpPalette)
	{
		BYTE* iDst = m_lpPalette;
		if (idx<m_nColors)
		{
			long ldx=idx*sizeof(RGBQUAD);
			rgb.rgbBlue = iDst[ldx++];
			rgb.rgbGreen=iDst[ldx++];
			rgb.rgbRed =iDst[ldx++];
			rgb.rgbReserved = iDst[ldx];
		}
	}

	return rgb;
}

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  �������ƣ�GetPixelIndex
* 
*  ����������int x    -- ������
*            int y    -- ������
* 
*  ����ֵ��  BYTE     -- ��(x,y)������ֵ�ڵ�ɫ���е��±�
*
*  ˵����    �ú������ڻ�ȡ(x,y)������ֵ�ڵ�ɫ���е��±ꡣ
*            ������ GetPaletteColor()������       
************************************************************************/

BYTE CDIB::GetPixelIndex(int x,int y)
{

    if ((m_lpBits==NULL)||(m_nColors==0)) return 0;
	BYTE*pBits=m_lpBits;

	if (m_nBitCount==8)
	{
		return (BYTE)*(pBits+y*GetWidthInBytes(m_nBitCount,m_nWidth)+x);
	}
	else 
	{
		BYTE pos;
		BYTE iDst= (BYTE)*(pBits+y*GetWidthInBytes(m_nBitCount,m_nWidth)+(x*m_nBitCount >> 3));
		if (m_nBitCount==4)
		{
			pos = (BYTE)(4*(1-x%2));
			iDst &= (0x0F<<pos);
			return (BYTE)(iDst >> pos);
		} 
		else if (m_nBitCount==1)
		{
			pos = (BYTE)(7-x%8);
			iDst &= (0x01<<pos);
			
			return (BYTE)(iDst >> pos);
		}
	}
	   
	return 0;
}



/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  �������ƣ�GetPixelColor
* 
*  ����������int x    -- ͼ��ĺ�����   
*            int y    -- ͼ���������
* 
*  ����ֵ��  RGBQUAD  -- ��(x,y)����Ӧ��RGBQUAD�ṹ������ֵ 
*
*  ˵����    �ú������ڻ�ȡ(x,y)��������ֵ��
*            ��LoadDIBToBuf()�����û��ߵ���ʹ��          
************************************************************************/

RGBQUAD CDIB::GetPixelColor(int x,int y)
{
	RGBQUAD rgb;
	rgb.rgbBlue = rgb.rgbGreen = rgb.rgbRed = rgb.rgbReserved = 0;
    if(m_lpBits == NULL)
		return rgb;

    y = m_nHeight -1 - y;  // add by maple,2004.5.14
	

	if (m_lpPalette)
	{   
		BYTE tmp;
		tmp = GetPixelIndex(x,y);
	    rgb = GetPaletteColor(tmp);
	} 
	else
	{
		BYTE* iDst  = m_lpBits + y*GetWidthInBytes(m_nBitCount,m_nWidth) + x*(m_nBitCount>>3);
		rgb.rgbBlue = *iDst++;
		rgb.rgbGreen= *iDst++;
		rgb.rgbRed  = *iDst;
    	rgb.rgbReserved =(BYTE) 0;
	}

	return rgb;
}
/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  �������ƣ�GetPixelColor2
* 
*  ����������int x    -- ͼ��ĺ�����   
*            int y    -- ͼ���������
* 
*  ����ֵ��  RGBQUAD  -- ��(x,y)����Ӧ��RGBQUAD�ṹ������ֵ 
*
*  ˵���� �˺���ר����256ɫ�Ҷ�ͼ�����ڻ�ȡ(x,y)��������ֵ��   
*         ��LoadDIBToBuf()�����û��ߵ���ʹ��   
************************************************************************/
BYTE CDIB::GetPixelColor2(int x,int y)
{
    if(m_lpBits == NULL)
		return (BYTE)0;
   
	y = m_nHeight -1 - y;   // add by maple, 2004.5.14
	

	BYTE tmpColor;
	    tmpColor = * (m_lpBits + y*GetWidthInBytes(m_nBitCount,m_nWidth) + x);
	return tmpColor;

}


/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  �������ƣ�SetPixelIndex
* 
*  ����������int x            -- ������
*            int y            -- ������
*            BYTE index       -- (x,y)������ֵ����Ӧ���ڵ�ɫ���е��±�
* 
*  ����ֵ��  void     
*
*  ˵����    �ú������±�index���滻ԭDIB�е�����
*            ������SetPixelColor()������
************************************************************************/

void CDIB::SetPixelIndex(int x,int y,BYTE index)
{

	if ((m_lpBits==NULL)||(m_nColors==0)||
		(x<0)||(y<0)||(x>=m_nWidth)||(y>=m_nHeight)) return ;

	BYTE * pBits = m_lpBits;
	
	if (m_nBitCount==8)
	{
	    *(pBits+y*GetWidthInBytes(m_nBitCount,m_nWidth) + x)=index;
		return;
	} 
	else 
	{
		BYTE pos;
		BYTE* iDst= pBits + y*GetWidthInBytes(m_nBitCount,m_nWidth) + (x*m_nBitCount >> 3);
		if (m_nBitCount==4)
		{
			pos = (BYTE)(4*(1-x%2));
			*iDst &= ~(0x0F<<pos);
			*iDst |= ((index & 0x0F)<<pos);
			return;
		} 
		else if (m_nBitCount==1)
		{
			pos = (BYTE)(7-x%8);
			*iDst &= ~(0x01<<pos);
			*iDst |= ((index & 0x01)<<pos);
			return;
		}
	}	
}


/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  �������ƣ�GetNearestIndex
* 
*  ����������RGBQUAD color  -- ��ɫ���һ����Ԫ�ṹ
*
*  ����ֵ��  BYTE           -- �����ɫ�嵥Ԫ����Ӧ������
*
*  ˵����    �ú��������õ������ɫ�嵥Ԫ����Ӧ��������
*            ����Ϊ���� SetPixelIndex()��һ�����������á�
************************************************************************/

BYTE CDIB::GetNearestIndex(RGBQUAD color)
{

	if ((m_lpBits==NULL)||(m_nColors==0)) return 0;

	BYTE* iDst = m_lpBits;
	long distance=200000;
	int i,j=0;
	long k,l;

	for(i=0,l=0;i<m_nColors;i++,l+=sizeof(RGBQUAD))
	{
		k = (iDst[l]-color.rgbBlue)*(iDst[l]-color.rgbBlue)+
			(iDst[l+1]-color.rgbGreen)*(iDst[l+1]-color.rgbGreen)+
			(iDst[l+2]-color.rgbRed)*(iDst[l+2]-color.rgbRed);
//		k = abs(iDst[l]-c.rgbBlue)+abs(iDst[l+1]-c.rgbGreen)+abs(iDst[l+2]-c.rgbRed);
		if (k==0)
		{
			j=i;
			break;
		}
		if (k<distance){
			distance=k;
			j=i;
		}
	} 
	
	return (BYTE)j;
}

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  �������ƣ�SetPixelColor
* 
*  ����������int x            -- ������
*            int y            -- ������
*            RGBQUAD  color   -- RGBQUAD�ṹ�ĵ�ɫ������ֵ
* 
*  ����ֵ��  void     
*
*  ˵����    �ú�����color�ж�Ӧ������ֵ����(x,y)��Ӧ������ֵ 
*            ������UpdateDIB(������
************************************************************************/

void CDIB:: SetPixelColor(int x,int y,RGBQUAD color)
{
    if ((m_lpBits==NULL)||(x<0)||(y<0)||
		  (x>=m_nWidth)||(y>=m_nHeight)) 
	return;

	y = m_nHeight -1 - y;   // add by maple, 2004.5.14
	
	BYTE * pBits = m_lpBits;

	if (m_nColors)
		SetPixelIndex(x,y,GetNearestIndex(color));
	else 
	{
		BYTE* iDst = pBits + y*GetWidthInBytes(m_nBitCount,m_nWidth) + x*3;
		*iDst++ = color.rgbBlue;
		*iDst++ = color.rgbGreen;
		*iDst   = color.rgbRed;
	}
}

/***********************************************************************
*  Description: this function was added by maple,2004.3.18
*
*  �������ƣ�SetPixelColor2
* 
*  ����������int x            -- ������
*            int y            -- ������
*            BYTE  color      -- ָ�������Ӧ������ֵ  
* 
*  ����ֵ��  void     
*
*  ˵����    �ú���ר����256ɫ�Ҷ�ͼ�� 
*            ��color�ж�Ӧ������ֵ����(x,y)��Ӧ������ֵ�� 
*            ������UpdateDIB(�����û��ߵ���ʹ�á�
************************************************************************/

void CDIB:: SetPixelColor2(int x,int y,BYTE color)
{
    if ((m_lpBits==NULL)||(x<0)||(y<0)||
		  (x>=m_nWidth)||(y>=m_nHeight)) 
	return;
    
    y = m_nHeight -1 - y;   // add by maple, 2004.5.14

	BYTE * pBits = m_lpBits;
 
	*(pBits+y*GetWidthInBytes(m_nBitCount,m_nWidth) + x) = color;		
}



/***********************************************************************
*  Description: this function was added by maple,2004.3.22
*
*  �������ƣ�CreatDIBFromBits
* 
*  ����������int nWidth    -- Ҫ�½���ͼ��Ŀ��   
*            int nHeight   -- Ҫ�½���ͼ��ĸ߶�
*            BYTE * buf    -- �������ֵ���ڴ�����ָ��
* 
*  ����ֵ��  bool
*
*  ˵����    �ú�������һ��������ֵ���ڴ�����������һ��DIB����
*            ������ͼ����24λλͼ��            
************************************************************************/

//load buf (nwidth*nheight) data to dib, if dib has data space, it is assumed dib also has size and nbits
//otherwise, allocate memory for dib data. 
bool CDIB::CreateDIBFromBits(int nWidth,int nHeight,BYTE * buf,int nBits)
{
    RGBQUAD color;

	if(m_lpBits!=NULL)     
	{
		if(nWidth!=m_nWidth||nHeight!=m_nHeight)
		{
			AfxMessageBox("Incompatible image size in CreateDIBFromBits!");
			return false;
		}
		BYTE * pBits = m_lpBits;

		memset(pBits,0,GetWidthInBytes(m_nBitCount,nWidth)*nHeight);
		int tick=nBits>>3;
		if(m_nBitCount==8)
		{
			for( int j=0; j<nHeight; j++)
			for( int i=0; i<nWidth; i++)
			{   
 				unsigned long temp=(long)j*(long)nWidth*tick+(long)i*tick;					
				BYTE gray;
				gray=buf[temp];		
			
				SetPixelColor2( i, j, gray ) ;
			} 		
		}
		else if(m_nBitCount>=16)
		{
		for( int j=0; j<nHeight; j++)
			for( int i=0; i<nWidth; i++)
			{   
 				unsigned long temp=(long)j*(long)nWidth*tick+(long)i*tick;				
				BYTE r,g,b;
				b=*(buf+temp);
				g=*(buf+temp+(1>=tick?0:1));
				r=*(buf+temp+(2>=tick?0:2));
				
				color.rgbRed   = r;
				color.rgbGreen = g;
				color.rgbBlue  = b;
				SetPixelColor( i, j, color ) ;
			} 		
		}
		else
		{
			AfxMessageBox("Image format not supported in CreateDIBFromBits!");
			return false;
		}
		return true;
	}
	else
	{
		
		m_nWidth=nWidth;
		m_nHeight=nHeight;
		m_nBitCount=24;
		int byteLine=BYTE_PER_LINE(nWidth, 24);
		m_lpBits=new BYTE[byteLine*nHeight];
		BYTE * pBits = m_lpBits;
		int tick=nBits>>3;
		memset(m_lpBits,0, sizeof(BYTE)*byteLine*nHeight);
		for( int j=0; j<nHeight; j++)
			for( int i=0; i<nWidth; i++)
			{   
				unsigned long temp=(long)j*(long)nWidth*tick+(long)i*tick;				
				
				BYTE* iDst = pBits + (nHeight -1 - j)*byteLine + i*3;
				*iDst++ =*(buf+temp);//blue
				*iDst++ =*(buf+temp+(1>=tick?0:1));//green
				*iDst   =*(buf+temp+(2>=tick?0:2));//red
			} 		
		return(true);
	}
}
//load dib to iplimage structure's imageData member
int CDIB::LoadDIBToIPL(char*imageData,int nBits)
{	
	if(m_lpBits == NULL)
		return 0;
	int widthStep=BYTE_PER_LINE(m_nWidth, 24);
	RGBQUAD color ;
	COLORREF colorref;
	BYTE* buf;
	if(m_nBitCount>=16&&nBits>=16)
	{
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				color = GetPixelColor( i, j );
				colorref =RGB(color.rgbRed, color.rgbGreen, color.rgbBlue);
				
				buf=(BYTE*)(imageData + widthStep*j+i*3);
				
				*(buf)=GetBValue(colorref);
				*(buf+1)=GetGValue(colorref);
				*(buf+2)=GetRValue(colorref);
			}
	}
	else if(m_nBitCount==8&&nBits>=16)
	{
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				BYTE color = GetPixelColor2( i, j );
				buf=(BYTE*)(imageData + widthStep*j+i*3);			
				
				*(buf)=color;
				*(buf+1)=color;
				*(buf+2)=color;
			}
	}
	else if(m_nBitCount>=16&&nBits==8)
	{
		widthStep=BYTE_PER_LINE(m_nWidth, 8);
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				color = GetPixelColor( i, j );
				colorref =RGB(color.rgbRed, color.rgbGreen, color.rgbBlue);
				
				buf=(BYTE*)(imageData + widthStep*j+i);
				
				*(buf)=GetBValue(colorref);
			}
	}
	else if(m_nBitCount==8&&nBits==8)
	{
		widthStep=BYTE_PER_LINE(m_nWidth, 8);
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				BYTE color = GetPixelColor2( i, j );
				buf=(BYTE*)(imageData + widthStep*j+i);				
				*(buf)=color;
			}
	}
	else	return 0;
	return 1;
	
}
//create dib from iplimage's imagedata, nbits is the bit count of image data for each pixel
int CDIB::CreateDIBFromIPL(int w,int h, char*imageData,int nBits)
{
    RGBQUAD color;

	if(m_lpBits!=NULL)     
	{
		if(w!=m_nWidth||h!=m_nHeight)
		{
			AfxMessageBox("Incompatible image size in CreateDIBFromIPL!");
			return false;
		}
		BYTE * pBits = m_lpBits;
		int byteLine=BYTE_PER_LINE(w, nBits);
		memset(pBits,0, GetWidthInBytes(m_nBitCount,w)*h);
		if(m_nBitCount==8)
		{
			int tick=nBits>>3;
			for( int j=0; j<h; j++)
			for( int i=0; i<w; i++)
			{   
 				BYTE *temp=((unsigned char*)imageData)+j*byteLine+i*tick;					
		
				SetPixelColor2( i, j,*temp) ;
			} 	
			
	
		}
		else if(m_nBitCount>=16)
		{
		int tick=nBits>>3;
		for( int j=0; j<h; j++)
			for( int i=0; i<w; i++)
			{   
				BYTE *temp=((unsigned char*)imageData)+j*byteLine+i*tick;	
 							
				BYTE r,g,b;
				b=*(temp);
				g=*(temp+(1>=tick?0:1));
				r=*(temp+(2>=tick?0:2));
				
				color.rgbRed   = r;
				color.rgbGreen = g;
				color.rgbBlue  = b;
				SetPixelColor( i, j, color ) ;
			} 		
		}
		else
		{AfxMessageBox("Unsupported format conversion between CDIB and IplImage!");
			return false;}
		return true;
	}
	else
	{		
		m_nWidth=w;
		m_nHeight=h;
		m_nBitCount=24;
		int byteLine=BYTE_PER_LINE(w, 24);
		m_lpBits=new BYTE[byteLine*h];
		BYTE * pBits = m_lpBits;
		int byteLine2=BYTE_PER_LINE(w, nBits);
		memset(m_lpBits,0, sizeof(BYTE)*byteLine*h);
		int tick=nBits>>3;
		for( int j=0; j<h; j++)
		{
			for( int i=0; i<w; i++)
			{   
				BYTE *temp=((unsigned char*)imageData)+j*byteLine2+i*tick;	
				BYTE r,g,b;
				b=*(temp);
				g=*(temp+(1>=tick?0:1));
				r=*(temp+(2>=tick?0:2));
				
				color.rgbRed   = r;
				color.rgbGreen = g;
				color.rgbBlue  = b;
				SetPixelColor( i, j, color ) ;
			} 	
		}
		return(true);
	}	
}

//load dib data to buf(width*height*3)
int CDIB::LoadDIBToBuf(BYTE *buf)
{

	if(m_lpBits == NULL)
		return 0;
	RGBQUAD color ;
	COLORREF colorref;
	if(m_nBitCount>=16)
	{
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				color = GetPixelColor( i, j );
				colorref =RGB(color.rgbRed, color.rgbGreen, color.rgbBlue);
				unsigned long temp=(long)j*(long)m_nWidth*3+(long)i*3;
			  	    *(buf+temp)=GetBValue(colorref);
			        *(buf+temp+1)=GetGValue(colorref);
				    *(buf+temp+2)=GetRValue(colorref);
			}
	}
	else if(m_nBitCount==8)
	{
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				BYTE color = GetPixelColor2( i, j );
				unsigned long temp=(long)j*(long)m_nWidth*3+(long)i*3;
			  	    *(buf+temp)=color;
			        *(buf+temp+1)=color;
				    *(buf+temp+2)=color;
			}
	}
	else
		return 0;
	
 	return 1;

}
int CDIB::LoadDIBToBuf(float *buf)
{

	if(m_lpBits == NULL)
		return 0;
	RGBQUAD color ;
	COLORREF colorref;
	if(m_nBitCount>=16)
	{
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				color = GetPixelColor( i, j );
				colorref =RGB(color.rgbRed, color.rgbGreen, color.rgbBlue);
				unsigned long temp=(long)j*(long)m_nWidth*3+(long)i*3;
			  	    *(buf+temp)=GetBValue(colorref);
			        *(buf+temp+1)=GetGValue(colorref);
				    *(buf+temp+2)=GetRValue(colorref);
			}
	}
	else if(m_nBitCount==8)
	{
		for( int j=0; j<m_nHeight; j++)
			for(int i=0; i<m_nWidth; i++) 
			{
				BYTE color = GetPixelColor2( i, j );
				unsigned long temp=(long)j*(long)m_nWidth*3+(long)i*3;
			  	    *(buf+temp)=color;
			        *(buf+temp+1)=color;
				    *(buf+temp+2)=color;
			}
	}
	else
		return 0;
	
 	return 1;

}
