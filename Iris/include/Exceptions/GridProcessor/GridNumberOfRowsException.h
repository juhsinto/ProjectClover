#ifndef __GRID_NUMBER_OF_ROWS_EXCEPTION_H__
#define __GRID_NUMBER_OF_ROWS_EXCEPTION_H__


class GridNumberOfRowsException
{
private:
    int rows;

public:
    GridNumberOfRowsException(int input_rows) : rows(input_rows) {}
    ~GridNumberOfRowsException() {}

    int getNumberOfRows() {
        return rows;
    }
};

#endif // __GRID_NUMBER_OF_ROWS_EXCEPTION_H__
