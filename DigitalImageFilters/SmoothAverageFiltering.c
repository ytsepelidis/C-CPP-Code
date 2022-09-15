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
void AverageFiltering();
void AccessOriginNeighbors (int x, int y);
int MaskAverage();

int main()
{
	FILE *fd1;
	FILE *fd2;
	
	fd1 = fopen("BlowingBubbles_416x240.yuv", "rb");
	fd2 = fopen("BlowingBubbles_416x240_Average.yuv", "wb");
	
	fread(yplane, 1, sizeof(yplane), fd1);
	fread(uplane, 1, sizeof(uplane), fd1);
	fread(vplane, 1, sizeof(vplane), fd1);
	
	CopyToTempPlane();
	AverageFiltering();
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

void AverageFiltering()
{
	int i1, i2;
	
	for (i1 = OFFSET; i1 < (HEIGHT - OFFSET); i1++)
	{
		for (i2 = OFFSET; i2 < (WIDTH - OFFSET); i2++)
		{
			AccessOriginNeighbors (i1, i2);
			TempPlane[i1][i2] = MaskAverage();
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

int MaskAverage()
{
	int i1, i2;
	int MaskSum = 0;
	int MaskAvg;
	
	for (i1 = 0; i1 < FILTER; i1++)
	{
		for (i2 = 0; i2 < FILTER; i2++)
		{
			MaskSum += FilteringMask[i1][i2];
		}
	}
	
	MaskAvg = MaskSum / FILTER_PIXELS;
	
	return MaskAvg;
}
