#include <stdio.h>
#define WIDTH 512
#define HEIGHT 512
#define HALF_WIDTH (WIDTH / 2)
#define HALF_HEIGHT (HEIGHT / 2)
#define PIXELS (WIDTH * HEIGHT)

unsigned char yplane[HEIGHT][WIDTH]; 				// array for the y component i.e. your greyscale image
unsigned char uplane[HALF_HEIGHT][HALF_WIDTH];	 	// array for the u component. Needed to create the .yuv output file
unsigned char vplane[HALF_HEIGHT][HALF_WIDTH]; 		// array for the v component. Needed to create the .yuv output file
unsigned char ErosionPlane[HEIGHT][WIDTH];

void GreyscaleToBinary();
int AveragePixelValue();
void Erosion();
void SubtractErosion();
void CopyToYPlane();

int main()
{
	FILE *fd1;
	FILE *fd2;
	
	fd1 = fopen("house_512x512.yuv", "rb");
	fd2 = fopen("house_512x512_filtered.yuv", "wb");
	
	fread(yplane, 1, sizeof(yplane), fd1);
	fread(uplane, 1, sizeof(uplane), fd1);
	fread(vplane, 1, sizeof(vplane), fd1);
	
	GreyscaleToBinary();
	Erosion();
	SubtractErosion();
	CopyToYPlane();
	
	fwrite(yplane, 1, sizeof(yplane), fd2);
	fwrite(uplane, 1, sizeof(uplane), fd2);
	fwrite(vplane, 1, sizeof(vplane), fd2);
	
	fclose(fd2);
	fclose(fd1);
	
	return 0;
}

void GreyscaleToBinary()
{
	int i1, i2;
	int PixelAvg;
	
	PixelAvg = AveragePixelValue();
	
	for (i1 = 0; i1 < HEIGHT; i1++)
	{
		for (i2 = 0; i2 < WIDTH; i2++)
		{
			if (yplane[i1][i2] < PixelAvg)
			{
    			yplane[i1][i2] = 0;
			}
			else
			{
				yplane[i1][i2] = 255;
			}
		}
	}
}

int AveragePixelValue()
{
	int i1, i2;
	int sum = 0;
	
	for (i1 = 0; i1 < HEIGHT; i1++)
	{
		for (i2 = 0; i2 < WIDTH; i2++)
		{
			sum += yplane[i1][i2];
		}
	}
	
	return sum / PIXELS;
}

void Erosion()
{
	int i1, i2, i3, i4;
	int ErosionCount;
	
	for (i1 = 0; i1 < HEIGHT; i1++)
	{
		for (i2 = 0; i2 < WIDTH; i2++)
		{
			if ((i1 == 0) || (i2 == 0) || (i1 == (HEIGHT-1)) || (i2 == (WIDTH-1)))
			{
				ErosionPlane[i1][i2] = 0;
			}
			else
			{
				ErosionCount = 0;
						
				for (i3 = i1-1; i3 <= i1+1; i3++)
				{
					for (i4 = i2-1; i4 <= i2+1; i4++)
					{
						if (yplane[i3][i4] == 255)
						{
							ErosionCount += 1;
						}
					}
				}
				
				if (ErosionCount == 9)
				{
					ErosionPlane[i1][i2] = 255;
				}
				else
				{
					ErosionPlane[i1][i2] = 0;
				}
			}
		}
	}
}

void SubtractErosion()
{
	int i1, i2;
	
	for (i1 = 0; i1 < HEIGHT; i1++)
	{
		for (i2 = 0; i2 < WIDTH; i2++)
		{
			if (yplane[i1][i2] == ErosionPlane[i1][i2])
			{
				ErosionPlane[i1][i2] = 0;
			}
			else
			{
				ErosionPlane[i1][i2] = 255;
			}
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
			yplane[i1][i2] = ErosionPlane[i1][i2];
		}
	}
}
