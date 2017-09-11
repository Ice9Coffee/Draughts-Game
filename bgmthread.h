#ifndef BGMTHREAD_H
#define BGMTHREAD_H

#include <QThread>
#include <QSoundEffect>

class BgmThread : public QThread
{
public:
    BgmThread(QString src, QObject* parent = nullptr);

protected:
    void run();

private:
    QSoundEffect bgm;
};

#endif // BGMTHREAD_H
