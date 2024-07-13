#ifndef LPIECE_H
#define LPIECE_H

#include "PieceBase.h"
#include "grid.h"
#include "ExtraBlock.h"
#include <QTimer>
#include<QImage>

class LPiece : public QObject, public PieceBase {
    Q_OBJECT //needed to handle slots
public:
    Grid * m_grid;
    LPiece(Grid * grid);

    std::vector<ExtraBlock*> *blocks_in_scene;

    void SetBlocksInScene(std::vector<ExtraBlock*> *t_blocks_in_scene){
        blocks_in_scene = t_blocks_in_scene;
    }


    // members of this group
    ExtraBlock *block1;
    ExtraBlock *block2;
    ExtraBlock *block3;
    ExtraBlock *block4;
    ExtraBlock* GetBlock(int id);
    // -------------------

    // formation stuff
    //int number_of_formations = 2; now in contructor
    int number_of_blocks = 4;
    //int formation = 1;            now in contructor
    int x_correction=30, y_correction=30; //collision positions (values for default formation)

    void SetFormation(int t_formation); //change visually (in group)
    // ----------------------------


    // move stuff
    int speed = 25;
    QTimer * m_timer;
    int limiter = 550;
    int movX=0,movY=0;
    //bool falling = true; //spawns falling     now in base class
    //bool controls = true; //user starts controlling -- now in contructor
    void moveRight();
    void moveLeft();
    // -----------------------------


public slots:
    void move();

};

#endif // LPIECE_H
