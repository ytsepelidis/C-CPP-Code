#include "functions.h"

int main()
{
    int i;
    const char* filename = "Kimono_1920x1080_24.yuv";
    const char* newFilename = "Kimono_1920x1080_24_compressed.yuv";
    FILE* fd1;
    FILE* fd2;

    // Opening files
    fd1 = OpenYUVFileRead(filename);
    fd2 = OpenYUVFileWrite(newFilename);

    // Encoding each frame
    for (i = 0; i < FRAMES; i++)
    {
        // Reading .yuv data to matrices
        fread(yplane, 1, sizeof(yplane), fd1);
        fread(uplane, 1, sizeof(uplane), fd1);
        fread(vplane, 1, sizeof(vplane), fd1);

        // Encoding one frame
        EncodeImage();

        // Writing compressed .yuv data to file
        fwrite(yplane, 1, sizeof(yplane), fd2);
        fwrite(uplane, 1, sizeof(uplane), fd2);
        fwrite(vplane, 1, sizeof(vplane), fd2);
    }

    // Closing files
    CloseFile(fd1, filename);
    CloseFile(fd2, newFilename);

    return 0;
}
