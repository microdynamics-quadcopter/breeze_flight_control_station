/******************************************************************************
 * Software License Agreement (GPL V3 License)
 *
 * Copyright (C) 2014 bushuhui
 * Copyright (c) 2016 myyerrol(Team MicroDynamics)
 *
 * This file is part of breeze_flight_control_station.
 *
 * breeze_flight_control_station is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.

 * breeze_flight_control_station is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with breeze_flight_control_station.  If not, see
 * <http://www.gnu.org/licenses/>.
 ******************************************************************************

 ******************************************************************************
 * File:
 * fcs_indicator_altitude.cpp
 *
 * Author:
 * bushuhui<bushuhui@gmail.com>
 * myyerrol<myyerrol@126.com>
 *
 * History:
 * <Authors>     <Date>         <Version>     <Operation>
 * bushuhui      2014.12.03     --           Finish altitude indicator
 * myyerrol      2016.07.12     1.0.0        Modify altitude indicator
 *
 * Description:
 * This .cpp file implements the indicator of altitude according to
 * qFlightInstruments project.
 *****************************************************************************/

#include <QString>
#include <QKeyEvent>
#include <QPen>
#include <QPainter>
#include <QRegion>
#include <QtMath>
#include <QDebug>
#include <fcs_indicator_altitude.h>

FlightAltitudeIndicator::FlightAltitudeIndicator(QWidget *parent) :
    QWidget(parent)
{
    widget_size_min_    = 200;
    widget_size_max_    = 600;
    widget_size_offset_ = 2;
    widget_size_curr_   = widget_size_min_ - 2 * widget_size_offset_;

    setMinimumSize(widget_size_min_, widget_size_min_);
    setMaximumSize(widget_size_max_, widget_size_max_);
    resize(widget_size_min_, widget_size_min_);

    setFocusPolicy(Qt::NoFocus);

    altitude_ = 0.0;
    height_   = 0.0;

    connect(this, SIGNAL(replotCanvas(void)), this, SLOT(updateCanvas(void)));
}

FlightAltitudeIndicator::~FlightAltitudeIndicator()
{
}

void FlightAltitudeIndicator::setAltitudeAndHeight(double altitude,
                                                   double height)
{
    altitude_ = altitude;
    height_   = height;
    emit replotCanvas();
}

void FlightAltitudeIndicator::setAltitude(double altitude)
{
    altitude_ = altitude;
    emit replotCanvas();
}

void FlightAltitudeIndicator::setHeight(double height)
{
    height_ = height;
    emit replotCanvas();
}

double FlightAltitudeIndicator::getAltitude(void)
{
    return altitude_;
}

double FlightAltitudeIndicator::getHeight(void)
{
    return height_;
}

void FlightAltitudeIndicator::updateCanvas(void)
{
    update();
}

void FlightAltitudeIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen_black(Qt::black);
    QBrush brush_sky(QColor(48, 172, 220));

    pen_black.setWidth(2);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);

    // Draw the background's color.
    do {
        painter.setPen(pen_black);
        painter.setBrush(brush_sky);
        painter.drawEllipse(-widget_size_curr_ / 2,
                            -widget_size_curr_ / 2,
                             widget_size_curr_,
                             widget_size_curr_);
    } while (false);

    // Draw the background's numbers and lines.
    do {
        int     yaw_line_number = 36;
        int     yaw_line_length = widget_size_curr_ / 25;
        int     font_size       = 10;
        double  unit_angular    = 360.0 / yaw_line_number;
        double  fx_a, fx_b, fy_a, fy_b;
        QString string;

        pen_black.setWidth(2);
        painter.setPen(pen_black);
        painter.setFont(QFont("", font_size));

        for (int i = 0; i < yaw_line_number; i++) {
            string = QString("%1").arg(i);

            fx_a = 0.0;
            fy_a = -widget_size_curr_ / 2 + widget_size_offset_;
            fx_b = 0.0;

            if (i % 3 == 0) {
                fy_b = fy_a + yaw_line_length;
                painter.drawLine(QPointF(fx_a, fy_a), QPointF(fx_b, fy_b));
                fy_b = fy_a + yaw_line_length + 4;
                painter.drawText(QRectF(-50, fy_b, 100, font_size + 2),
                                 Qt::AlignCenter, string);
            }
            else {
                fy_b = fy_a + yaw_line_length / 2;
                painter.drawLine(QPointF(fx_a, fy_a), QPointF(fx_b, fy_b));
            }

            painter.rotate(-unit_angular);
        }
    } while (false);

    // Draw the arrow.
    do {
        int    arrow_size = widget_size_curr_ / 12;
        double fx_a, fx_b, fx_c, fy_a, fy_b, fy_c;

        painter.rotate(-100 * altitude_);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(QColor(Qt::red)));

        fx_a = 0;
        fy_a = -widget_size_curr_ / 2.5 + widget_size_offset_;
        fx_b = fx_a - arrow_size / 2;
        fy_b = fy_a + arrow_size;
        fx_c = fx_a + arrow_size / 2;
        fy_c = fy_a + arrow_size;

        QPointF points[3] = {
            QPointF(fx_a, fy_a),
            QPointF(fx_b, fy_b),
            QPointF(fx_c, fy_c)
        };

        painter.drawPolygon(points, 3);
        painter.rotate(100 * altitude_);
    } while (false);

    // Draw the text.
    do {
        int    font_size = 15;
        double fx_a, fy_a;

        fx_a = -50;
        fy_a = -widget_size_curr_ / 3.5 + widget_size_offset_;

        painter.setPen(pen_black);
        painter.setBrush(Qt::NoBrush);
        painter.setFont(QFont("", font_size));
        painter.drawText(QRectF(fx_a, fy_a, 100, font_size + 5),
                         Qt::AlignCenter, "x 0.1m");
    } while (false);

    // Draw the altitude and height.
    do {
        int     font_size = 15;
        int     fx, fy, width, height;
        QString string;

        width  = 120;
        height = (font_size + 10) * 2;
        fx     = - width  / 2;
        fy     = - height / 2;

        pen_black.setWidth(2);
        painter.setPen(pen_black);
        painter.setBrush(QBrush(Qt::white));
        painter.setFont(QFont("", font_size));
        painter.drawRect(fx, fy, width, height);

        if (altitude_ >= 9.99) {
            altitude_ = 9.99;
        }
        if (altitude_ <= -9.99) {
            altitude_ = -9.99;
        }
        if (height_ >= 9.99) {
            height_ = 9.99;
        }
        if (height_ <= -9.99) {
            height_ = -9.99;
        }

        string.sprintf("ALT: %.2lf m", altitude_);
        painter.drawText(QRectF(fx, fy + 2, width, height / 2),
                         Qt::AlignCenter, string);

        string.sprintf("HEI: %.2lf m", height_);
        painter.drawText(QRectF(fx, fy + height / 2, width, height / 2),
                         Qt::AlignCenter, string);
    } while (false);
}

void FlightAltitudeIndicator::resizeEvent(QResizeEvent *)
{
    widget_size_curr_ = qMin(width(), height()) - 2 * widget_size_offset_;
}

void FlightAltitudeIndicator::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_U: {
            altitude_ += VALUE_STEP_ALTITUDE;
            break;
        }
        case Qt::Key_I: {
            altitude_ -= VALUE_STEP_ALTITUDE;
            break;
        }
        case Qt::Key_J: {
            height_ += VALUE_STEP_HEIGHT;
            break;
        }
        case Qt::Key_K: {
            height_ -= VALUE_STEP_HEIGHT;
            break;
        }
        default: {
            QWidget::keyPressEvent(event);
            break;
        }
    }

    update();
}
