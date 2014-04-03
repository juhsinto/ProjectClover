#ifndef __IMAGE_DIMENSION_INVALID_EXCEPTION_H__
#define __IMAGE_DIMENSION_INVALID_EXCEPTION_H__


class ImageDimensionInvalidException
{
private:
    std::string image;

public:
    ImageDimensionInvalidException(std::string input_image) : image(input_image) {}
    ~ImageDimensionInvalidException() {}

    std::string getImageName() {
        return image;
    }
};

#endif // __IMAGE_DIMENSION_INVALID_EXCEPTION_H__
