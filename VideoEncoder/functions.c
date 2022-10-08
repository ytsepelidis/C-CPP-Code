#include "functions.h"

FILE* OpenYUVFileWrite(const char* filename)
{
    FILE* fd;
    errno_t err;

    err = fopen_s(&fd, filename, "wb");
    if (err == 0)
    {
        printf("The file '%s' was successfully opened.\n", filename);
        return fd;
    }
    else
    {
        printf("The file '%s' failed to open.\n", filename);
        exit(EXIT_FAILURE);
    }
}

FILE* OpenYUVFileRead(const char* filename)
{
    FILE* fd;
    errno_t err;

    err = fopen_s(&fd, filename, "rb");
    if (err == 0)
    {
        printf("The file '%s' was successfully opened.\n", filename);
        return fd;
    }
    else
    {
        printf("The file '%s' failed to open.\n", filename);
        exit(EXIT_FAILURE);
    }
}

void CloseFile(FILE* fd, const char* filename)
{
    errno_t err;

    if (fd)
    {
        err = fclose(fd);
        if (err == 0)
        {
            printf("The file '%s' was closed.\n", filename);
        }
        else
        {
            printf("The file '%s' failed to close.\n", filename);
            exit(EXIT_FAILURE);
        }
    }
}

void PrintMatrix(const double mtx[N][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%.2f\t", mtx[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

void MultiplyMatrices(const double mtx1[N][N], const double mtx2[N][N], double dest[N][N])
{
    int i, j, k;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            dest[i][j] = 0;
            for (k = 0; k < N; k++)
            {
                dest[i][j] += mtx1[i][k] * mtx2[k][j];
            }
        }
    }
}

void Copy8x8PixelBlock(double P[N][N], int row, int col)
{
    int i, j;
    int k = 0;
    int l = 0;

    for (i = row; i < row + N; i++)
    {
        for (j = col; j < col + N; j++)
        {
            P[k][l] = (double) yplane[i][j];
            l += 1;
        }
        k += 1;
        l = 0;
    }
}

void CopyNew8x8PixelBlockToYPlane(double P[N][N], int row, int col)
{
    int i, j;
    int k = 0;
    int l = 0;

    for (i = row; i < row + N; i++)
    {
        for (j = col; j < col + N; j++)
        {
            yplane[i][j] = (unsigned char)P[k][l];
            l += 1;
        }
        k += 1;
        l = 0;
    }
}

void DCTransform(const double D[N][N], const double DTranspose[N][N], const double P[N][N], double Coef[N][N])
{
    double temp[N][N];

    // Coef = D * P * DTranspose
    MultiplyMatrices(D, P, temp);
    MultiplyMatrices(temp, DTranspose, Coef);
}

void InverseDCTransform(const double D[N][N], const double DTranspose[N][N], const double Coef[N][N], double P[N][N])
{
    int i, j;
    double temp[N][N];

    // P = DTranspose * Coef * D
    MultiplyMatrices(DTranspose, Coef, temp);
    MultiplyMatrices(temp, D, P);

    // Rounding new Pixel values
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            P[i][j] = round(P[i][j]);
        }
    }
}

void Quantize8x8Block(const double Q[N][N], const double Coef[N][N], double C[N][N])
{
    int i, j;

    // Quantizing 8x8 block
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            C[i][j] = round(Coef[i][j] / Q[i][j]);
        }
    }
}

void Scale8x8Block(const double Q[N][N], const double C[N][N], double Coef[N][N])
{
    int i, j;

    // Scaling 8x8 block
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            Coef[i][j] = C[i][j] * Q[i][j];
        }
    }
}

void EncodeImage()
{
    int i, j;
    double c;
    double P[N][N];
    double D[N][N];
    double DTranspose[N][N];
    double Coef[N][N];
    double C[N][N];
    double Q[N][N] = {
        { 16, 11, 10, 16, 24, 40, 51, 61 },
        { 12, 12, 14, 19, 26, 58, 60, 55 },
        { 14, 13, 16, 24, 40, 57, 69, 56 },
        { 14, 17, 22, 29, 51, 87, 80, 62 },
        { 18, 22, 37, 56, 68, 109, 103, 77 },
        { 24, 35, 55, 64, 81, 104, 113, 92 },
        { 49, 64, 78, 87, 103, 121, 120, 101 },
        { 72, 92, 95, 98, 112, 100, 103, 99 }
    };

    // DCT and DCT Transpose kernel calculation
    for (i = 0; i < N; i++)
    {
        if (i == 0)
        {
            c = sqrt(1 / (double)N);
        }
        else
        {
            c = sqrt(2 / (double)N);
        }

        for (j = 0; j < N; j++)
        {
            D[i][j] = c * cos(((2 * j + 1) * i * PI) / (2 * N));
            DTranspose[j][i] = D[i][j];
        }
    }

    // Frame traversal for 8x8 blocks
    for (i = 0; i < H; i = i + N)
    {
        for (j = 0; j < W; j = j + N)
        {
            // Copying pixel values to temp 8x8 pixel block P
            Copy8x8PixelBlock(P, i, j);

            // Applying DCT for Coefficient 8x8 block Coef
            DCTransform(D, DTranspose, P, Coef);

            // Quantizing Coefficient values and storing in block C
            Quantize8x8Block(Q, Coef, C);

            // Run Length Encoding
            CopyNew8x8PixelBlockToYPlane(C, i, j);

            // Scaling Quantized values for estimated Coefficient values
            Scale8x8Block(Q, C, Coef);

            // Applying Inverse DCT for new 8x8 Pixel block P
            InverseDCTransform(D, DTranspose, Coef, P);

            // Copying new 8x8 Pixel block to yplane
            CopyNew8x8PixelBlockToYPlane(P, i, j);
        }
    }
}

void DecodeImage()
{
    int i, j;
    double c;
    double P[N][N];
    double D[N][N];
    double DTranspose[N][N];
    double Coef[N][N];
    double C[N][N];
    double Q[N][N] = {
        { 16, 11, 10, 16, 24, 40, 51, 61 },
        { 12, 12, 14, 19, 26, 58, 60, 55 },
        { 14, 13, 16, 24, 40, 57, 69, 56 },
        { 14, 17, 22, 29, 51, 87, 80, 62 },
        { 18, 22, 37, 56, 68, 109, 103, 77 },
        { 24, 35, 55, 64, 81, 104, 113, 92 },
        { 49, 64, 78, 87, 103, 121, 120, 101 },
        { 72, 92, 95, 98, 112, 100, 103, 99 }
    };

    // DCT and DCT Transpose kernel calculation
    for (i = 0; i < N; i++)
    {
        if (i == 0)
        {
            c = sqrt(1 / (double)N);
        }
        else
        {
            c = sqrt(2 / (double)N);
        }

        for (j = 0; j < N; j++)
        {
            D[i][j] = c * cos(((2 * j + 1) * i * PI) / (2 * N));
            DTranspose[j][i] = D[i][j];
        }
    }

    // Frame traversal for 8x8 blocks
    for (i = 0; i < H; i = i + N)
    {
        for (j = 0; j < W; j = j + N)
        {
            // Run Length Decoding

            // Scaling Quantized values for estimated Coefficient values
            Scale8x8Block(Q, C, Coef);
            printf("Estimated Coefficient values:\n");
            PrintMatrix(Coef);

            // Applying Inverse DCT for new 8x8 Pixel block P
            InverseDCTransform(D, DTranspose, Coef, P);
            printf("New Pixel values:\n");
            PrintMatrix(P);
        }
    }
}