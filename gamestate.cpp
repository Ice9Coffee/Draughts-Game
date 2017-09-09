#include "gamestate.h"

#include <QDebug>
#include <memory>

GameState::GameState(Draughts* initBoard) :
    board(new Draughts[100])
{
    memset(board, null, 100*sizeof(Draughts));

    if(initBoard == nullptr) { //normal start.
        for(int i = 0; i < 20; ++i)
            board[(i/5)*10+(i%5*2+1-(i/5)%2)] = white;
        for(int i = 0; i < 20; ++i)
            board[(6+i/5)*10+(i%5*2+1-(i/5)%2)] = black;
    }
    else { //custom start.
        memcpy_s(board,     100*sizeof(Draughts),
                 initBoard, 100*sizeof(Draughts));
    }


    /*/for test...
    board[12] = white;
    board[16] = black;
    board[18] = black;
    board[29] = white;
    board[32] = white;
    board[43] = black;
    board[52] = white;
    board[65] = white;
    board[72] = white;
    board[85] = black;
    //*/
    //for(int i = 0; i < 100; ++i) qDebug() << board[i];
}

QPoint GameState::move(QPoint from, QPoint to)
{
    int ifrom = from.y()*10 + from.x();
    int ito = to.y()*10 + to.x();
    int dx = (to.x() - from.x()) / abs(to.x() - from.x());
    int dy = (to.y() - from.y()) / abs(to.y() - from.y());
    QPoint dir(dx, dy);
    QPoint eat(-1, -1);

    board[ito] = board[ifrom];
    board[ifrom] = null;
    //吃子.
    for(QPoint p(from); p != to; p += dir) {
        if(board[p.y()*10 + p.x()] != null) {
            board[p.y()*10 + p.x()] = null;
            eat = p;
            break;
        }
    }

    //返回被吃位置...
    return eat;
}

wayNode* GameState::findWays(Draughts forColor)
{
    maxDepth = 0;
    int m_dep = 0;
    wayNode* virNode = new wayNode(-1, -1);
    for(int i = 0; i < 100; ++i) {

        if((board[i] & color) != forColor) continue;

        wayNode* newNode = new wayNode(i%10, i/10);
        qDebug() << "searching" << '(' << i%10 << ',' << i/10 << ')';
        int d = growTree(newNode, 0);
        if(d >= m_dep) {
            if(d > m_dep) {
                m_dep = d;
                for(wayNode* nd: virNode->child) delete nd;
                virNode->child.clear();
                qDebug() << "virNode cleared!";
            }
            if(d > maxDepth) {
                maxDepth = d;
                virNode->child.push_back(newNode);
                qDebug() << QString("new start point (%0, %1)").arg(i%10).arg(i/10);
            }
            else if(d == maxDepth) {
                virNode->child.push_back(newNode);
                qDebug() << QString("new start point (%0, %1)").arg(i%10).arg(i/10);
            }
            else delete newNode;
        }
    }

    //black 0~1; white 2~3; king 0~3
    int dx[]{-1, +1, -1, +1};
    int dy[]{-1, -1, +1, +1};

    if(maxDepth == 0) { //normal move...
        qDebug() << "Cannot eat any piece, try normal move...";
        for(wayNode* nd: virNode->child) delete nd;
        virNode->child.clear();
        qDebug() << "virNode cleared!";

        for(int i = 0; i < 100; ++i) {

            if((board[i] & color) != forColor) continue;

            bool canNormalMove = false;
            int x = i%10, y = i/10;
            wayNode* nd = new wayNode(x, y);

            int kStart, kEnd;
            if(board[i] == black) { kStart = 0; kEnd = 2; }
            else if (board[i] == white) { kStart = 2; kEnd = 4; }
            else if (board[i] & king) { kStart = 0; kEnd = 4; }
            else qDebug() << "Error happened when trying normal move!!!";

            for(int k = kStart; k < kEnd; ++k) {
                int nx = x + dx[k], ny =  y + dy[k];
                while(inBoard(nx, ny) && board[ny*10 + nx] == null) {
                    canNormalMove = true;
                    nd->child.push_back(new wayNode(nx, ny));
                    nx += dx[k];
                    ny += dy[k];
                    if(!(board[i] & king)) break; //不是王时，仅深入一格
                }
            }

            if(canNormalMove) {
                virNode->child.push_back(nd);
                qDebug() << QString("added normal move from (%0, %1)").arg(i%10).arg(i/10);
            } else delete nd;
        }
    }

    return virNode;
}

int GameState::growTree(wayNode *root, const int depth)
{//DFS
    int m_dep = depth;
    int x = root->pos.x();
    int y = root->pos.y();
    Draughts m_piece = board[y*10 + x];

    int dx[]{-1, +1, -1, +1};
    int dy[]{-1, -1, +1, +1};

    //!!!!!!!目前仅考虑兵，不考虑王...
    //! 王吃子寻路已完成

    //bool isEnd = true;

    for(int i=0; i<4; ++i) { //尝试4方向吃子...
        if(board[10*y + x] & king) { //王
            int nx = x+dx[i], ny = y+dy[i];
            //找到非空格
            while(board[10*ny + nx] == null && inBoard(nx, ny))
            { nx += dx[i]; ny += dy[i]; }
            if(!inBoard(nx, ny)) continue;
            //判定是否能吃子.
            if(   (board[10*ny + nx] & color) != (board[10*y + x] & color)
               && !(board[ny*10 + nx] & detected)) {
                int n2x = nx + dx[i], n2y = ny + dy[i];
                while(board[10*n2y + n2x] == null && inBoard(n2x, n2y)) {
                    qDebug() << QString("board[%0](%1,%2) can eat, depth = %3").arg(10*y + x).arg(x).arg(y).arg(depth)
                             << QString("deep into board[%0](%1,%2)...").arg(n2y*10 + n2x).arg(n2x).arg(n2y);
                    board[  y*10 +   x] =  null;
                    board[n2y*10 + n2x] =  m_piece;
                    board[ ny*10 +  nx] |= detected;

                    auto* newNode = new wayNode(n2x, n2y);
                    int d = growTree(newNode, depth+1);

                    if(d >= m_dep) {
                        if(d > m_dep) {
                            m_dep = d;
                            for(wayNode* nd: root->child) delete nd; //析构无用结点，防止内存泄漏
                            root->child.clear();
                        }
                        if(d > maxDepth) {
                            maxDepth = d;
                            root->child.push_back(newNode);
                        }
                        else if(d == maxDepth) {
                            root->child.push_back(newNode);
                        }
                        else delete newNode;
                    }

                    board[ ny*10 +  nx] &= ~detected;
                    board[n2y*10 + n2x] =  null;
                    board[  y*10 +   x] =  m_piece;

                    n2x += dx[i];
                    n2y += dy[i];
                }
            }
        }
        else { //兵
            int nx = x+dx[i], n2x = x + 2*dx[i];
            int ny = y+dy[i], n2y = y + 2*dy[i];
            //略过越界...
            if(!inBoard(nx, ny) || !inBoard(n2x, n2y)) continue;
            if(   (board[ny*10 + nx] & color) != (board[10*y + x] & color)
               && board[ny*10 + nx] != null && board[n2y*10 + n2x] == null
               && !(board[ny*10 + nx] & detected)) {
                qDebug() << QString("board[%0](%1,%2) can eat, depth = %3").arg(10*y + x).arg(x).arg(y).arg(depth)
                         << QString("deep into board[%0](%1,%2)...").arg(n2y*10 + n2x).arg(n2x).arg(n2y);
                //isEnd = false;

                board[  y*10 +   x] =  null;
                board[n2y*10 + n2x] =  m_piece;
                board[ ny*10 +  nx] |= detected;

                auto* newNode = new wayNode(n2x, n2y);
                int d = growTree(newNode, depth+1);

                if(d >= m_dep) {
                    if(d > m_dep) {
                        m_dep = d;
                        for(wayNode* nd: root->child) delete nd; //析构无用结点，防止内存泄漏
                        root->child.clear();
                    }
                    if(d > maxDepth) {
                        maxDepth = d;
                        root->child.push_back(newNode);
                    }
                    else if(d == maxDepth) {
                        root->child.push_back(newNode);
                    }
                    else delete newNode;
                }

                board[ ny*10 +  nx] &= ~detected;
                board[n2y*10 + n2x] =  null;
                board[  y*10 +   x] =  m_piece;

            }
        }
    }

    qDebug() << QString("board[%0](%1,%2) search completed, m_dep = %3").arg(10*y + x).arg(x).arg(y).arg(m_dep);
    return m_dep;
}
