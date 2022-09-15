#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 8

int clip(int x);

unsigned char yplane[1080][1920];						// Stores pixel values
unsigned char pred0_plane[1080][1920];					// Stores Horizontal prediction values
unsigned char pred1_plane[1080][1920];					// Stores Vertical prediction values
unsigned char pred2_plane[1080][1920];					// Stores DC prediction values
unsigned char pred3_plane[1080][1920];					// Stores Plane prediction values
unsigned char best_pred_input_plane[1080][1920];		// Stores the best prediction values for DCT input
unsigned char best_pred_output_plane[1080][1920];		// Stores the best prediction values for RDCT output

int main()
{
	int block_i = 1080 / N;
	int block_j = 1920 / N;
	
	FILE *fd1;
	// FILE *fd2;
	
	int i, j, k, l, m, n;		// Counters
	double P[N][N];				// Pixel value 8x8 matrix
	double Pred0[N][N];			// Holds Horizontal prediction values
	double Pred1[N][N];			// Holds Vertical prediction values
	double Pred2[N][N];			// Holds DC prediction values
	double Pred3[N][N];			// Holds Plane prediction values
	
	// Sums used by DC prediction
	int dc_pred_sum0 = 0;
	int dc_pred_sum1 = 0;
	
	// Variables used in Plane prediction
	int a, b, c;
	int H, V;
	int res;
	
	fd1 = fopen("Kimono_1920x1080_24.yuv", "rb");
    // fd2 = fopen("Kimono_1920x1080_24_fr1.yuv", "wb");
    
    fread(yplane, 1, sizeof(yplane), fd1);
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Intra Prediction ~~~~~~~~~~~~~~~~~~~~~~~~~
	for (i = 0; i < block_i; i++)
	{
		for (j = 0; j < block_j; j++)
		{			
			// ~~~~~~~~~~~~~ Horizontal prediction ~~~~~~~~~~~~~
			if (i != 0)
			{
				for (k = 0; k < N; k++)
				{
					for (l = 0; l < N; l++)
					{
						Pred0[k][l] = yplane[i*N - 1][j*N + l];
					}
				}
			}
			
			// ~~~~~~~~~~~~~ Verctical prediction ~~~~~~~~~~~~~
			if (j != 0)
			{
				for (k = 0; k < N; k++)
				{
					for (l = 0; l < N; l++)
					{
						Pred1[k][l] = yplane[i*N + k][j*N - 1];
					}
				}
			}
			
			// ~~~~~~~~~~~~~ DC prediction ~~~~~~~~~~~~~
			// 1st case, both exist
			if (i != 0 && j != 0)
			{
				for (k = 0; k < N; k++)
				{
					for (l = 0; l < N; l++)
					{
						for (m = 0; m < N; m++)
						{
							dc_pred_sum0 += yplane[i*N + m][j*N - 1];
							dc_pred_sum1 += yplane[i*N - 1][j*N + m + 8];
						}
						
						Pred2[k][l] = (unsigned int)(dc_pred_sum0 + dc_pred_sum1) >> 4;
						
						dc_pred_sum0 = 0;
						dc_pred_sum1 = 0;
					}
				}
			}
			
			// 2nd case, vertical ones exist
			if (j != 0)
			{
				for (k = 0; k < N; k++)
				{
					for (l = 0; l < N; l++)
					{
						for (m = 0; m < N; m++)
						{
							dc_pred_sum0 += yplane[i*N + m][j*N - 1] + 4;
						}
						Pred2[k][l] = (unsigned int)(dc_pred_sum0) >> 3;
						
						dc_pred_sum0 = 0;
					}
				}
			}
			
			// 3rd case, horizontal ones exist
			if (i != 0)
			{
				for (k = 0; k < N; k++)
				{
					for (l = 0; l < N; l++)
					{
						for (m = 0; m < N; m++)
						{
							dc_pred_sum0 += yplane[j*N + m][j*N - 1] + 4;
						}
						Pred2[k][l] = (unsigned int)(dc_pred_sum0) >> 3;
						
						dc_pred_sum0 = 0;
					}
				}
			}
			
			// 4th case
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					Pred2[k][l] = 128;
				}
			}
			
			// ~~~~~~~~~~~~~ Plane prediction ~~~~~~~~~~~~~
			if (i != 0 && j != 0)
			{
				for (k = 0; k < N; k++)
				{
					for (l = 0; l < N; l++)
					{
						// H and V sum calculation
						for (m = 0; m < 3; m++)
						{
							H += (m + 1)*(yplane[i*N + 4 + m][j*N - 1] - yplane[i*N + 2 - m][j*N - 1]);
							V += (m + 1)*(yplane[i*N - 1][j*N + 4 + m] - yplane[i*N - 1][j*N + 2 - m]);
						}
						
						a = 16*(yplane[i*N - 1][j*N - 1] + yplane[j*N - 1][i*N - 1]);
						b = (unsigned int)(17*H + 16) >> 5;
						c = (unsigned int)(17*V + 16) >> 5;
						
						res = (unsigned int)(a + b*(k - 3) + c*(l - 3) + 16) >> 3;
						
						Pred3[k][l] = clip(res);
					}
				}
			}
			
			// Stores prediction values from pred 8x8 blocks to Full HD frames to compare with each other later
			for (k = 0; k < N; k++)
			{
				for (l = 0; l < N; l++)
				{
					pred0_plane[i*N + k][j*N + l] = Pred0[k][l];
					pred1_plane[i*N + k][j*N + l] = Pred1[k][l];
					pred2_plane[i*N + k][j*N + l] = Pred2[k][l];
					pred3_plane[i*N + k][j*N + l] = Pred3[k][l];
				}
			}
		}
	}
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Finding the Best Prediction for DCT input ~~~~~~~~~~~~~~~~~~~~~~~~~
	int min_pred;
	int pred1;
	int pred2;
	int pred3;
	
	for (i = 0; i < block_i; i++)
	{
		for (j = 0; j < block_j; j++)
		{
			min_pred = abs((int)yplane[i][j] - (int)pred0_plane[i][j]);
			pred1 = abs((int)yplane[i][j] - (int)pred1_plane[i][j]);
			pred2 = abs((int)yplane[i][j] - (int)pred2_plane[i][j]);
			pred3 = abs((int)yplane[i][j] - (int)pred3_plane[i][j]);
			
			if (min_pred > pred1) min_pred = pred1;
			if (min_pred > pred2) min_pred = pred2;
			if (min_pred > pred3) min_pred = pred3;
			
			best_pred_input_plane[i][j] = min_pred;
		}
	}
	
	printf("Best predicted values found.\n");
	
	/*
	// ~~~~~~~~~~~~~~~~~~~~~~~~~ Finding the Best Prediction for RDCT output ~~~~~~~~~~~~~~~~~~~~~~~~~
	int max_pred;
	int pred_1;
	int pred_2;
	int pred_3;
	
	for (i = 0; i < block_i; i++)
	{
		for (j = 0; j < block_j; j++)
		{
			max_pred = (int)yplane[i][j] + (int)pred0_plane[i][j];
			pred_1 = (int)yplane[i][j] + (int)pred1_plane[i][j];
			pred_2 = (int)yplane[i][j] + (int)pred2_plane[i][j];
			pred_3 = (int)yplane[i][j] + (int)pred3_plane[i][j];
			
			if (max_pred < pred_1) max_pred = pred_1;
			if (max_pred < pred_2) max_pred = pred_2;
			if (max_pred < pred_3) max_pred = pred_3;
			
			best_pred_output_plane[i][j] = max_pred;
		}
	}
	*/
	
	/*
    fwrite(yplane, 1, sizeof(yplane), fd2);

    fclose(fd2);
    
    fclose(fd1);
	*/
	
	printf("\nThe program has ended.\n");
	
	return 0;
}

int clip(int x)
{
	if (x < 0)
	{
		return 0;
	}
	else if (x > 255)
	{
		return 255;
	}
	else
	{
		return x;
	}
}
