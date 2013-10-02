#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <gst/gst.h>
#include <QFileDialog>

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



void MainWindow::streamButtonClick(){


}


void MainWindow::loadButtonClick(){
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    QString filtroSeleccionado=".mp3";
    QString Nombre = QFileDialog::getOpenFileName(this,
                                trUtf8(""),
                                ui->labelArchivo->text(),
                                trUtf8("Archivos ogg(*.ogg);;Todos los archivos (*);;Archivos mp3 (*.mp3)"),
                                &filtroSeleccionado, 0);
    if (!Nombre.isEmpty())
        ui->labelArchivo->setText(Nombre);
}


int ispause=0;
void MainWindow::pauseButtonClick(){
    if(ispause==0){
        ui->pauseButton->setText("Unpase");
        ispause=1;
        player.pause();

    }
    else{
        ui->pauseButton->setText("Pause");
        ispause=0;
        player.unpause();
    }
}
int playing=0;
void MainWindow::playButtonClick()
{
    if(playing==0){
        ui->playButton->setText("Stop");
        ui->pauseButton->setEnabled(true);
        playing=1;
        std::string s=ui->labelArchivo->text().toStdString();
        char *name=new char[s.size()+1];
        name[s.size()]=0;
        memcpy(name,s.c_str(),s.size());
        player.play(name);

    }
    else{
        ui->playButton->setText("Play");
        ui->pauseButton->setEnabled(false);
        playing=0;
        player.stop();
    }
}



