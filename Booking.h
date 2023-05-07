//
// Created by GBT B450M-S2H on 14.04.2023.
//

#ifndef PG2PRAKTIKUM1_BOOKING_H
#define PG2PRAKTIKUM1_BOOKING_H
#include <iostream>


class Booking {
protected:
    std::string id;
    double price;
    std::string fromDate;
    std::string toDate;
    long travelId;

public:
    Booking(std::string _id, double _price, std::string _fromDate, std::string _toDate, long _travelId);
    virtual ~Booking();
    virtual std::string showDetails() = 0;

    const std::string &getId() const;

    double getPrice() const;

    const std::string &getFromDate() const;

    const std::string &getToDate() const;

    long getTravelId() const;

};


#endif //PG2PRAKTIKUM1_BOOKING_H
