#include "functions.h"

int main()
{
    const char* seq = "YachtRide_1920x1080_120fps_420_8bit_YUV.yuv";
    const char* newSeq = "YachtRide_1920x1080_120fps_420_8bit_YUV_compressed.yuv";

    // Main function
    TransformVideo(seq, newSeq);

    exit(EXIT_SUCCESS);
}