#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>

#define FRAMES 240
#define W 1920
#define H 1080
#define W_H (W / 2)
#define H_H (H / 2)
#define N 8
#define PI 3.141592

unsigned char yplane[H][W];     // array for the y component i.e. your greyscale image
unsigned char uplane[H_H][W_H]; // array for the u component. Needed to create the .yuv output file
unsigned char vplane[H_H][W_H]; // array for the v component. Needed to create the .yuv output file

FILE* OpenYUVFileWrite(const char* filename);
FILE* OpenYUVFileRead(const char* filename);
void CloseFile(FILE* fd, const char* filename);
void PrintMatrix(const double mtx[N][N]);
void MultiplyMatrices(const double mtx1[N][N], const double mtx2[N][N], double dest[N][N]);
void Copy8x8PixelBlock(double P[N][N], int row, int col);
void CopyNew8x8PixelBlockToYPlane(double P[N][N], int row, int col);
void DCTransform(const double D[N][N], const double DTranspose[N][N], const double P[N][N], double Coef[N][N]);
void InverseDCTransform(const double D[N][N], const double DTranspose[N][N], const double Coef[N][N], double P[N][N]);
void Quantize8x8Block(const double Q[N][N], const double Coef[N][N], double C[N][N]);
void Scale8x8Block(const double Q[N][N], const double C[N][N], double Coef[N][N]);
void EncodeImage();
void DecodeImage();