#include <stdio.h>
#include <stdlib.h>

#define H 1080                          // Frame Height
#define W 1920                          // Frame Width
#define N 4                             // Block size
#define FRAMES 1                        // Frames to transform
#define B ((H * W) / (N * N))           // Number of NxN blocks in frame

unsigned char yplane[H][W];             // array for the y component i.e. your greyscale image
unsigned char uplane[H / 2][W / 2];     // array for the u component. Needed to create the .yuv output file
unsigned char vplane[H / 2][W / 2];     // array for the v component. Needed to create the .yuv output file

FILE* OpenYUVFileWrite(const char* filename);
FILE* OpenYUVFileRead(const char* filename);
void CloseFile(FILE* fd, const char* filename);
void PrintMatrix(const double mtx[N][N]);
void MultiplyMatrices(const double mtx1[N][N], const double mtx2[N][N], double dest[N][N]);
void CopyNxNPixelBlock(double P[N][N], int row, int col);
void ApplyIntegerTransform(const double T[N][N], const double T_transp[N][N], const double P[N][N], double Y[N][N]);
void AddSquaredCoefVals(const double P[N][N], double avgCoefEnergyMtx[N][N]);
void CalculateAvgCoefEnergy(double avgCoefEnergyMtx[N][N]);
void TransformImage(const double DCT[N][N], const double DCT_transp[N][N], const double DST[N][N], const double DST_transp[N][N]);
void TransformVideo(const char* seq, const char* newSeq);