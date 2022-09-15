#include <stdio.h>
#include <stdbool.h>
#define WIDTH 416
#define HEIGHT 240
#define FILTER 3
#define HALF_WIDTH (WIDTH / 2)
#define HALF_HEIGHT (HEIGHT / 2)
#define PIXELS (WIDTH * HEIGHT)
#define FILTER_PIXELS (FILTER * FILTER)
#define OFFSET (FILTER / 2)

unsigned char yplane[HEIGHT][WIDTH]; 				// array for the y component i.e. your greyscale image
unsigned char uplane[HALF_HEIGHT][HALF_WIDTH];	 	// array for the u component. Needed to create the .yuv output file
unsigned char vplane[HALF_HEIGHT][HALF_WIDTH]; 		// array for the v component. Needed to create the .yuv output file
unsigned char TempPlane[HEIGHT][WIDTH];
unsigned char FilteringMask[FILTER][FILTER];

void CopyToTempPlane();
void CopyToYPlane();
void MedianFiltering();
void AccessOriginNeighbors (int x, int y);
int MaskMedian();
void Copy2DTo1D (int* LinearMask);
void BubbleSort (int array[], int size);

int main()
{
	FILE *fd1;
	FILE *fd2;
	
	fd1 = fopen("BlowingBubbles_416x240.yuv", "rb");
	fd2 = fopen("BlowingBubbles_416x240_Median.yuv", "wb");
	
	fread(yplane, 1, sizeof(yplane), fd1);
	fread(uplane, 1, sizeof(uplane), fd1);
	fread(vplane, 1, sizeof(vplane), fd1);
	
	CopyToTempPlane();
	MedianFiltering();
	CopyToYPlane();
		
	fwrite(yplane, 1, sizeof(yplane), fd2);
	fwrite(uplane, 1, sizeof(uplane), fd2);
	fwrite(vplane, 1, sizeof(vplane), fd2);
	
	fclose(fd2);
	fclose(fd1);
	
	return 0;
}

void CopyToTempPlane()
{
	int i1, i2;
	
	for (i1 = 0; i1 < HEIGHT; i1++)
	{
		for (i2 = 0; i2 < WIDTH; i2++)
		{
			TempPlane[i1][i2] = yplane[i1][i2];
		}
	}
}

void CopyToYPlane()
{
	int i1, i2;
	
	for (i1 = 0; i1 < HEIGHT; i1++)
	{
		for (i2 = 0; i2 < WIDTH; i2++)
		{
			yplane[i1][i2] = TempPlane[i1][i2];
		}
	}
}

void MedianFiltering()
{
	int i1, i2;
	
	for (i1 = OFFSET; i1 < (HEIGHT - OFFSET); i1++)
	{
		for (i2 = OFFSET; i2 < (WIDTH - OFFSET); i2++)
		{
			AccessOriginNeighbors (i1, i2);
			TempPlane[i1][i2] = MaskMedian();
		}
	}
}

void AccessOriginNeighbors (int x, int y)
{
	int i1, i2, i3, i4;
	
	i3 = 0;
	for (i1 = (x - OFFSET); i1 <= (x + OFFSET); i1++)
	{
		i4 = 0;
		for (i2 = (y - OFFSET); i2 <= (y + OFFSET); i2++)
		{
			FilteringMask[i3][i4] = yplane[i1][i2];
			
			i4 += 1;
		}
		
		i3 += 1;
	}
}

int MaskMedian()
{
	int LinearMask[FILTER*FILTER];
	int MedianPos;
	int MedianElem;
			
	Copy2DTo1D (LinearMask);
	BubbleSort (LinearMask, FILTER*FILTER);
		
	MedianPos = (FILTER*FILTER) / 2;
	MedianElem = LinearMask[MedianPos];	
	
	return MedianElem;
}

void Copy2DTo1D (int* LinearMask)
{
	int i1, i2;
	int i3 = 0;
	
	for (i1 = 0; i1 < FILTER; i1++)
	{
		for (i2 = 0; i2 < FILTER; i2++)
		{
			LinearMask[i3] = FilteringMask[i1][i2];
			
			i3++;
		}
	}
}

void BubbleSort (int array[], int size)
{
	int temp;
	int i1, i2;
	bool found;
	
	for (i1 = 1; i1 < size; i1++)
	{
		found = false;
		
		for (i2 = (size - 1); i2 >= i1; i2--)
		{
			if (array[i2] < array[i2 - 1])
			{
				temp = array[i2];
				array[i2] = array[i2 - 1];
				array[i2 - 1] = temp;
				
				found = true;
			}
		}
		
		if (found == false)
		{
			break;
		}
	}
}
