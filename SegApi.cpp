#include "stdafx.h"
#include "GlobalApi.h"
#include <queue>
#include "susan.h"
//#include "math.h"
//#include <direct.h>

using namespace std;
//following freeman code style as introduced in opencv reference
//roadseed points a memory space of size width*height with 1 denotes roads 0 for background 
//after processing, 2 for centerline
//vector<int>terminal returns the end pixels in form of serial number in roads after pruning
/*void RoadThin(BYTE*roadseed, int Width,int Height,vector<int>&terminal)
{
	int sernum=0,i,j,L=Height*Width;
	int x,y,tsize,hsize;
	BYTE*roads=new BYTE[L];
	memcpy(roads,roadseed,sizeof(BYTE)*L);
	ThinnerRosenfeld(roads,Height,Width);
	//	int **img2,xsize,ysize;
	/*	ysize=Width; 
	xsize=Height;
	img2=new int *[ysize];
	for (i=0; i<ysize; i++)
	img2[i] =new int[xsize];
	for(i=0;i<Height;i++)
	{
		for(j=0;j<Width;j++)
		{	      
			img2[j][i]=roads[i*Width+j];				
		}
	}
	trimming(img2,xsize,ysize,3);	*/
	//detect end nodes
/*	vector<int> edgecode;
	vector<int>::iterator vint;
	int normin[8]={0},dest[8]={0};
	int k,count,runlen,next;
//x,y coordinate for pixel, tsize the size of stack storing terminals, 
	//hsize terminals deleted during pruning
	const int dangle=10;
	bool flag;
		//do not take care border pixels
	for(i=0;i<Height;++i)
	{
		sernum=i*Width;
		roads[sernum]=0;
		roads[sernum+Width-1]=0;
	}
	sernum=(Height-1)*Width;
	for(j=0;j<Width;++j)
	{
		roads[j]=0;
		roads[sernum+j]=0;
	}
	for(i=1;i<Height-1;++i)
	{
		for(j=1;j<Width-1;++j)
		{
			sernum=i*Width+j;
			if(roads[sernum]==1)
			{
				normin[0]=sernum+1;
				normin[1]=sernum+1-Width;
				normin[2]=sernum-Width;
				normin[3]=sernum-1-Width;
				normin[4]=sernum-1;
				normin[5]=sernum-1+Width;
				normin[6]=sernum+Width;
				normin[7]=sernum+1+Width;
				count=0;
				for(k=0;k<8;++k)
				{
					if(roads[normin[k]])
					{
						dest[count]=k;
						++count;
					}
				}
				//there are spur lines on the left border of the image			

				//1 0 0  count==4	0	1	0 count==4
				//1 1 1	on the  	1	1	1 in the interior				
				//1 0 0 image border0	1	0 for cross		
			
				switch(count)
				{				
				case 1:
					terminal.push_back(sernum);
					break;
				case 2:
					if((dest[1]-dest[0]==1)||(dest[1]-dest[0]==7))
						terminal.push_back(sernum);
					break;
				case 3:
				//	terminal.push_back(sernum);
					break;
				case 4: break;
					default:
				AfxMessageBox("damn!singular point exists!");break;
				
				}
			}
		}
	}

	//prune dangling edges, note that the pruned edge may be longer than later processed spurs 
	tsize=terminal.size();
	hsize=0;
	for(i=0;i<tsize;++i)//for each end point
	{
		sernum=terminal[i];//position in image array
		assert(roads[sernum]>0);
			
		runlen=0;
		flag=true;//true for proceeding, false for halt while loop
		edgecode.clear();
		while(runlen<dangle)
		{
			assert(roads[sernum]==1);
			x=sernum%Width;
			y=sernum/Width;
			for(j=0;j<8;++j)
				normin[j]=0;
			if(x==0)
			{
				if(y==0)
				{					
					normin[0]=roads[sernum+1];
					normin[6]=roads[sernum+Width];
					normin[7]=roads[sernum+1+Width];
				}
				else if(y==Height-1)
				{
					normin[0]=roads[sernum+1];
					normin[1]=roads[sernum+1-Width];
					normin[2]=roads[sernum-Width];
				}
				else
				{
					normin[0]=roads[sernum+1];
					normin[1]=roads[sernum+1-Width];
					normin[2]=roads[sernum-Width];
					normin[6]=roads[sernum+Width];
					normin[7]=roads[sernum+1+Width];					
				}
			}
			else if(x==Width-1)
			{
				if(y==0)
				{
					normin[4]=roads[sernum-1];
					normin[5]=roads[sernum-1+Width];
					normin[6]=roads[sernum+Width];
				}
				else if(y==Height-1)
				{
					normin[2]=roads[sernum-Width];
					normin[3]=roads[sernum-1-Width];
					normin[4]=roads[sernum-1];
				}
				else
				{
					normin[2]=roads[sernum-Width];
					normin[3]=roads[sernum-1-Width];
					normin[4]=roads[sernum-1];
					normin[5]=roads[sernum-1+Width];
					normin[6]=roads[sernum+Width];
					
				}
			}
			else
			{
				if(y==0)
				{
					normin[0]=roads[sernum+1];
					normin[4]=roads[sernum-1];
					normin[5]=roads[sernum-1+Width];
					normin[6]=roads[sernum+Width];
					normin[7]=roads[sernum+1+Width];
				}
				else if(y==Height-1)
				{
					normin[0]=roads[sernum+1];
					normin[1]=roads[sernum+1-Width];
					normin[2]=roads[sernum-Width];
					normin[3]=roads[sernum-1-Width];
					normin[4]=roads[sernum-1];
				}
				else
				{
					normin[0]=roads[sernum+1];
					normin[1]=roads[sernum+1-Width];
					normin[2]=roads[sernum-Width];
					normin[3]=roads[sernum-1-Width];
					normin[4]=roads[sernum-1];
					normin[5]=roads[sernum-1+Width];
					normin[6]=roads[sernum+Width];
					normin[7]=roads[sernum+1+Width];
					
				}
			}					
			count=0;
			for(k=0;k<8;++k)
			{
				if(normin[k])
				{
					dest[count]=k;
					++count;
				}
			}
			switch(count)
			{				
			case 1:
				if(runlen>0)					
				{
					AfxMessageBox("small edge with length less than 7 exist!");
					flag=false;
				}
				next=dest[0];
				edgecode.push_back(sernum);
				++runlen;
				break;
			case 2:
				assert(runlen>0);
				next=next>3?next-4:next+4;
				next=dest[0]+dest[1]-next;
				
				edgecode.push_back(sernum);
				++runlen;
				break;
			case 3://we don't push back sernum here, bacause if that sernum is nullified in roads
				//the other spurs may not be found or fall into dead loops
			case 4://010
				   //111
				   //010
				flag=false;
				break;
			default: 
				AfxMessageBox("neighbor count illegal!");
				break;
				
			}
			if(flag==false)
				break;
			switch(next)
			{
			case 0:
				++sernum;
				break;
			case 1:
				sernum=sernum-Width+1;
				break;
			case 2:
				sernum=sernum-Width;
				break;
			case 3:
				sernum=sernum-Width-1;
				break;
			case 4:
				sernum=sernum-1;
				break;
			case 5:
				sernum=sernum+Width-1;
				break;
			case 6:
				sernum=sernum+Width;
				break;
			case 7:
				sernum=sernum+Width+1;
				break;
			default:
				AfxMessageBox("next neighbor index exceeds bound!");
			}				
			
		}
		assert(runlen==edgecode.size());
		if(runlen<dangle)//negative pixels in edgecode
		{
			for(k=0;k<runlen;++k)			
				roads[edgecode[k]]=0;
			++hsize;
		}	
	}
	edgecode.clear();
	//get really terminals
	hsize=tsize-hsize;
	vint=terminal.begin();
	while(vint!=terminal.end())
	{
		if(roads[*vint]==0)
		{			
			terminal.erase(vint);
		}
		else
		{
			++vint;
		}
	}
	assert(hsize==terminal.size());
	for(i=0;i<L;++i)
		roadseed[i]+=roads[i];

	//show result
/*	CvSize bound=cvSize(Width,Height);
	IplImage*portal=cvCreateImage(bound, IPL_DEPTH_8U , 3);
	cvNamedWindow("Portal", 0);		
	cvZero(portal);
	for(i=0;i<Height;++i)
	{
		for(j=0;j<Width;++j)
		{				
			if(roads[i*Width+j]>0)//(img2[j][i]==1)
			{	uchar* temp_ptr = &((uchar*)(portal->imageData + portal->widthStep*i))[j*3];
			temp_ptr[0]=0;
			temp_ptr[1]=255;
			temp_ptr[2]=0;
			}		
		}   
	}
	count=terminal.size();
	for(k=0;k<count;++k)
	{
		i=terminal[k]/Width;
		j=terminal[k]%Width;
		CvPoint center=cvPoint(j,i);		
		cvCircle( portal, center, 2, CV_RGB (255, 0, 0 ),1);
	}

	cvFlip(portal);
	cvShowImage("Portal", portal);
	cvWaitKey(0);
	cvDestroyWindow("Portal");
	cvReleaseImage(&portal);*
	
	delete[]roads;

}*/
//process binary image data stored in EM with white background
void Morph(BYTE*EM, int Width,int Height,int opt)
{
	int i,j;
	int *img;
	int **img2;
	int xsize,ysize;
	//2 3 5 outperform others, test image has bright background
	//before processing negative or binarize might be required
	//note the order of height and width in the parameters list of each thinning operation
	switch(opt)
	{
	case 1:
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{	      
				EM[i*Width+j]=~EM[i*Width+j];
			}
		}
		
		ThinnerPavlidis(EM,Height,Width);
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{
				
				if(EM[i*Width+j]!=0)
					EM[i*Width+j]=(BYTE)255;
			}   
		}
		break;
	case 2:	
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{	      
				EM[i*Width+j]=~EM[i*Width+j];
			}
		}
		
		ThinnerHilditch(EM,Height,Width);
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{
				
				if(EM[i*Width+j]!=0)
					EM[i*Width+j]=(BYTE)255;
			}   
		}
		break;
	case 3:
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{	      
				if(EM[i*Width+j]==0)
					EM[i*Width+j]=1;
				
				else
					EM[i*Width+j]=0;
			}
		}
		ThinnerRosenfeld(EM,Height,Width);
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{
				
				if(EM[i*Width+j]==1)
					EM[i*Width+j]=(BYTE)255;
				
				else
					EM[i*Width+j]=(BYTE)0;
			}   
		}
		break;
	case 4:

		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{	      
				EM[i*Width+j]=~EM[i*Width+j];
			}
		}
		ThiningDIBSkeleton (EM,Width,Height);
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{
				
				if(EM[i*Width+j]!=0)
					EM[i*Width+j]=(BYTE)255;
			}   
		}
		break;
	case 5://from image-j1 
		img=new int[Height*Width];
	
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{	      
				if(EM[i*Width+j]==0)
					img[i*Width+j]=1;				
				else
					img[i*Width+j]=0;
			}
		}
		Thin_2(img,0, 0 , (int)Height,(int)Width);
		
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{				
				if(img[i*Width+j]!=0)
					EM[i*Width+j]=(BYTE)0;
				else EM[i*Width+j]=(BYTE)255;
			}   
		}
		delete[]img;
		break;
	case 6://from mmofunc not effective
		ysize=Width; 
		xsize=Height;
		img2=new int *[ysize];
		for (i=0; i<ysize; i++)
			img2[i] =new int[xsize];
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{	      
				if(EM[i*Width+j]==0)
					img2[j][i]=1;				
				else
					img2[j][i]=0;
			}
		}
		thinning(img2,xsize,ysize,3,3);
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{				
				if(img2[j][i]!=0)
					EM[i*Width+j]=(BYTE)255;
			}   
		}
		for (i=0; i<ysize; i++)
		{
			delete[] img2[i];
			
		}
		delete[]img2;
	
		break;
	case 7://THIN BLACK THREAD IN BINARY IMAGE
		ysize=Width; 
		xsize=Height;
		img2=new int *[ysize];
		for (i=0; i<ysize; i++)
			img2[i] =new int[xsize];
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{	      
				if(EM[i*Width+j]==0)
					img2[j][i]=1;				
				else
					img2[j][i]=0;
			}
		}
		trimming(img2,xsize,ysize,3);
		for(i=0;i<Height;i++)
		{
			for(j=0;j<Width;j++)
			{				
				if(img2[j][i]!=0)
					EM[i*Width+j]=(BYTE)0;
				else
					EM[i*Width+j]=255;
			}   
		}
		for (i=0; i<ysize; i++)
		{
			delete[] img2[i];		
		}
		delete[]img2;
	
		break;
	default: break;
	}	
}
BYTE* EdgeSusan(BYTE*EM, int w,int h,int BT,bool Trixtri)
{
	int L=h*w,y=0;
	BYTE*pPE=EM;
	susanedge(EM,w,h,BT,Trixtri);//mark edge pixels black on white background 
	//convert to 255 standing for edge and 0 for nonedge
	/*for(y=0;y<L;++y)
	{
			*pPE=255-(*pPE);						
			++pPE;			
	}*/
	//if more accurate tuning is necessary, susan is optional.
	return EM;
}
void GaussianFilter(BYTE*EM,int Width, int Height,float sigma)
{
	int L=Width*Height;
	BYTE*pUnchSmooth=new BYTE[L];

	GaussianSmooth(EM, Width, Height, sigma, pUnchSmooth);
	memcpy(EM,pUnchSmooth,sizeof(BYTE)*L);
	delete[]pUnchSmooth;
}
void EdgeMag(BYTE*EM,int w,int h)
{

	float *exEM = new float[w*h];
	float max=0.f,min=256.f;
	float *ptr=exEM;
	int i=0,j=0;
	// ����SOBEL FILTER compute gradient absolute value
	Gradient(EM,w,h,exEM);
	for(j=0;j<h;++j)
		for(i=0;i<w;++i)
		{
			if(*ptr>max)
				max=*ptr;
			if(*ptr<min)
				min=*ptr;
			++ptr;
		}
		ptr=exEM;
		BYTE*p2=EM;
		max-=min;
	for(j=0;j<h;++j)
		for(i=0;i<w;++i)
		{
			*p2=(*ptr-min)/max*255;
			++p2;++ptr;
		}
	delete []exEM;
	exEM=NULL;
}
//edgepoints finds edge candidate points in edge channel
//input:edge channel with arbitrary size output: edge map with 255 for edge point
//the edge channel is processed by gaussian filter and then canny filter
void EdgePoints(BYTE* EM,int w,int h,float sig, float low, float high)
{
	BYTE* input = new BYTE[w*h];
	memcpy(input,EM,sizeof(BYTE)*w*h);
	// ����canny�������б߽���ȡ
	Canny(input,w,h, sig,low,high, EM);
	
	delete[] input;
}

MyLUV*  GetNearPixelsLUV(int xPos, int yPos
	  , MyLUV* inLUVs, int picWidth, int picHeight
	  , int inScale, int& outWidth, int& outHeight)
//�õ�LUV����
{
	//���ȼ�����ܵ����������
	int templeft = xPos - inScale;
	int tempright = xPos + inScale;
	int left, right, up, down;//������������±߽磻
	if (templeft>0 && tempright<picWidth)
	{
		left = inScale;
		right = inScale;
	}else
	{
		if (templeft<=0)
		{
			left = xPos;
		}else
		{
			left = inScale;
		}
		if (tempright>=picWidth)
		{
			right = picWidth - xPos - 1;
		}else
		{
			right = inScale;
		}
	}

	int temptop = yPos - inScale;
	int tempbottom = yPos + inScale;
	if ( temptop>0 && tempbottom<picHeight )
	{
		up = inScale;
		down = inScale;
	}else
	{
		if (temptop<=0)
		{
			up = yPos;
		}else
		{
			up = inScale;
		}
		if (tempbottom>=picHeight)
		{
			down = picHeight - yPos - 1;
		}else
		{
			down = inScale;
		}
	}

	outWidth = left + right + 1;
	outHeight = up + down + 1;

	LONG outlen = outWidth * outHeight;
	MyLUV* outarr = new MyLUV[outlen];

	LONG pos = 0;
	for (int x=0; x<outWidth; x++)
	{
		for (int y=0; y<outHeight; y++)
		{
			pos = ( y*outWidth + x );
			//����������ĵ�x,y��Ԫ����ԭ�����е�X��Yλ�ã�
			int originx = xPos - left + x;
			int originy = yPos - up + y;
			//�������ͼ�������е�λ�ã�
			LONG inpos = (originy * picWidth + originx); 
			outarr[pos].l = inLUVs[inpos].l;
			outarr[pos].u = inLUVs[inpos].u;
			outarr[pos].v = inLUVs[inpos].v;
		}
	}
	
	return outarr;
}

void  GetNearPixelsGreenExt(int xPos, int yPos
	    , BYTE* inPixels, int picWidth, int picHeight
	    , int radius, BYTE** outArr)
//�õ���������ֵ(������,Gͨ��),����λ�ô�0��ʼ����, ��Ե���Գ����أ�
{
	int matrixwidth = (radius*2+1);
	BYTE* temparr = new BYTE[matrixwidth*matrixwidth];//����ָ��������

	LONG pos = 0;
	int rposx, rposy;
	rposx = rposy = 0;//��ͼ���е�λ�ã�

	for (int y=-radius; y<=radius; y++)
	{
		rposy = yPos+y;
		if (rposy<0)
		{
			rposy = -rposy;
		}else if (rposy>=picHeight)
		{
			rposy = picHeight - (rposy-picHeight);
		}
		
		for (int x=-radius; x<=radius; x++)
		{
			rposx = xPos+x;
			if (rposx<0)
			{
				rposx = -rposx;
			}else if (rposx>=picWidth)
			{
				rposx = picWidth - (rposx-picWidth);
			}

			//�������ͼ�������е�λ�ã�
			LONG inpos = ( rposy*picWidth + rposx ) * 3 + 1;//RGB��ɫֵ��
			//����������е�λ�ã�
			LONG pos = ( (y+radius)*matrixwidth + (x+radius) );
			temparr[pos] = ( inPixels[inpos] );
		}
	}

	*outArr = temparr;
}


void  GetNearPixelsExt(int xPos, int yPos
	, BYTE* inPixels, int picWidth, int picHeight
	, int radius, BYTE** outArr)
//�õ���������ֵ(������),����λ�ô�0��ʼ����, ��Ե�����أ�
{
	int matrixwidth = (radius*2+1);
	BYTE* temparr = new BYTE[matrixwidth*matrixwidth*3];//����ָ��������

	LONG pos = 0;
	int rposx, rposy;
	rposx = rposy = 0;//��ͼ���е�λ�ã�

	for (int y=-radius; y<=radius; y++)
	{
		rposy = yPos+y;
		if (rposy<0)
		{
			rposy = -rposy;
		}else if (rposy>=picHeight)
		{
			rposy = picHeight - (rposy-picHeight);
		}
		
		for (int x=-radius; x<=radius; x++)
		{
			rposx = xPos+x;
			if (rposx<0)
			{
				rposx = -rposx;
			}else if (rposx>=picWidth)
			{
				rposx = picWidth - (rposx-picWidth);
			}

			//�������ͼ�������е�λ�ã�
			LONG inpos = ( rposy*picWidth + rposx ) * 3;//RGB��ɫֵ��
			//����������е�λ�ã�
			LONG pos = ( (y+radius)*matrixwidth + (x+radius) ) * 3;
			temparr[pos] = inPixels[inpos];
			temparr[pos+1] = inPixels[inpos+1];
			temparr[pos+2] = inPixels[inpos+2];
		}
	}

	*outArr = temparr;
}


BYTE*  GetNearPixels(int xPos, int yPos, 
      BYTE* inPixels, int picWidth, int picHeight, int inScale, 
	  int& outWidth, int& outHeight)
//�õ���������ֵ, ����λ�ô�0��ʼ������
{
	//���ȼ�����ܵ����������
	int templeft = xPos - inScale;
	int tempright = xPos + inScale;
	int left, right, up, down;//������������±߽磻
	if (templeft>0 && tempright<picWidth)
	{
		//outWidth = inScale * 2 + 1;//��1����������ر���
		left = inScale;
		right = inScale;
	}else
	{
		if (templeft<=0)
		{
			//outWidth += xPos;//����ȫ�������ڣ�
			left = xPos;
		}else
		{
			left = inScale;
		}
		if (tempright>=picWidth)
		{
			//outWidth += picWidth - xPos - 1;//����ȫ�������ڣ�
			right = picWidth - xPos - 1;
		}else
		{
			right = inScale;
		}

/*
		if (outWidth>picWidth)
		{
			outWidth = picWidth;//��������ͼ�����ȣ�
		}
*/
	}

	int temptop = yPos - inScale;
	int tempbottom = yPos + inScale;
	if ( temptop>0 && tempbottom<picHeight )
	{
		//outWidth = inScale * 2 + 1;//��1����������ر���
		up = inScale;
		down = inScale;
	}else
	{
		if (temptop<=0)
		{
			up = yPos;
		}else
		{
			up = inScale;
		}
		if (tempbottom>=picHeight)
		{
			down = picHeight - yPos - 1;
		}else
		{
			down = inScale;
		}
/*
		if (outWidth>picWidth)
		{
			outWidth = picWidth;//��������ͼ�����ȣ�
		}
*/
	}

	outWidth = left + right + 1;
	outHeight = up + down + 1;

	LONG outlen = outWidth * outHeight;
	BYTE* outarr = new BYTE[outlen*3];

	LONG pos = 0;
	for (int x=0; x<outWidth; x++)
	{
		for (int y=0; y<outHeight; y++)
		{
			pos = ( y*outWidth + x ) * 3;//RGB��ɫֵ��
			//����������ĵ�x,y��Ԫ����ԭ�����е�X��Yλ�ã�
			int originx = xPos - left + x;
			int originy = yPos - up + y;
			//�������ͼ�������е�λ�ã�
			LONG inpos = (originy * picWidth + originx) * 3; 
			outarr[pos] = inPixels[inpos];
			outarr[pos+1] = inPixels[inpos+1];
			outarr[pos+2] = inPixels[inpos+2];
		}
	}
	
	return outarr;
}
void GetGradient(BYTE* image, int width, int height
		, FLOAT* deltar, FLOAT* deltasita)
//�õ�����ͼ����ݶȣ�
{
	//��������������ˮƽ�ʹ�ֱ�����ϵ��ݶ�,��Ե���ݶȼ�Ϊ0��
	int* deltaxarr;
	int* deltayarr;
	int grawidth = width;
	int graheight = height;
	int deltacount = grawidth * graheight;
	deltaxarr = new int[deltacount];
	deltayarr = new int[deltacount];

    //�ݲ������Ե�㣻
	for (int y=1; y<graheight-1; y++)
	{
		for (int x=1; x<grawidth-1; x++)
		{
			int inarrpos = ((y)*width + (x))*3 + 1;//��������е�λ�ã�
			int deltaarrpos = y*grawidth + x;//���ݶ������е�λ�ã�
			//������㣻
			deltaxarr[deltaarrpos] = (int) ( (
				image[((y-1)*width + (x+1))*3 + 1] //����
				+ image[((y)*width + (x+1))*3 + 1] //��
				+ image[((y+1)*width + (x+1))*3 + 1] //����
				- image[((y-1)*width + (x-1))*3 + 1] //����
				- image[((y)*width + (x-1))*3 + 1] //��
				- image[((y+1)*width + (x-1))*3 + 1] ) / 3 );//����
			deltayarr[deltaarrpos] = (int) ( ( 
				image[((y-1)*width + (x+1))*3 + 1] //����
				+ image[((y-1)*width + (x))*3 + 1] //��
				+ image[((y-1)*width + (x-1))*3 + 1] //����
				- image[((y+1)*width + (x-1))*3 + 1] //����
				- image[((y+1)*width + (x))*3 + 1] //��
				- image[((y+1)*width + (x+1))*3 + 1]) / 3 );//����
		}
	}

	//��Ե��Ϊ���ڲ���ֵ��
	for (y=0; y<graheight; y++)
	{
		int x1 = 0;
		int pos1 = y*grawidth + x1;
		deltaxarr[pos1] = deltaxarr[pos1+1];
		deltayarr[pos1] = deltayarr[pos1+1];
		int x2 = grawidth-1;
		int pos2 = y*grawidth + x2;
		deltaxarr[pos2] = deltaxarr[pos2-1];
		deltayarr[pos2] = deltayarr[pos2-1];
	}
	for (int x=0; x<grawidth; x++)
	{
		int y1 = 0;
		int pos1 = x;
		int inner = x + grawidth;//��һ�У�
		deltaxarr[pos1] = deltaxarr[inner];
		deltayarr[pos1] = deltayarr[inner];
		int y2 = graheight-1;
		int pos2 = y2*grawidth + x;
		inner = pos2 - grawidth;//��һ�У�
		deltaxarr[pos2] = deltaxarr[inner];
		deltayarr[pos2] = deltayarr[inner];
	}


	for (y=0; y<graheight; y++)
	{
		for (x=0; x<grawidth; x++)
		{
			int temppos = y*grawidth + x;
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
			deltasita[temppos] = (FLOAT) ( atan( 
				(FLOAT)deltayarr[temppos]
				/ (FLOAT)deltaxarr[temppos] ) + PI/2. );
			deltar[temppos] = (FLOAT) sqrt((DOUBLE) 
				( deltayarr[temppos]*deltayarr[temppos]
				+ deltaxarr[temppos]*deltaxarr[temppos] ) );
		}
	}

	delete [] deltaxarr; deltaxarr = NULL; //ɾ��ˮƽ�ʹ�ֱ�ݶ����飻
	delete [] deltayarr; deltayarr = NULL;
}

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
void  FloodVincent(MyImageGraPt* imiarr,int imageWidth,int imageHeight, int* graddarr, int minh, int maxh, int* flagarr, int& outrgnumber)
{
	const int INIT = -2;
	const int MASK = -1;
	const int WATERSHED = 0;
	int h = 0;
	int imagelen = imageWidth * imageHeight;
	for (int i=0; i<imagelen; i++)
	{
		flagarr[i] = INIT;
	}
	//memset(flagarr, INIT, sizeof(int)*imagelen);
	int* imd = new int[imagelen];//�������飬ֱ�Ӵ�ȡ��
	for (i=0; i<imagelen; i++)
	{
		imd[i] = 0;
	}
	//memset(imd, 0, sizeof(int)*imagelen);
	std::queue <int> myqueue;
	int curlabel = 0;//����ر�ǣ�

	for (h=minh; h<=maxh; h++)
	{
		int stpos = graddarr[h];
		int edpos = graddarr[h+1];
		for (int ini=stpos; ini<edpos; ini++)
		{
			int x = imiarr[ini].x;
			int y = imiarr[ini].y;
			int ipos = y*imageWidth + x;
			flagarr[ipos] = MASK;
			//���¼��õ������Ƿ��ѱ������ĳ�����ˮ�룬���ǣ��򽫸õ����fifo;
			int left = ipos - 1;
			if (x-1>=0) 
			{
				if (flagarr[left]>=0)
				{
					imd[ipos] = 1;
					myqueue.push(ipos);//��λ��ѹ��fifo;
					continue;
				}				
			}
			int right = ipos + 1;
			if (x+1<imageWidth) 
			{
				if (flagarr[right]>=0) 
				{
					imd[ipos] = 1;
					myqueue.push(ipos);//��λ��ѹ��fifo;
					continue;
				}
			}
			int up = ipos - imageWidth;
			if (y-1>=0) 
			{
				if (flagarr[up]>=0)
				{
					imd[ipos] = 1;
					myqueue.push(ipos);//��λ��ѹ��fifo;
					continue;
				}				
			}
			int down = ipos + imageWidth;
			if (y+1<imageHeight)
			{
				if (flagarr[down]>=0) 
				{
					imd[ipos] = 1;
					myqueue.push(ipos);//��λ��ѹ��fifo;
					continue;
				}			
			}
		}

    	//���¸����Ƚ��ȳ�������չ������أ�
		int curdist = 1; myqueue.push(-99);//�����ǣ�
		while (TRUE)
		{
			int p = myqueue.front();
			myqueue.pop();
			if (p == -99)
			{
				if ( myqueue.empty() )
				{
					break;
				}else
				{
					myqueue.push(-99);
					curdist = curdist + 1;
					p = myqueue.front();
					myqueue.pop();
				}
			}

			//������p������
			int y = (int) (p/imageWidth);
			int x = p - y*imageWidth;
			int left = p - 1;
			if  (x-1>=0)
			{
				if ( ( (imd[left]<curdist) && flagarr[left]>0)
					|| (flagarr[left]==0) ) 
				{
					if ( flagarr[left]>0 )
					{
						//ppei����ĳ���򣨲��Ƿ�ˮ�룩��
						if ( (flagarr[p]==MASK) 
							|| (flagarr[p]==WATERSHED) )
						{
							//������Ϊ�ڵ���������
							flagarr[p] = flagarr[left];
						}else if (flagarr[p]!=flagarr[left])
						{
							//ԭ�������������ڸ�������ͬ����Ϊ��ˮ�룻
							//flagarr[p] = WATERSHED;
						}
					}else if (flagarr[p]==MASK)//ppeiΪ���룻
					{
						flagarr[p] = WATERSHED;
					}
				}else if ( (flagarr[left]==MASK) && (imd[left]==0) )
				//ppei����MASK�ĵ㣬����δ��ǣ�������ĳ��Ҳ���Ƿ�ˮ�룩;
				{
					imd[left] = curdist + 1; myqueue.push(left);
				}
			}
			
			int right = p + 1;
			if (x+1<imageWidth) 
			{
				if ( ( (imd[right]<curdist) &&  flagarr[right]>0)
					|| (flagarr[right]==0) )
				{
					if ( flagarr[right]>0 )
					{
						//ppei����ĳ���򣨲��Ƿ�ˮ�룩��
						if ( (flagarr[p]==MASK) 
							|| (flagarr[p]==WATERSHED) )
						{
							//������Ϊ�ڵ���������
							flagarr[p] = flagarr[right];
						}else if (flagarr[p]!=flagarr[right])
						{
							//ԭ�������������ڸ�������ͬ����Ϊ��ˮ�룻
							//flagarr[p] = WATERSHED;
						}
					}else if (flagarr[p]==MASK)//ppeiΪ���룻
					{
						flagarr[p] = WATERSHED;
					}
				}else if ( (flagarr[right]==MASK) && (imd[right]==0) )
					//ppei����MASK�ĵ㣬����δ��ǣ�������ĳ��Ҳ���Ƿ�ˮ�룩;
				{
					imd[right] = curdist + 1; myqueue.push(right);
				}
			}
			
			int up = p - imageWidth;
			if (y-1>=0) 
			{
				if ( ( (imd[up]<curdist) &&  flagarr[up]>0)
					|| (flagarr[up]==0) )
				{
					if ( flagarr[up]>0 )
					{
						//ppei����ĳ���򣨲��Ƿ�ˮ�룩��
						if ( (flagarr[p]==MASK) 
							|| (flagarr[p]==WATERSHED) )
						{
							//������Ϊ�ڵ���������
							flagarr[p] = flagarr[up];
						}else if (flagarr[p]!=flagarr[up])
						{
							//ԭ�������������ڸ�������ͬ����Ϊ��ˮ�룻
							//flagarr[p] = WATERSHED;
						}
					}else if (flagarr[p]==MASK)//ppeiΪ���룻
					{
						flagarr[p] = WATERSHED;
					}
				}else if ( (flagarr[up]==MASK) && (imd[up]==0) )
					//ppei����MASK�ĵ㣬����δ��ǣ�������ĳ��Ҳ���Ƿ�ˮ�룩;
				{
					imd[up] = curdist + 1; myqueue.push(up);
				}
			}
			
			int down = p + imageWidth;
			if (y+1<imageHeight) 
			{
				if ( ( (imd[down]<curdist) &&  flagarr[down]>0)
					|| (flagarr[down]==0) )
				{
					if ( flagarr[down]>0 )
					{
						//ppei����ĳ���򣨲��Ƿ�ˮ�룩��
						if ( (flagarr[p]==MASK) 
							|| (flagarr[p]==WATERSHED) )
						{
							//������Ϊ�ڵ���������
							flagarr[p] = flagarr[down];
						}else if (flagarr[p]!=flagarr[down])
						{
							//ԭ�������������ڸ�������ͬ����Ϊ��ˮ�룻
							//flagarr[p] = WATERSHED;
						}
					}else if (flagarr[p]==MASK)//ppeiΪ���룻
					{
						flagarr[p] = WATERSHED;
					}
				}else if ( (flagarr[down]==MASK) && (imd[down]==0) )
					//ppei����MASK�ĵ㣬����δ��ǣ��Ȳ���ĳ��Ҳ���Ƿ�ˮ�룩;
				{
					imd[down] = curdist + 1; myqueue.push(down);
				}	
			}

		}//����������ص���չ��

		//���´����·��ֵ���أ�
		for (ini=stpos; ini<edpos; ini++)
		{
			int x = imiarr[ini].x;
			int y = imiarr[ini].y;
			int ipos = y*imageWidth + x;
			imd[ipos] = 0;//�������о���
			if (flagarr[ipos]==MASK)
			{
				//����ǰ����չ��õ���ΪMASK����õ��Ϊ����ص�һ����ʼ��;
				curlabel = curlabel + 1;
				myqueue.push(ipos); 
				flagarr[ipos] = curlabel;
				
				while ( myqueue.empty()==FALSE )
				{
					int ppei = myqueue.front();
					myqueue.pop();
					int ppeiy = (int) (ppei/imageWidth);
			        int ppeix = ppei - ppeiy*imageWidth;
					
					int ppeileft = ppei - 1;
					if ( (ppeix-1>=0) && (flagarr[ppeileft]==MASK) )
					{
						myqueue.push(ppeileft);//��λ��ѹ��fifo;
						flagarr[ppeileft] = curlabel;
					}
					int ppeiright = ppei + 1;
					if ( (ppeix+1<imageWidth) && (flagarr[ppeiright]==MASK) )
					{
						myqueue.push(ppeiright);//��λ��ѹ��fifo;
						flagarr[ppeiright] = curlabel;
					}
					int ppeiup = ppei - imageWidth;
					if ( (ppeiy-1>=0) && (flagarr[ppeiup]==MASK) )
					{
						myqueue.push(ppeiup);//��λ��ѹ��fifo;
						flagarr[ppeiup] = curlabel;
					}
					int ppeidown = ppei + imageWidth;
					if ( (ppeiy+1<imageHeight) && (flagarr[ppeidown]==MASK) )
					{
						myqueue.push(ppeidown);//��λ��ѹ��fifo;
						flagarr[ppeidown] = curlabel;
					}					
				}				
			}
		}//���ϴ����·��ֵ���أ�

	}

	outrgnumber = curlabel;	
	delete [] imd; imd = NULL;
}

#define NearMeasureBias 200.0//�ж�������ɫ���Ƶ���ֵ��
void  MergeRgs(MyRgnInfo* rginfoarr, int rgnumber, int* flag, int width, int height, int* outmerge, int& rgnum)
//�ϲ���������
{
	//////////////////////////////////////////////////////////////////////////
	//1�������������������飻
	//2������ɨ�������Ѱ�Ҽ�С����
	//3����ÿ����С����A���������������ҵ��������ߣ�
	//4������������B���ϲ���������Ϣˢ�£����ڼ�С����A����������
	//   ɾ����������B����������������ɾ����������B����Ӧ�����
	//   ��������B����������s�ӵ���С����A����������ȥ��
	//5����¼�ϲ���Ϣ����һ����ר�Ŵ�Ÿ���Ϣ��������ĵ�A��Ԫ��ֵ��ΪB��
	//6���ж��Ƿ���Ϊ��С���������򷵻�3��
	//7���Ƿ����������Ѵ�����ϣ������򷵻�2��
	//
	//   ���ڸ���������������̫�࣬��˲����ڽ�������Ϊ�洢�ṹ��
	//////////////////////////////////////////////////////////////////////////
	CString* neiarr = new CString[rgnumber+1];//��һ�����ã�
	int* mergearr = outmerge;//��¼�ϲ�������飻
	MyMath myMath;
	//�����������飻
	for (int y=0; y<height; y++)
	{
		int lstart = y * width;
		for (int x=0; x<width; x++)
		{
			int pos = lstart + x;
			int left=-1, right=-1, up=-1, down=-1;
			myMath.GetNeiInt(x, y, pos, width, height
		, left, right, up, down);//��pos���ĸ�����
			//ȷ����ˢ����������Ϣ��
			int curid = flag[pos];
			AddNeiOfCur(curid, left, right
				, up, down, flag, neiarr);
		}
	}//�����������飻
	
	//������Ϣ�����е���Ч��Ϣ��1��ʼ����i��λ�ô�ŵ�i���������Ϣ��
	for (int rgi=1; rgi<=rgnumber; rgi++)
	{
		//ɨ�����������Ҽ�С����
		LONG allpoints = width * height;
		LONG nmin = (LONG) (allpoints / 400);
		int curid = rgi;

		//rginfoarr[rgi].isflag��ʼΪFALSE���ڱ��ϲ������������ΪTRUE��
		while ( ( (rginfoarr[rgi].ptcount)<nmin ) 
			&& !rginfoarr[rgi].isflag )
		{
			//����Ϊ��С������������������������ӽ��ߣ�
			CString neistr = neiarr[curid];
			int nearid = FindNearestNei(curid, neistr, rginfoarr, mergearr);
			//�ϲ�curid��nearid��
			MergeTwoRgn(curid, nearid, neiarr
				, rginfoarr, mergearr);			
		} 
	}

	//�����ٺϲ��������򣬣����۴�С��,�������Ҫ��ֱ�ӽ�����ѭ��ע�͵������ˣ�
	int countjjj = 0;
	//������Ϣ�����е���Ч��Ϣ��1��ʼ����i��λ�ô�ŵ�i���������Ϣ��
	for (int ii=1; ii<=rgnumber; ii++)
	{
		if (!rginfoarr[ii].isflag)
		{
			int curid = ii;
			MergeNearest(curid, rginfoarr, neiarr, mergearr);
		}
	}



	int counttemp = 0;
	for (int i=0; i<rgnumber; i++)
	{
		if (!rginfoarr[i].isflag)
		{
			counttemp ++;
		}
	}

	rgnum = counttemp;

	delete [] neiarr; neiarr = NULL;
}
int FindMergedRgn(int idint, int* mergearr)
//�ҵ�idint�������ϲ��������ţ�
{
	int outid = idint;
	while ( mergearr[outid] > 0 )
	{
		outid = mergearr[outid];
	}
	return outid;
}
void  MergeNearest(int curid, MyRgnInfo* rginfoarr, CString* neiarr, int* mergearr)
//�ϲ���������
{
	//���δ���������������ƣ���ϲ���
	//CString neistr = neiarr[curid];
	FLOAT cl, cu, cv;
	cl = rginfoarr[curid].l;//��ǰ����LUVֵ��
	cu = rginfoarr[curid].u;
	cv = rginfoarr[curid].v;
	BOOL loopmerged = TRUE;//һ��ѭ�����Ƿ��кϲ��������������ޣ����˳�ѭ����

	while (loopmerged)
	{
		loopmerged = FALSE;
		CString tempstr = neiarr[curid];//���ڱ������ڲ�����
		while (tempstr.GetLength()>0)
		{
			int pos = tempstr.Find(" ");
			ASSERT(pos>=0);
			CString idstr = tempstr.Left(pos);
			tempstr.Delete(0, pos+1);
			
			int idint = (int) strtol(idstr, NULL, 10);
			//�жϸ����Ƿ��ѱ��ϲ������ǣ���һֱ�ҵ�������ǰ�����ţ�
			idint = FindMergedRgn(idint, mergearr);
			if (idint==curid)
			{
				continue;//��������ѱ��ϲ�����ǰ����������
			}
			FLOAT tl, tu, tv;
			tl = rginfoarr[idint].l;//��ǰ�����������LUVֵ;
			tu = rginfoarr[idint].u;
			tv = rginfoarr[idint].v;
			DOUBLE tempdis = pow(tl-cl, 2) 
				+ pow(tu-cu, 2) + pow(tv-cv, 2);
			if (tempdis<NearMeasureBias)
			{
				MergeTwoRgn(curid, idint, neiarr, rginfoarr, mergearr);
				cl = rginfoarr[curid].l;//��ǰ����LUVֵˢ�£�
				cu = rginfoarr[curid].u;
				cv = rginfoarr[curid].v;
				loopmerged = TRUE;
			}		
		}
	}
}

void  MergeTwoRgn(int curid, int nearid
	, CString* neiarr, MyRgnInfo* rginfoarr, int* mergearr)
//��nearid�ϲ���curid��ȥ�����ºϲ��������Ϣ������¼�úϲ���
{
	//������Ϣ��nearid��Ӧ��ı����Ϊ�ѱ��ϲ���
	rginfoarr[nearid].isflag = TRUE;
	//���ºϲ����LUV��Ϣ��
	LONG ptincur = rginfoarr[curid].ptcount;
	LONG ptinnear = rginfoarr[nearid].ptcount;
	DOUBLE curpercent = (FLOAT)ptincur / (FLOAT)(ptincur+ptinnear);
	rginfoarr[curid].ptcount = ptincur + ptinnear;
	rginfoarr[curid].l = (FLOAT) ( curpercent * rginfoarr[curid].l
		+ (1-curpercent) * rginfoarr[nearid].l );
	rginfoarr[curid].u = (FLOAT) ( curpercent * rginfoarr[curid].u
		+ (1-curpercent) * rginfoarr[nearid].u );
	rginfoarr[curid].v = (FLOAT) ( curpercent * rginfoarr[curid].v
		+ (1-curpercent) * rginfoarr[nearid].v );
	//��nearid������ӵ�curid��������ȥ��
	AddBNeiToANei(curid, nearid, neiarr, mergearr);
	//��¼�úϲ���
	mergearr[nearid] = curid;
}

void  AddBNeiToANei(int curid, int nearid, CString* neiarr, int* mergearr)
//��nearid������ӵ�curid��������ȥ��
{
	//�ȴ�curid�������а�nearidɾȥ��
/*
	CString tempstr;
	tempstr.Format("%d ", nearid);
	int temppos = neiarr[curid].Find(tempstr, 0);
	while (temppos>0 && neiarr[curid].GetAt(temppos-1)!=' ')
	{
		temppos = neiarr[curid].Find(tempstr, temppos+1);
	}
	if (temppos>=0)
	{
		//�����ڽ���Ϊ�ϲ��������������ԣ�
		neiarr[curid].Delete(temppos, tempstr.GetLength());
	}
*/
    //��nearid���������μӵ�curid��������ȥ��
	CString neistr = neiarr[nearid];
	CString curstr = neiarr[curid];
	//һ��˵������С��������Ӧ�ý��٣���ˣ�Ϊ����ߺϲ��ٶȣ���
	//curstr�ӵ�neistr��ȥ��Ȼ�󽫽������neiarr[curid];
	while ( curstr.GetLength()>0 )
	{
		int pos = curstr.Find(" ");		
		ASSERT(pos>=0);
		CString idstr = curstr.Left(pos);
        curstr.Delete(0, pos+1);
		int idint = (int) strtol(idstr, NULL, 10);
		idint = FindMergedRgn(idint, mergearr);
		idstr += " ";
		if ( (idint == curid) || (idint == nearid) )
		{
			continue;//�������뱾�����ڣ�
		}else
		{
			if ( neistr.Find(idstr, 0) >= 0 )
			{
				continue;
			}else
			{
				neistr += idstr;//�ӵ�������ȥ;
			}
		}		
	}
	neiarr[curid] = neistr;
/*
	CString toaddneis = neiarr[nearid];
	while (toaddneis.GetLength()>0)
	{
		int pos = toaddneis.Find(" ");		
		ASSERT(pos>=0);
		CString idstr = toaddneis.Left(pos);
        toaddneis.Delete(0, pos+1);
		int idint = (int) strtol(idstr, NULL, 10);
		idint = FindMergedRgn(idint, mergearr);
		idstr += " ";
		if ( (idint == curid) || (idint == nearid) )
		{
			continue;//�������뱾�����ڣ�
		}else
		{
			if ( neiarr[curid].Find(idstr, 0) >= 0 )
			{
				continue;
			}else
			{
				neiarr[curid] += idstr;//�ӵ�������ȥ;
			}
		}		
	}
*/
}


int  FindNearestNei(int curid, CString neistr, MyRgnInfo* rginfoarr, int* mergearr)
//Ѱ��neistr����curid��ӽ����������ظ���id�ţ�
{
	int outid = -1;
	DOUBLE mindis = 999999;
	FLOAT cl, cu, cv;
	cl = rginfoarr[curid].l;//��ǰ����LUVֵ��
	cu = rginfoarr[curid].u;
	cv = rginfoarr[curid].v;

	CString tempstr = neistr;//���ڱ������ڲ�����
	while (tempstr.GetLength()>0)
	{
		int pos = tempstr.Find(" ");
		ASSERT(pos>=0);
		CString idstr = tempstr.Left(pos);
		tempstr.Delete(0, pos+1);

		int idint = (int) strtol(idstr, NULL, 10);
		//�жϸ����Ƿ��ѱ��ϲ������ǣ���һֱ�ҵ�������ǰ�����ţ�
		idint = FindMergedRgn(idint, mergearr);
		if (idint==curid)
		{
			continue;//��������ѱ��ϲ�����ǰ����������
		}
		FLOAT tl, tu, tv;
		tl = rginfoarr[idint].l;//��ǰ�����������LUVֵ;
		tu = rginfoarr[idint].u;
		tv = rginfoarr[idint].v;
		DOUBLE tempdis = pow(tl-cl, 2) 
			+ pow(tu-cu, 2) + pow(tv-cv, 2);
		if (tempdis<mindis)
		{
			mindis = tempdis;//������Ͷ�Ӧ��������ID��
			outid = idint;
		}		
	}

	return outid;
}

int  FindMergedRgnMaxbias(int idint, int* mergearr, int bias)
//����ֵ��ֹ���Һϲ���������coarse watershed, 
//�����߱��뱣֤idint��Ч������mergearr[idint]>0��
//�Լ�mergearr��Ч������mergearr[idint]<idint;
{
	int outid = idint;
	while ( mergearr[outid]<bias )
	{
		outid = mergearr[outid];
	}
	return mergearr[outid];
}


void  AddNeiRgn(int curid, int neiid, CString* neiarr)
//����neiidΪcurid��������
{
	CString tempneis = neiarr[curid];//��ǰ����������
	CString toaddstr;
	toaddstr.Format("%d ", neiid);

	int temppos = tempneis.Find(toaddstr, 0);
	while (temppos>0 && neiarr[curid].GetAt(temppos-1)!=' ')
	{
		temppos = neiarr[curid].Find(toaddstr, temppos+1);
	}
	
	if ( temppos<0 )
	{
		//��ǰ��������û��tempneis,�����
		neiarr[curid] += toaddstr;
	}
}

void  AddNeiOfCur(int curid, int left, int right, int up, int down, int* flag, CString* neiarr)
//ˢ�µ�ǰ���������������
{
	int leftid, rightid, upid, downid;
	leftid = rightid = upid = downid = curid;
	if (left>=0)
	{
		leftid = flag[left];
		if (leftid!=curid)
		{
			//�ڵ�������һ��, ���������Ϣ��
			AddNeiRgn(curid, leftid, neiarr);
		}
	}
	if (right>0)
	{
		rightid = flag[right];
		if (rightid!=curid)
		{
			//�ڵ�������һ��, ���������Ϣ��
			AddNeiRgn(curid, rightid, neiarr);
		}
	}
	if (up>=0)
	{
		upid = flag[up];
		if (upid!=curid)
		{
			//�ڵ�������һ��, ���������Ϣ��
			AddNeiRgn(curid, upid, neiarr);
		}
	}
	if (down>0)
	{
		downid = flag[down];
		if (downid!=curid)
		{
			//�ڵ�������һ��, ���������Ϣ��
			AddNeiRgn(curid, downid, neiarr);
		}
	}
}
/*************************************************************************
 *
 * \�������ƣ�
 *   RegionSegFixThreshold()
 *
 * \�������:
 *   CDIB * pDib					- ָ��CDIB���ָ�룬����ԭʼͼ����Ϣ
 *   int nThreshold					- ����ָ����ֵ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   1���߼�����ʾ��Ӧ����Ϊǰ������0��ʾ����
 *   ��ֵ�ָ�Ĺؼ�����������ֵ��ѡȡ����ֵ��ѡȡһ��Ӧ����ʵ�ʵ�Ӧ�ö�
 *   ����趨��
 *
 *************************************************************************
 */
/*void RegionSegFixThreshold(CDIB * pDib, int nThreshold)
{
	//����ͼ���������
	int y;

	//����ͼ��ĺ�����
	int x;

	//ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	//ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	
	//ͼ�����ݵ�ָ��
	LPBYTE  pImageData = pDib->m_lpImage;

	for(y=0; y<nHeight ; y++ )
		for(x=0; x<nWidth ; x++ )
		{
			if( *(pImageData+y*nSaveWidth+x) < nThreshold)
				*(pImageData+y*nSaveWidth+x) = 0;
		}
}*/

/*************************************************************************
 *
 * \�������ƣ�
 *   RegionSegAdaptive()
 *
 * \�������:
 *   CDIB * pDib					- ָ��CDIB���ָ�룬����ԭʼͼ����Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   1���߼�����ʾ��Ӧ����Ϊǰ������0��ʾ����
 *   ��ֵ�ָ�Ĺؼ�����������ֵ��ѡȡ����ֵ��ѡȡһ��Ӧ����ʵ�ʵ�Ӧ�ö�
 *   ����趨���������У���ֵ���ǹ̶��ģ����Ǹ���ͼ�����ص�ʵ�����ʶ��趨�ġ�
 *   ���������ͼ��ֳ��ĸ���ͼ��Ȼ�����ÿ����ͼ��ľ�ֵ�����ݾ�ֵ������ֵ
 *   ��ֵֻ��Ӧ���ڶ�Ӧ����ͼ��
 *
 *************************************************************************
 */
/*void RegionSegAdaptive(CDIB * pDib)
{
	//����ͼ���������
	int y;

	//����ͼ��ĺ�����
	int x;

	//ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	//ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	//ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;

	// �ֲ���ֵ
	int nThd[2][2] ;

	// ��ͼ���ƽ��ֵ
	int nLocAvg ;
	
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x����
		for(x=0; x<nWidth/2 ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// �����ֵ
	nLocAvg /= ( (nHeight/2) * (nWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nThd[0][0] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
	// y����
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x����
		for(x=0; x<nWidth/2 ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[0][0])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x����
		for(x=0; x<nWidth/2 ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// �����ֵ
	nLocAvg /= ( (nHeight - nHeight/2) * (nWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nThd[1][0] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
	// y����
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x����
		for(x=0; x<nWidth/2 ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[1][0])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x����
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// �����ֵ
	nLocAvg /= ( (nHeight/2) * (nWidth - nWidth/2) ) ;
	
	// ������ֵΪ��ͼ���ƽ��ֵ
	nThd[0][1] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
	// y����
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x����
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[0][1])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x����
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// �����ֵ
	nLocAvg /= ( (nHeight - nHeight/2) * (nWidth - nWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nThd[1][1] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
	// y����
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x����
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[1][1])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
		}
	}
	
	// Ϊ����ʾ������ʾ���߼�1�ú�ɫ��ʾ���߼�0�ð�ɫ��ʾ
	for(y=0; y<nHeight ; y++ )
	{
		// x����
		for(x=0; x<nWidth ; x++ )
		{
			lpImage[y*nSaveWidth + x] = 255 - lpImage[y*nSaveWidth + x] ;
		}
	}
}*/

/*************************************************************************
 *
 * \�������ƣ�
 *   RobertsOperator()
 *
 * \�������:
 *   CDIB * pDib		- ָ��CDIB���ָ�룬����ԭʼͼ����Ϣ
 *   double * pdGrad	- ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Roberts����
 *
 *************************************************************************
 */
/*void RobertsOperator(CDIB * pDib, double * pdGrad)
{
	// ����ͼ���������
	int y;

	// ����ͼ��ĺ�����
	int x;

	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	// ͼ�����ݵ�ָ��
	LPBYTE  pImageData = pDib->m_lpImage;

	// ��ʼ��
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}


	// ���濪ʼ����Roberts���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
	// ������λ��ͼ�����ݵ��ڲ������������ѭ����������
	// y<nHeight-1 ������y<nHeight����Ӧ��x����Ҳ��x<nWidth-1
	// ������x<nWidth

	//����������������ʾRoberts���ӵ�һ��ģ�����������ֵ
	int nUpLeft;
	int nDownRight;

	// ����������������ʾRoberts���ӵڶ���ģ�����������ֵ
	int nUpRight;
	int nDownLeft;

	// ����������������ʾRoberts���Ӽ���Ľ��
	int nValueOne;
	int nValueTwo;

	// ��ʱ����
	double dGrad;

	for(y=0; y<nHeight-1 ; y++ )
		for(x=0 ; x<nWidth-1 ; x++ )
		{
			// Roberts���ӵ�һ��ģ����Ҫ������ֵ
			nUpLeft		=*(pImageData+y*nSaveWidth+x) ; 
			nDownRight	=*(	pImageData+(y+1)*nSaveWidth+x+1	);
			nDownRight	*=-1;

			//Roberts���ӵĵ�һ��ģ�������
			nValueOne	=nUpLeft+nDownRight	;

			// Roberts���ӵڶ���ģ����Ҫ������ֵ
			nUpRight	=*(	pImageData+y*nSaveWidth+x+1	)		;
			nDownLeft	=*(	pImageData+(y+1)*nSaveWidth+x	);
			nDownLeft	*=-1;

			// Roberts���ӵĵڶ���ģ�������
			nValueTwo	=nUpRight+nDownLeft;

			// ��������ƫ������ƽ����
			dGrad=nValueOne*nValueOne + nValueTwo*nValueTwo;

			// ����
			dGrad=pow(dGrad,0.5);

			// ��������ŷʽ����
			*(pdGrad+y*nWidth+x)=dGrad;
		}
}*/
/*************************************************************************
 *
 * \�������ƣ�
 *   LaplacianOperator()
 *
 * \�������:
 *   CDIB * pDib		- ָ��CDIB���ָ�룬����ԭʼͼ����Ϣ
 *   double * pdGrad	- ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   LaplacianOperator����,�Ƕ������ӣ�����Roberts����������Ҫ����ģ�����
 *   �ݶȣ�LaplacianOperator����ֻҪһ�����ӾͿ��Լ����ݶȡ�������Ϊ������
 *   ������Ϣ���������Ƚ�����
 *
 *************************************************************************
 */
/*void LaplacianOperator(CDIB * pDib, double * pdGrad)
{
		// ����ͼ���������
	int y;

	// ����ͼ��ĺ�����
	int x;

	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	// ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;

	// ��ʼ��
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}

	// ����ģ��ϵ��
	static int nWeight[3][3] ;
	nWeight[0][0] = -1 ;   
	nWeight[0][1] = -1 ;   
	nWeight[0][2] = -1 ;   
	nWeight[1][0] = -1 ;   
	nWeight[1][1] =  8 ;   
	nWeight[1][2] = -1 ;   
	nWeight[2][0] = -1 ;   
	nWeight[2][1] = -1 ;   
	nWeight[2][2] = -1 ;   



	//�������������ʾLaplacian��������ֵ
	int nTmp[3][3];
	
	// ��ʱ����
	double dGrad;

	// ģ��ѭ�����Ʊ���
	int yy ;
	int xx ;

	
	// ���濪ʼ����Laplacian���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
	// ������λ��ͼ�����ݵ��ڲ������������ѭ����������
	// y<nHeight-2 ������y<nHeight����Ӧ��x����Ҳ��x<nWidth-2
	// ������x<nWidth
	for(y=1; y<nHeight-2 ; y++ )
		for(x=1 ; x<nWidth-2 ; x++ )
		{
			dGrad = 0 ; 
			// Laplacian������Ҫ�ĸ�������ֵ

			// ģ���һ��
			nTmp[0][0] = lpImage[(y-1)*nSaveWidth + x - 1 ] ; 
			nTmp[0][1] = lpImage[(y-1)*nSaveWidth + x     ] ; 
			nTmp[0][2] = lpImage[(y-1)*nSaveWidth + x + 1 ] ; 

			// ģ��ڶ���
			nTmp[1][0] = lpImage[y*nSaveWidth + x - 1 ] ; 
			nTmp[1][1] = lpImage[y*nSaveWidth + x     ] ; 
			nTmp[1][2] = lpImage[y*nSaveWidth + x + 1 ] ; 

			// ģ�������
			nTmp[2][0] = lpImage[(y+1)*nSaveWidth + x - 1 ] ; 
			nTmp[2][1] = lpImage[(y+1)*nSaveWidth + x     ] ; 
			nTmp[2][2] = lpImage[(y+1)*nSaveWidth + x + 1 ] ; 
			
			// �����ݶ�
			for(yy=0; yy<3; yy++)
				for(xx=0; xx<3; xx++)
				{
					dGrad += nTmp[yy][xx] * nWeight[yy][xx] ;
				}
			
			// �ݶ�ֵд���ڴ�
			*(pdGrad+y*nWidth+x)=dGrad;
		}

}*/

/*************************************************************************
 *
 * \�������ƣ�
 *   RegionGrow()
 *
 * \�������:
 *   CDIB * pDib					- ָ��CDIB���ָ�룬����ԭʼͼ����Ϣ
 *   unsigned char * pUnRegion		- ָ���������������ָ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   pUnRegionָ��ָ����������洢�����������Ľ��������1���߼�����ʾ
 *	 ��Ӧ����Ϊ��������0��ʾΪ����������
 *   ��������һ����������Ƚ���Ҫ�����⣺
 *		1. ���ӵ��ѡȡ
 *		2. ����׼��
 *		3. ��ֹ����
 *	 ������Ϊ��������������Ҫ�������������ÿ���������ĺû�ֱ�ӹ�ϵ��
 *	 ���������Ľ����
 *	 �����������ӵ�ѡȡΪͼ������ģ�����׼�����������ص�����ֵС��
 *	 nThreshold, ��ֹ������һֱ���е���û����������׼����Ҫ������ʱΪֹ
 *
 *************************************************************************
 */
/*void RegionGrow(CDIB * pDib, unsigned char * pUnRegion, int nThreshold)
{
	static int nDx[]={-1,0,1,0};
	static int nDy[]={0,1,0,-1};

	// ����ͼ���������
//	int y;

	// ����ͼ��ĺ�����
//	int x;

	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	// ��ʼ��
	memset(pUnRegion,0,sizeof(unsigned char)*nWidth*nHeight);

	// ���ӵ�
	int nSeedX, nSeedY;

	// �������ӵ�Ϊͼ�������
	nSeedX = nWidth /2 ;
	nSeedY = nHeight/2 ;

	// �����ջ���洢����
	int * pnGrowQueX ;
	int * pnGrowQueY ;
	
	// ����ռ�
	pnGrowQueX = new int [nWidth*nHeight];
	pnGrowQueY = new int [nWidth*nHeight];

	// ͼ�����ݵ�ָ��
	unsigned char *  pUnchInput =(unsigned char * )pDib->m_lpImage;
	
	// �����ջ�������յ�
	// ��nStart=nEnd, ��ʾ��ջ��ֻ��һ����
	int nStart ;
	int nEnd   ;

	//��ʼ��
	nStart = 0 ;
	nEnd   = 0 ;

	// �����ӵ������ѹ��ջ
	pnGrowQueX[nEnd] = nSeedX;
	pnGrowQueY[nEnd] = nSeedY;

	// ��ǰ���ڴ��������
	int nCurrX ;
	int nCurrY ;

	// ѭ�����Ʊ���
	int k ;

	// ͼ��ĺ�������,�����Ե�ǰ���ص�4������б���
	int xx;
	int yy;

	while (nStart<=nEnd)
	{
		// ��ǰ���ӵ������
		nCurrX = pnGrowQueX[nStart];
		nCurrY = pnGrowQueY[nStart];					

		// �Ե�ǰ���4������б���
		for (k=0; k<4; k++)	
		{	
			// 4�������ص�����
			xx = nCurrX+nDx[k];
			yy = nCurrY+nDy[k];
			
			// �ж�����(xx��yy) �Ƿ���ͼ���ڲ�
			// �ж�����(xx��yy) �Ƿ��Ѿ������
			// pUnRegion[yy*nWidth+xx]==0 ��ʾ��û�д���

			// �����������ж�����(xx��yy)�͵�ǰ����(nCurrX,nCurrY) ����ֵ��ľ���ֵ
			if (	(xx < nWidth) && (xx>=0) && (yy<nHeight) && (yy>=0) 
				    && (pUnRegion[yy*nWidth+xx]==0) 
					&& abs(pUnchInput[yy*nSaveWidth+xx] - pUnchInput[nCurrY*nSaveWidth+nCurrX])<nThreshold )
			{
				// ��ջ��β��ָ�����һλ
				nEnd++;

				// ����(xx��yy) ѹ��ջ
				pnGrowQueX[nEnd] = xx;
				pnGrowQueY[nEnd] = yy;

				// ������(xx��yy)���ó��߼�1��255��
				// ͬʱҲ���������ش����
				pUnRegion[yy*nWidth+xx] = 255 ;
			}
		}
		nStart++;
	}

	// �ͷ��ڴ�
	delete []pnGrowQueX;
	delete []pnGrowQueY;
    pnGrowQueX = NULL ;
	pnGrowQueY = NULL ;
}*/

//void DFT_2D(CDIB * pDib,double * pTrRstRpart, double * pTrRstIpart)
//{}


//void IDFT_2D(CDIB * pDib,double * pTrRstRpart, double * pTrRstIpart)
//{}

/*************************************************************************
 *
 * \�������ƣ�
 *   SobelOperator()
 *
 * \�������:
 *   CDIB * pDib		  - ָ��CDIB���ָ�룬����ԭʼͼ����Ϣ
 *   double * pdGrad	- ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Sobe����
 *
 *   ���б߽�ָ�
 *
 *************************************************************************
 */
//void SobelOperator(CDIB * pDib, double * pdGrad)
//{}

/*************************************************************************
 *
 * \�������ƣ�
 *   PrewittOperator()
 *
 * \�������:
 *   CDIB * pDib		  - ָ��CDIB���ָ�룬����ԭʼͼ����Ϣ
 *   double * pdGrad	- ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Prewitt����
 *
 *   ���б߽�ָ�
 *
 *************************************************************************
 */
//void PrewittOperator(CDIB * pDib, double * pdGrad)
//{}

/*************************************************************************
 *
 * \�������ƣ�
 *   EdgeTrack()
 *
 * \�������:
 *   CDIB * pDib					- ָ��CDIB���ָ�룬����ԭʼͼ����Ϣ
 *   unsigned char * pUnEdgeTrack	- ָ��߽���ٽ����ָ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   pUnEdgeTrackָ��ָ����������洢�˱߽���ٵĽ��������1���߼�����ʾ
 *	 ��Ӧ����Ϊ�߽�㣬0��ʾΪ�Ǳ߽��
 *
 *   ���б߽�ָ�
 *
 *************************************************************************
 */
//void EdgeTrack(CDIB * pDib, unsigned char * pUnEdgeTrack)
//{}


/*************************************************************************
 *
 * \�������ƣ�
 *   MakeGauss()
 *
 * \�������:
 *   double sigma									        - ��˹�����ı�׼��
 *   double **pdKernel										- ָ���˹���������ָ��
 *   int *pnWindowSize										- ���ݵĳ���
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���������������һ��һά�ĸ�˹�������������ݣ������ϸ�˹���ݵĳ���Ӧ
 *   �������޳��ģ�����Ϊ�˼���ļ򵥺��ٶȣ�ʵ�ʵĸ�˹����ֻ�������޳���
 *   pnWindowSize�������ݳ���
 *   
 *************************************************************************
 */
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize)
{
	// ѭ�����Ʊ���
	int i   ;
	
	// ��������ĵ�
	int nCenter;

	// �����ĳһ�㵽���ĵ�ľ���
	double dDis;


	// �м����
	double  dValue; 
	double  dSum;
	dSum = 0 ; 
	double pie;
	pie=3.14159;
	// ���鳤�ȣ����ݸ����۵�֪ʶ��ѡȡ[-3*sigma, 3*sigma]���ڵ����ݡ�
	// ��Щ���ݻḲ�Ǿ��󲿷ֵ��˲�ϵ��
	*pnWindowSize = 1 + 2 * (int)ceil(3 * sigma);
	
	// ����
	nCenter = (*pnWindowSize) / 2;
	
	// �����ڴ�
	*pdKernel = new double[*pnWindowSize] ;
	
	for(i=0; i< (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1/2)*dDis*dDis/(sigma*sigma)) / (sqrt(2 * pie) * sigma );
		(*pdKernel)[i] = dValue ;
		dSum += dValue;
	}
	
	// ��һ��
	for(i=0; i<(*pnWindowSize) ; i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   GaussianSmooth()
 *
 * \�������:
 *   unsigned char * pUnchImg				- ָ��ͼ�����ݵ�ָ��
 *   int nWidth											- ͼ�����ݿ��
 *   int nHeight										- ͼ�����ݸ߶�
 *   double dSigma									- ��˹�����ı�׼��
 *   unsigned char * pUnchSmthdImg	- ָ�򾭹�ƽ��֮���ͼ������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Ϊ����ֹ���������ø�˹�˲���ͼ������˲����˲��ȶ�x������У�Ȼ���
 *   y������С�
 *   
 *************************************************************************
 */
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, 
										double sigma, unsigned char * pUnchSmthdImg)
{
	// ѭ�����Ʊ���
  int y;
	int x;
	
	int i;
	
	// ��˹�˲��������鳤��
	
	int nWindowSize;
	
	//  ���ڳ��ȵ�1/2
	int	nHalfLen;   
	
	// һά��˹�����˲���
	double *pdKernel ;
	
	// ��˹ϵ����ͼ�����ݵĵ��
	double  dDotMul     ;
	
	// ��˹�˲�ϵ�����ܺ�
	double  dWeightSum     ;          
  
	// �м����
	double * pdTmp ;
	
	// �����ڴ�
	pdTmp = new double[nWidth*nHeight];
	
	// ����һά��˹�����˲���
	// MakeGauss(sigma, &dKernel, &nWindowSize);
	MakeGauss(sigma, &pdKernel, &nWindowSize) ;
	
	// MakeGauss���ش��ڵĳ��ȣ����ô˱������㴰�ڵİ볤
	nHalfLen = nWindowSize / 2;
	
  // x��������˲�
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if( (i+x) >= 0  && (i+x) < nWidth )
				{
					dDotMul += (double)pUnchImg[y*nWidth + (i+x)] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pdTmp[y*nWidth + x] = dDotMul/dWeightSum ;
		}
	}
	
	// y��������˲�
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if( (i+y) >= 0  && (i+y) < nHeight )
				{
					dDotMul += (double)pdTmp[(y+i)*nWidth + x] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pUnchSmthdImg[y*nWidth + x] = (unsigned char)(int)dDotMul/dWeightSum ;
		}
	}

	// �ͷ��ڴ�
	delete []pdKernel;
	pdKernel = NULL ;
	
	delete []pdTmp;
	pdTmp = NULL;
}


/*************************************************************************
 *
 * \�������ƣ�
 *   DirGrad()
 *
 * \�������:
 *   unsigned char *pUnchSmthdImg					- ������˹�˲����ͼ��
 *   int nWidht														- ͼ����
 *   int nHeight      										- ͼ��߶�
 *   int *pnGradX                         - x����ķ�����
 *   int *pnGradY                         - y����ķ�����
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����������㷽���������õ�΢��������(-1 0 1) �� (-1 0 1)'(ת��)
 *   �����ʱ��Ա߽����ز��������⴦��
 *   
 *   
 *************************************************************************
 */
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
										 int *pnGradX , int *pnGradY)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;
	
	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pnGradX[y*nWidth+x] = (int) ( pUnchSmthdImg[y*nWidth+min(nWidth-1,x+1)]  
													          - pUnchSmthdImg[y*nWidth+max(0,x-1)]     );
		}
	}

	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			pnGradY[y*nWidth+x] = (int) ( pUnchSmthdImg[min(nHeight-1,y+1)*nWidth + x]  
																		- pUnchSmthdImg[max(0,y-1)*nWidth+ x ]     );
		}
	}
}
/*************************************************************************
 *
 * \�������ƣ�
 *   GradMagnitude()
 *
 * \�������:
 *   int *pnGradX                         - x����ķ�����
 *   int *pnGradY                         - y����ķ�����
 *   int nWidht														- ͼ����
 *   int nHeight      										- ͼ��߶�
 *   int *pnMag                           - �ݶȷ���   
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����������÷����������ݶȷ��ȣ���������DirGrad��������Ľ��
 *   
 *************************************************************************
 */
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag)
{
	
	// ѭ�����Ʊ���
	int y ;
	int x ;

	// �м����
	double dSqtOne;
	double dSqtTwo;
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dSqtOne = pnGradX[y*nWidth + x] * pnGradX[y*nWidth + x];
			dSqtTwo = pnGradY[y*nWidth + x] * pnGradY[y*nWidth + x];
			pnMag[y*nWidth + x] = (int)(sqrt(dSqtOne + dSqtTwo) + 0.5);
		}
	}
}
/*************************************************************************
 *
 * \�������ƣ�
 *   NonmaxSuppress()
 *
 * \�������:
 *   int *pnMag                - �ݶ�ͼ
 *   int *pnGradX							 - x����ķ�����	
 *   int *pnGradY              - y����ķ�����
 *   int nWidth                - ͼ�����ݿ��
 *   int nHeight               - ͼ�����ݸ߶�
 *   unsigned char *pUnchRst   - ����NonmaxSuppress�����Ľ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ֹ�ݶ�ͼ�зǾֲ���ֵ������ء�
 *   
 *************************************************************************
 */
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
										int nHeight,	unsigned char *pUnchRst)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;
	int nPos;

	// x�����ݶȷ���
	int gx  ;
	int gy  ;

	// ��ʱ����
	int g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;
	
	// ����ͼ���Ե����Ϊ�����ܵı߽��
	for(x=0; x<nWidth; x++)		
	{
		pUnchRst[x] = 0 ;
		pUnchRst[nHeight-1+x] = 0;
	}
	for(y=0; y<nHeight; y++)		
	{
		pUnchRst[y*nWidth] = 0 ;
		pUnchRst[y*nWidth + nWidth-1] = 0;
	}

	for(y=1; y<nHeight-1; y++)
	{
		for(x=1; x<nWidth-1; x++)
		{
			nPos = y*nWidth + x ;
			
			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if(pnMag[nPos] == 0 )
			{
				pUnchRst[nPos] = 0 ;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pnMag[nPos] ;
				
				// x��y������
				gx = pnGradX[nPos]  ;
				gy = pnGradY[nPos]  ;

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx)) 
				{
					// �����ֵ�ı���
					weight = fabs(gx)/fabs(gy); 

					g2 = pnMag[nPos-nWidth] ; 
					g4 = pnMag[nPos+nWidth] ;
					
					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pnMag[nPos-nWidth-1] ;
						g3 = pnMag[nPos+nWidth+1] ;
					} 

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//		 C         
					//	g3 g4  
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					} 
				}
				
				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs(gy)/fabs(gx); 
					
					g2 = pnMag[nPos+1] ; 
					g4 = pnMag[nPos-1] ;
					
					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pnMag[nPos+nWidth+1] ;
						g3 = pnMag[nPos-nWidth-1] ;
					} 
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;
					
					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pUnchRst[nPos] = 128 ;
					}
					else
					{
						// �������Ǳ߽��
						pUnchRst[nPos] = 0 ;
					}
				}
			} //else
		} // for

	}
} 

/*************************************************************************
 *
 * \�������ƣ�
 *   TraceEdge()
 *
 * \�������:
 *   int    x									- ��������x���� 
 *   int    y									- ��������y����
 *   int nLowThd							- �ж�һ�����Ƿ�Ϊ�߽��ĵ���ֵ
 *   unsigned char *pUnchEdge - ��¼�߽��Ļ�����
 *   int *pnMag               - �ݶȷ���ͼ
 *   int nWidth               - ͼ�����ݿ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ݹ����  
 *   ��(x,y)������������б߽��ĸ��٣�����ֻ����pUnchEdge��û�д�����
 *   �����Ǳ߽�����Щ����(=128),����ֵΪ0�����õ㲻�����Ǳ߽�㣬����ֵ
 *   Ϊ255�����õ��Ѿ�������Ϊ�߽�㣬�����ٿ���
 *   
 *   
 *************************************************************************
 */
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) 
{ 
	// ��8�������ؽ��в�ѯ
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;
	
	int k  ;
	
	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if(pUnchEdge[yy*nWidth+xx] == 128  && pnMag[yy*nWidth+xx]>=nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pUnchEdge[yy*nWidth+xx] = 255 ;

			// �Ըõ�Ϊ���Ľ��и���
			TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
		}
	}
} 

/*************************************************************************
 *
 * \�������ƣ�
 *   EstimateThreshold()
 *
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *   int *pnThdHigh           - ����ֵ
 *   int *pnThdLow            - ����ֵ
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - ����non-maximum����������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����non-maximum����������pUnchEdge��ͳ��pnMag��ֱ��ͼ��ȷ����ֵ��
 *   ��������ֻ��ͳ��pUnchEdge�п���Ϊ�߽�����Щ���ء�Ȼ������ֱ��ͼ��
 *   ����dRatioHigh���ø���ֵ���洢��pnThdHigh������dRationLow�͸���ֵ��
 *   ���õ���ֵ���洢��*pnThdLow��dRatioHigh��һ�ֱ����������ݶ�С��
 *   *pnThdHigh��������Ŀռ��������Ŀ�ı�����dRationLow����*pnThdHigh
 *   ��*pnThdLow�ı��������������canny�㷨��ԭ������߸�����һ�����䡣
 *
 *************************************************************************
 */
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 unsigned char * pUnchEdge, double dRatioHigh, double dRationLow) 
{ 
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;
	
	// ������Ĵ�С���ݶ�ֵ�ķ�Χ�йأ�������ñ�������㷨����ô�ݶȵķ�Χ���ᳬ��pow(2,10)
	int nHist[1024] ;

	// ���ܵı߽���Ŀ
	int nEdgeNb     ;

	// ����ݶ�ֵ
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 
	
	// ��ʼ��
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if(pUnchEdge[y*nWidth+x]==128)
			{
				nHist[ pnMag[y*nWidth+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k;
		}
		
		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pnThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);
	
	k = 1;
	nEdgeNb = nHist[1];
	
	// �������ֵ
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// ���ø���ֵ
	*pnThdHigh = k ;

	// ���õ���ֵ
	*pnThdLow  = (int)((*pnThdHigh) * dRationLow+ 0.5);
}

/*************************************************************************
 *
 * \�������ƣ�
 *   Hysteresis()
 *
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - ��¼�߽��Ļ�����
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ������ʵ�����ơ��������󡱵�һ�����ܣ�Ҳ���ǣ��ȵ���EstimateThreshold
 *   �����Ծ���non-maximum����������pUnchSpr����һ������ֵ��Ȼ���ж�
 *   pUnchSpr�п��ܵı߽�����(=128)���ݶ��ǲ��Ǵ��ڸ���ֵnThdHigh�������
 *   ����ֵ�󣬸õ㽫��Ϊһ���߽����㣬����TraceEdge�������Ѷ�Ӧ�ñ߽�
 *   �����������ҳ�������󣬵������������ʱ�������������û�б���־��
 *   �߽�㣬��ô��һ�����Ǳ߽�㡣
 *   
 *************************************************************************
 */
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, unsigned char *pUnchEdge)
{
	// ѭ�����Ʊ���
	int y;
	int x;

	int nThdHigh ;
	int nThdLow  ;

	int nPos;

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh, 
		               &nThdLow, pUnchEdge,dRatioHigh, dRatioLow);

  // ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	// TraceEdge���������ٸõ��Ӧ�ı߽�
   for(y=0; y<nHeight; y++)
	 {
      for(x=0; x<nWidth; x++)
			{
				nPos = y*nWidth + x ; 

				// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
				// һ���߽�����
				if((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
				{
					// ���øõ�Ϊ�߽��
					pUnchEdge[nPos] = 255;
					TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
				}
      }
   }

	 // ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
   for(y=0; y<nHeight; y++)
	 {
		 for(x=0; x<nWidth; x++)
		 {
			 nPos = y*nWidth + x ; 
			 if(pUnchEdge[nPos] != 255)
			 {
				 // ����Ϊ�Ǳ߽��
				 pUnchEdge[nPos] = 0 ;
			 }
		 }
	 }
}


/*************************************************************************
 *
 * \�������ƣ�
 *   Canny()
 *
 * \�������:
 *   unsigned char *pUnchImage- ͼ������
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *   double sigma             - ��˹�˲��ı�׼����
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - canny���Ӽ����ķָ�ͼ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   canny�ָ����ӣ�����Ľ��������pUnchEdge�У��߼�1(255)��ʾ�õ�Ϊ
 *   �߽�㣬�߼�0(0)��ʾ�õ�Ϊ�Ǳ߽�㡣�ú����Ĳ���sigma��dRatioLow
 *   dRatioHigh������Ҫָ���ġ���Щ������Ӱ��ָ��߽����Ŀ�Ķ���
 *************************************************************************
 */
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge)
{
	// ������˹�˲����ͼ������
	unsigned char * pUnchSmooth ;
  
	// ָ��x��������ָ��
	int * pnGradX ; 

	// ָ��y��������ָ��
	int * pnGradY ;

	// �ݶȵķ���
	int * pnGradMag ;

	pUnchSmooth  = new unsigned char[nWidth*nHeight] ;
	pnGradX      = new int [nWidth*nHeight]          ;
	pnGradY      = new int [nWidth*nHeight]          ;
	pnGradMag    = new int [nWidth*nHeight]          ;

	// ��ԭͼ������˲�
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth) ;

	// ���㷽����
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY) ;

	// �����ݶȵķ���
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag) ;

	// Ӧ��non-maximum ����
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge) ;

	// Ӧ��Hysteresis���ҵ����еı߽�
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// �ͷ��ڴ�
	delete []pnGradX      ;
	pnGradX      = NULL ;
	delete []pnGradY      ;
	pnGradY      = NULL ;
	delete []pnGradMag    ;
	pnGradMag    = NULL ;
	delete []pUnchSmooth ;
	pUnchSmooth  = NULL ;
}
void Gradient(unsigned char*pImage, int nWidth, int nHeight,float* pEdge)
{
	int X,Y,I,J;
	float sumX,sumY,SUM;
	int			GX[3][3];
	int			GY[3][3];
	
	/* 3x3 GX Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html */
	GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
	GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
	GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;
	
	/* 3x3 GY Sobel mask.  Ref: www.cee.hw.ac.uk/hipr/html/sobel.html */
	GY[0][0] =  1; GY[0][1] =  2; GY[0][2] =  1;
	GY[1][0] =  0; GY[1][1] =  0; GY[1][2] =  0;
	GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;
	
	
	for(Y=0; Y<=(nHeight-1); Y++)  {
		for(X=0; X<=(nWidth-1); X++)  {
			sumX = 0;
			sumY = 0;
			
			/* image boundaries */
			if(Y==0 || Y==nHeight-1)
				SUM = 0;
			else if(X==0 || X==nWidth-1)
				SUM = 0;
			
			/* Convolution starts here */
			else   {
				
				/*-------X GRADIENT APPROXIMATION------*/
				for(I=-1; I<=1; I++)  {
					for(J=-1; J<=1; J++)  {
						sumX = sumX + (float)( (*(pImage+ X + I +(Y + J)*nWidth)) *	
							GX[I+1][J+1]);
					}
				}
				
				/*-------Y GRADIENT APPROXIMATION-------*/
				for(I=-1; I<=1; I++)  {
					for(J=-1; J<=1; J++)  {
						sumY = sumY + (float)( (*(pImage+ X + I + 
							(Y + J)*nWidth)) * GY[I+1][J+1]);
					}
				}
				
				/*---GRADIENT MAGNITUDE APPROXIMATION (Myler p.218)----*/
				SUM = abs(sumX) + abs(sumY);
			}
			*(pEdge+ X + Y*nWidth) = SUM;
		}
	}	
}

