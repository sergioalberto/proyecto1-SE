#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <reproductor.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    reproductor player;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
        void playButtonClick();
        void loadButtonClick();
        void streamButtonClick();
        void pauseButtonClick();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H