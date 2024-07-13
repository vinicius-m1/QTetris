#include "invertedL.h"
#include<QImage>

InvertedLPiece::InvertedLPiece(Grid * grid)
{
    //changing values from parent class
    number_of_formations = 4;
    controls = true;
    formation = 1;


    m_grid = grid; // saves received grid to pass to blocks

    QBrush brush(QImage(":/images/green.png"));
    //QBrush test(QImage(":/images/yellow.png"));  // debug color
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
    block2->setPos(0,-30);
    addToGroup(block2);

    block3 = new ExtraBlock(grid,true);
    block3->setBrush(brush);
    block1->setPen(pen);
    block3->setPos(0,-60);
    addToGroup(block3);

    block4 = new ExtraBlock(grid,true);
    block4->setBrush(brush);
    block1->setPen(pen);
    block4->setPos(30,-60);
    addToGroup(block4);


    //          MOVE RELATED STUFF

    //conect slot to timer, when timer runs out the slot is executed
    QTimer * timer = new QTimer();
    m_timer = timer;
    connect(timer,SIGNAL(timeout()), this, SLOT(move()) );

    timer->start(500);//short waiting after spawn

    // -------------------------------------------------------------
}

ExtraBlock *InvertedLPiece::GetBlock(int id)
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


void InvertedLPiece::SetFormation(int t_formation)
{
    bool able_to_change = true; // not implemented yet in this piece
        // no need to check if blocks still exist, you shouldn't have control at this point anyway

    if (t_formation == 1 ){
        if(able_to_change){
            formation = t_formation;
            block1->setPos(0,0);
            block2->setPos(0,-30);
            block3->setPos(0,-60);
            block4->setPos(30,-60);
        } else return;
    }

    else if(t_formation == 2){
        if(able_to_change){
            formation = t_formation;
            block1->setPos(0,0);
            block2->setPos(30,0);
            block3->setPos(60,0);
            block4->setPos(60,30);
        }else return;
    }

    else if(t_formation == 3){
        if(able_to_change){
            formation = t_formation;
            block1->setPos(0,0);
            block2->setPos(0,-30);
            block3->setPos(0,-60);
            block4->setPos(-30,0);
        }else return;
    }

    else if(t_formation == 4){
        if(able_to_change){
            formation = t_formation;
            block1->setPos(0,0);
            block2->setPos(30,0);
            block3->setPos(60,0);
            block4->setPos(0,-30);
        }else return;
    }

}

void InvertedLPiece::moveRight()
{
    int correction = 30; // formation 1 default
    if (formation ==2 || formation==4)
        correction=60;
    if(formation==3)
        correction =0;


    if (x()+10+(correction) > 520)//verify boundaries
        return;

    setPos(x()+30,y());
}

void InvertedLPiece::moveLeft()
{
    int correction = 0; // formation 2,1,4 default
    if (formation ==3)
        correction=30;


    if (x()-10-correction < 250)//verify boundaries
        return;

    setPos(x()-30,y());
}


void InvertedLPiece::move()
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
            if (block1 && block1->exist && block4 && block4->exist){ // weird, but no seg fault bcs nullptr checked first
                if(formation == 1){
                    // block1 takes the fall for block2
                    m_grid->RemoveOccupied(x(),y());
                    block1->exist = false;
                    blocks_in_scene->erase(std::remove(blocks_in_scene->begin(), blocks_in_scene->end(), block1), blocks_in_scene->end());
                    delete block1;
                    block1 = nullptr;

                    // add 2 back in block in scene, set it to exist, set it to color
                    block2->exist=true;
                    QBrush brush(QImage(":/images/green.png"));
                    block2->setBrush(brush);
                    blocks_in_scene->push_back(block2);
                }
                else if (formation == 3){
                    // block 3 takes the fall for block 2
                    m_grid->RemoveOccupied(x(),y()-60);
                    blocks_in_scene->erase(std::remove(blocks_in_scene->begin(), blocks_in_scene->end(), block3), blocks_in_scene->end());
                    delete block3;
                    block3 = nullptr;

                    // add 2 back in block in scene, set it to exist, set it to color
                    block2->exist=true;
                    QBrush brush(QImage(":/images/green.png"));
                    block2->setBrush(brush);
                    blocks_in_scene->push_back(block2);
                } else{
                    //or just do normal
                    delete block2;
                    block2 = nullptr;
                }
            } else {
                //or just do normal
                delete block2;
                block2 = nullptr;
            }
        }
    }
    if (block3){
        if(block3->exist){ //if block exist
            exist =true;
        } else{//or just do normal
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

    if (formation ==1){
        if (block1)
            block1->virtual_position = {x(),y()};
        if (block2)
            block2->virtual_position = {x(),y()-30};
        if (block3)
            block3->virtual_position = {x(),y()-60};
        if (block4)
            block4->virtual_position = {x()+30,y()-60};
    }
    if (formation ==2){
        if (block1)
            block1->virtual_position = {x(),y()};
        if (block2)
            block2->virtual_position = {x()+30,y()};
        if (block3)
            block3->virtual_position = {x()+60,y()};
        if (block4)
            block4->virtual_position = {x()+60,y()+30};
    }
    if (formation ==3){
        if (block1)
            block1->virtual_position = {x(),y()};
        if (block2)
            block2->virtual_position = {x(),y()-30};
        if (block3)
            block3->virtual_position = {x(),y()-60};
        if (block4)
            block4->virtual_position = {x()-30,y()};
    }
    if (formation ==4){
        if (block1)
            block1->virtual_position = {x(),y()};
        if (block2)
            block2->virtual_position = {x()+30,y()};
        if (block3)
            block3->virtual_position = {x()+60,y()};
        if (block4)
            block4->virtual_position = {x(),y()-30};
    }


    // ------------------------------------------------------------------------------------



    //--- -----------------------------// -----------------------------// -------------------------------------
    // ----------------------------- CHECK IF COLLISION WITH OCCUPIED GRID SLOT (AND LIMIT) -------------------------------
    //--- -----------------------------// -----------------------------// -------------------------------------

    bool able_to_move = true;

    if (formation == 1){
        // check 4 || 3 , check 2, check 1

        if (block4 && block3 && !block1 && !block2){
            able_to_move = !( m_grid->IsOccupied(x()+30,destination-60) || m_grid->IsOccupied(x(),destination-60) || (y() - 30 >= limiter));
            //qDebug()<<"first running";
        }
        else if (block2 && block4 && !block1){
            //qDebug()<<"second running";
            able_to_move = !( m_grid->IsOccupied(x()+30,destination-60) || m_grid->IsOccupied(x(),destination-30) || (y()>= limiter));
        }
        else if (block1 && block4){
            //qDebug()<<"third running";
            able_to_move = !( m_grid->IsOccupied(x()+30,destination-60) || m_grid->IsOccupied(x(),destination) || (y() + 30 >= limiter));
        }
        else if (block1 && !block4){
            //qDebug()<<"forth running";
            able_to_move = !(m_grid->IsOccupied(x(),destination) || (y() + 30 >= limiter));
        }
        else if (block2){
            able_to_move = !(m_grid->IsOccupied(x(),destination-30) || (y()>= limiter));
            //qDebug()<<"fifth running";
        }
    }

    if (formation == 2){
        if (!block4)
            able_to_move = !( m_grid->IsOccupied(x(),destination) ||  m_grid->IsOccupied(x()+30,destination) || m_grid->IsOccupied(x()+60,destination) || (y() + 60 >= limiter) );
        if (!block1 && !block2 && !block3 && block4)
            able_to_move = !(m_grid->IsOccupied(x()+60,destination+30) || (y() +60>= limiter));
        if (block1 && block4)
            able_to_move = !(m_grid->IsOccupied(x()+60,destination+30) || m_grid->IsOccupied(x()+30,destination) || m_grid->IsOccupied(x(),destination) || (y() +60>= limiter));
    };

    if (formation == 3){

        if (block1 && block4 && !block3 && !block2){
            able_to_move = !( m_grid->IsOccupied(x(),destination) || m_grid->IsOccupied(x()-30,destination) || (y() + 30 >= limiter));
            //qDebug()<<"first running";
        }
        else if (block2 && block3 && !block1){
            //qDebug()<<"second running";
            able_to_move = !(m_grid->IsOccupied(x(),destination-30) || (y()>= limiter));
        }
        else if(block1 && block2 && block3 && block4){
            able_to_move = !( m_grid->IsOccupied(x(),destination) || m_grid->IsOccupied(x()-30,destination) || (y() + 30 >= limiter));
            //qDebug()<<"third running";
        }
        else if (block3 && !block1 && !block2){
            //qDebug()<<"forth running";
            able_to_move = !(m_grid->IsOccupied(x(),destination-60) || (y() - 30 >= limiter));
        }
        else if (block2 && !block3 && !block1){
            able_to_move = !(m_grid->IsOccupied(x(),destination-30) || (y()>= limiter));
            //qDebug()<<"fifth running";
        }
        else if(!block3 && block1)
            able_to_move = !( m_grid->IsOccupied(x(),destination) || m_grid->IsOccupied(x()-30,destination) || (y() + 30 >= limiter));
    };

    if (formation == 4){
        if (block1 && block2 && block3 &&  !block4)
            able_to_move = !( m_grid->IsOccupied(x(),destination) ||  m_grid->IsOccupied(x()+30,destination) || m_grid->IsOccupied(x()+60,destination) || (y()+30>= limiter) );
        else if (!block1 && block4)
            able_to_move = !(m_grid->IsOccupied(x(),destination-30) || (y()>= limiter));
        else // if all exist
            able_to_move = !(m_grid->IsOccupied(x(),destination) || m_grid->IsOccupied(x()+60,destination) || m_grid->IsOccupied(x()+30,destination) || (y()+30>= limiter));
    };

    if (able_to_move == false){
        m_timer->stop();
        // decrease time to reduce performance hogging
        m_timer->start(200);

        controls = false; //player loses control

        falling = false;

        if (block1)
            m_grid->SetOccupied( block1->virtual_position.first,block1->virtual_position.second );  //block 1
        if (block2)
            m_grid->SetOccupied(block2->virtual_position.first,block2->virtual_position.second);  //block 2
        if (block3)
            m_grid->SetOccupied(block3->virtual_position.first,block3->virtual_position.second);  //block 3
        if (block4)
            m_grid->SetOccupied(block4->virtual_position.first,block4->virtual_position.second);  //block 4

        //qDebug() << "clock stopped bcs occupied. sqr not move";

        return;
    }

    //--- -----------------------------// -----------------------------// -------------------------------------
    //--- -----------------------------// -----------------------------// -------------------------------------


    //--------------------------------------------------------------------------------------
    // IF BELLOW IS FREE
    //--------------------------------------------------------------------------------------

    if (falling == false){ //if was stopped and now moving, remove old "seat"
        if (block1)
            m_grid->RemoveOccupied(block1->virtual_position.first,block1->virtual_position.second); //try using virtual positions
        if (block2)
            m_grid->RemoveOccupied(block2->virtual_position.first,block2->virtual_position.second);
        if (block3)
            m_grid->RemoveOccupied(block3->virtual_position.first,block3->virtual_position.second);
        if (block4)
            m_grid->RemoveOccupied(block4->virtual_position.first,block4->virtual_position.second);
    }

    falling = true;
    m_timer->stop(); //getting timer back to speed
    m_timer->start(speed);


    setPos(x()+movX,y()-movY);

};

