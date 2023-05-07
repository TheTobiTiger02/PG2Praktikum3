//
// Created by GBT B450M-S2H on 14.04.2023.
//

#include "FlightBooking.h"
#include <iomanip>
using namespace std;

FlightBooking::FlightBooking(std::string id, double price, std::string fromDate, std::string toDate, long travelId,
                             std::string _fromDestination, std::string _toDestination, std::string _airline, char bookingChar) : Booking(id, price, fromDate, toDate, travelId), fromDestination(_fromDestination), toDestination(_toDestination), airline(_airline) {
    bookingClasses['Y'] = "Economy class";
    bookingClasses['W'] = "Premium Economy class";
    bookingClasses['J'] = "Business class";
    bookingClasses['F'] = "First class";



    bookingClass = bookingClasses[bookingChar];

}
FlightBooking::~FlightBooking() {
    std::cout << "FlightBooking object destroyed" << std::endl;
}

string FlightBooking::showDetails() {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << price;

    return "Flugbuchung von " + fromDestination + " nach " + toDestination + " mit " + airline +
    " am " + fromDate + " mit der Buchungsklasse " + bookingClass + ". Preis: " + ss.str() + " Euro";

}

const string &FlightBooking::getFromDestination() const {
    return fromDestination;
}

const string &FlightBooking::getToDestination() const {
    return toDestination;
}

const string &FlightBooking::getAirline() const {
    return airline;
}

std::string FlightBooking::getBookingClass() const {
    return bookingClass;
}

