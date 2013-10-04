#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int playing=0;
void MainWindow::playButtonClick()
{

    if(playing==0){
        ui->playButton->setText("Stop");
        playing=1;
        int s=ui->puertoLabel->text().toInt();
        if(ui->puertoLabel->text()!=""){
            player.stream(s);
        }else{
            ui->puertoLabel->setText("5000");
            player.stream(5000);

        }



    }
    else{
        ui->playButton->setText("Play");
        playing=0;
        player.stop();
    }

}

void MainWindow::portChanged(){
    ui->puertoLabel->setText(ui->puertoEntry->text());
}


