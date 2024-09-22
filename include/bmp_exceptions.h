#pragma once
#include <exception>

namespace BMP {

class BMPException : public std::exception {
   public:
    const char *what() const noexcept override { return "BMP Exception."; }
};

class BMPOpenError : public BMPException {
   public:
    const char *what() const noexcept override { return "Cant open image."; }
};

class BMPDisplayNotOpenedImage : public BMPException {
   public:
    const char *what() const noexcept override {
        return "Not opened image cant be displayed.";
    }
};

class BMPUnsupportedFormat : public BMPException {
   public:
    const char *what() const noexcept override {
        return "Unsupported BMP format.";
    }
};

}  // namespace BMP
