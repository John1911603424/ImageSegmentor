//***************************************************************************
// �ļ���Thinner.h
// ���ܣ����ֲ�ͬ��ϸ���㷨
//***************************************************************************

void beforethin(unsigned char *ip, unsigned char *jp, 
				unsigned long lx, unsigned long ly);
void ThinnerHilditch(void *image, unsigned long lx, unsigned long ly);
void ThinnerPavlidis(void *image, unsigned long lx, unsigned long ly);
void ThinnerRosenfeld(void *image, unsigned long lx, unsigned long ly);
//ע��ú���lWidthӦ����Height��
BOOL WINAPI ThiningDIBSkeleton (unsigned char* lpDIBBits, LONG lWidth, LONG lHeight);
//the following two functions from image_j1 program
	int Thin_1(int *x ,  int N1,int M1,int N2,int M2);//M1,M2 ��ʾ���
	int Thin_2(int *x ,  int N1,int M1,int N2,int M2) ; // ϸ���㷨
