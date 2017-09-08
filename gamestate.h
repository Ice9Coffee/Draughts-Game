#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QPoint>
#include <QVector>

enum Draughts {
    //         16        8         4         2     1
    //      detected  isking   iswhite   isblack   0
    null = 0,
    black = 2,
    white = 4,
    blackking = 10,
    whiteking = 12,

    color = 6,
    king = 8,
    detected = 16

};

inline Draughts operator|= (Draughts& d1, const Draughts& d2)
{ return d1 = (Draughts)(d1 | d2); }
inline Draughts operator&= (Draughts& d1, const Draughts& d2)
{ return d1 = (Draughts)(d1 & d2); }
inline Draughts operator~ (const Draughts& d)
{ return (Draughts)~((int)d); }


struct wayNode {
    QPoint pos;
    QVector<wayNode*> child;
    wayNode(const QPoint& p) : pos(p) {}
    wayNode(const int x, const int y) : pos(x, y) {}
    ~wayNode() {for(wayNode* nd: child) delete nd;}
};

class GameState
{
public:
    GameState();

    QPoint move(QPoint from, QPoint to);

    Draughts *board;

    wayNode* findWays(Draughts color);

private:
    int maxDepth;

    int growTree(wayNode* root, const int depth);

    inline bool inBoard(int x, int y) { return 0<=x && x<10 && 0<=y && y<10; }
};

#endif // GAMESTATE_H
