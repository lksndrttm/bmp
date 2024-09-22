#include <iostream>

#include "bmp.h"

int main(int argc, char *argv[]) {
    try {
        BMP::BMPImage img;
        img.openBMP(argv[1]);
        img.displayBMP();
    } catch (const BMP::BMPException &e) {
        std::cout << e.what() << "\n";
    }

    return 0;
}
