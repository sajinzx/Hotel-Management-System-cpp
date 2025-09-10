#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QLabel>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include "header.h"
#include "mainwindow.h"

class LoginWindow : public QDialog

{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    Hotel* getHotel() const { return hotel; }

private slots:
    void showLoginFields();
    void showSignInFields();
    void onLoginClicked();
    void onSignInClicked();

private:
    QPushButton *loginButton;
    QPushButton *signInButton;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLabel *labelUsername;
    QLabel *labelPassword;
    QVBoxLayout *layout;
    Hotel* hotel;
    bool fieldsShown;
};

#endif // LOGINWINDOW_H
