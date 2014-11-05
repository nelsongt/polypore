#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QFileDialog>
#include <QPainter>
#include <QSvgGenerator>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEventLoop>
#include <QMessageBox>

#include "field.h"
#include "cycle.h"
#include "renderer.h"


class Window : public QWidget
{
    Q_OBJECT

    Field *field;
    Renderer *display;
    Cycle *cycle;
    QLabel *graphics;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *thermalButton;
    QPushButton *resetButton;
    QPushButton *startDisplay;
    QPushButton *stopDisplay;
    QPushButton *pictureButton;
    QPushButton *loadButton;
    QPushButton *startLoadButton;
    QPushButton *stopLoadButton;
    QPushButton *skipBackButton;
    QPushButton *skipForwardButton;
    QPushButton *rewindButton;
    QPushButton *fastForwardButton;
    QPushButton *cancelButton;

    QHBoxLayout *buttonLayout;
    QVBoxLayout *mainLayout;

    QEventLoop q;

    bool stopped;
    bool cancelled;
    quint16 skip;
    quint16 recordedImagesCounter;

public:

    Window(QWidget *parent = 0);

    enum Mode { NormalMode, LoadMode, RunningMode };
    Mode CurrentMode;
    void updateInterface(Mode mode);

public slots:

    void cycleStart();
    void cycleDone();
    void enableDisplay();
    void disableDisplay();
    void saveSvg();
    void loadFromFile();
    void startLoad();
    void stopLoad();
    void rewind();
    void fastForward();
    void cancelLoad();
    void dataErrorField(QString);
    void dataErrorInitial(QString);

signals:

    void windowDone();
    void stepDone();
};


#endif // WINDOW_H
