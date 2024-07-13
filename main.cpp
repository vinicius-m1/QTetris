#include <QApplication>
#include <QGraphicsScene>
#include "Tetris.h"
#include <QGraphicsView>
#include <QImage>
#include <QPushButton>

int TetrisGame(QApplication *a);
void Menu(int argc, char *argv[]);


int main(int argc, char *argv[])
{
    Menu(argc, argv);
    //Tetris(argc, argv);

}

void Menu(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Create a scene
    QGraphicsScene scene;

    QGraphicsView view(&scene);
    view.show();
    view.setFixedSize(400,600);
    scene.setSceneRect(0,0,400,600);
    scene.setBackgroundBrush(QBrush(QImage(":/images/background_menu.png")));

    // hide scrollbar
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Create the play button
    QPushButton* playButton = new QPushButton(QString("Play"));
    //playButton->setStyleSheet("background-color: blue; color: white;");
    //playButton->setVisible(false);
    playButton->setStyleSheet("background-color: transparent; border: none; color: rgba(0,0,0,0);");
    int bxPos = 150;
    int byPos = 320;
    playButton->setGeometry(bxPos, byPos, 170,40);

    QObject::connect(playButton, &QPushButton::clicked, &a, [&](){
        //pressed play
        view.hide();
        TetrisGame(&a);
    });
    scene.addWidget(playButton);

    //quit button
    QPushButton* quitButton = new QPushButton(QString("Quit"));
    quitButton->setStyleSheet("background-color: transparent; border: none; color: rgba(0,0,0,0);");
    int qxPos = 220;
    int qyPos = 370;
    quitButton->setGeometry(qxPos, qyPos, 170, 40);
    QObject::connect(quitButton, &QPushButton::clicked, &a, &QApplication::quit);
    scene.addWidget(quitButton);

    a.exec();
}

int TetrisGame(QApplication *a){

    //QApplication a(argc, argv);
    // create a scene
    QGraphicsScene * scene = new QGraphicsScene();

    // create spawner
    Tetris * rect = new Tetris();
    rect->setRect(0,0,30,10);
    QPen pen(Qt::NoPen); // remove ugly border
    rect->setPen(pen);

    // add the item to the scene
    scene->addItem(rect);
    //make rect focusable
    rect->setFlag(QGraphicsItem::ItemIsFocusable);
    rect->setFocus();


    // add a view to visualize the scene
    QGraphicsView * view = new QGraphicsView(scene);

    // hide scrollbar
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->show();

    //definir tamanho da janela
    view->setFixedSize(800,600);

    //setando localizacao(na janela) e tamanho da cena
    scene->setSceneRect(0,0,800,600);

    rect->setPos(view->width()/2, view->height() - rect->rect().height());
    rect->setPos(370,65); //top middle
    scene->setBackgroundBrush(QBrush(QImage(":/images/Untitled.png")));


    // setting score panel
    rect->score = new QGraphicsTextItem;
    scene->addItem(rect->score);
    rect->score->setDefaultTextColor(Qt::white);
    QFont font("Helvetica", 16);
    font.setBold(true);
    rect->score->setFont(font);
    rect->score->setPos(700,111);


    // setting score panel
    rect->stats = new QGraphicsTextItem;
    scene->addItem(rect->stats);
    rect->stats->setDefaultTextColor(Qt::white);
    QFont font2("Helvetica", 12);
    font2.setBold(true);
    rect->stats->setFont(font2);
    rect->stats->setPos(20,150);

    // setting next piece picture frame
    rect->next_piece_pic = new QGraphicsRectItem;
    rect->next_piece_pic->setRect(690,145,60,60);
    rect->next_piece_pic->setPen(pen);

    scene->addItem(rect->next_piece_pic);
    return 0;
};

