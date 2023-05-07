//
// Created by GBT B450M-S2H on 21.04.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TravelAgencyUI.h" resolved

#include "TravelAgencyUI.h"
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>
#include <QLineEdit>
#include <QAbstractButton>
#include <QTranslator>
#include <QInputDialog>

TravelAgencyUI::TravelAgencyUI(TravelAgency *_travelAgency, QWidget *parent) : QMainWindow(parent), ui(new Ui::TravelAgencyUI), travelAgency(_travelAgency) {
    ui->setupUi(this);
    connect(ui->actionEinlesen, SIGNAL(triggered()), this, SLOT(onFileOpen()));
    connect(ui->actionSuchen, SIGNAL(triggered(bool)), this, SLOT(onCustomerSearch()));
    //connect(ui->listWidget, &QListWidget::itemDoubleClicked(), this, SLOT(&onBookingListItemDoubleClicked()));
    //connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &TravelAgencyUI::onBookingListItemDoubleClicked);
    //this->setFixedSize(1200, 800);
    ui->tabWidget->setVisible(false);
    ui->customerGroupBox->setVisible(false);
    ui->customerTravelsTabWidget->setColumnWidth(0, 100);
    ui->customerTravelsTabWidget->setColumnWidth(1, 150);
    ui->customerTravelsTabWidget->setColumnWidth(2, 150);
    //ui->customerTravelsTabWidget->setRowCount(ui->customerTravelsTabWidget->rowCount() + 1);
    //ui->customerTravelsTabWidget->setItem(0, 1, new QTableWidgetItem("1234"));



}

TravelAgencyUI::~TravelAgencyUI() {
    delete ui;
}

void TravelAgencyUI::onFileOpen() {
    QString filePath = QFileDialog::getOpenFileName(this, "Datei auswählen", "*.json");
    travelAgency->readFile(filePath.toStdString());
    /*ui->listWidget->clear();
    addBookingToListWidget(travelAgency->getBookings());
     */


}

void TravelAgencyUI::onCustomerSearch() {
    msgBox = new QMessageBox();
    long customerId = QInputDialog::getInt(this, "Kund*Innensuche", "ID");
    Customer* customer = travelAgency->findCustomer(customerId);
    if(customer == nullptr){
        msgBox->setWindowTitle("Kunde nicht gefunden");
        msgBox->setText("Der Kunde mit der ID " + QString::fromStdString(std::to_string(customerId)) + " konnte nicht gefunden werden");
        msgBox->exec();
        return;
    }
    ui->customerGroupBox->setVisible(true);
    ui->customerIdLineEdit->setText(QString::fromStdString(std::to_string(customer->getId())));
    ui->customerNameLineEdit->setText(QString::fromStdString(customer->getName()));

    QLocale locale(QLocale::German);

    for(auto t : travelAgency->getAllTravels()){
        if(t->getCustomerId() == customerId){
            int currentRow = ui->customerTravelsTabWidget->rowCount();
            ui->customerTravelsTabWidget->setRowCount(currentRow + 1);
            ui->customerTravelsTabWidget->setItem(currentRow, TRAVEL_ID, new QTableWidgetItem(QString::fromStdString(std::to_string(t->getId()))));
            ui->customerTravelsTabWidget->setItem(currentRow, TRAVEL_BEGIN, new QTableWidgetItem(locale.toString(t->getStartDate(), "dddd, d. MMMM yyyy")));
            ui->customerTravelsTabWidget->setItem(currentRow, TRAVEL_END, new QTableWidgetItem(locale.toString(t->getEndDate(), "dddd, d. MMMM yyyy")));
        }
    }







    //long customerId = QInputDialog::getInt(this, "test", "1234");
    std::cout << customer->getName();
}

/*void TravelAgencyUI::addBookingToListWidget(std::vector<Booking *> bookings) {
    for(auto b : bookings){
        QString bookingStr = QString(QString::fromStdString(b->showDetails()));

        QListWidgetItem* item = new QListWidgetItem(bookingStr, ui->listWidget);
        QSharedPointer<Booking*> bookingPtr = QSharedPointer<Booking*>::create(b);
        QVariant bookingData;
        bookingData.setValue(bookingPtr);
        item->setData(Qt::UserRole, bookingData);
    }
}
 */

/*void TravelAgencyUI::onBookingListItemDoubleClicked(QListWidgetItem* item) {
    QVariant bookingData = item->data(Qt::UserRole);
    QSharedPointer<Booking*> bookingPtr = bookingData.value<QSharedPointer<Booking*>>();
    Booking* booking = *bookingPtr;
    if(FlightBooking* flightBooking = dynamic_cast<FlightBooking*>(booking)){
        ui->tabWidget->setCurrentWidget(ui->flightTab);
        ui->flightIdLineEdit->setText(QString::fromStdString(flightBooking->getId()));
        ui->flightFromDateEdit->setDate(QDate::fromString(QString::fromStdString(flightBooking->getFromDate()), "yyyyMMdd"));
        ui->flightToDateEdit->setDate(QDate::fromString(QString::fromStdString(flightBooking->getToDate()), "yyyyMMdd"));
        ui->fromAirportLineEdit->setText(QString::fromStdString(flightBooking->getFromDestination()));
        ui->toAirportLineEdit->setText(QString::fromStdString(flightBooking->getToDestination()));
        ui->airlineLineEdit->setText(QString::fromStdString(flightBooking->getAirline()));
        ui->flightPriceEdit->setValue(flightBooking->getPrice());
    }
    else if(HotelBooking* hotelBooking = dynamic_cast<HotelBooking*>(booking)){
        ui->tabWidget->setCurrentWidget(ui->hotelTab);
        ui->hotelIdLineEdit->setText(QString::fromStdString(hotelBooking->getId()));
        ui->hotelFromDateEdit->setDate(QDate::fromString(QString::fromStdString(hotelBooking->getFromDate()), "yyyyMMdd"));
        ui->hotelToDateEdit->setDate(QDate::fromString(QString::fromStdString(hotelBooking->getToDate()), "yyyyMMdd"));
        ui->hotelLineEdit->setText(QString::fromStdString(hotelBooking->getHotel()));
        ui->cityLineEdit->setText(QString::fromStdString(hotelBooking->getTown()));
        ui->hotelPriceEdit->setValue(hotelBooking->getPrice());
    }
    else if(RentalCarReservation* rentalCarReservation = dynamic_cast<RentalCarReservation*>(booking)){
        ui->tabWidget->setCurrentWidget(ui->rentalCarTab);
        ui->rentalCarIdLineEdit->setText(QString::fromStdString(rentalCarReservation->getId()));
        ui->rentalCarFromDateEdit->setDate(QDate::fromString(QString::fromStdString(rentalCarReservation->getFromDate()), "yyyyMMdd"));
        ui->rentalCarToDateEdit->setDate(QDate::fromString(QString::fromStdString(rentalCarReservation->getToDate()), "yyyyMMdd"));
        ui->pickupLocationLineEdit->setText(QString::fromStdString(rentalCarReservation->getPickupLocation()));
        ui->returnLocationLineEdit->setText(QString::fromStdString(rentalCarReservation->getReturnLocation()));
        ui->companyLineEdit->setText(QString::fromStdString(rentalCarReservation->getCompany()));
        ui->rentalCarPriceEdit->setValue(rentalCarReservation->getPrice());
    }

}
 */
