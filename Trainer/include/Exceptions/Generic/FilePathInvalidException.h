#ifndef __FILE_PATH_INVALID_EXCEPTION_H__
#define __FILE_PATH_INVALID_EXCEPTION_H__


class FilePathInvalidException
{
private:
    std::string file_path;

public:
    FilePathInvalidException(std::string input_path) : file_path(input_path) {}
    ~FilePathInvalidException() {}

    std::string getPath() {
        return file_path;
    }

};

#endif // __FILE_PATH_INVALID_EXCEPTION_H__
