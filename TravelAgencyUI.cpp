//
// Created by GBT B450M-S2H on 21.04.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TravelAgencyUI.h" resolved

#include "TravelAgencyUI.h"
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>
#include <QAbstractButton>
#include <QTranslator>
#include <QInputDialog>

TravelAgencyUI::TravelAgencyUI(TravelAgency *_travelAgency, QWidget *parent) : QMainWindow(parent),
                                                                               ui(new Ui::TravelAgencyUI),
                                                                               travelAgency(_travelAgency) {

    ui->setupUi(this);
    connect(ui->actionEinlesen, SIGNAL(triggered()), this, SLOT(onFileOpen()));
    connect(ui->actionSuchen, SIGNAL(triggered(bool)), this, SLOT(onCustomerSearch()));
    connect(ui->customerTravelsTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem * )), this,
            SLOT(onCustomerTravelListDoubleClicked()));
    connect(ui->travelBookingsTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem * )), this,
            SLOT(onTravelBookingListDoubleClicked()));
    connect(ui->saveBookingButton, SIGNAL(clicked(bool)), this, SLOT(onSaveBookingsButtonClicked()));
    connect(ui->cancelBookingButton, SIGNAL(clicked(bool)), this, SLOT(onCancelBookingsButtonClicked()));

    connect(ui->bookingStartDateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(onStartDateChanged()));
    connect(ui->bookingEndDateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(onEndDateChanged()));
    connect(ui->bookingPriceEdit, SIGNAL(valueChanged(double)), this, SLOT(onPriceChanged()));

    connect(ui->flightStartLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onStartAirportChanged()));
    connect(ui->flightEndLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onEndAirportChanged()));
    connect(ui->flightAirlineLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onAirlineChanged()));
    connect(ui->flightClassLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onBookingClassChanged()));

    connect(ui->hotelNameLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onHotelNameChanged()));
    connect(ui->hotelTownLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onHotelTownChanged()));
    connect(ui->hotelRoomLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onHotelRoomTypeChanged()));

    connect(ui->rentalPickupLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onPickupLocationChanged()));
    connect(ui->rentalReturnLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onReturnLocationChanged()));
    connect(ui->rentalCompanyLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onCompanyChanged()));
    connect(ui->rentalClassLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onVehicleClassChanged()));


    ui->customerGroupBox->setVisible(false);
    ui->customerTravelsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->customerTravelsTableWidget->setColumnWidth(0, 100);
    ui->customerTravelsTableWidget->setColumnWidth(1, 200);
    ui->customerTravelsTableWidget->setColumnWidth(2, 200);

    ui->travelGroupBox->setVisible(false);
    ui->travelBookingsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->travelBookingsTableWidget->setColumnWidth(0, 90);
    ui->travelBookingsTableWidget->setColumnWidth(1, 160);
    ui->travelBookingsTableWidget->setColumnWidth(2, 160);
    ui->travelBookingsTableWidget->setColumnWidth(3, 90);

    ui->bookingGroupBox->setVisible(false);


}


TravelAgencyUI::~TravelAgencyUI() {
    delete ui;
}

void TravelAgencyUI::onFileOpen() {
    QString filePath = QFileDialog::getOpenFileName(this, "Datei auswählen", "*.json");
    travelAgency->readFile(filePath.toStdString());

}

void TravelAgencyUI::onCustomerSearch() {
    msgBox = new QMessageBox();
    long customerId = QInputDialog::getInt(this, "Kund*Innensuche", "ID");
    Customer *customer = travelAgency->findCustomer(customerId);
    if (customer == nullptr) {
        msgBox->setWindowTitle("Kunde nicht gefunden");
        msgBox->setText("Der Kunde mit der ID " + QString::fromStdString(std::to_string(customerId)) +
                        " konnte nicht gefunden werden");
        msgBox->exec();
        return;
    }
    ui->customerGroupBox->setVisible(true);
    ui->customerIdLineEdit->setText(QString::fromStdString(std::to_string(customer->getId())));
    ui->customerNameLineEdit->setText(QString::fromStdString(customer->getName()));

    loadTravels(customerId);
    ui->travelGroupBox->setVisible(false);
    ui->bookingGroupBox->setVisible(false);


}

void TravelAgencyUI::loadTravels(long customerId) {
    ui->customerTravelsTableWidget->clearContents();
    QLocale locale(QLocale::German);
    int currentRow = 0;
    for (auto t: travelAgency->getAllTravels()) {
        if (t->getCustomerId() == customerId) {

            ui->customerTravelsTableWidget->setRowCount(currentRow + 1);
            ui->customerTravelsTableWidget->setItem(currentRow, TRAVEL_ID, new QTableWidgetItem(
                    QString::fromStdString(std::to_string(t->getId()))));
            ui->customerTravelsTableWidget->setItem(currentRow, TRAVEL_BEGIN, new QTableWidgetItem(
                    locale.toString(t->getStartDate(), "dddd, d. MMMM yyyy")));
            ui->customerTravelsTableWidget->setItem(currentRow, TRAVEL_END, new QTableWidgetItem(
                    locale.toString(t->getEndDate(), "dddd, d. MMMM yyyy")));
            currentRow = ui->customerTravelsTableWidget->rowCount();
        }
    }
}

void TravelAgencyUI::onCustomerTravelListDoubleClicked() {
    long travelId = ui->customerTravelsTableWidget->item(ui->customerTravelsTableWidget->currentRow(),
                                                         0)->text().toLong();
    loadBookings(travelId);
    ui->bookingGroupBox->setVisible(false);
}

void TravelAgencyUI::loadBookings(long travelId) {
    QLocale locale(QLocale::German);
    int currentRow = 0;
    ui->travelGroupBox->setVisible(true);
    ui->travelBookingsTableWidget->clearContents();
    ui->travelIdLineEdit->setText(QString::number(travelId));
    bookingIndices.clear();
    int index = 0;
    for (auto b: travelAgency->getAllBookings()) {
        if (b->getTravelId() == travelId) {
            ui->travelBookingsTableWidget->setRowCount(currentRow + 1);
            QTableWidgetItem *imageItem = new QTableWidgetItem();
            imageItem->setIcon(getBookingIcon(b));
            ui->travelBookingsTableWidget->setItem(currentRow, BOOKING_TYPE, imageItem);
            ui->travelBookingsTableWidget->setItem(currentRow, BOOKING_START, new QTableWidgetItem(
                    locale.toString(b->getFromDate(), "dddd, d. MMMM yyyy")));
            ui->travelBookingsTableWidget->setItem(currentRow, BOOKING_END, new QTableWidgetItem(
                    locale.toString(b->getToDate(), "dddd, d. MMMM yyyy")));
            ui->travelBookingsTableWidget->setItem(currentRow, BOOKING_PRICE,
                                                   new QTableWidgetItem(QString::number(b->getPrice())));
            currentRow = ui->travelBookingsTableWidget->rowCount();

            bookingIndices[index] = b;
            index++;
        }
    }
}

QIcon TravelAgencyUI::getBookingIcon(Booking *booking) {

    if (FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(booking)) {
        return QIcon("../Images/Flight.png");
    } else if (HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(booking)) {
        return QIcon("../Images/Hotel.png");
    } else if (RentalCarReservation *rentalCarReservation = dynamic_cast<RentalCarReservation *>(booking)) {
        return QIcon("../Images/Car.png");
    }


}

void TravelAgencyUI::onTravelBookingListDoubleClicked() {
    activeBooking = bookingIndices[ui->travelBookingsTableWidget->currentRow()];
    loadBookingDetails();
}

void TravelAgencyUI::loadBookingDetails() {
    ui->bookingGroupBox->setVisible(true);
    ui->bookingIdLineEdit->setText(QString::fromStdString(activeBooking->getId()));
    ui->bookingStartDateEdit->setDate(activeBooking->getFromDate());
    ui->bookingEndDateEdit->setDate(activeBooking->getToDate());
    ui->bookingPriceEdit->setValue(activeBooking->getPrice());

    if (FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->flightTab);
        ui->hotelTab->setEnabled(false);
        ui->rentalTab->setEnabled(false);
        ui->flightStartLineEdit->setText(QString::fromStdString(flightBooking->getFromDestination()));
        ui->flightEndLineEdit->setText(QString::fromStdString(flightBooking->getToDestination()));
        ui->flightAirlineLineEdit->setText(QString::fromStdString(flightBooking->getAirline()));
        ui->flightClassLineEdit->setText(QString::fromStdString(flightBooking->getBookingClass()));
    } else if (HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->hotelTab);
        ui->flightTab->setEnabled(false);
        ui->rentalTab->setEnabled(false);
        ui->hotelNameLineEdit->setText(QString::fromStdString(hotelBooking->getHotel()));
        ui->hotelTownLineEdit->setText(QString::fromStdString(hotelBooking->getTown()));
        ui->hotelRoomLineEdit->setText(QString::fromStdString(hotelBooking->getRoomType()));
    } else if (RentalCarReservation *rentalCarReservation = dynamic_cast<RentalCarReservation *>(activeBooking)) {
        ui->bookingTabWidget->setCurrentWidget(ui->rentalTab);
        ui->flightTab->setEnabled(false);
        ui->hotelTab->setEnabled(false);
        ui->rentalPickupLineEdit->setText(QString::fromStdString(rentalCarReservation->getPickupLocation()));
        ui->rentalReturnLineEdit->setText(QString::fromStdString(rentalCarReservation->getReturnLocation()));
        ui->rentalCompanyLineEdit->setText(QString::fromStdString(rentalCarReservation->getCompany()));
        ui->rentalClassLineEdit->setText(QString::fromStdString(rentalCarReservation->getVehicleClass()));
    }
}


void TravelAgencyUI::onSaveBookingsButtonClicked() {
    Booking *booking = travelAgency->findBooking(ui->bookingIdLineEdit->text().toStdString());
    booking->setFromDate(ui->bookingStartDateEdit->date().toString("yyyyMMdd").toStdString());
    booking->setToDate(ui->bookingEndDateEdit->date().toString("yyyyMMdd").toStdString());
    booking->setPrice(ui->bookingPriceEdit->value());

    if (FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(booking)) {
        flightBooking->setFromDestination(ui->flightStartLineEdit->text().toStdString());
        flightBooking->setToDestination(ui->flightEndLineEdit->text().toStdString());
        flightBooking->setAirline(ui->flightAirlineLineEdit->text().toStdString());
        flightBooking->setBookingClass(ui->flightClassLineEdit->text().toStdString());
    } else if (HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(booking)) {
        hotelBooking->setHotel(ui->hotelNameLineEdit->text().toStdString());
        hotelBooking->setTown(ui->hotelTownLineEdit->text().toStdString());
        hotelBooking->setRoomType(ui->hotelRoomLineEdit->text().toStdString());
    } else if (RentalCarReservation *rentalCarReservation = dynamic_cast<RentalCarReservation *>(booking)) {
        rentalCarReservation->setPickupLocation(ui->rentalPickupLineEdit->text().toStdString());
        rentalCarReservation->setReturnLocation(ui->rentalReturnLineEdit->text().toStdString());
        rentalCarReservation->setCompany(ui->rentalCompanyLineEdit->text().toStdString());
        rentalCarReservation->setVehicleClass(ui->rentalClassLineEdit->text().toStdString());
    }
    loadTravels(ui->customerIdLineEdit->text().toLong());
    loadBookings(ui->travelIdLineEdit->text().toLong());

    ui->saveBookingButton->setEnabled(false);
    ui->cancelBookingButton->setEnabled(false);

}


void TravelAgencyUI::onCancelBookingsButtonClicked() {
    loadBookingDetails();
    ui->saveBookingButton->setEnabled(false);
    ui->cancelBookingButton->setEnabled(false);
}

void TravelAgencyUI::onStartDateChanged() {
    ui->saveBookingButton->setEnabled(activeBooking->getFromDate() != ui->bookingStartDateEdit->date());
    ui->cancelBookingButton->setEnabled(activeBooking->getFromDate() != ui->bookingStartDateEdit->date());
}

void TravelAgencyUI::onEndDateChanged() {
    ui->saveBookingButton->setEnabled(activeBooking->getToDate() == ui->bookingEndDateEdit->date());
    ui->cancelBookingButton->setEnabled(activeBooking->getToDate() == ui->bookingEndDateEdit->date());
}

void TravelAgencyUI::onPriceChanged() {
    ui->saveBookingButton->setEnabled(activeBooking->getPrice() != ui->bookingPriceEdit->value());
    ui->cancelBookingButton->setEnabled(activeBooking->getPrice() != ui->bookingPriceEdit->value());
}

void TravelAgencyUI::onStartAirportChanged() {
    FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(activeBooking);
    ui->saveBookingButton->setEnabled(flightBooking->getFromDestination() != ui->flightStartLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(flightBooking->getFromDestination() != ui->flightStartLineEdit->text().toStdString());
}

void TravelAgencyUI::onEndAirportChanged() {
    FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(activeBooking);
    ui->saveBookingButton->setEnabled(flightBooking->getToDestination() != ui->flightEndLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(flightBooking->getToDestination() != ui->flightEndLineEdit->text().toStdString());
}

void TravelAgencyUI::onAirlineChanged() {
    FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(activeBooking);
    ui->saveBookingButton->setEnabled(flightBooking->getAirline() != ui->flightAirlineLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(flightBooking->getAirline() != ui->flightAirlineLineEdit->text().toStdString());
}

void TravelAgencyUI::onBookingClassChanged() {
    FlightBooking *flightBooking = dynamic_cast<FlightBooking *>(activeBooking);
    ui->saveBookingButton->setEnabled(flightBooking->getBookingClass() != ui->flightClassLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(flightBooking->getBookingClass() != ui->flightClassLineEdit->text().toStdString());
}

void TravelAgencyUI::onHotelNameChanged() {
    HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(activeBooking);
    ui->saveBookingButton->setEnabled(hotelBooking->getHotel() != ui->hotelNameLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(hotelBooking->getHotel() != ui->hotelNameLineEdit->text().toStdString());
}

void TravelAgencyUI::onHotelTownChanged() {
    HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(activeBooking);
    ui->saveBookingButton->setEnabled(hotelBooking->getTown() != ui->hotelTownLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(hotelBooking->getTown() != ui->hotelTownLineEdit->text().toStdString());
}

void TravelAgencyUI::onHotelRoomTypeChanged() {
    HotelBooking *hotelBooking = dynamic_cast<HotelBooking *>(activeBooking);
    ui->saveBookingButton->setEnabled(hotelBooking->getRoomType() != ui->hotelRoomLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(hotelBooking->getRoomType() != ui->hotelRoomLineEdit->text().toStdString());
}

void TravelAgencyUI::onPickupLocationChanged() {
    RentalCarReservation* rentalCarReservation = dynamic_cast<RentalCarReservation*>(activeBooking);
    ui->saveBookingButton->setEnabled(rentalCarReservation->getPickupLocation() != ui->rentalPickupLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(rentalCarReservation->getPickupLocation() != ui->rentalPickupLineEdit->text().toStdString());
}

void TravelAgencyUI::onReturnLocationChanged() {
    RentalCarReservation* rentalCarReservation = dynamic_cast<RentalCarReservation*>(activeBooking);
    ui->saveBookingButton->setEnabled(rentalCarReservation->getReturnLocation() != ui->rentalReturnLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(rentalCarReservation->getReturnLocation() != ui->rentalReturnLineEdit->text().toStdString());
}

void TravelAgencyUI::onCompanyChanged() {
    RentalCarReservation* rentalCarReservation = dynamic_cast<RentalCarReservation*>(activeBooking);
    ui->saveBookingButton->setEnabled(rentalCarReservation->getCompany() != ui->rentalCompanyLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(rentalCarReservation->getCompany() != ui->rentalCompanyLineEdit->text().toStdString());
}

void TravelAgencyUI::onVehicleClassChanged() {
    RentalCarReservation* rentalCarReservation = dynamic_cast<RentalCarReservation*>(activeBooking);
    ui->saveBookingButton->setEnabled(rentalCarReservation->getVehicleClass() != ui->rentalClassLineEdit->text().toStdString());
    ui->cancelBookingButton->setEnabled(rentalCarReservation->getVehicleClass() != ui->rentalClassLineEdit->text().toStdString());
}




