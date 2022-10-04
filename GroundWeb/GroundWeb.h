#pragma once

#include <QtWidgets/QWidget>
#include "ui_GroundWeb.h"

class GroundWeb : public QWidget
{
    Q_OBJECT
public slots:
    void on_userInputField_returnPressed();

public:
    GroundWeb(QWidget *parent = nullptr);
    ~GroundWeb();

private:
    Ui::GroundWebClass ui;
};
