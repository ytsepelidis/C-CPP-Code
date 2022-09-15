#include <stdio.h>
#include <math.h>

#define PI 3.14159265
#define N 8

unsigned char yplane[1080][1920];				// Stores pixel values
unsigned char coef_plane[1080][1920];			// Stores coefficient values
unsigned char quant_plane[1080][1920];			// Stores the quantized coefficient values
unsigned char encoded_plane[1080][1920];		// Stores the final encoded values

int main()
{
	int block_i = 1080 / N;
	int block_j = 1920 / N;
	
	FILE *fd1;
	// FILE *fd2;
	
	int i, j, k, l, m, n;			// Counters
	double P[N][N];					// Pixel value 8x8 matrix
	double D[N][N];					// DCT 8x8 matrix
	double Coef[N][N];				// Coefficient 8x8 matrix
	double elementSum = 0;			// Used for matrix multiplication
	
	// Variables used to calculate the DCT 8x8 Matrix 
	double C;
	double numerator;
	double cosine;
	
	// Dequantization matrix
	double Q[N][N] = {
		{16, 11, 10, 16, 24, 40, 51, 61},
		{12, 12, 14, 19, 26, 58, 60, 55},
		{14, 13, 16, 24, 40, 57, 69, 56},
		{14, 17, 22, 29, 51, 87, 80, 62},
		{18, 22, 37, 56, 68, 109, 103, 77},
		{24, 35, 55, 64, 81, 104, 113, 92},
		{49, 64, 78, 87, 103, 121, 120, 101},
		{72, 92, 95, 98, 112, 100, 103, 99}
	};
	
	fd1 = fopen("Kimono_1920x1080_24.yuv", "rb");
    // fd2 = fopen("Kimono_1920x1080_24_fr1.yuv", "wb");
    
    fread(yplane, 1, sizeof(yplane), fd1);
	
	printf("Image decompression has begun.\n");
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Run Length Decoding ~~~~~~~~~~~~~~~~~~~~~~~~~
	
	// printf("\nThe Run Length Decoding process has ended.\n");
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Scaling Process ~~~~~~~~~~~~~~~~~~~~~~~~~ 
	for (i = 0; i < block_i; i++)
	{
		for (j = 0; j < block_j; j++)
		{
			// Access 8x8 block with estimated quantized values 
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					coef_plane[i*N + k][j*N + l] = quant_plane[i*N + k][j*N + l] * Q[k][l];
				}
			}
		}
	}
	
	printf("\nThe Scaling process has ended.\n");
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Reverse DCT Process ~~~~~~~~~~~~~~~~~~~~~~~~~
	for (i = 0; i < block_i; i++)
	{
		for (j = 0; j < block_j; j++)
		{
			// Access 8x8 block of the estimated coefficient values
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					Coef[k][l] = coef_plane[i*N + k][j*N + l];
				}
			}
			
			// Matrix multiplication D(Transpose) * Coef
			for (m = 0; m < N; m++)
			{
				for (n = 0; n < N; n++)
				{
					for (k = 0; k < N; k++)
					{
						elementSum += D[k][m]*Coef[k][n];	// I inverted the values representing rows with the columns
															// and vice versa to mimic the transposed D matrix.
					}
					
					P[m][n] = elementSum;
					elementSum = 0;
				}
			}
			
			// Matrix multiplication with current P * D
			for (m = 0; m < N; m++)
			{
				for (n = 0; n < N; n++)
				{
					for (k = 0; k < N; k++)
					{
						elementSum += Coef[m][k]*D[k][n];
					}
					
					P[m][n] = elementSum;
					elementSum = 0;
				}
			}
			
			// Store estimated pixel values from current 8x8 block to a Full HD frame
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					yplane[i*N + k][j*N + l] = P[k][l];
				}
			}
		}
	}
	
	printf("\nThe Reverse DCT process has ended.\n");
	
	printf("\nThe image has been decompressed.\n");
	
	/*
    fwrite(yplane, 1, sizeof(yplane), fd2);

    fclose(fd2);
    
    fclose(fd1);
	*/
	
	return 0;
}
