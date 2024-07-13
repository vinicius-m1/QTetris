#include "cube.h"
#include<QImage>
#include<QObject>
CubePiece::CubePiece(Grid * grid)
{

    //changing values from parent class
    number_of_formations = 1;
    controls = true;
    formation = 1;


    m_grid = grid; // saves received grid to pass to blocks

    QBrush brush(QImage(":/images/blue.png"));
    //QBrush test(QImage(":/images/green.png"));  // debug color
    QPen pen(Qt::NoPen); // remove ugly border

    // create individual blocks and add them to the group
    block1 = new ExtraBlock(grid,true); // piece_mode = true
    block1->setBrush(brush);
    block1->setPen(pen);
    block1->setPos(0, 0); // (position based on group)
    addToGroup(block1);

    block2 = new ExtraBlock(grid,true);
    block2->setBrush(brush);
    block1->setPen(pen);
    block2->setPos(30,0);
    addToGroup(block2);

    block3 = new ExtraBlock(grid,true);
    block3->setBrush(brush);
    block1->setPen(pen);
    block3->setPos(0,30);
    addToGroup(block3);

    block4 = new ExtraBlock(grid,true);
    block4->setBrush(brush);
    block1->setPen(pen);
    block4->setPos(30,30);
    addToGroup(block4);


    //          MOVE RELATED STUFF

    //conect slot to timer, when timer runs out the slot is executed
    QTimer * timer = new QTimer();
    m_timer = timer;
    connect(timer,SIGNAL(timeout()), this, SLOT(move()) );

    timer->start(500);//short waiting after spawn

    // -------------------------------------------------------------
}

ExtraBlock *CubePiece::GetBlock(int id)
{
    // handles blocks when Tetris asks
    switch(id){
    case(1):
        if (block1)
            return(block1);
        break;
    case(2):
        if (block2)
            return(block2);
        break;
    case(3):
        if (block3)
            return(block3);
        break;
    case(4):
        if (block4)
            return(block4);
        break;
    default:
        return(nullptr);
        break;
    }
    return(nullptr);
}


void CubePiece::SetFormation(int t_formation)
{
    return; //cube doesnt change
}

void CubePiece::moveRight()
{
    // correction already accounts for formation
    if (x()+10+(30) > 520)//verify boundaries
        return;

    setPos(x()+30,y());
}

void CubePiece::moveLeft()
{

    // correction already accounts for formation
    if (x()-10 < 250)//verify boundaries
        return;

    setPos(x()-30,y());
}


void CubePiece::move()
{

    //CHECK IF GROUP SHOULD STILL EXIST
    bool exist=false;

    if (block1){
        if(block1->exist){ //if block exist
            exist =true;
        } else{
            delete block1;
            block1 = nullptr;
        }
    }
    if (block2){
        if(block2->exist){ //if block exist
            exist =true;
        } else{
            delete block2;
            block2 = nullptr;
        }
    }
    if (block3){
        if(block3->exist){ //if block exist
            exist =true;
        } else{
            delete block3;
            block3 = nullptr;
        }
    }
    if (block4){
        if(block4->exist){ //if block exist
            exist =true;
        } else{
            delete block4;
            block4 = nullptr;
        }
    }

    if (!exist){
        m_timer->stop();
        //qDebug()<< "      the square stopped. group doenst exist anymore";
        return;
    }
    // -----------------------------------


    movY = -5;
    int destination = (y() - (movY-25)); //(25 is a constant offset)


    //update virtual positions
    //update virtual positions (for extrablocks inside group be treated as parts of scene(deletion))
    if (block1)
        block1->virtual_position.second = (y());
    if (block2)
        block2->virtual_position.second = (y());
    if (block3)
        block3->virtual_position.second = (y()+(y_correction));
    if (block4)
        block4->virtual_position.second = (y()+(y_correction));
    // ------------------------------------------------------------------------------------



    //--- -----------------------------// -----------------------------// -------------------------------------
    // ----------------------------- CHECK IF COLLISION WITH OCCUPIED GRID SLOT (AND LIMIT) -------------------------------
    //--- -----------------------------// -----------------------------// -------------------------------------

    bool able_to_move = true;
        // if (1 AND 2 = free OR 3 AND 4 = free)

    if (block1 && block2)
        able_to_move = (!((m_grid->IsOccupied(x(),destination) || (m_grid->IsOccupied(x()+x_correction,destination))) || (y()+30>=limiter)));
    if (block3 && block4)
        able_to_move = (!((m_grid->IsOccupied(x(),destination+y_correction) || (m_grid->IsOccupied(x()+x_correction,destination+y_correction))) || (y()+30+y_correction>=limiter)));


    if (able_to_move == false){
        m_timer->stop();
        // decrease time to reduce performance hogging
        m_timer->start(200);

        controls = false; //player loses control

        falling = false;
        if (block1)
            m_grid->SetOccupied(x(),y());  //block 1
        if (block2)
            m_grid->SetOccupied(x()+x_correction,y());  //block 2
        if (block3)
            m_grid->SetOccupied(x(),y()+(y_correction));  //block 3
        if (block4)
            m_grid->SetOccupied(x()+(x_correction),y()+(y_correction));  //block 4

        //qDebug() << "clock stopped bcs occupied. square not move";

        return;
    }

    //--- -----------------------------// -----------------------------// -------------------------------------
    //--- -----------------------------// -----------------------------// -------------------------------------


    //--------------------------------------------------------------------------------------
    // IF BELLOW IS FREE
    //--------------------------------------------------------------------------------------

    if (falling == false){ //if was stopped and now moving, remove old "seat"
        if (block1)
            m_grid->RemoveOccupied(x(),y()); //try using virtual positions
        if (block2)
            m_grid->RemoveOccupied(x()+x_correction,y());
        if (block3)
            m_grid->RemoveOccupied(x(),y()+y_correction);
        if (block4)
            m_grid->RemoveOccupied(x()+x_correction,y()+y_correction);
    }

    falling = true;
    m_timer->stop(); //getting timer back to speed
    m_timer->start(speed);


    setPos(x()+movX,y()-movY);

};

