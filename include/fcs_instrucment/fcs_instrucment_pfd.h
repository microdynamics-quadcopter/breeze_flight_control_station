/******************************************************************************
 * Software License Agreement (GPL V3 License)
 *
 * Copyright (C) 2013 Marek M. Cel
 * Copyright (c) 2018 myyerrol(Team MicroDynamics)
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
 * fcs_instrucment_pfd.h
 *
 * Author:
 * Marek M. Cel<marekcel@marekcel.pl>
 * myyerrol<myyerrol@126.com>
 *
 * History:
 * <Authors>     <Date>         <Version>     <Operation>
 * Marek M. Cel  2013.--.--     --            Finish PFD instrument
 * myyerrol      2018.03.10     1.0.0         Modify PFD instrument
 *
 * Description:
 * This .h file defines some elements for implementing the instrument of
 * Primary Flight Display(PFD) according to QFlightInstruments project.
 *****************************************************************************/

#ifndef FCS_INSTRUCMENT_PFD_H
#define FCS_INSTRUCMENT_PFD_H

#include <QGraphicsView>
#include <QGraphicsSvgItem>

// Primary Flight Display Instrument Widget.
class FCSInstrucmentPFD : public QGraphicsView
{
    Q_OBJECT

public:
    // Altimeter pressure units.
    enum PressureUnit
    {
        STD, // Standard (displays STD instead of numerical value).
        MB,  // Milibars.
        IN   // Inches of mercury.
    };
public:
    explicit FCSInstrucmentPFD(QWidget *parent = 0);
    ~FCSInstrucmentPFD();
    // Reinitates widget.
    void reinitPFD(void);
    // Refreshes(redraws) widget.
    void updatePFD(void);
    // Set roll angle.
    // param1: roll angle [deg].
    inline void setPFDAngleRoll(float angle_roll)
    {
        pfd_panel_adi_->setADIAngleRoll(angle_roll);
    }
    // Set pitch angle.
    // param1: pitch angle [deg].
    inline void setPFDAnglePitch(float angle_pitch)
    {
        pfd_panel_adi_->setADIAnglePitch(angle_pitch);
    }
    // Set flight path marker.
    // param1: angle of attach [deg].
    // param2: angle of sideslip [deg].
    // param3: flight path marker visibility.
    inline void setPFDFlightPathMarker(float angle_attack,
                                       float angle_sideslip,
                                       bool visible = true)
    {
        pfd_panel_adi_->setADIFlightPathMarker(angle_attack, angle_sideslip,
                                               visible);
    }
    // Set slip or skid.
    // param1: normalized slip or skid(range from -1.0 to 1.0).
    inline void setPFDSlipSkid(float slip_skid)
    {
        pfd_panel_adi_->setADISlipSkid(slip_skid);
    }
    // Set turn rate.
    // param1: normalized turn rate (range from -1.0 to 1.0), hash marks
    // positions are set to be -0.5 and 0.5.
    inline void setPFDTurnRate(float turn_rate)
    {
        pfd_panel_adi_->setADITurnRate(turn_rate);
    }
    // Set horizontal deviation bar position.
    // param1: normalized horizontal deviation bar position (range from -1.0 to 1.0).
    // param2: horizontal deviation bar visibility.
    inline void setPFDDeviateBarPositionH(float bar_h, bool visible = true)
    {
        pfd_panel_adi_->setADIDeviateBarPositionH(bar_h, visible);
    }
    // Set vertical deviation bar position.
    // param1: normalized vertical deviation bar position (range from -1.0 to 1.0).
    // param2: vertical deviation bar visibility.
    inline void setPFDDeviateBarPositionV(float bar_v, bool visible = true)
    {
        pfd_panel_adi_->setADIDeviateBarPositionV(bar_v, visible);
    }
    // Set horizontal deviation dot position.
    // param1: normalized horizontal deviation dot position (range from -1.0 to 1.0).
    // param2: horizontal deviation dot visibility.
    inline void setPFDDeviateDotPositionH(float dot_pos, bool visible = true)
    {
        pfd_panel_adi_->setADIDeviateDotPositionH(dot_pos, visible);
    }
    // Set vertical deviation dot position.
    // param1: normalized vertical deviation dot position (range from -1.0 to 1.0).
    // param2: vertical deviation dot visibility.
    inline void setPFDDeviateDotPositionV(float dot_pos, bool visible = true)
    {
        pfd_panel_adi_->setADIDeviateDotPositionV(dot_pos, visible);
    }
    // Set Altitude.
    // param1: altitude (dimensionless numeric value).
    inline void setPFDAltitude(float altitude)
    {
        pfd_panel_alt_->setALTAltitude(altitude);
    }
    // Set pressure.
    // param1: pressure (dimensionless numeric value).
    // param2: pressure unit according to PressureUnit.
    inline void setPFDPressure(float pressure, PressureUnit pressure_unit)
    {
        pfd_panel_alt_->setALTPressure(pressure, pressure_unit);
    }
    // Set airspeed.
    // param1: airspeed (dimensionless numeric value).
    inline void setPFDAirspeed(float airspeed)
    {
        pfd_panel_asi_->setASIAirspeed(airspeed);
    }
    // Set Mach number.
    // param1: Mach number.
    inline void setPFDMachNumber(float mach_number)
    {
        pfd_panel_asi_->setASIMachNumber(mach_number);
    }
    // Set heading.
    // param1: heading [deg].
    inline void setPFDHeading(float heading)
    {
        pfd_panel_hsi_->setHSIHeading(heading);
    }
    // Set climb rate.
    // param1: climb rate (dimensionless numeric value).
    inline void setPFDClimbRate(float climb_rate)
    {
        pfd_panel_vsi_->setVSIClimbRate(climb_rate);
    }
protected:
    void resizeEvent(QResizeEvent *event);
private:
    // Attitude Director Indicator(ADI) Panel.
    class PanelADI
    {
    public:
        PanelADI(QGraphicsScene *scene);
        ~PanelADI();
        void initADI(float scale_x, float scale_y);
        void updateADI(float scale_x, float scale_y);
        void setADIAngleRoll(float angle_roll);
        void setADIAnglePitch(float angle_pitch);
        void setADIFlightPathMarker(float angle_attack, float angle_sideslip,
                                    bool visible = true );
        void setADISlipSkid(float slip_skid);
        void setADITurnRate(float turn_rate);
        void setADIDeviateBarPositionH(float bar_h, bool visible = true);
        void setADIDeviateBarPositionV(float bar_v, bool visible = true);
        void setADIDeviateDotPositionH(float dot_h, bool visible = true);
        void setADIDeviateDotPositionV(float dot_v, bool visible = true);
    private:
        void resetADI(void);
        void updateADILadd(float delta, float roll_sin, float roll_cos);
        void updateADILaddBack(float delta, float roll_sin, float roll_cos);
        void updateADIAngleRoll(void);
        void updateADiSlipSkip(float roll_sin, float roll_cos);
        void updateADITurnRate(void);
        void updateADIFlightPath(void);
        void updateADIBars(void);
        void updateADIDots(void);
    private:
        QGraphicsScene    *adi_scene_;
        QGraphicsSvgItem  *adi_item_back_;
        QGraphicsSvgItem  *adi_item_ladd_;
        QGraphicsSvgItem  *adi_item_roll_;
        QGraphicsSvgItem  *adi_item_slip_;
        QGraphicsSvgItem  *adi_item_turn_;
        QGraphicsSvgItem  *adi_item_path_;
        QGraphicsSvgItem  *adi_item_mark_;
        QGraphicsSvgItem  *adi_item_bar_h_;
        QGraphicsSvgItem  *adi_item_bar_v_;
        QGraphicsSvgItem  *adi_item_dot_h_;
        QGraphicsSvgItem  *adi_item_dot_v_;
        QGraphicsSvgItem  *adi_item_mask_;
        QGraphicsSvgItem  *adi_item_scale_h_;
        QGraphicsSvgItem  *adi_item_scale_v_;
        // ADI basic variables.
        float adi_angle_roll_;
        float adi_angle_pitch_;
        float adi_angle_attack_;
        float adi_angle_sideslip_;
        float adi_slip_skid_;
        float adi_turn_rate_;
        float adi_bar_h_;
        float adi_bar_v_;
        float adi_dot_h_;
        float adi_dot_v_;
        // ADI visible variables.
        bool adi_path_valid_;
        bool adi_path_visible_;
        bool adi_bar_h_visible_;
        bool adi_bar_v_visible_;
        bool adi_dot_h_visible_;
        bool adi_dot_v_visible_;
        // ADI delta variables.
        float adi_ladd_delta_x_new_;
        float adi_ladd_delta_x_old_;
        float adi_ladd_delta_y_new_;
        float adi_ladd_delta_y_old_;
        float adi_ladd_back_delta_x_new_;
        float adi_ladd_back_delta_x_old_;
        float adi_ladd_back_delta_y_new_;
        float adi_ladd_back_delta_y_old_;
        float adi_slip_delta_x_new_;
        float adi_slip_delta_x_old_;
        float adi_slip_delta_y_new_;
        float adi_slip_delta_y_old_;
        float adi_turn_delta_x_new_;
        float adi_turn_delta_x_old_;
        float adi_path_delta_x_new_;
        float adi_path_delta_x_old_;
        float adi_path_delta_y_new_;
        float adi_path_delta_y_old_;
        float adi_mark_delta_x_new_;
        float adi_mark_delta_x_old_;
        float adi_mark_delta_y_new_;
        float adi_mark_delta_y_old_;
        float adi_bar_h_delta_x_new_;
        float adi_bar_h_delta_x_old_;
        float adi_bar_v_delta_y_new_;
        float adi_bar_v_delta_y_old_;
        float adi_dot_h_delta_x_new_;
        float adi_dot_h_delta_x_old_;
        float adi_dot_v_delta_y_new_;
        float adi_dot_v_delta_y_old_;
        // ADI scale variables.
        float adi_scale_x_;
        float adi_scale_y_;
        // ADI deflection variables.
        const float adi_original_pix_per_deg_;
        const float adi_delta_ladd_back_max_;
        const float adi_delta_ladd_back_min_;
        const float adi_max_slip_deflection_;
        const float adi_max_turn_deflection_;
        const float adi_max_bars_deflection_;
        const float adi_max_dots_deflection_;
        // ADI original variables.
        QPointF adi_original_adi_ctr_;
        QPointF adi_original_back_pos_;
        QPointF adi_original_ladd_pos_;
        QPointF adi_original_roll_pos_;
        QPointF adi_original_slip_pos_;
        QPointF adi_original_turn_pos_;
        QPointF adi_original_path_pos_;
        QPointF adi_original_bar_h_pos_;
        QPointF adi_original_bar_v_pos_;
        QPointF adi_original_dot_h_pos_;
        QPointF adi_original_dot_v_pos_;
        QPointF adi_original_scale_h_pos_;
        QPointF adi_original_scale_v_pos_;
        // ADI z variables.
        const int adi_back_z_;
        const int adi_ladd_z_;
        const int adi_roll_z_;
        const int adi_slip_z_;
        const int adi_path_z_;
        const int adi_bars_z_;
        const int adi_dots_z_;
        const int adi_scales_z_;
        const int adi_mask_z_;
        const int adi_turn_z_;
    };
    // Altimeter(ALT) Panel.
    class PanelALT
    {
    public:
        PanelALT(QGraphicsScene *scene);
        ~PanelALT();
        void initALT(float scale_x, float scale_y);
        void updateALT(float scale_x, float scale_y);
        void setALTAltitude(float altitude);
        void setALTPressure(float pressure, int pressure_unit);
    private:
        void resetALT(void);
        void updateALTAltitude(void);
        void updateALTPressure(void);
        void updateALTScale(void);
        void updateALTScaleLabels(void);
    private:
        QGraphicsScene    *alt_scene_;
        QGraphicsSvgItem  *alt_item_back_;
        QGraphicsSvgItem  *alt_item_scale1_;
        QGraphicsSvgItem  *alt_item_scale2_;
        QGraphicsTextItem *alt_item_label1_;
        QGraphicsTextItem *alt_item_label2_;
        QGraphicsTextItem *alt_item_label3_;
        QGraphicsSvgItem  *alt_item_ground_;
        QGraphicsSvgItem  *alt_item_frame_;
        QGraphicsTextItem *alt_item_altitude_;
        QGraphicsTextItem *alt_item_pressure_;
        // ALT color variables.
        QColor alt_frame_text_color_;
        QColor alt_press_text_color_;
        QColor alt_labels_color_;
        // ALT font variables.
        QFont  alt_frame_text_font_;
        QFont  alt_labels_font_;
        // ALT float variables.
        float alt_altitude_;
        float alt_pressure_;
        // ALT pressure unit.
        int alt_pressure_unit_;
        // ALT delta variables.
        float alt_scale1_delta_y_new_;
        float alt_scale1_delta_y_old_;
        float alt_scale2_delta_y_new_;
        float alt_scale2_delta_y_old_;
        float alt_ground_delta_y_new_;
        float alt_ground_delta_y_old_;
        float alt_labels_delta_y_new_;
        float alt_labels_delta_y_old_;
        // ALT scale variables.
        float alt_scale_x_;
        float alt_scale_y_;
        // ALT original float variables.
        const float alt_original_pix_per_alt_;
        const float alt_original_scale_height_;
        const float alt_original_labels_x_;
        const float alt_original_label1_y_;
        const float alt_original_label2_y_;
        const float alt_original_label3_y_;
        // ALT original QPointF variables.
        QPointF alt_original_back_pos_;
        QPointF alt_original_scale1_pos_;
        QPointF alt_original_scale2_pos_;
        QPointF alt_original_ground_pos_;
        QPointF alt_original_frame_pos_;
        QPointF alt_original_altitude_ctr_;
        QPointF alt_original_pressure_ctr_;
        // ALT z variables.
        const int alt_back_z_;
        const int alt_scale_z_;
        const int alt_labels_z_;
        const int alt_ground_z_;
        const int alt_frame_z_;
        const int alt_frame_text_z_;
    };
    // Airspeed Indicator(ASI) Panel.
    class PanelASI
    {
    public:
        PanelASI(QGraphicsScene *scene);
        ~PanelASI();
        void initASI(float scale_x, float scale_y);
        void updateASI(float scale_x, float scale_y);
        void setASIAirspeed(float airspeed);
        void setASIMachNumber(float mach_number);
    private:
        void resetASI(void);
        void updateASIAirspeed(void);
        void updateASIScale(void);
        void updateASIScaleLabels(void);
    private:
        QGraphicsScene    *asi_scene_;
        QGraphicsSvgItem  *asi_item_back_;
        QGraphicsSvgItem  *asi_item_scale1_;
        QGraphicsSvgItem  *asi_item_scale2_;
        QGraphicsTextItem *asi_item_label1_;
        QGraphicsTextItem *asi_item_label2_;
        QGraphicsTextItem *asi_item_label3_;
        QGraphicsTextItem *asi_item_label4_;
        QGraphicsTextItem *asi_item_label5_;
        QGraphicsTextItem *asi_item_label6_;
        QGraphicsTextItem *asi_item_label7_;
        QGraphicsSvgItem  *asi_item_frame_;
        QGraphicsTextItem *asi_item_airspeed_;
        QGraphicsTextItem *asi_item_mach_number_;
        // ASI color variables.
        QColor asi_frame_text_color_;
        QColor asi_labels_color_;
        // ASI font variables.
        QFont  asi_frame_text_font_;
        QFont  asi_labels_font_;
        // ASI float variables.
        float asi_airspeed_;
        float asi_mach_number_;
        // ASI delta variables.
        float asi_scale1_delta_y_new_;
        float asi_scale1_delta_y_old_;
        float asi_scale2_delta_y_new_;
        float asi_scale2_delta_y_old_;
        float asi_labels_delta_y_new_;
        float asi_labels_delta_y_old_;
        // ASI scale variables.
        float asi_scale_x_;
        float asi_scale_y_;
        // ASI original float variables.
        const float asi_original_pix_rer_spd_;
        const float asi_original_scale_height_;
        const float asi_original_labels_x_;
        const float asi_original_label1_y_;
        const float asi_original_label2_y_;
        const float asi_original_label3_y_;
        const float asi_original_label4_y_;
        const float asi_original_label5_y_;
        const float asi_original_label6_y_;
        const float asi_original_label7_y_;
        // ASI original QPointF variables.
        QPointF asi_original_back_pos_;
        QPointF asi_original_scale1_pos_;
        QPointF asi_original_scale2_pos_;
        QPointF asi_original_frame_pos_;
        QPointF asi_original_airspeed_ctr_;
        QPointF asi_original_mach_number_ctr_;
        // ASI z variables.
        const int asi_back_z_;
        const int asi_scale_z_;
        const int asi_labels_z_;
        const int asi_frame_z_;
        const int asi_frame_text_z_;
    };
    // Horizontal Situation Indicator(HSI) Panel.
    class PanelHSI
    {
    public:
        PanelHSI(QGraphicsScene *scene);
        ~PanelHSI();
        void initHSI(float scale_x, float scale_y);
        void updateHSI(float scale_x, float scale_y);
        void setHSIHeading(float heading);
    private:
        void resetHSI(void);
        void updateHSIHeading(void);
    private:
        QGraphicsScene    *hsi_scene_;
        QGraphicsSvgItem  *hsi_item_back_;
        QGraphicsSvgItem  *hsi_item_face_;
        QGraphicsSvgItem  *hsi_item_marks_;
        QGraphicsTextItem *hsi_item_frame_text_;
        // HSI color variable.
        QColor hsi_frame_text_color_;
        // HSI font variable.
        QFont  hsi_frame_text_font_;
        // HSI heading variable.
        float hsi_heading_;
        // HSI scale variables.
        float hsi_scale_x_;
        float hsi_scale_y_;
        // HSI original variables.
        QPointF hsi_original_hsi_ctr_;
        QPointF hsi_original_back_pos_;
        QPointF hsi_original_face_pos_;
        QPointF hsi_original_marks_pos_;
        QPointF hsi_original_frame_text_ctr_;
        // HSI z variables.
        const int hsi_back_z_;
        const int hsi_face_z_;
        const int hsi_marks_z_;
        const int hsi_frame_text_z_;
    };
    // Vertical Speed Indicator(VSI) Panel.
    class PanelVSI
    {
    public:
        PanelVSI(QGraphicsScene *scene);
        ~PanelVSI();
        void initVSI(float scale_x, float scale_y);
        void updateVSI(float scale_x, float scale_y);
        void setVSIClimbRate(float climb_rate);
    private:
        void resetVSI(void);
        void updateVSIClimbRate(void);
    private:
        QGraphicsScene    *vsi_scene_;
        QGraphicsSvgItem  *vsi_item_scale_;
        QGraphicsSvgItem  *vsi_item_arrow_;
        // VSI climb rate.
        float vsi_climb_rate_;
        // VSI delta variables.
        float vsi_arrow_delta_y_new_;
        float vsi_arrow_delta_y_old_;
        // VSI scale variables.
        float vsi_scale_x_;
        float vsi_scale_y_;
        // VSI original float variables.
        const float vsi_original_marke_height_;
        const float vsi_original_pix_per_spd1_;
        const float vsi_original_pix_per_spd2_;
        const float vsi_original_pix_per_spd4_;
        // VSI original QPointF variables.
        QPointF vsi_original_scale_pos_;
        QPointF vsi_original_arrow_pos_;
        // VSI z variables.
        const int vsi_scale_z_;
        const int vsi_arrow_z_;
    };
private:
    void initPFD(void);
    void resetPFD(void);
    void updatePFDView(void);
private:
    QGraphicsScene *pfd_scene_;
    PanelADI       *pfd_panel_adi_;
    PanelALT       *pfd_panel_alt_;
    PanelASI       *pfd_panel_asi_;
    PanelHSI       *pfd_panel_hsi_;
    PanelVSI       *pfd_panel_vsi_;
    // PFD svg items
    QGraphicsSvgItem *pfd_item_back_;
    QGraphicsSvgItem *pfd_item_mask_;
    // PFD scale variables.
    float pfd_scale_x_;
    float pfd_scale_y_;
    // PFD original variables.
    const int pfd_original_height_;
    const int pfd_original_width_;
    // PFD z variables.
    const int pfd_back_z_;
    const int pfd_mask_z_;
};

#endif // FCS_INSTRUCMENT_PFD_H
