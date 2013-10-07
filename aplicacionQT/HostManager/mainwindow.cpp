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
    sample_palette.setColor(QPalette::WindowText, Qt::white);

    ui->label_4->setPalette(sample_palette);
    ui->label_3->setPalette(sample_palette);
    ui->label_6->setPalette(sample_palette);
    sample_palette.setColor(QPalette::Window, Qt::white);
    sample_palette.setColor(QPalette::WindowText, Qt::blue);
    ui->labelArchivo->setPalette(sample_palette);

    CancionLIsta = "";

}

MainWindow::~MainWindow()
{
    delete ui;
}


int state = 0;
void MainWindow::streamButtonClick(){
    if(state==0){
        ui->pushButton->setText("Stop");
        state=1;
        char *name, *ip;
        int port;
        std::string s = CancionLIsta;
        name=new char[s.size()+1];
        name[s.size()]=0;
        memcpy(name,s.c_str(),s.size());

        int indice = ui->datosList->currentRow();

        s = ui->datosList->item(indice,1)->text().toStdString();
        //ui->l->setText(ui->datosList->item(indice,2)->text());
        ip = new char[s.size()+1];
        ip[s.size()]=0;
        memcpy(ip,s.c_str(),s.size());

        if(ui->datosList->item(indice,2)->text().count()>0){
             port = ui->datosList->item(indice,2)->text().toInt();;
        }else{
            port = 5000;
        }
         player.stream(name,ip, port);
    }
    else{
        ui->pushButton->setText("Stream");
        state=0;
        player.stop();
    }
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

        int size = ui->datosList_2->rowCount();
        ui->datosList_2->insertRow(size);
        QTableWidgetItem *nuevo = new QTableWidgetItem(ui->labelArchivo->text());
        ui->datosList_2->setItem(size,0,nuevo);

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



void MainWindow::on_agregarButton_clicked()
{
    if(ui->portEntry_2->text().count()>0 && ui->ipEntry->text().count()>0){
        int size = ui->datosList->rowCount();
        ui->datosList->insertRow(size);
        QTableWidgetItem *nuevo = new QTableWidgetItem(ui->portEntry_2->text());
        QTableWidgetItem *nuevo2 = new QTableWidgetItem(ui->ipEntry->text());
        QTableWidgetItem *nuevo3 = new QTableWidgetItem(ui->portEntry->text());

        ui->datosList->setItem(size,0,nuevo);
        ui->datosList->setItem(size,1,nuevo2);
        //ui->datosList->setItem(size,2,nuevo3);

        if(ui->portEntry->text().count()==0){
            nuevo3 = new QTableWidgetItem("5000");
        }
        ui->datosList->setItem(size,2,nuevo3);
    }
}
