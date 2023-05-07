//
// Created by GBT B450M-S2H on 07.05.2023.
//

#include "Travel.h"

Travel::Travel(long _id, long _customerId) : id(_id), customerId(_customerId) {

}

void Travel::addBooking(Booking *booking) {
    travelBookings.push_back(booking);
}

long Travel::getId() const {
    return id;
}

long Travel::getCustomerId() const {
    return customerId;
}

const std::vector<Booking *> &Travel::getTravelBookings() const {
    return travelBookings;
}

QDate Travel::getStartDate() {
    QDate startDate = QDate::fromString(QString::fromStdString(travelBookings.at(0)->getFromDate()), "yyyyMMdd");
    for(auto b : travelBookings){
        QDate newDate = QDate::fromString(QString::fromStdString(b->getFromDate()), "yyyyMMdd");
        if(newDate < startDate){
            startDate = newDate;
        }
    }
    return startDate;
}

QDate Travel::getEndDate() {
    QDate endDate = QDate::fromString(QString::fromStdString(travelBookings.at(0)->getToDate()), "yyyyMMdd");
    for(auto b : travelBookings){
        QDate newDate = QDate::fromString(QString::fromStdString(b->getToDate()), "yyyyMMdd");
        if(newDate > endDate){
            endDate = newDate;
        }
    }
    return endDate;
}
