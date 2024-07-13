#include "line.h"
#include<QImage>

LinePiece::LinePiece(Grid * grid)
{

    //changing values from parent class
    number_of_formations = 2;
    controls = true;
    formation = 1;


    m_grid = grid; // saves received grid to pass to blocks

    QBrush brush(QImage(":/images/red.png"));
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
    block3->setPos(60,0);
    addToGroup(block3);

    block4 = new ExtraBlock(grid,true);
    block4->setBrush(brush);
    block1->setPen(pen);
    block4->setPos(90,0);
    addToGroup(block4);


    //          MOVE RELATED STUFF

    //conect slot to timer, when timer runs out the slot is executed
    QTimer * timer = new QTimer();
    m_timer = timer;
    connect(timer,SIGNAL(timeout()), this, SLOT(move()) );

    timer->start(500); //short waiting after spawn

    // -------------------------------------------------------------
}

ExtraBlock *LinePiece::GetBlock(int id)
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


void LinePiece::SetFormation(int t_formation)
{
    bool able_to_change = true;

    // change piece formation visually in the group
    // 1 - default (horizontal line)
    // 2 - vertical line

    if (t_formation == 1){

        //if(m_grid->IsOccupied(x(),y()))
        //    able_to_change = false;       //better when every move ends in a valid slot
        if(able_to_change && (m_grid->IsOccupied(x()+30,y())))
            able_to_change = false;
        if(able_to_change && (m_grid->IsOccupied(x()+60,y())))
            able_to_change = false;
        if(able_to_change && (m_grid->IsOccupied(x()+90,y())))
            able_to_change = false;


        // no need to check if blocks still exist, you shouldn't have control at this point anyway

        //remove old positions from occupied vector (uses old coord correction, so should be fine)
        if(able_to_change){

            formation = t_formation;

            m_grid->RemoveOccupied(x(),y()); //block 1 does not move
            block1->setPos(0,0);

            m_grid->RemoveOccupied(x()+x_correction,y()+y_correction); //old pos
            block2->setPos(30,0);

            m_grid->RemoveOccupied(x()+(x_correction*2),y()+(y_correction*2));//old pos
            block3->setPos(60,0);

            m_grid->RemoveOccupied(x()+(x_correction*3),y()+(y_correction*3));//old pos
            block4->setPos(90,0);

            // change piece formation on collisions
            y_correction = 0;
            x_correction = 30;

        } else return;

    }

    else if(t_formation == 2){

        if(able_to_change && (m_grid->IsOccupied(x(),y()+30)))
            able_to_change = false;
        if(able_to_change && (m_grid->IsOccupied(x(),y()+60)))
            able_to_change = false;
        if(able_to_change && (m_grid->IsOccupied(x(),y()+90)))
            able_to_change = false;

        //move pieces on occupied vector
        if(able_to_change){

            formation = t_formation;

            m_grid->RemoveOccupied(x(),y()); //block 1 does not move
            block1->setPos(0,0);

            m_grid->RemoveOccupied(x()+x_correction,y()+y_correction);
            block2->setPos(0,30);

            m_grid->RemoveOccupied(x()+(x_correction*2),y()+(y_correction*2));
            block3->setPos(0,60);

            m_grid->RemoveOccupied(x()+(x_correction*3),y()+(y_correction*3));
            block4->setPos(0,90);

            // change piece formation on collisions
            x_correction = 0;
            y_correction = 30;
        }else return;
    }

}

void LinePiece::moveRight()
{
    // correction already accounts for formation
    if (x()+10+(x_correction*3) > 520)//verify boundaries
        return;

    setPos(x()+30,y());
}

void LinePiece::moveLeft()
{

    // correction already accounts for formation
    if (x()-10 < 250)//verify boundaries
        return;

    setPos(x()-30,y());
}


void LinePiece::move()
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
            // THIS CODE IS TO FIX PROBLEM WHEN BLOCK IN THE MIDDLE OF PIECE IS DELETED.
            if((block1) && (formation == 2)){

                //undo what my rect did
                block2->exist=true;
                QBrush brush(QImage(":/images/red.png"));
                block2->setBrush(brush);
                blocks_in_scene->push_back(block2);

                //block 1 takes the fall for block2
                m_grid->RemoveOccupied(x(),y());
                // remove from blocks_in_scene
                blocks_in_scene->erase(std::remove(blocks_in_scene->begin(), blocks_in_scene->end(), block1), blocks_in_scene->end());
                block1->exist=false; //remove
                delete block1;
                block1 = nullptr;

            } else{ //or just do normal
                delete block2;
                block2 = nullptr;
            }
        }
    }
    if (block3){
        if(block3->exist){ //if block exist
            exist =true;
        } else{
            // THIS CODE IS TRY TO FIX PROBLEM WHEN BLOCK IN THE MIDDLE OF PIECE IS DELETED.
            if ((block2 || block1) && (formation == 2)){
                //undo what my rect did
                block3->exist=true;
                QBrush brush(QImage(":/images/red.png"));
                block3->setBrush(brush);
                blocks_in_scene->push_back(block3);

                if(block1){
                //block 1 takes the fall for block3
                m_grid->RemoveOccupied(x(),y());
                block1->exist=false; //remove
                QBrush brush(QImage(":/images/red.png"));
                block2->setBrush(brush);
                blocks_in_scene->erase(std::remove(blocks_in_scene->begin(), blocks_in_scene->end(), block1), blocks_in_scene->end());
                delete block1;
                block1 = nullptr;
                } else{ //if block 1 doennt exist
                //block2 takes the fall for block3
                m_grid->RemoveOccupied(x(),y()+y_correction);
                blocks_in_scene->erase(std::remove(blocks_in_scene->begin(), blocks_in_scene->end(), block2), blocks_in_scene->end());
                block2->exist=false; //remove
                delete block2;
                block2 = nullptr;
                }


            } else{//or just do normal
                delete block3;
                block3 = nullptr;
            }
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
        block2->virtual_position.second = (y()+y_correction);
    if (block3)
        block3->virtual_position.second = (y()+(y_correction*2));
    if (block4)
        block4->virtual_position.second = (y()+(y_correction*3));
    // ------------------------------------------------------------------------------------



    //--- -----------------------------// -----------------------------// -------------------------------------
    // ----------------------------- CHECK IF COLLISION WITH OCCUPIED GRID SLOT (AND LIMIT) -------------------------------
    //--- -----------------------------// -----------------------------// -------------------------------------

    bool able_to_move = true;

    if (formation == 1){

        // all need to be free
        if (block1)
            able_to_move = (!(m_grid->IsOccupied(x(),destination) || (y()+30>=limiter)));
        if (block2 && able_to_move)
            able_to_move = (!(m_grid->IsOccupied(x()+x_correction,destination+y_correction) || ((y()+(y_correction+30))>=limiter) ));
        if (block3 && able_to_move)
            able_to_move = (!(m_grid->IsOccupied(x()+(x_correction*2),destination+(y_correction*2)) || ((y()+(30+y_correction*2))>=limiter) ));
        if (block4 && able_to_move)
            able_to_move = (!(m_grid->IsOccupied(x()+(x_correction*3),destination+(y_correction*3)) || ((y()+(30+y_correction*4))>=limiter)) );
    }

    if (formation == 2){
        // in the vertical formation, only one need to be free, bcs the others are above something
        // checks from bottom to top (in vert its the most important)
        // [!]problem if middle of the line is cut

        if (block4 && able_to_move)
            able_to_move = (!(m_grid->IsOccupied(x()+(x_correction*3),destination+(y_correction*3)) || ((y()+(30+y_correction*3))>=limiter)));
        else if ((able_to_move) && block3)
            able_to_move = (!(m_grid->IsOccupied(x()+(x_correction*2),destination+(y_correction*2)) || ((y()+(30+y_correction*2))>=limiter)));
        else if (able_to_move && block2)
            able_to_move = (!(m_grid->IsOccupied(x()+x_correction,destination+y_correction) || ((y()+(30+y_correction))>=limiter)));
        else if (able_to_move && block1)
            able_to_move = (!(m_grid->IsOccupied(x(),destination) || (y()+30>=limiter)));
    };

    if (able_to_move == false){
        m_timer->stop();
        // decrease time to reduce performance hogging
        m_timer->start(200);

        controls = false; //player loses control

        falling = false;
        if (block1)
            m_grid->SetOccupied(x(),y());  //block 1
        if (block2)
            m_grid->SetOccupied(x()+x_correction,y()+y_correction);  //block 2
        if (block3)
            m_grid->SetOccupied(x()+(x_correction*2),y()+(y_correction*2));  //block 3
        if (block4)
            m_grid->SetOccupied(x()+(x_correction*3),y()+(y_correction*3));  //block 4

        //qDebug() << "clock stopped bcs occupied. line not move";

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
            m_grid->RemoveOccupied(x()+x_correction,y()+y_correction);
        if (block3)
            m_grid->RemoveOccupied(x()+x_correction+x_correction,y()+y_correction+y_correction);
        if (block4)
            m_grid->RemoveOccupied(x()+x_correction+x_correction+x_correction,y()+y_correction+y_correction+y_correction);
    }

    falling = true;
    m_timer->stop(); //getting timer back to speed
    m_timer->start(speed);


    setPos(x()+movX,y()-movY);

};

