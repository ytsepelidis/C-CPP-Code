#include <stdio.h>
#include <math.h>

#define PI 3.14159265
#define N 8
/*
	The DCT and Quantization processes work fine however, 
	the RLE part does not store the information 
	provided by the function it uses.
	That is why the RL Decoding part in the imageDecompression.c
	program is empty.
*/
int rle(int temp, int i, int j, int zero_cnt);		// Counts zeroes before non-zero elements

unsigned char yplane[1080][1920];					// Stores pixel values
unsigned char coef_plane[1080][1920];				// Stores coefficient values
unsigned char quant_plane[1080][1920];				// Stores the quantized coefficient values
unsigned char encoded_plane[1080][1920];			// Stores the final encoded values

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
	double rle_block[N][N]; 		// Matrix that will be used for the RLE process
	double elementSum = 0;			// Used for matrix multiplication
	
	// Variables used to calculate the DCT 8x8 Matrix 
	double C;
	double numerator;
	double cosine;
	
	// Quantization matrix
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
	
	// Used for Run Length Encoding
	int array_temp;
	int zero_cnt = 0;
	
	// Used for Zig-Zag traversal of matrices
	int crnt_i;
	int crnt_j;
	int firstRow;
	int firstCol;
	int finalRow;
	int finalCol;
	
	fd1 = fopen("Kimono_1920x1080_24.yuv", "rb");
    // fd2 = fopen("Kimono_1920x1080_24_fr1.yuv", "wb");
    
    fread(yplane, 1, sizeof(yplane), fd1);
    
    printf("Image compression has begun.\n");
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ DCT Process ~~~~~~~~~~~~~~~~~~~~~~~~~
	for (i = 0; i < block_i; i++)
	{
		for (j = 0; j < block_j; j++)
		{
			// Access 8x8 block with pixel values
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					P[k][l] = yplane[i*N + k][j*N + l];
				}
			}
			
			// DCT Matrix before multiplication with 8x8 matrix with pixel values
			for (m = 0; m < N; m++)
			{
				if (m == 0)
				{
					C = sqrt(1 / 8.0);
				}
				else
				{
					C = sqrt(2 / 8.0);
				}
				
				for (n = 0; n < N; n++)
				{
					numerator = (2*n + 1)*m*PI;
					cosine = cos(numerator / (2.0 * N));
					D[m][n] = C*cosine;
				}
			}
			
			// Matrix multiplication D * P
			for (m = 0; m < N; m++)
			{
				for (n = 0; n < N; n++)
				{
					for (k = 0; k < N; k++)
					{
						elementSum += D[m][k]*P[k][n];
					}
					
					Coef[m][n] = elementSum;
					elementSum = 0;
				}
			}
			
			// Matrix multiplication with current Coef * D(Transpose)
			for (m = 0; m < N; m++)
			{
				for (n = 0; n < N; n++)
				{
					for (k = 0; k < N; k++)
					{
						elementSum += Coef[m][k]*D[n][k];	// I inverted the values representing rows with the columns 
															// and vice versa to mimic the transposed D matrix.
					}
					
					Coef[m][n] = elementSum;
					elementSum = 0;
				}
			}
			
			// Store coefficient values from current 8x8 block to a Full HD frame
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					coef_plane[i*N + k][j*N + l] = Coef[k][l];
				}
			}
		}
	}
	
	printf("\nThe DCT process has ended.\n");
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Quantization Process ~~~~~~~~~~~~~~~~~~~~~~~~~
	for (i = 0; i < block_i; i++)
	{
		for (j = 0; j < block_j; j++)
		{
			// Accesses 8x8 block with coefficient values and quantizes them
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					quant_plane[i*N + k][j*N + l] = round(coef_plane[i*N + k][j*N + l] / Q[k][l]);
				}
			}
		}
	}
	
	printf("\nThe Quantization process has ended.\n");
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Run Length Encoding ~~~~~~~~~~~~~~~~~~~~~~~~~
	for (i = 0; i < block_i; i++)
	{
		for (j = 0; j < block_j; j++)
		{
			// Access 8x8 block with quantized values
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					rle_block[k][l] = quant_plane[i*N + k][j*N + l];
				}
			}
			
			crnt_i = 0;
			crnt_j = 0;
			firstRow = 0;
			firstCol = 0;
			finalRow = N-1;
			finalCol = N-1;
			
			// Zig-Zag traversal of matrix with Run Length Encoding
			while (crnt_i != finalRow || crnt_j != finalCol)
			{
				if (crnt_i == firstRow && crnt_j == firstCol)
				{
					array_temp = rle_block[crnt_i][crnt_j];
					zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
				}
				
				if (crnt_i == firstRow && crnt_j != finalCol)
				{
					crnt_j += 1;
					array_temp = rle_block[crnt_i][crnt_j];
					zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
					
					while (crnt_j != firstCol)
					{
						crnt_i += 1;
						crnt_j -= 1;
						array_temp = rle_block[crnt_i][crnt_j];
						zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
					}
				}
				else if (crnt_j == firstCol && crnt_i != finalRow)
				{
					crnt_i += 1;
					array_temp = rle_block[crnt_i][crnt_j];
					zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
					
					while (crnt_i != firstRow)
					{
						crnt_i -= 1;
						crnt_j += 1;
						array_temp = rle_block[crnt_i][crnt_j];
						zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
					}
				}
				else if (crnt_i == finalRow)
				{
					crnt_j += 1;
					array_temp = rle_block[crnt_i][crnt_j];
					zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
					
					while (crnt_j != finalCol)
					{
						crnt_i -= 1;
						crnt_j += 1;
						array_temp = rle_block[crnt_i][crnt_j];
						zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
					}
				}
				else if (crnt_j == finalCol)
				{
					crnt_i += 1;
					array_temp = rle_block[crnt_i][crnt_j];
					zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
					
					while (crnt_i != finalRow)
					{
						crnt_i += 1;
						crnt_j -= 1;
						array_temp = rle_block[crnt_i][crnt_j];
						zero_cnt = rle (array_temp, crnt_i, crnt_j, zero_cnt);
					}
				}
			}
		}
	}
	
	printf("\nThe Run Length Encoding process has ended.\n");
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Copying encoded values to yplane ~~~~~~~~~~~~~~~~~~~~~~~~~
	for (i = 0; i < 1080; i++)
	{
		for (j = 0; j < 1920; j++)
		{
			yplane[i][j] = quant_plane[i][j];
		}
	}
	
	printf("\nThe image has been compressed.\n");
	
	/*
    fwrite(yplane, 1, sizeof(yplane), fd2);

    fclose(fd2);
    
    fclose(fd1);
	*/
	
	return 0;
}

int rle(int temp, int i, int j, int zero_cnt)
{
	if (temp != 0)
	{
		// printf("There were %d zeroes before quantized value %d from rle_block[%d][%d].\n", zero_cnt, temp, i, j);
		zero_cnt = 0;
		return zero_cnt;
	}
	else
	{
		zero_cnt += 1;
		return zero_cnt;
	}
}
