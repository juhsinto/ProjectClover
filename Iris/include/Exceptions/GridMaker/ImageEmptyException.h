#ifndef __EMPTY_IMAGE_EXCEPTION_H__
#define __EMPTY_IMAGE_EXCEPTION_H__


class ImageEmptyException
{
private:
    std::string image;

public:
    ImageEmptyException(std::string input_image) : image(input_image) {}
    ~ImageEmptyException() {}

    std::string getImageName() {
        return image;
    }
};

#endif // __EMPTY_IMAGE_EXCEPTION_H__
