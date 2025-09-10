#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include "header.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setHotel(Hotel* h) { hotel = h; }

private slots:
    void onItemClicked(QListWidgetItem *item);

private:
    QListWidget *menuList;
    QTextEdit *outputText;
    QVBoxLayout *layout;
    Hotel* hotel;
};

#endif // MAINWINDOW_H
