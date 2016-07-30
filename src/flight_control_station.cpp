/***********************************************************************
 *  Software License Agreement (BSD License)
 *
 *  Copyright (c) 2016, Team MicroDynamics
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of the Team MicroDynamics nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 ***********************************************************************

 ***********************************************************************
 *  History:
 *  <Authors>        <Date>        <Operation>
 *  myyerrol         2016.7.12     Finish the basic gui
 *
 *  Description:
 *  This .cpp file implements the gui of flight control station.
 **********************************************************************/

#include <QDebug>
#include <QKeyEvent>
#include <flight_control_station.h>
#include <ui_flight_control_station.h>

FlightControlStation::FlightControlStation(QWidget *parent,
                                           std::string serial_url,
                                           std::string config_addr) :
    QMainWindow(parent),
    ui(new Ui::FlightControlStation),
    serial_interface_(serial_url, config_addr)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Flight Control Station V1"));

    this->setFixedSize(width(), height());
    this->setFocus();

    timer_ = new QTimer(this);
    timer_->start(1000);

    flight_attitude_indicator_ = new FlightAttitudeIndicator(this);
    flight_altitude_indicator_ = new FlightAltitudeIndicator(this);
    flight_compass_indicator_  = new FlightCompassIndicator(this);

    ui->verticalLayoutAttitude->addWidget(flight_attitude_indicator_);
    ui->verticalLayoutAltitude->addWidget(flight_altitude_indicator_);
    ui->verticalLayoutCompass->addWidget(flight_compass_indicator_);

    connect(timer_, SIGNAL(timeout()), this, SLOT(updateTimerOperation()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionAbout, SIGNAL(triggered()), this,
            SLOT(openAboutWidget()));

    count_ = 0;

    acc_x_actual_ = acc_y_actual_ = acc_z_actual_ = 0.0;
    att_r_actual_ = att_p_actual_ = att_y_actual_ = 0.0;
    acc_x_target_ = acc_y_target_ = acc_z_target_ = 0.0;
    att_r_target_ = att_p_target_ = att_y_target_ = 0.0;

    motor_thrust_acutal_ = motor_thrust_target_ = 0.0;

    robot_alt_actual_ = robot_alt_target_ = 0.0;
    robot_hei_actual_ = robot_hei_target_ = 0.0;

    battery_capacity_ = 0.0;

    motor_speed_actual_.resize(4, 0.0);
    motor_speed_target_.resize(4, 0.0);

    motor_mileage_actual_.resize(4, 0.0);
    motor_mileage_target_.resize(4, 0.0);

    if (serial_interface_.getFlagInit()) {
        std::cout << "Initialize system successfully, ready for communication!"
                  << std::endl;
    }
    else {
        std::cerr << "Failed to initialized system, check the hardware!"
                  << std::endl;
    }
}

FlightControlStation::~FlightControlStation()
{
    delete ui;
}

void FlightControlStation::openAboutWidget(void)
{
    about_widget_ = new QMessageBox(this);
    about_widget_->setWindowTitle(tr("About"));
    about_widget_->setText(
        QString("Breeze quadcopter flight control station can implement ") +
        QString("the real-time monitoring and control on the computer ")   +
        QString("client.\n\n\t\t\t                Team MicroDynamics"));
    about_widget_->setStandardButtons(QMessageBox::Yes);
    about_widget_->setDefaultButton(QMessageBox::Yes);
    about_widget_->exec();
}

void FlightControlStation::updateTimerOperation(void)
{
    serial_interface_.checkShakeHandState();
    serial_interface_.updateCommandState(READ_ROBOT_IMU, count_);
    serial_interface_.updateCommandState(READ_MOTOR_SPEED, count_);
    serial_interface_.updateCommandState(READ_MOTOR_MILEAGE, count_);
    serial_interface_.updateCommandState(READ_MOTOR_THRUST, count_);
    serial_interface_.updateCommandState(READ_ROBOT_HEIGHT, count_);
    serial_interface_.updateCommandState(READ_ROBOT_SYSTEM_INFO, count_);

    updateBufferRead();
    updateUIFromRead();

    count_++;
}

void FlightControlStation::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_W: {
        }
        case Qt::Key_S: {
            break;
        }
        case Qt::Key_A: {
            break;
        }
        case Qt::Key_D: {
            break;
        }
        case Qt::Key_Q: {
            break;
        }
        case Qt::Key_E: {
            break;
        }
        case Qt::Key_U: {
            break;
        }
        case Qt::Key_I: {
            break;
        }
        case Qt::Key_J: {
            break;
        }
        case Qt::Key_K: {
            break;
        }
        default: {
            break;
        }
    }
}

void FlightControlStation::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void FlightControlStation::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

void FlightControlStation::updateBufferRead(void)
{
    acc_x_actual_ =
        serial_interface_.getDataType()->robot_imu_actual_.acc.acc_x;
    acc_y_actual_ =
        serial_interface_.getDataType()->robot_imu_actual_.acc.acc_y;
    acc_z_actual_ =
        serial_interface_.getDataType()->robot_imu_actual_.acc.acc_z;

    att_r_actual_ =
        serial_interface_.getDataType()->robot_imu_actual_.att.att_r;
    att_p_actual_ =
        serial_interface_.getDataType()->robot_imu_actual_.att.att_p;
    att_y_actual_ =
        serial_interface_.getDataType()->robot_imu_actual_.att.att_y;

    motor_speed_actual_[0] =
        serial_interface_.getDataType()->motor_speed_actual_.motor_a;
    motor_speed_actual_[1] =
        serial_interface_.getDataType()->motor_speed_actual_.motor_b;
    motor_speed_actual_[2] =
        serial_interface_.getDataType()->motor_speed_actual_.motor_c;
    motor_speed_actual_[3] =
        serial_interface_.getDataType()->motor_speed_actual_.motor_d;

    motor_mileage_actual_[0] =
        serial_interface_.getDataType()->motor_mileage_actual_.motor_a;
    motor_mileage_actual_[1] =
        serial_interface_.getDataType()->motor_mileage_actual_.motor_b;
    motor_mileage_actual_[2] =
        serial_interface_.getDataType()->motor_mileage_actual_.motor_c;
    motor_mileage_actual_[3] =
        serial_interface_.getDataType()->motor_mileage_actual_.motor_d;

    motor_thrust_acutal_ =
        serial_interface_.getDataType()->motor_thrust_actual_.thrust;

    robot_alt_actual_ =
        serial_interface_.getDataType()->robot_height_actual_.alt;
    robot_hei_actual_ =
        serial_interface_.getDataType()->robot_height_actual_.hei;

    battery_capacity_ =
    serial_interface_.getDataType()->robot_system_info_actual_.battery_capacity;
}

void FlightControlStation::updateBufferWrite(void)
{
    serial_interface_.getDataType()->robot_imu_target_.acc.acc_x =
        acc_x_target_;
    serial_interface_.getDataType()->robot_imu_target_.acc.acc_y =
        acc_y_target_;
    serial_interface_.getDataType()->robot_imu_target_.acc.acc_z =
        acc_z_target_;

    serial_interface_.getDataType()->robot_imu_target_.att.att_r =
        att_r_target_;
    serial_interface_.getDataType()->robot_imu_target_.att.att_p =
        att_p_target_;
    serial_interface_.getDataType()->robot_imu_target_.att.att_y =
        att_y_target_;

    serial_interface_.getDataType()->motor_speed_target_.motor_a =
        motor_speed_target_[0];
    serial_interface_.getDataType()->motor_speed_target_.motor_b=
        motor_speed_target_[1];
    serial_interface_.getDataType()->motor_speed_target_.motor_c =
        motor_speed_target_[2];
    serial_interface_.getDataType()->motor_speed_target_.motor_d =
        motor_speed_target_[3];

    serial_interface_.getDataType()->motor_thrust_target_.thrust =
        motor_thrust_target_;

    serial_interface_.getDataType()->robot_height_target_.alt =
        robot_alt_target_;
    serial_interface_.getDataType()->robot_height_target_.hei =
        robot_hei_target_;
}

void FlightControlStation::updateUIFromRead(void)
{
    float value_roll;
    float value_pitch;
    float value_yaw;
    float value_alt;
    float value_hei;

    QString att_r, att_p, att_y;
    QString alt, hei;
    QString thrust;
    QString motor_a, motor_b, motor_c, motor_d;
    QString battery_capacity;

    att_r.sprintf("%.2f", att_r_actual_);
    att_p.sprintf("%.2f", att_p_actual_);
    att_y.sprintf("%.2f", att_y_actual_);
    alt.sprintf("%.2f", robot_alt_actual_);
    hei.sprintf("%.2f", robot_hei_actual_);

    ui->lineEditDataRoll->setText(att_r);
    ui->lineEditDataPitch->setText(att_p);
    ui->lineEditDataYaw->setText(att_y);
    ui->lineEditDataAltitude->setText(alt);
    ui->lineEditDataHeight->setText(hei);

    value_roll  = flight_attitude_indicator_->getRoll();
    value_pitch = flight_attitude_indicator_->getPitch();
    value_yaw   = flight_compass_indicator_->getYaw();
    value_alt   = flight_altitude_indicator_->getAltitude();
    value_hei   = flight_altitude_indicator_->getHeight();

    flight_attitude_indicator_->setRoll(value_roll + att_r_actual_);
    flight_attitude_indicator_->setPitch(value_pitch + att_p_actual_);
    flight_compass_indicator_->setYaw(value_yaw + att_y_actual_);
    flight_altitude_indicator_->setAltitude(value_alt + robot_alt_actual_);
    flight_altitude_indicator_->setHeight(value_hei + robot_hei_actual_);
}

void FlightControlStation::updateUIFromWrite(void)
{
}
