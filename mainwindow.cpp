#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtWidgets/QFileDialog>

#include <QtCore/QDebug>
#include <QtCore/QStandardPaths>
#include <QtCore/QDirIterator>
#include <QtCore/QCryptographicHash>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _rootDir("C:\\"),
    _files()
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openDirectory_pb_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Open Directory"),
                                                    QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->directory_le->setText(dir);
        _rootDir.setPath(dir);
    }
    iterate();

}

void MainWindow::iterate()
{
    qDebug() << "_rootDir" << _rootDir;//debugtool:
    QDirIterator iterator(_rootDir.absolutePath(), QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        iterator.next();
        if (!iterator.fileInfo().isDir()) {
            _files << iterator.filePath();
        }
    }
    calculate();
}

void MainWindow::calculate()
{
    QCryptographicHash cryptoSha1(QCryptographicHash::Sha1);
    QCryptographicHash cryptoMd5(QCryptographicHash::Md5);
    QByteArray data;
    QFile fileToWrite("D:\\checksums.txt");
    fileToWrite.open(QIODevice::ReadWrite);
    QTextStream out(&fileToWrite);
    int total = _files.size();
    int counter = 1;
    QString md5Result;
    QString sha1Result;
    out << "FilePath" << ";" << "sha1Result"<< ";" << "md5Result" << "\r\n";
    foreach (const QString &filePath, _files) {
        qDebug() << "File " << counter << "/" << total;//debugtool:
        ++counter;
        QFile file(filePath);
        file.open(QFile::ReadOnly);
        while(!file.atEnd()){
            data = file.read(8192);
            if (ui->md5_cb->isChecked()) {
                cryptoMd5.addData(data);
            }
            if (ui->sha1_cb->isChecked()) {
                cryptoSha1.addData(data);
            }
        }
        if (ui->md5_cb->isChecked()) {
            md5Result = cryptoMd5.result().toHex();
            _filesMd5.insert(filePath, md5Result);
        }
        if (ui->sha1_cb->isChecked()) {
            sha1Result = cryptoSha1.result().toHex();
            _filesSha1.insert(filePath, sha1Result);
        }

        out << filePath << ";" << sha1Result<< ";" << md5Result << ";\r\n";
        sha1Result = QString("");
        md5Result = QString("");
    }
    qDebug() << "End of work for today";//debugtool:

}
