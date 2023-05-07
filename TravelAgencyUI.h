//
// Created by GBT B450M-S2H on 21.04.2023.
//

#ifndef PG2PRAKTIKUM2_MAINWINDOW_H
#define PG2PRAKTIKUM2_MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "TravelAgency.h"
#include "ui_TravelAgencyUI.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TravelAgencyUI; }
QT_END_NAMESPACE

class TravelAgencyUI : public QMainWindow {
Q_OBJECT

public:
    TravelAgencyUI(TravelAgency *_travelAgency, QWidget *parent = nullptr );
    ~TravelAgencyUI() override;
    void addBookingToListWidget(std::vector<Booking*> bookings);

private:

    enum columnIndices{TRAVEL_ID, TRAVEL_BEGIN, TRAVEL_END};
    QMessageBox* msgBox;
    TravelAgency *travelAgency;
    Ui::TravelAgencyUI *ui;

private slots:
    void onFileOpen();
    void onCustomerSearch();

    //void onBookingListItemDoubleClicked(QListWidgetItem* item);
};


#endif //PG2PRAKTIKUM2_MAINWINDOW_H
