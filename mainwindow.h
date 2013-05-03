#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QHash>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_openDirectory_pb_clicked();

private:
    void iterate();
    void calculate();

    Ui::MainWindow *ui;
    QDir _rootDir;
    QStringList _files;
    QHash<QString, QString> _filesSha1; //<absolutePath, sha1>
    QHash<QString, QString> _filesMd5; //<absolutePath, md5>
};

#endif // MAINWINDOW_H
