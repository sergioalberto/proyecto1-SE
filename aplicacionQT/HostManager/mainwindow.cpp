#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <gst/gst.h>
#include <QFileDialog>
#include <QString>
#include <iostream>

std::string CancionLIsta;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, Qt::transparent);
    sample_palette.setColor(QPalette::WindowText, Qt::red);

    //ui->label->setPalette(sample_palette);
    //ui->ipLabel->setPalette(sample_palette);
    sample_palette.setColor(QPalette::Window, Qt::white);
    sample_palette.setColor(QPalette::WindowText, Qt::blue);
    ui->labelArchivo->setPalette(sample_palette);

    CancionLIsta = "";

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::streamButtonClick(){
    std::string s = CancionLIsta;
    char *name=new char[s.size()+1];
    name[s.size()]=0;
    memcpy(name,s.c_str(),s.size());

    s=ui->ipEntry->text().toStdString();
    char *ip=new char[s.size()+1];
    name[s.size()]=0;
    memcpy(name,s.c_str(),s.size());
    int port = ui->portEntry->text().toInt();
    player.stream(name,ip, port);

}


void MainWindow::loadButtonClick(){
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.*)"));
    QString filtroSeleccionado=".mp3";
    QString Nombre = QFileDialog::getOpenFileName(this,
                                trUtf8(""),
                                ui->labelArchivo->text(),
                                trUtf8("Archivos mp3 (*.mp3);;Archivos ogg(*.ogg);;Todos los archivos (*)"),
                                &filtroSeleccionado, 0);
    if (!Nombre.isEmpty()){
        CancionLIsta = Nombre.toStdString();
        ui->labelArchivo->setText(getNameMusuic(Nombre));
        std::cout << getNameMusuic(ui->labelArchivo->text()).toStdString() << std::endl;

    }
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
        std::string s = CancionLIsta;
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

QString MainWindow::getNameMusuic(QString name){

    QString resul;
    //printf("Name: %s\n", name.at(7));
    QString::SectionFlag flag = QString::SectionSkipEmpty;
    resul = name;
    //resul.section('/',-1);

    for (int i = name.size()-1; i > 0; i--){
        if (resul.at(i).toAscii() == '/'){
            resul.remove(0, i+1);
            //std::cout << resul.at(i).toAscii() << std::endl;
            break;
        }
    }
    //printf("Name: %s\n", resul.at(1).);
    return resul;

}


