#include "loginwindow.h"
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent), hotel(new Hotel), fieldsShown(false)
{
    setWindowTitle("Hotel Management - Login/Signup");
    setMinimumSize(300, 200);

    loginButton = new QPushButton("Login", this);
    signInButton = new QPushButton("Sign In", this); // Changed from "Register" to "Sign In"
    usernameEdit = nullptr;
    passwordEdit = nullptr;
    labelUsername = nullptr;
    labelPassword = nullptr;

    layout = new QVBoxLayout(this);
    layout->addWidget(loginButton);
    layout->addWidget(signInButton);
    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::showLoginFields);
    connect(signInButton, &QPushButton::clicked, this, &LoginWindow::showSignInFields);
}

LoginWindow::~LoginWindow()
{
    delete hotel;
}

void LoginWindow::showLoginFields() {
    if (!fieldsShown) {
        labelUsername = new QLabel("Username:", this);
        labelPassword = new QLabel("Password:", this);
        usernameEdit = new QLineEdit(this);
        passwordEdit = new QLineEdit(this);
        passwordEdit->setEchoMode(QLineEdit::Password);
        QPushButton *submitButton = new QPushButton("Submit", this);

        layout->insertWidget(0, labelUsername);
        layout->insertWidget(1, usernameEdit);
        layout->insertWidget(2, labelPassword);
        layout->insertWidget(3, passwordEdit);
        layout->insertWidget(4, submitButton);

        connect(submitButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
        fieldsShown = true;
    }
}

void LoginWindow::showSignInFields() {
    if (!fieldsShown) {
        labelUsername = new QLabel("Username:", this);
        labelPassword = new QLabel("Password:", this);
        usernameEdit = new QLineEdit(this);
        passwordEdit = new QLineEdit(this);
        passwordEdit->setEchoMode(QLineEdit::Password);
        QPushButton *submitButton = new QPushButton("Submit", this);

        layout->insertWidget(0, labelUsername);
        layout->insertWidget(1, usernameEdit);
        layout->insertWidget(2, labelPassword);
        layout->insertWidget(3, passwordEdit);
        layout->insertWidget(4, submitButton);

        connect(submitButton, &QPushButton::clicked, this, &LoginWindow::onSignInClicked);
        fieldsShown = true;
    }
}

void LoginWindow::onLoginClicked() {
    User user;
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Username and password cannot be empty.");
        return;
    }

    try {
        QFile userFile("users.txt");
        if (!userFile.open(QIODevice::ReadOnly | QIODevice::Text)) throw std::runtime_error("Cannot open users.txt for reading");
        QTextStream ifs(&userFile);
        User tempUser;
        bool found = false;
        while (!ifs.atEnd()) {
            ifs >> tempUser;
            if (tempUser.getUsername() == username && tempUser.getPassword() == password) {
                user = tempUser;
                found = true;
                break;
            }
        }
        userFile.close();
        if (!found) {
            QMessageBox::warning(this, "Login Failed", "Invalid try.");
            return;
        }
        hotel->setCurrentUser(user);
        accept();
        MainWindow *mainWindow = new MainWindow(this);
        mainWindow->setHotel(hotel);
        mainWindow->show();
    } catch (const std::runtime_error& e) {
        QMessageBox::warning(this, "Error", QString("File operation failed: %1").arg(e.what()));
    }
}

void LoginWindow::onSignInClicked() {
    User user;
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Sign In Failed", "Username and password cannot be empty.");
        return;
    }

    try {
        QFile userFile("users.txt");
        if (userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream ifs(&userFile);
            User tempUser;
            while (!ifs.atEnd()) {
                ifs >> tempUser;
                if (tempUser.getUsername() == username) {
                    QMessageBox::warning(this, "Sign In Failed", "Username already exists.");
                    userFile.close();
                    return;
                }
            }
            userFile.close();
        }

        if (userFile.open(QIODevice::Append | QIODevice::Text)) {
            QString hotelName = QInputDialog::getText(this, "Sign In", "Hotel Name:");
            int totalRooms = QInputDialog::getInt(this, "Sign In", "Total Rooms:", 0, 0, 1000, 1);
            if (hotelName.isEmpty() || totalRooms <= 0) {
                QMessageBox::warning(this, "Sign In Failed", "Hotel Name and positive Total Rooms are required.");
                userFile.close();
                return;
            }
            user = User(username, password, hotelName, totalRooms);
            QTextStream ofs(&userFile);
            ofs << user;
            userFile.close();
            QMessageBox::information(this, "Success", "Sign In successful!");
            hotel->setCurrentUser(user);
            accept();
            MainWindow *mainWindow = new MainWindow(this);
            mainWindow->setHotel(hotel);
            mainWindow->show();
        } else {
            throw std::runtime_error("Cannot open users.txt for appending");
        }
    } catch (const std::runtime_error& e) {
        QMessageBox::warning(this, "Error", QString("File operation failed: %1").arg(e.what()));
    }
}
