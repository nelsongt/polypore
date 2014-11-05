#include "window.h"


Window::Window(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<QPicture>("QPicture");

    // Create the backend components
    field = new Field(this);
    cycle = new Cycle(this,field,1);
    display = new Renderer(this,cycle);


    // Create the frontend components
    graphics = new QLabel;
    startButton = new QPushButton("Start");
    stopButton = new QPushButton("Stop");
    thermalButton = new QPushButton("Thermalize");
    resetButton = new QPushButton("Reset");
    startDisplay = new QPushButton("Enable Graphics");
    stopDisplay = new QPushButton("Disable Graphics");
    pictureButton = new QPushButton("Save SVG");
    loadButton = new QPushButton("Load Data");
    startLoadButton = new QPushButton("Start");
    stopLoadButton  = new QPushButton("Stop");
    skipBackButton = new QPushButton("Skip Back");
    skipForwardButton = new QPushButton("Skip Forward");
    rewindButton = new QPushButton("Rewind");
    fastForwardButton = new QPushButton("Fast Forward");
    cancelButton = new QPushButton("Cancel");


    // Enable the button functions
    connect(startButton, SIGNAL(clicked()), cycle, SLOT(startSimulation()));
    connect(cycle, SIGNAL(cycleStart()), this, SLOT(cycleStart()));
    connect(stopButton, SIGNAL(clicked()), cycle, SLOT(stopSimulation()));
    connect(cycle, SIGNAL(finished()), this, SLOT(cycleDone()));
    connect(resetButton, SIGNAL(clicked()), cycle, SLOT(resetSimulation()));
    connect(startDisplay, SIGNAL(clicked()), this, SLOT(enableDisplay()));
    connect(stopDisplay, SIGNAL(clicked()), this, SLOT(disableDisplay()));
    connect(startLoadButton, SIGNAL(clicked()), this, SLOT(startLoad()));
    connect(stopLoadButton, SIGNAL(clicked()), this, SLOT(stopLoad()));
    connect(skipBackButton, SIGNAL(clicked()), &q, SLOT(quit()));
    connect(skipForwardButton, SIGNAL(clicked()), &q, SLOT(quit()));
    connect(rewindButton, SIGNAL(clicked()), this, SLOT(rewind()));
    connect(fastForwardButton, SIGNAL(clicked()), this, SLOT(fastForward()));
    connect(pictureButton, SIGNAL(clicked()), this, SLOT(saveSvg()));
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadFromFile()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelLoad()));


    // Connect initial front end to the back end
    connect(cycle, SIGNAL(stepDone()), display, SLOT(start()));
    connect(cycle, SIGNAL(resetDone()), display, SLOT(start()));
    connect(this, SIGNAL(stepDone()), display, SLOT(start()));
    //connect(display, SIGNAL(pictureChanged(QPicture)), &q, SLOT(quit()));
    connect(display, SIGNAL(pictureChanged(QPicture)), graphics, SLOT(setPicture(QPicture)));

    // Connect the error messages
    connect(this, SIGNAL(windowDone()), cycle->field, SLOT(buildField()));  // replace later for gui purposes
    connect(field, SIGNAL(inputError(QString)), this, SLOT(dataErrorField(QString)));
    connect(cycle, SIGNAL(inputError(QString)), this, SLOT(dataErrorInitial(QString)));

    // This sets the layout of the main window GUI
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(stopButton);
    //buttonLayout->addWidget(thermalButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(startDisplay);
    buttonLayout->addWidget(stopDisplay);
    buttonLayout->addWidget(startLoadButton);
    buttonLayout->addWidget(stopLoadButton);
    buttonLayout->addWidget(skipBackButton);
    buttonLayout->addWidget(skipForwardButton);
    buttonLayout->addWidget(rewindButton);
    buttonLayout->addWidget(fastForwardButton);
    buttonLayout->addWidget(pictureButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(graphics);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Set the initial GUI options
    graphics->setPicture(display->picture);
    stopButton->setEnabled(false);
    startDisplay->setEnabled(false);
    //resetButton->setEnabled(false);

    updateInterface(NormalMode);
    setWindowTitle(tr("DNA Simulation"));

    stopped = false;
    cancelled = false;
    skip = 1;
    recordedImagesCounter = 0;

    emit windowDone();
}



// This will enable one to playback old runs, it's limited for now
void Window::loadFromFile()
{
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Load Data File"), "",
            tr("All Files (*)"));

        if (fileName.isEmpty())
            return;
        else {

            QFile inFile(fileName);

            if (!inFile.open( QIODevice::ReadOnly | QIODevice::Text )) {
                QMessageBox::information(this, tr("Unable to open file"),
                    inFile.errorString());
                return;
            }


            this->updateInterface(LoadMode);

            QTextStream stream( &inFile );
            QList<QString> temp;
            QString temp1 = "0";
            QString line;

            quint32 counter = 0;

            for( unsigned int n=0; n <= (beadNumber * 3); n++ )
            {
                temp << temp1;
            }


            QTextStream out2(stdout);
            //QFile gyrad("./output/gyrad");
            //gyrad.open( QIODevice::WriteOnly );
            //QTextStream info;
            //info.setDevice(&gyrad);

            QFile outPut("./output/stuff");
            outPut.open( QIODevice::WriteOnly );
            QTextStream out;
            out.setDevice(&outPut);

            quint32 translocating = 0;
            bool finished = 0;
            qreal tranStart = 0;
            qreal sum = 0;
            qreal radgy = 0;
            aVector3D CoM = aVector3D();

            bool loop = true;
            while (loop == true)
            {
                for (qint16 j = skip; j > 0; j--)
                    line = stream.readLine(); // line of text excluding '\n'

                // This conditional will halt the playback until q.quit() is called
                if (stopped == true)
                    q.exec();

                if (line == "" || cancelled == true)
                {
                    loop = false;
                    out2 << "stuff" << endl;
                }

                else
                {

                //we now have a line of text with a bunch of tabs inside it
                //we want to parse it, tab by tab, and get out the fields we want: n n+1 n+2 ...

                for( unsigned int n=0; n <= (beadNumber * 3); n++ )
                {
                    temp[n] = line.section('\t',n,n);
                }

                //out2 << temp[0] << endl;

                //line is now loaded

                for( unsigned int i=0; i < beadNumber; i++ )
                {
                    cycle->dnachain[i].pos.setX(temp[(i*3)+1].toDouble());
                    cycle->dnachain[i].pos.setY(temp[(i*3)+2].toDouble());
                    cycle->dnachain[i].pos.setZ(temp[(i*3)+3].toDouble());
                }


                CoM = aVector3D();
                for( unsigned int j=0; j < beadNumber; j++)
                {
                    CoM += cycle->dnachain[j].pos;
                }

                CoM = CoM / beadNumber;

                //out << temp[0].toDouble() * timeStep << "     \t" << CoM.y() << endl;

                translocating = 0;
                for  ( unsigned int i = 0; i < beadNumber; i++ )
                {
                    //if (cycle->dnachain[i].pos.y() > memStart && cycle->dnachain[i].pos.y() < memEnd)
                    if (CoM.y() > memStart && CoM.y() < memEnd)
                    {
                        translocating = 1;
                    }
                }

                if (translocating == 0 && CoM.y() < poreCenterY)
                {
                    translocating = 0;
                    tranStart = temp[0].toDouble();
                }
                else if (translocating == 0 && finished == 0)
                {
                    QFile trData("./output/translocate." + QString::number(beadNumber) + "beads.dat");
                    trData.open( QIODevice::WriteOnly );
                    QTextStream tr;
                    tr.setDevice(&trData);
                    //tr << (temp[0].toDouble() - tranStart) << endl;
                    trData.close();
                    finished = 1;
                }


                /*sum = 0;
                for( unsigned int j=0; j < beadNumber; j++)
                {
                    for( unsigned int k=0; k < beadNumber; k++)
                    {
                        aVector3D r1rmean = cycle->dnachain[j].pos - cycle->dnachain[k].pos;
                        sum += r1rmean.lengthSquared();
                    }
                }*/

                /*quint32 i = 0;
                out << counter << "     \t";
                while (i < beadNumber)
                {
                    out << cycle->dnachain[i].pos.x() << "     \t" << cycle->dnachain[i].pos.y() << "     \t" << cycle->dnachain[i].pos.z() << "     \t";
                    i++;
                }
                out << endl;*/

                //out2 << sum << endl;

                /*radgy = sum / (2 * beadNumber * beadNumber);

                out2 << sum << endl;

                info << counter << "   \t" << CoM.y() << "   \t" << radgy << "   \t" << endl;*/

                //out2 << "Current frame: " << counter << endl;
                emit stepDone();

                while(!display->isFinished());
                   q.processEvents();

                counter++;
                }
            }
            //gyrad.close();
            inFile.close();
            this->updateInterface(NormalMode);
        }
}


//  These slots control the button display

void Window::cycleStart()
{
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    resetButton->setEnabled(false);
    //thermalButton->setEnabled(false);
}

void Window::cycleDone()
{
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
    resetButton->setEnabled(true);
    //thermalButton->setEnabled(true);
}

void Window::enableDisplay()
{
    startDisplay->setEnabled(false);
    stopDisplay->setEnabled(true);
    disconnect(cycle, 0, cycle, 0);
    connect(cycle, SIGNAL(stepDone()), display, SLOT(start()));
}

void Window::disableDisplay()
{
    stopDisplay->setEnabled(false);
    startDisplay->setEnabled(true);
    disconnect(cycle, 0, display, 0);
    disconnect(display, 0, cycle, 0);
}

void Window::startLoad()
{
    startLoadButton->setEnabled(false);
    stopLoadButton->setEnabled(true);
    skipBackButton->setEnabled(false);
    skipForwardButton->setEnabled(false);
    rewindButton->setEnabled(true);
    fastForwardButton->setEnabled(true);
    stopped = false;
    q.quit();
}

void Window::stopLoad()
{
    startLoadButton->setEnabled(true);
    stopLoadButton->setEnabled(false);
    skipBackButton->setEnabled(true);
    skipForwardButton->setEnabled(true);
    rewindButton->setEnabled(false);
    fastForwardButton->setEnabled(false);
    stopped = true;
}

void Window::saveSvg()
{
    QSvgGenerator generator;
    generator.setFileName(QString::number(recordedImagesCounter)+".svg");
    generator.setViewBox(QRect(0, 0, display->picture.width(), display->picture.height()));
    generator.setTitle(tr("SVG File"));
    generator.setDescription(tr("SVG Image"));

    QPainter painter;
    painter.begin(&generator);
    painter.drawPicture(0, 0, display->picture);
    painter.end();

    recordedImagesCounter++;
}

void Window::rewind()
{
    if (skip == 1)
        return;
    else
        skip = skip / 2;
}

void Window::fastForward()
{
    skip = skip * 2;
}

void Window::cancelLoad()
{
    cancelled = true;
    q.quit();
}


// Output some errors in case files cannot be loaded -- not working

void Window::dataErrorField(QString error)
{
    startButton->setEnabled(false);
    stopButton->setEnabled(false);
    resetButton->setEnabled(false);
    thermalButton->setEnabled(false);

    QMessageBox::information(this, error,
                             "Unable to open electric field data file, \"input/output.all.dat\". Only previous runs will be available.");
    return;
}

void Window::dataErrorInitial(QString error)
{
    startButton->setEnabled(false);
    stopButton->setEnabled(false);
    resetButton->setEnabled(false);
    thermalButton->setEnabled(false);

    //QMessageBox::information(this, tr("Unable to open initial condition data file, \"./input/initial." + QString::number(beadNumber) + "beads.dat\". Only previous runs will be available."),
    //   error);
    return;


    //else
    //{
    //   QTextStream out(stdout);
    //    out << "Initial Positions Data Error";
    //}
}


// Updates the button interface
void Window::updateInterface(Mode mode)
{
    CurrentMode = mode;

    switch (CurrentMode) {

    case NormalMode:

        startButton->setEnabled(true);
        stopButton->setEnabled(false);
        resetButton->setEnabled(true);

        startLoadButton->hide();
        stopLoadButton->hide();
        skipBackButton->hide();
        skipForwardButton->hide();
        rewindButton->hide();
        fastForwardButton->hide();
        cancelButton->hide();

        startButton->show();
        stopButton->show();
        //thermalButton->show();
        resetButton->show();
        startDisplay->show();
        stopDisplay->show();
        loadButton->show();

        break;

    case LoadMode:

        startLoadButton->setEnabled(false);
        stopLoadButton->setEnabled(true);
        skipBackButton->setEnabled(false);
        skipForwardButton->setEnabled(false);
        rewindButton->setEnabled(true);
        fastForwardButton->setEnabled(true);

        startLoadButton->show();
        stopLoadButton->show();
        skipBackButton->show();
        skipForwardButton->show();
        rewindButton->show();
        fastForwardButton->show();
        cancelButton->show();

        startButton->hide();
        stopButton->hide();
        //thermalButton->hide();
        resetButton->hide();
        startDisplay->hide();
        stopDisplay->hide();
        loadButton->hide();

        break;

    case RunningMode:

        break;
    }
}
