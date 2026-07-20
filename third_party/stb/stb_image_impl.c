/* stb_image is compiled here once. Defining STB_IMAGE_IMPLEMENTATION in any
 * other file would duplicate every stb_image symbol and break linking. */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
