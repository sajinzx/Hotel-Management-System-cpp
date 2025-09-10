#include "mainwindow.h"
#include <QWidget>
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), hotel(nullptr)
{
    setWindowTitle("Hotel Management System");
    setMinimumSize(400, 300);

    menuList = new QListWidget(this);
    outputText = new QTextEdit(this);
    outputText->setReadOnly(true);

    layout = new QVBoxLayout();
    layout->addWidget(menuList);
    layout->addWidget(outputText);
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    QStringList items = {
        "1. Add Room",
        "2. Add Guest",
        "3. Make Reservation",
        "4. Display Available Rooms",
        "5. Display Reservations",
        "6. Display Guests",
        "7. Save Data",
        "8. Load Data",
        "9. Exit"
    };
    menuList->addItems(items);

    connect(menuList, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onItemClicked(QListWidgetItem *item) {
    outputText->clear();
    int choice = item->text().split(".")[0].toInt();
    switch (choice) {
    case 1: {
        bool ok;
        int roomNumber = QInputDialog::getInt(this, "Add Room", "Room Number:", 0, 0, 1000, 1, &ok);
        double price = QInputDialog::getDouble(this, "Add Room", "Price Per Night:", 0.0, 0.0, 1000.0, 2, &ok);
        if (ok && hotel) hotel->addRoom(Room(roomNumber, price));
        outputText->append("Room added successfully.");
        break;
    }
    case 2: {
        bool ok;
        QString name = QInputDialog::getText(this, "Add Guest", "Name:");
        QString contact = QInputDialog::getText(this, "Add Guest", "Contact Info:");
        int guestID = QInputDialog::getInt(this, "Add Guest", "Guest ID:", 0, 0, 1000, 1, &ok);
        if (ok && hotel) hotel->addGuest(Guest(name, contact, guestID));
        outputText->append("Guest added successfully.");
        break;
    }
    case 3: {
        bool ok;
        int reservationID = QInputDialog::getInt(this, "Make Reservation", "Reservation ID:", 0, 0, 1000, 1, &ok);
        int guestID = QInputDialog::getInt(this, "Make Reservation", "Guest ID:", 0, 0, 1000, 1, &ok);
        int roomNumber = QInputDialog::getInt(this, "Make Reservation", "Room Number:", 0, 0, 1000, 1, &ok);
        QString checkIn = QInputDialog::getText(this, "Make Reservation", "Check-In Date (YYYY-MM-DD):");
        QString checkOut = QInputDialog::getText(this, "Make Reservation", "Check-Out Date (YYYY-MM-DD):");
        if (ok && hotel) {
            Guest* guest = hotel->findGuestByID(guestID);
            Room* room = hotel->findRoomByNumber(roomNumber);
            if (guest && room && room->getStatus() == "Available") {
                hotel->addReservation(Reservation(reservationID, *guest, *room, checkIn, checkOut));
                outputText->append("Reservation made successfully.");
            } else {
                outputText->append("Error: Guest, room not found, or room not available.");
            }
        }
        break;
    }
    case 4:
        if (hotel) hotel->displayAvailableRooms(outputText);
        break;
    case 5:
        if (hotel) hotel->displayReservations(outputText);
        break;
    case 6:
        if (hotel) hotel->displayGuests(outputText);
        break;
    case 7:
        if (hotel) hotel->saveData();
        outputText->append("Data saved successfully.");
        break;
    case 8:
        if (hotel) hotel->loadData();
        outputText->append("Data loaded successfully.");
        break;
    case 9:
        close();
        break;
    default:
        outputText->append("Invalid choice.");
    }
}
