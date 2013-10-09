#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <gst/gst.h>
#include <QFileDialog>
#include <QString>
#include <iostream>
#include <fstream>
std::string CancionLIsta;

using namespace std;
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
   // QFile a = new QFile();
    CancionLIsta = "";    



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::loadIpData(){
    //archivador.prueba();
    QString filtroSeleccionado=".txt";
    QString archivo = QFileDialog::getOpenFileName(this,
                                trUtf8(""),
                                ui->labelArchivo->text(),
                                trUtf8("Archivos txt(*.txt);;Todos los archivos (*)"),
                                &filtroSeleccionado, 0);
    std::string s = archivo.toStdString();
    char *name=new char[s.size()+1];
    name[s.size()]=0;
    memcpy(name,s.c_str(),s.size());
    ifstream fe(name);
    char cadena[128];

    while(!fe.eof()) {
        //fe >> cadena;
        fe.getline(cadena, 128);
        QString string(cadena);
        getListaIpData(string);
        cout << cadena << endl;
    }
    ui->datosList->removeRow(ui->datosList->rowCount()-1);
}

void MainWindow::getListaIpData(QString datos){

    QString temp;
    //printf("Name: %s\n", name.at(7));
    //QString::SectionFlag flag = QString::SectionSkipEmpty;
    temp = datos;
    //resul.section('/',-1);
    int indice=0;
    int size = ui->datosList->rowCount();
    ui->datosList->insertRow(size);
    for (int i = datos.size()-1; i >= 0; i--){
        if (temp.at(i).toAscii() == '/'){
            temp.remove(0, i+1);


            QTableWidgetItem *nuevo = new QTableWidgetItem(temp);
            //QTableWidgetItem *nuevo2 = new QTableWidgetItem(ui->ipEntry->text());
            //QTableWidgetItem *nuevo3 = new QTableWidgetItem(ui->portEntry->text());

            ui->datosList->setItem(size,indice,nuevo);
            indice++;
            temp= datos;
            temp.remove(i,datos.size()-1);
            //std::cout << resul.at(i).toAscii() << std::endl;
        }
    }
    //printf("Name: %s\n", resul.at(1).);

}


int state = 0;
void MainWindow::streamButtonClick(){


    if(ui->datosList->currentRow()>=0 && CancionLIsta.length()>0){
        if(state==0){
            ui->streamButton->setText("Stop");
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
            ui->streamButton->setText("Stream");
            state=0;
            player.stop();
        }
    }
}

void MainWindow::eliminarCliente(){
    if(ui->datosList->currentRow()>=0){
        int indice = ui->datosList->currentRow();
        ui->datosList->removeRow(indice);
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
         //std::cout << CancionLIsta << std::endl;
        ui->labelArchivo->setText(getNameMusic(Nombre));
      //  std::cout << getNameMusic(ui->labelArchivo->text()).toStdString() << std::endl;

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
    if(CancionLIsta.length()>0){
        if(playing==0){
            ui->playButton->setText("Stop");
            ui->pauseButton->setEnabled(true);
            playing=1;
            std::string s = CancionLIsta;

            char *name=new char[s.size()+1];
            name[s.size()]=0;
            memcpy(name,s.c_str(),s.size());
            printf(name);
            player.play(name);

        }
        else{
            ui->playButton->setText("Play");
            ui->pauseButton->setEnabled(false);
            playing=0;
            player.stop();
        }
    }
}

QString MainWindow::getNameMusic(QString name){

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
