//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "Booking.h"

Booking::Booking(std::string _id, double _price, std::string _fromDate, std::string _toDate, long _travelId) : id(_id), price(_price), fromDate(_fromDate), toDate(_toDate), travelId(_travelId) {
}
Booking::~Booking() {
    std::cout << "Booking object destroyed" << std::endl;
}

const std::string &Booking::getId() const {
    return id;
}

double Booking::getPrice() const {
    return price;
}

const std::string &Booking::getFromDate() const {
    return fromDate;
}

const std::string &Booking::getToDate() const {
    return toDate;
}

long Booking::getTravelId() const {
    return travelId;
}


