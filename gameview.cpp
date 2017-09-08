#include "const.h"
#include "gameview.h"

#include <QDebug>

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent), bgPix(":/pic/board"), way(nullptr)
{
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-SCENE_R, -SCENE_R, 2*SCENE_R, 2*SCENE_R);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(1, 1);
    setMinimumSize(2*SCENE_R, 2*SCENE_R);

    //add pieces...
    state = new GameState;
    for(int i = 0; i < 100; ++i) {
        if(state->board[i] == null) continue;
        Piece *piece = new Piece(this, state->board[i]);
        scene->addItem(piece);
        piece->setPos((i%10)*CELL_R*2 + CELL_R - SCENE_R,
                      (i/10)*CELL_R*2 + CELL_R - SCENE_R);
    }

    //game start...
    movingColor = white; //set to white and call turn(), so the first one is black.
    turn();

    /*/for debug..
    way = state->findWays(black);
    for(wayNode* nd: way->child) {
        qreal x = nd->pos.x()*CELL_R*2 + CELL_R - SCENE_R;
        qreal y = nd->pos.y()*CELL_R*2 + CELL_R - SCENE_R;
        auto* item = scene->itemAt(x, y, transform());
        if(qgraphicsitem_cast<Piece*>(item))
            qgraphicsitem_cast<Piece*>(item)->setTakable(true);
    }*/
}

void GameView::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();
    QRectF bgRect(0.0, 0.0, bgPix.width(), bgPix.height());
    painter->drawPixmap(sceneRect, bgPix, bgRect);

}

void GameView::tempo(Piece* p, QPoint from, QPoint to) {

    wayNode* ndFrom = nullptr;
    wayNode* ndTo   = nullptr;
    QPointF finalPos;

    //检验合法性...
    for(wayNode* nd: way->child)
        if(nd->pos == from) { ndFrom = nd; break; }
    if(ndFrom != nullptr) {
        for(wayNode* nd: ndFrom->child)
            if(nd->pos == to) { ndTo = nd; break; }
        if( ndTo != nullptr ) {
            //合法性判定结束！开始移动...
            QPoint eat = state->move(from, to);
            if(eat.x() != -1 && eat.y() != -1) { //吃子
                QPointF eatPos(eat.x()*CELL_R*2 + CELL_R - SCENE_R,
                               eat.y()*CELL_R*2 + CELL_R - SCENE_R);
                auto* item = scene->itemAt(eatPos, transform());
                if(qgraphicsitem_cast<Piece*>(item)) scene->removeItem(item);
            }
            finalPos.rx() = to.x()*CELL_R*2 + CELL_R - SCENE_R;
            finalPos.ry() = to.y()*CELL_R*2 + CELL_R - SCENE_R;
            way = ndFrom;
            p->setPos(finalPos);
            if(ndTo->child.isEmpty()) {
                turn();
            }
        }
        else { //illegal, go back.
            finalPos.rx() = from.x()*CELL_R*2 + CELL_R - SCENE_R;
            finalPos.ry() = from.y()*CELL_R*2 + CELL_R - SCENE_R;
            p->setPos(finalPos);
        }
    }
    else { //shoudn't happen in the final version.
        finalPos.rx() = from.x()*CELL_R*2 + CELL_R - SCENE_R;
        finalPos.ry() = from.y()*CELL_R*2 + CELL_R - SCENE_R;
        p->setPos(finalPos);
    }

    return;
}

void GameView::turn() {
    movingColor = (movingColor == white ? black : white);
    qDebug() << (movingColor == black ? "Black" : "White") << "Turn! planting tree...";
    if(way) delete way;
    way = state->findWays(movingColor);
    if(way->child.isEmpty()) {
        //ofColor lose....
        qDebug() << "Draughts:" << (movingColor == black ? "Black" : "White") << "lose!!!!!!!";
        return;
    }
    for(wayNode* nd: way->child) {
        qreal x = nd->pos.x()*CELL_R*2 + CELL_R - SCENE_R;
        qreal y = nd->pos.y()*CELL_R*2 + CELL_R - SCENE_R;
        auto* item = scene->itemAt(x, y, transform());
        if(qgraphicsitem_cast<Piece*>(item))
            qgraphicsitem_cast<Piece*>(item)->setTakable(true);
    }
    qDebug() << "Tree planted!" << (movingColor == black ? "Black" : "White") << "please.";
    update();
}

