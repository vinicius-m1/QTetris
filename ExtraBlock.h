#ifndef EXTRABLOCK_H
#define EXTRABLOCK_H
#include "grid.h"
#include <QGraphicsRectItem>
#include <QObject>
#include <QTimer>
#include <stdlib.h>
#include <QGraphicsScene>

class ExtraBlock : public QObject, public QGraphicsRectItem{
    Q_OBJECT //needed to handle slots
public:
    QTimer * m_timer;
    bool falling = true; //spawns falling --  now in base class

    std::pair<int,int> virtual_position;
    int movX,movY;
    int limiter = 550;
    bool exist = true;
    bool piece_mode = false;
    Grid * m_grid;
    ExtraBlock(Grid * grid, bool t_piece_mode=false);

public slots:
    void move();
    //void duplication();


};




#endif // EXTRABLOCK_H
