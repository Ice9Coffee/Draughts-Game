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

void GameView::turn() {
    movingColor = (movingColor == white ? black : white);
    qDebug() << (movingColor == black ? "Black" : "White") << "Turn! planting tree...";
    if(way) delete way;
    way = state->findWays(movingColor);

    if(way->child.isEmpty()) {
        //TODO: ofColor lose....
        qDebug() << "Draughts:"
                 << (movingColor == black ? "Black" : "White") << "lose!!!!!!!";
        return;
    }

    for(wayNode* nd: way->child) hlPiece(nd); //VFX.

    qDebug() << "Tree planted!"
             << (movingColor == black ? "Black" : "White") << "please.";
    update();
}

void GameView::tempo(Piece* p, const QPoint from, const QPoint to) {

    wayNode* ndFrom = nullptr;
    wayNode* ndTo   = nullptr;
    QPointF finalPos;

    hdWayHL(); //VFX.  已松开鼠标.

    //检验合法性...
    for(wayNode* nd: way->child) if(nd->pos == from) { ndFrom = nd; break; }
    if(ndFrom != nullptr) {
        for(wayNode* nd: ndFrom->child) if(nd->pos == to) { ndTo = nd; break; }
        if( ndTo != nullptr ) {

            //合法性判定结束！执行一着棋
            //TODO: 发信...
            //吃子.
            QPoint eat = state->move(from, to);
            if(eat.x() != -1 && eat.y() != -1) {
                QPointF eatPos(eat.x()*CELL_R*2 + CELL_R - SCENE_R,
                               eat.y()*CELL_R*2 + CELL_R - SCENE_R);
                auto* item = scene->itemAt(eatPos, transform());
                if(qgraphicsitem_cast<Piece*>(item)) scene->removeItem(item);
            }
            //移动.
            finalPos.rx() = to.x()*CELL_R*2 + CELL_R - SCENE_R;
            finalPos.ry() = to.y()*CELL_R*2 + CELL_R - SCENE_R;
            p->setPos(finalPos);
            way = ndFrom;
            //到底时，回合结束
            if(ndTo->child.isEmpty()) {
                //TODO: 升变...
                int idx = 10*to.y() + to.x();
                Draughts piece = state->board[idx];
                if((to.y() == 0 && ((piece & color) == black))
                    || (to.y() == 9 && ((piece & color) == white)))
                {
                    state->board[idx] |= king;
                    p->drt |= king;
                }

                turn();
            }
            else { //VFX.
                hlPiece(ndTo);
                hlWay(ndTo);
            }
        }
        else { //illegal, go back.
            finalPos.rx() = from.x()*CELL_R*2 + CELL_R - SCENE_R;
            finalPos.ry() = from.y()*CELL_R*2 + CELL_R - SCENE_R;
            p->setPos(finalPos);
            //VFX.
            if(way->pos.x() == -1 && way->pos.y() == -1)
                for(wayNode* nd: way->child) hlPiece(nd);
            else hlPiece(ndFrom);
        }
    }
    else { //shoudn't happen in the final version.
        finalPos.rx() = from.x()*CELL_R*2 + CELL_R - SCENE_R;
        finalPos.ry() = from.y()*CELL_R*2 + CELL_R - SCENE_R;
        p->setPos(finalPos);
        //VFX.
        if(way->pos.x() == -1 && way->pos.y() == -1)
            for(wayNode* nd: way->child) hlPiece(nd);
        else hlPiece(ndFrom);
    }

    return;
}

void GameView::hlWay(wayNode *root)
{
    for(wayNode* nd: root->child) {
        qreal x = nd->pos.x()*CELL_R*2 + CELL_R - SCENE_R;
        qreal y = nd->pos.y()*CELL_R*2 + CELL_R - SCENE_R;
        auto* hl = new wayHighlight;
        scene->addItem(hl);
        hl->setPos(x, y);
        qDebug() << "hlWay at (" << x << ',' << y << ')';
    }
    update();
}

void GameView::hlWay(QPoint pos)
{
    //a helper function.
    for(wayNode* nd: way->child) {
        if(nd->pos == pos) {
            hlWay(nd);
            return;
        }
    }
    //shouldn't reach here.
    qDebug() << "Error in GameView::hlWay!!!!!";
}

void GameView::hlPiece(wayNode *nd)
{
    qreal x = nd->pos.x()*CELL_R*2 + CELL_R - SCENE_R;
    qreal y = nd->pos.y()*CELL_R*2 + CELL_R - SCENE_R;
    auto* hl = new pieceHighlight;
    scene->addItem(hl);
    hl->setPos(x, y);
    qDebug() << "hlPiece at (" << x << ',' << y << ')';
    update();
}

void GameView::hdWayHL()
{
    for(QGraphicsItem* item: scene->items()) {
        if(qgraphicsitem_cast<wayHighlight*>(item))
            scene->removeItem(item);
    }
    qDebug() << "wayHL hidden!";
    update();
}

void GameView::hdPieceHL()
{
    for(QGraphicsItem* item: scene->items()) {
        if(qgraphicsitem_cast<pieceHighlight*>(item))
            scene->removeItem(item);
    }
    qDebug() << "pieceHL hidden!";
    update();
}

