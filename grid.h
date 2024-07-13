#ifndef GRID_H
#define GRID_H
//#include <sys/types.h>
#include <QObject>
#include <vector>

//#include "ExtraBlock.h"
//#include "Tetris.h"
#include "QDebug"

#include"algorithm"

class Grid : public QObject{

protected:



public:
    std::vector<std::pair<int,int>> occupied;
    std::vector<int> row_to_destroy; // from grid DestroyRow to Tetris.cpp direct access
    int row_horizontal_size = 10; // max quant of block in each row
    bool game_over =false;
    //std::vector<ExtraBlock::ExtraBlock*> blocks_in_scene; //public so Tetris.cpp can access

    bool IsOccupied(int x,int y);

    int SetOccupied(int x,int y);

    void DestroyRow(int y);

    void RemoveOccupied(int x,int y);

};



#endif // GRID_H
