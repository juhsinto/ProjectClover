#ifndef __GRID_NUMBER_OF_COLS_EXCEPTION_H__
#define __GRID_NUMBER_OF_COLS_EXCEPTION_H__


class GridNumberOfColsException
{
private:
    int cols;

public:
    GridNumberOfColsException(int input_cols) : cols(input_cols) {}
    ~GridNumberOfColsException() {}

    int getNumberOfCols() {
        return cols;
    }
};

#endif // __GRID_NUMBER_OF_COLS_EXCEPTION_H__
