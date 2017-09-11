#include "bgmthread.h"

BgmThread::BgmThread(QString src, QObject *parent) : QThread(parent) {
    bgm.setSource(QUrl::fromLocalFile(src));
    bgm.setLoopCount(QSoundEffect::Infinite);
    bgm.setVolume(0.30f);
}

void BgmThread::run() {
    bgm.play();
}
