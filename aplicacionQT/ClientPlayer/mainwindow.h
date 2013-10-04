#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <streamer.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    streamer player;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
        void playButtonClick();
        void portChanged();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
