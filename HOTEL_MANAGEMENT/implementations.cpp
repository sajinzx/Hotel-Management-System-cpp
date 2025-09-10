#include "header.h"
#include <QInputDialog>
#include <QMessageBox>

// User Class Implementation
User::User() : totalRooms(0) {}
User::User(const QString& username, const QString& password, const QString& hotelName, int totalRooms)
    : username(username), password(password), hotelName(hotelName), totalRooms(totalRooms) {}
QString User::getUsername() const { return username; }
QString User::getPassword() const { return password; }
QString User::getHotelName() const { return hotelName; }
int User::getTotalRooms() const { return totalRooms; }
void User::setUsername(const QString& username) { this->username = username; }
void User::setPassword(const QString& password) { this->password = password; }
void User::setHotelName(const QString& hotelName) { this->hotelName = hotelName; }
void User::setTotalRooms(int totalRooms) { this->totalRooms = totalRooms; }
QTextStream& operator<<(QTextStream& ofs, const User& user) { ofs << user.username << "\n" << user.password << "\n" << user.hotelName << "\n" << user.totalRooms << "\n"; return ofs; }
QTextStream& operator>>(QTextStream& ifs, User& user) { ifs >> user.username >> user.password >> user.hotelName >> user.totalRooms; return ifs; }

// Guest Class Implementation
Guest::Guest() : guestID(0) {}
Guest::Guest(const QString& name, const QString& contactInfo, int guestID) : name(name), contactInfo(contactInfo), guestID(guestID) {}
QString Guest::getName() const { return name; }
QString Guest::getContactInfo() const { return contactInfo; }
int Guest::getGuestID() const { return guestID; }
void Guest::setName(const QString& name) { this->name = name; }
void Guest::setContactInfo(const QString& contactInfo) { this->contactInfo = contactInfo; }
void Guest::setGuestID(int guestID) { this->guestID = guestID; }
QTextStream& operator<<(QTextStream& ofs, const Guest& guest) { ofs << guest.guestID << "\n" << guest.name << "\n" << guest.contactInfo << "\n"; return ofs; }
QTextStream& operator>>(QTextStream& ifs, Guest& guest) { ifs >> guest.guestID >> guest.name >> guest.contactInfo; return ifs; }

// Room Class Implementation
Room::Room() : roomNumber(0), pricePerNight(0.0), status("Available") {}
Room::Room(int roomNumber, double pricePerNight) : roomNumber(roomNumber), pricePerNight(pricePerNight), status("Available") {}
int Room::getRoomNumber() const { return roomNumber; }
QString Room::getStatus() const { return status; }
double Room::getPricePerNight() const { return pricePerNight; }
void Room::setStatus(const QString& status) { this->status = status; }
void Room::setPricePerNight(double price) { this->pricePerNight = price; }
QTextStream& operator<<(QTextStream& ofs, const Room& room) { ofs << room.roomNumber << "\n" << room.pricePerNight << "\n" << room.status << "\n"; return ofs; }
QTextStream& operator>>(QTextStream& ifs, Room& room) { ifs >> room.roomNumber >> room.pricePerNight >> room.status; return ifs; }

// Reservation Class Implementation
Reservation::Reservation() : reservationID(0) {}
Reservation::Reservation(int reservationID, const Guest& guest, Room& room, const QString& checkInDate, const QString& checkOutDate)
    : reservationID(reservationID), guest(guest), room(room), checkInDate(checkInDate), checkOutDate(checkOutDate) {
    room.setStatus("Occupied");
}
int Reservation::getReservationID() const { return reservationID; }
Guest Reservation::getGuest() const { return guest; }
Room Reservation::getRoom() const { return room; }
QString Reservation::getCheckInDate() const { return checkInDate; }
QString Reservation::getCheckOutDate() const { return checkOutDate; }

int Reservation::daysBetween() const {
    QDate checkIn = QDate::fromString(checkInDate, "yyyy-MM-dd");
    QDate checkOut = QDate::fromString(checkOutDate, "yyyy-MM-dd");
    if (!checkIn.isValid() || !checkOut.isValid() || checkOut <= checkIn) return 1;
    return checkIn.daysTo(checkOut);
}

double Reservation::calculateTotalCost() const {
    return room.getPricePerNight() * daysBetween(); // Cost per night * total days stayed
}

QTextStream& operator<<(QTextStream& ofs, const Reservation& reservation) {
    ofs << reservation.reservationID << "\n" << reservation.guest.getGuestID() << "\n" << reservation.room.getRoomNumber() << "\n"
        << reservation.checkInDate << "\n" << reservation.checkOutDate << "\n";
    return ofs;
}
QTextStream& operator>>(QTextStream& ifs, Reservation& reservation) {
    int guestID, roomNumber;
    ifs >> reservation.reservationID >> guestID >> roomNumber >> reservation.checkInDate >> reservation.checkOutDate;
    return ifs;
}

// Hotel Class Implementation
void Hotel::addRoom(const Room& room) { rooms.push_back(room); }
void Hotel::addReservation(const Reservation& reservation) { reservations.push_back(reservation); }
void Hotel::addGuest(const Guest& guest) { guests.push_back(guest); }

void Hotel::displayAvailableRooms(QTextEdit* output) const {
    if (rooms.isEmpty() || std::none_of(rooms.begin(), rooms.end(), [](const Room& r) { return r.getStatus() == "Available"; })) {
        output->append("Not Available");
    } else {
        output->append("Available Rooms:");
        for (const Room& room : rooms) {
            if (room.getStatus() == "Available") {
                output->append(QString("Room Number: %1, Price Per Night: $%2")
                                   .arg(room.getRoomNumber()).arg(room.getPricePerNight()));
            }
        }
    }
}

void Hotel::displayReservations(QTextEdit* output) const {
    if (reservations.isEmpty()) {
        output->append("Not Available");
    } else {
        output->append("Reservations:");
        for (const Reservation& reservation : reservations) {
            output->append(QString("Reservation ID: %1, Guest: %2, Room: %3, Check-In: %4, Check-Out: %5, Total Cost: $%6")
                               .arg(reservation.getReservationID())
                               .arg(reservation.getGuest().getName())
                               .arg(reservation.getRoom().getRoomNumber())
                               .arg(reservation.getCheckInDate())
                               .arg(reservation.getCheckOutDate())
                               .arg(reservation.calculateTotalCost()));
        }
    }
}

void Hotel::displayGuests(QTextEdit* output) const {
    if (guests.isEmpty()) {
        output->append("Not Available");
    } else {
        output->append("Guests:");
        for (const Guest& guest : guests) {
            output->append(QString("Guest ID: %1, Name: %2, Contact: %3")
                               .arg(guest.getGuestID()).arg(guest.getName()).arg(guest.getContactInfo()));
        }
    }
}

void Hotel::saveData() const {
    try {
        QFile roomFile("rooms.txt");
        if (!roomFile.open(QIODevice::WriteOnly | QIODevice::Text)) throw std::runtime_error("Cannot open rooms.txt for writing");
        QTextStream ofs(&roomFile);
        for (const Room& room : rooms) ofs << room;
        roomFile.close();

        QFile guestFile("guests.txt");
        if (!guestFile.open(QIODevice::WriteOnly | QIODevice::Text)) throw std::runtime_error("Cannot open guests.txt for writing");
        QTextStream ofs2(&guestFile);
        for (const Guest& guest : guests) ofs2 << guest;
        guestFile.close();

        QFile reservationFile("reservations.txt");
        if (!reservationFile.open(QIODevice::WriteOnly | QIODevice::Text)) throw std::runtime_error("Cannot open reservations.txt for writing");
        QTextStream ofs3(&reservationFile);
        for (const Reservation& reservation : reservations) ofs3 << reservation;
        reservationFile.close();
    } catch (const std::runtime_error& e) {
        qDebug() << "Error saving data:" << e.what();
    }
}

void Hotel::loadData() {
    try {
        QFile roomFile("rooms.txt");
        if (!roomFile.open(QIODevice::ReadOnly | QIODevice::Text)) throw std::runtime_error("Cannot open rooms.txt for reading");
        QTextStream ifs(&roomFile);
        Room room;
        while (!ifs.atEnd()) { ifs >> room; rooms.push_back(room); }
        roomFile.close();

        QFile guestFile("guests.txt");
        if (!guestFile.open(QIODevice::ReadOnly | QIODevice::Text)) throw std::runtime_error("Cannot open guests.txt for reading");
        QTextStream ifs2(&guestFile);
        Guest guest;
        while (!ifs2.atEnd()) { ifs2 >> guest; guests.push_back(guest); }
        guestFile.close();

        QFile reservationFile("reservations.txt");
        if (!reservationFile.open(QIODevice::ReadOnly | QIODevice::Text)) throw std::runtime_error("Cannot open reservations.txt for reading");
        QTextStream ifs3(&reservationFile);
        Reservation reservation;
        while (!ifs3.atEnd()) { ifs3 >> reservation; reservations.push_back(reservation); }
        reservationFile.close();
    } catch (const std::runtime_error& e) {
        qDebug() << "Error loading data:" << e.what();
    }
}

Room* Hotel::findRoomByNumber(int roomNumber) {
    for (Room& room : rooms) {
        if (room.getRoomNumber() == roomNumber) {
            return &room;
        }
    }
    return nullptr;
}

Guest* Hotel::findGuestByID(int guestID) {
    for (Guest& guest : guests) {
        if (guest.getGuestID() == guestID) {
            return &guest;
        }
    }
    return nullptr;
}

void Hotel::setCurrentUser(const User& user) { currentUser = user; }
User Hotel::getCurrentUser() const { return currentUser; }

// Login Function
bool login(User& user) {
    QString username = QInputDialog::getText(nullptr, "Login", "Username:");
    QString password = QInputDialog::getText(nullptr, "Login", "Password:", QLineEdit::Password);

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(nullptr, "Login Failed", "Username and password cannot be empty.");
        return false;
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
            QMessageBox::warning(nullptr, "Login Failed", "Invalid try.");
            return false;
        }
        return true;
    } catch (const std::runtime_error& e) {
        QMessageBox::warning(nullptr, "Error", QString("File operation failed: %1").arg(e.what()));
        return false;
    }
}

// Register Function
void registerUser(User& user) {
    QString username, password, hotelName;
    int totalRooms;
    bool ok;
    username = QInputDialog::getText(nullptr, "Register", "Username:", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    password = QInputDialog::getText(nullptr, "Register", "Password:", QLineEdit::Password, "", &ok);
    if (!ok) return;
    hotelName = QInputDialog::getText(nullptr, "Register", "Hotel Name:", QLineEdit::Normal, "", &ok);
    if (!ok) return;
    totalRooms = QInputDialog::getInt(nullptr, "Register", "Total Rooms:", 0, 0, 1000, 1, &ok);
    if (!ok) return;

    if (username.isEmpty() || password.isEmpty() || hotelName.isEmpty() || totalRooms <= 0) {
        QMessageBox::warning(nullptr, "Registration Failed", "All fields are required and total rooms must be positive.");
        return;
    }

    user = User(username, password, hotelName, totalRooms);

    try {
        QFile userFile("users.txt");
        if (userFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream ifs(&userFile);
            User tempUser;
            while (!ifs.atEnd()) {
                ifs >> tempUser;
                if (tempUser.getUsername() == username) {
                    QMessageBox::warning(nullptr, "Registration Failed", "Username already exists.");
                    userFile.close();
                    return;
                }
            }
            userFile.close();
        }

        if (userFile.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream ofs(&userFile);
            ofs << user;
            userFile.close();
            QMessageBox::information(nullptr, "Success", "Registration successful!");
        } else {
            throw std::runtime_error("Cannot open users.txt for appending");
        }
    } catch (const std::runtime_error& e) {
        QMessageBox::warning(nullptr, "Error", QString("File operation failed: %1").arg(e.what()));
    }
}
