#include "functions.h"

FILE* OpenYUVFileWrite(const char* filename)
{
    // Opening file with error handling
    FILE* fd;
    errno_t err;

    err = fopen_s(&fd, filename, "wb");
    if (err == 0)
    {
        printf("The file '%s' was successfully opened.\n", filename);
    }
    else
    {
        printf("The file '%s' failed to open.\n", filename);
        exit(EXIT_FAILURE);
    }

    return fd;
}

FILE* OpenYUVFileRead(const char* filename)
{
    // Opening file with error handling
    FILE* fd;
    errno_t err;

    err = fopen_s(&fd, filename, "rb");
    if (err == 0)
    {
        printf("The file '%s' was successfully opened.\n", filename);
    }
    else
    {
        printf("The file '%s' failed to open.\n", filename);
        exit(EXIT_FAILURE);
    }

    return fd;
}

void CloseFile(FILE* fd, const char* filename)
{
    // Closing file with error handling
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

    // Printing matrix
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%8.2lf\t", mtx[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}

void MultiplyMatrices(const double mtx1[N][N], const double mtx2[N][N], double dest[N][N])
{
    int i, j, k;

    // Matrix multiplication
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

void CopyNxNPixelBlock(double P[N][N], int row, int col)
{
    int i, j;
    int k = 0;
    int l = 0;

    for (i = row; i < row + N; i++)
    {
        for (j = col; j < col + N; j++)
        {
            // Copying source pixels to temp NxN block
            P[k][l] = (double)yplane[i][j];
            l += 1;
        }
        k += 1;
        l = 0;
    }
}

void ApplyIntegerTransform(const double T[N][N], const double T_transp[N][N], const double P[N][N], double Y[N][N])
{
    double temp[N][N];

    // Y = TN * P * TN_transp
    MultiplyMatrices(T, P, temp);
    MultiplyMatrices(temp, T_transp, Y);
}

void AddSquaredCoefVals(const double P[N][N], double avgCoefEnergyMtx[N][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            // Adding squared coef value to corresponding matrix
            avgCoefEnergyMtx[i][j] += P[i][j] * P[i][j];
        }
    }
}

void CalculateAvgCoefEnergy(double avgCoefEnergyMtx[N][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            // Dividing by the number of NxN blocks
            avgCoefEnergyMtx[i][j] = avgCoefEnergyMtx[i][j] / B;
        }
    }
}

void TransformImage(const double DCT[N][N], const double DCT_transp[N][N], const double DST[N][N], const double DST_transp[N][N])
{
    int i, j;
    double P[N][N];
    double Y_DCT[N][N];
    double Y_DST[N][N];
    double avgCoefEnergyOrig[N][N] = { 0 };
    double avgCoefEnergyDCT[N][N] = { 0 };
    double avgCoefEnergyDST[N][N] = { 0 };

    // Frame traversal for NxN blocks
    for (i = 0; i < H; i = i + N)
    {
        for (j = 0; j < W; j = j + N)
        {
            // Copying source pixels to temp NxN block
            CopyNxNPixelBlock(P, i, j);

            // Applying DCT
            ApplyIntegerTransform(DCT, DCT_transp, P, Y_DCT);

            // Applying DST
            ApplyIntegerTransform(DST, DST_transp, P, Y_DST);

            // Adding squared coef values to matrices for avg coef energy calculation
            AddSquaredCoefVals(P, avgCoefEnergyOrig);
            AddSquaredCoefVals(Y_DCT, avgCoefEnergyDCT);
            AddSquaredCoefVals(Y_DST, avgCoefEnergyDST);
        }
    }

    // Calculating avg coef energy
    CalculateAvgCoefEnergy(avgCoefEnergyOrig);
    CalculateAvgCoefEnergy(avgCoefEnergyDCT);
    CalculateAvgCoefEnergy(avgCoefEnergyDST);

    // Printing results
    printf("Average Coefficient Energy of Original source:\n");
    PrintMatrix(avgCoefEnergyOrig);
    printf("Average Coefficient Energy of DCT values:\n");
    PrintMatrix(avgCoefEnergyDCT);
    printf("Average Coefficient Energy of DST values:\n");
    PrintMatrix(avgCoefEnergyDST);
}

void TransformVideo(const char* seq, const char* newSeq)
{
    int i;
    FILE* fd1 = NULL;
    FILE* fd2 = NULL;

    // 4x4 integer matrices divided by 128
    const double DCT4[4][4] = {
        { 0.50,  0.50,  0.50,  0.50, },
        { 0.65,  0.28, -0.28, -0.65, },
        { 0.50, -0.50, -0.50,  0.50, },
        { 0.28, -0.65,  0.65, -0.28  },
    };

    const double DCT4_transp[4][4] = {
        { 0.50,  0.65,  0.50,  0.28, },
        { 0.50,  0.28, -0.50, -0.65, },
        { 0.50, -0.28, -0.50,  0.65, },
        { 0.50, -0.65,  0.50, -0.28  }
    };

    const double DST4[4][4] = {
        { 0.23,  0.43,  0.58,  0.66, },
        { 0.58,  0.58,  0.00, -0.58, },
        { 0.66, -0.23, -0.58,  0.43, },
        { 0.43, -0.66,  0.58, -0.23  }
    };

    const double DST4_transp[4][4] = {
        { 0.23,  0.58,  0.66,  0.43, },
        { 0.43,  0.58, -0.23, -0.66, },
        { 0.58,  0.00, -0.58,  0.58, },
        { 0.66, -0.58,  0.43, -0.23  }
    };

    // Opening files
    fd1 = OpenYUVFileRead(seq);
    fd2 = OpenYUVFileWrite(newSeq);

    // Transforming given number of frames
    for (i = 0; i < FRAMES; i++)
    {
        fread(yplane, 1, sizeof(yplane), fd1);
        fread(uplane, 1, sizeof(uplane), fd1);
        fread(vplane, 1, sizeof(vplane), fd1);

        TransformImage(DCT4, DCT4_transp, DST4, DST4_transp);

        fwrite(yplane, 1, sizeof(yplane), fd2);
        fwrite(uplane, 1, sizeof(uplane), fd2);
        fwrite(vplane, 1, sizeof(vplane), fd2);
    }

    // Closing files
    CloseFile(fd1, seq);
    CloseFile(fd2, newSeq);
}