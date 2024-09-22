#pragma once
#include <memory>
#include <string>

#include "bmp_exceptions.h"

namespace BMP {

class BMPImage {
  public:
    BMPImage();
    ~BMPImage();

    void openBMP(const std::string &fileName);
    void displayBMP();
    void closeBMP();

  private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

}  // namespace BMP
