#include "bmp.h"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace BMP {

struct BMPImage::Impl {

    #pragma pack(2)
    struct BITMAPFILEHEADER {
        std::uint16_t bfType;
        std::uint32_t bfSize;
        std::uint16_t bfReserved1;
        std::uint16_t bfReserved2;
        std::uint32_t bfOfBits;
    };

    #pragma pack(2)
    struct BITMAPINFOHEADER {
        std::uint32_t biSize;
        std::int32_t biWidth;
        std::int32_t biHeight;
        std::uint16_t biPlanes;
        std::uint16_t biBitCount;
        std::uint32_t biCompression;
        std::uint32_t biSizeImage;
        std::int32_t biXPelsPerMeter;
        std::int32_t biYPelsPerMeter;
        std::uint32_t biClrUsed;
        std::uint32_t biClrImportant;
    };

    struct Pixel {
        Pixel(std::uint8_t r, std::uint8_t g, std::uint8_t b) : r(r), g(g), b(b) {}
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;

        bool is_white() 
        { 
            return (r == 255 && g == 255 && b == 255);
        }
    };

    std::vector<Pixel> imageData;

    std::unique_ptr<BITMAPFILEHEADER> bmfh = nullptr;
    std::unique_ptr<BITMAPINFOHEADER> bmih = nullptr;

    bool is_open()
    {
       return bmih != nullptr; 
    }

    void openBMP(const std::string &fileName) {
        if(is_open()){
            closeBMP();
        }

        std::fstream fs(fileName, std::ios::in | std::ios::binary);
        if (!fs.is_open()) {
            throw BMPOpenError();
        }
        
        bmfh = std::make_unique<BITMAPFILEHEADER>();
        bmih = std::make_unique<BITMAPINFOHEADER>();

        readHeaders(fs);

        if (!(bmih->biBitCount == 24 || bmih->biBitCount == 32)) {
            closeBMP();
            throw BMPUnsupportedFormat();
        }

        readImageData(fs);
    }

    void readHeaders(std::fstream &bmpFile) {
        bmpFile.read(reinterpret_cast<char*>(bmfh.get()), sizeof(BITMAPFILEHEADER));
        bmpFile.read(reinterpret_cast<char*>(bmih.get()), sizeof(BITMAPINFOHEADER));
    }

    void read24BitImageData(std::fstream &bmpFile) {
        bmpFile.seekg(bmfh->bfOfBits);
        std::uint8_t r, g, b;

        imageData.reserve(std::abs(bmih->biHeight) * bmih->biWidth);

        for (int i = 0; i < std::abs(bmih->biHeight); ++i) {
            for (int j = 0; j < bmih->biWidth; ++j) {
                bmpFile.read(reinterpret_cast<char *>(&b), sizeof(b));
                bmpFile.read(reinterpret_cast<char *>(&g), sizeof(g));
                bmpFile.read(reinterpret_cast<char *>(&r), sizeof(r));

                imageData.emplace_back(r, g, b);
            }
        }
    }

    void read32BitImageData(std::fstream &bmpFile) {
        bmpFile.seekg(bmfh->bfOfBits);
        std::uint8_t r, g, b, a;

        imageData.reserve(std::abs(bmih->biHeight) * bmih->biWidth);

        for (int i = 0; i < std::abs(bmih->biHeight); ++i) {
            for (int j = 0; j < bmih->biWidth; ++j) {
                bmpFile.read(reinterpret_cast<char *>(&b), sizeof(b));
                bmpFile.read(reinterpret_cast<char *>(&g), sizeof(g));
                bmpFile.read(reinterpret_cast<char *>(&r), sizeof(r));
                bmpFile.read(reinterpret_cast<char *>(&a), sizeof(a));

                imageData.emplace_back(r, g, b);
            }
        }
    }

    void readImageData(std::fstream &bmpFile) {
        if (bmih->biBitCount == 24) {
            read24BitImageData(bmpFile);
        } else if (bmih->biBitCount == 32) {
            read32BitImageData(bmpFile);
        }
    }

    void displayBMP() {
        if (!is_open()) {
            throw BMPDisplayNotOpenedImage();
        }

        char white = 'X';
        char black = '.';

        if (bmih->biHeight > 0) {
            std::string buf(bmih->biWidth, '0');
            for (size_t i = 1; i <= imageData.size(); ++i) {
                if (imageData[imageData.size() - i].is_white()) {
                    buf[(imageData.size() - i) % bmih->biWidth] = white;
                } else {
                    buf[(imageData.size() - i) % bmih->biWidth] = black;
                }
                if (i % bmih->biWidth == 0) {
                    std::cout << buf;
                    std::cout << "\n";
                }
            }
        } else {
            for (size_t i = 0; i < imageData.size(); ++i) {
                if (i % bmih->biWidth == 0 && i > 0) {
                    std::cout << "\n";
                }
                if (imageData[i].is_white()) {
                    std::cout << white;
                } else {
                    std::cout << black;
                }
            }
        }
    }

    void closeBMP() {
        imageData.clear();
        imageData.shrink_to_fit();
        bmih = nullptr;
        bmfh = nullptr;
    }
};

BMPImage::BMPImage() : pImpl(std::make_unique<Impl>()) 
{

}

BMPImage::~BMPImage()
{ 
}

void BMPImage::openBMP(const std::string &fileName) {
    pImpl->openBMP(fileName);
}

void BMPImage::displayBMP() 
{ 
    pImpl->displayBMP();
}

void BMPImage::closeBMP() 
{ 
    pImpl->closeBMP();
}

}  // namespace BMP
