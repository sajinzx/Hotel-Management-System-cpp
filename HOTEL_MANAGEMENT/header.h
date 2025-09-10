#ifndef HOTEL_H
#define HOTEL_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QTextEdit>

class User {
private:
    QString username;
    QString password;
    QString hotelName;
    int totalRooms;
public:
    User();
    User(const QString& username, const QString& password, const QString& hotelName, int totalRooms);
    QString getUsername() const;
    QString getPassword() const;
    QString getHotelName() const;
    int getTotalRooms() const;
    void setUsername(const QString& username);
    void setPassword(const QString& password);
    void setHotelName(const QString& hotelName);
    void setTotalRooms(int totalRooms);
    friend QTextStream& operator<<(QTextStream& ofs, const User& user);
    friend QTextStream& operator>>(QTextStream& ifs, User& user);
};

class Guest {
private:
    QString name;
    QString contactInfo;
    int guestID;
public:
    Guest();
    Guest(const QString& name, const QString& contactInfo, int guestID);
    QString getName() const;
    QString getContactInfo() const;
    int getGuestID() const;
    void setName(const QString& name);
    void setContactInfo(const QString& contactInfo);
    void setGuestID(int guestID);
    friend QTextStream& operator<<(QTextStream& ofs, const Guest& guest);
    friend QTextStream& operator>>(QTextStream& ifs, Guest& guest);
};

class Room {
private:
    int roomNumber;
    QString status; // "Available", "Occupied", "Maintenance"
    double pricePerNight;
public:
    Room();
    Room(int roomNumber, double pricePerNight);
    int getRoomNumber() const;
    QString getStatus() const;
    double getPricePerNight() const;
    void setStatus(const QString& status);
    void setPricePerNight(double price);
    friend QTextStream& operator<<(QTextStream& ofs, const Room& room);
    friend QTextStream& operator>>(QTextStream& ifs, Room& room);
};

class Reservation {
private:
    int reservationID;
    Guest guest;
    Room room;
    QString checkInDate;
    QString checkOutDate;
public:
    Reservation();
    Reservation(int reservationID, const Guest& guest, Room& room, const QString& checkInDate, const QString& checkOutDate);
    int getReservationID() const;
    Guest getGuest() const;
    Room getRoom() const;
    QString getCheckInDate() const;
    QString getCheckOutDate() const;
    int daysBetween() const;
    double calculateTotalCost() const;
    friend QTextStream& operator<<(QTextStream& ofs, const Reservation& reservation);
    friend QTextStream& operator>>(QTextStream& ifs, Reservation& reservation);
};

class Hotel {
private:
    QVector<Room> rooms;
    QVector<Reservation> reservations;
    QVector<Guest> guests;
    User currentUser;
public:
    void addRoom(const Room& room);
    void addReservation(const Reservation& reservation);
    void addGuest(const Guest& guest);
    void displayAvailableRooms(QTextEdit* output) const;
    void displayReservations(QTextEdit* output) const;
    void displayGuests(QTextEdit* output) const;
    void saveData() const;
    void loadData();
    Room* findRoomByNumber(int roomNumber);
    Guest* findGuestByID(int guestID);
    void setCurrentUser(const User& user);
    User getCurrentUser() const;
};

bool login(User& user);
void registerUser(User& user);

#endif
