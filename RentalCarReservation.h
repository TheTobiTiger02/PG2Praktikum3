//
// Created by GBT B450M-S2H on 14.04.2023.
//

#ifndef PG2PRAKTIKUM1_RENTALCARRESERVATION_H
#define PG2PRAKTIKUM1_RENTALCARRESERVATION_H
#include <iostream>
#include "Booking.h"

class RentalCarReservation : public Booking{

private:
    std::string pickupLocation;
    std::string returnLocation;
    std::string company;
    std::string vehicleClass;

public:
    RentalCarReservation(std::string id, double price, std::string fromDate, std::string toDate, long travelId, std::string _pickupLocation, std::string _returnLocation, std::string _company, std::string _vehicleClass);
    ~RentalCarReservation();
    std::string showDetails();

    const std::string &getPickupLocation() const;

    const std::string &getReturnLocation() const;

    const std::string &getCompany() const;

    const std::string &getVehicleClass() const;
};


#endif //PG2PRAKTIKUM1_RENTALCARRESERVATION_H