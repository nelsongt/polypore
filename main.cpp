#include <QApplication>
#include "include/window.h"
#include "include/console.h"

int main(int argc, char *argv[])
{
    // seed generator for srand -> requires QTime
    qsrand(QDateTime::currentDateTime().toTime_t() ^ (getpid() + (getpid() << 15)));
    char video[] = "-v";
    //qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()) * static_cast<unsigned int>(getpid()));

    if( argc > 1 )
    {
        if( !strcmp( argv[1], video ))
        {
            QApplication app(argc, argv);
            Window window;
            window.show();

            return app.exec();
        }
        else
        {
            fprintf(stderr, "Incorrect runtime flag\n");
            exit(1);
        }
    }
    else
    {
        QCoreApplication app(argc, argv);
        Console console;

        QObject::connect(&console,SIGNAL(finished()),&app, SLOT(quit()),Qt::QueuedConnection);
        return app.exec();
    }

    return 1;
}
