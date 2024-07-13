#ifndef PIECEBASE_H
#define PIECEBASE_H

#include <QGraphicsItemGroup>

// this is the base class for all pieces groups
// so all can be in current_piece
// this should have all methods used by current piece as virtual
// variables too

class PieceBase : public QGraphicsItemGroup {

public:

    //change this values in contructors
    bool controls = true;
    int formation = 0;
    int number_of_formations = 0;
    bool falling = true;


    // to be overritten methods
    virtual void moveRight(){return;};

    virtual void moveLeft(){return;};

    virtual void SetFormation(int t_formation){return;};


};
#endif // PIECEBASE_H
