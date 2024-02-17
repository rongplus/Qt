#pragma once
//
//  MainWindow.h
//  MainWindow
//
//  Created by Younghwan Mun on 2016-08-05.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ConfigDialog;
class SnifferMC1322x;


class  SiteSurveyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SiteSurveyWindow(QWidget *parent = 0);
    ~SiteSurveyWindow();
public:
	/**
	* @brief receivedFrame() calls addZTCFrameViewItem(ZB_RX, casted frame)
	* @param frmBin
	*/
	void receivedFrame(const QByteArray& frmBin);

public slots:    

	void show_config();
    /**
     * @brief openSniffer open sniffering
     */
    void openSniffer();

    /**
     * @brief closeSniffer() close sniffering
     */
    void closeSniffer();

    /**
     * @brief about() shows program information
     */
    void about();

    /**
     * @brief clearAll() clears all view information
     */
    void clearAll();

    /**
     * @brief updateDeviceView() updates device information in device manager view
     */
    void updateDeviceMgrView();

    /**
     * @brief selectedZTCFrameItem() occurs when table items are selected in ztc frame view
     * @param row
     * @param col
     */
    void selectedZTCFrameItem(int row, int col);

    /**
     * @brief selectedMACFrameItem() occurs when table items are selected in mac frame view
     * @param row
     * @param col
     */
    void selectedMACFrameItem(int row, int col);

    /**
     * @brief selectedNWKInfoItem() occurs when table items are selected in network information view
     * @param row
     * @param col
     */
    void selectedNWKInfoItem(int row, int col);

    /**
     * @brief selectedFrameTrafficItem() occurs when table items are selected in frame traffic view
     * @param row
     * @param col
     */
    void selectedFrameTrafficItem(int row, int col);

    /**
     * @brief showErrorMessage() shows error message in status bar
     * @param code
     * @param msg
     */
    void showErrorMessage(int code, const QString& msg);

    /**
     * @brief receivedBytes() calls addRawViewItem(ZB_RX, bin)
     * @param bin
     */
    void receivedBytes(const QByteArray& bin);

    /**
     * @brief transmittedBytes() calls addRawViewItem(ZB_TX, bin)
     * @param bin
     */
    void transmittedBytes(const QByteArray& bin);

    

    /**
     * @brief transmittedFrame calls addZTCFrameViewItem(ZB_TX, casted frame)
     * @param frmBin
     */
    void transmittedFrame(const QByteArray& frmBin);

private:
    /**
     * @brief addRawPacketItem() adds a table item with row bytes
     * @param idx
     */
    void addRawPacketItem(int idx);

    /**
     * @brief addZTCFrameItem() adds a table item with ztc frame
     * @param idx
     */
    void addZTCFrameItem(int idx);

    /**
     * @brief addMACFrameItem() adds a table item with mac frame
     * @param idx
     */
    void addMACFrameItem(int idx);

    /**
     * @brief addNWKInfoItem() adds a table item with network information
     * @param idx
     */
    void addNWKInfoItem(int idx);

    /**
     * @brief addTrafficItem() adds a table item in traffic view
     * @param idx
     */
    void addTrafficItem(int idx);

    /**
     * @brief updateFrameDecode() updates decoded frame information by index
     * @param idx
     */
    void updateFrameDecode(int idx);

    /**
     * @brief updateFrameHexView() updates frame hex view by index
     * @param idx
     */
    void updateFrameHexView(int idx);

    /**
     * @brief addZTCFrameDecode() adds decoded ztc frame information in tree view
     * @param idx
     */
    void addZTCFrameDecode(int idx);

    /**
     * @brief addZTCDataFrameDecode() adds decoded ztc data frame information in tree view
     * @param idx
     */
    void addZTCDataFrameDecode(int idx);

    /**
     * @brief addMACFrameDecode() adds decoded mac frame information in tree view
     * @param idx
     */
    void addMACFrameDecode(int idx);

    /**
     * @brief addMACBeaconFrameDecode() adds decoded mac beacon frame information in tree view
     * @param idx
     */
    void addMACBeaconFrameDecode(int idx);

    /**
     * @brief addMACDataFrameDecode() adds decoded mac data frame information in tree view
     * @param idx
     */
    void addMACDataFrameDecode(int idx);

    /**
     * @brief addMACAckFrameDecode() adds decoded mac acknowledge frame information in tree view
     * @param idx
     */
    void addMACAckFrameDecode(int idx);

    /**
     * @brief addMACCommandFrameDecode() addsdecoded mac command frame information in tree view
     * @param idx
     */
    void addMACCommandFrameDecode(int idx);

    /**
     * @brief addNWKFrameDecode() adds decoded network frame information in tree view
     * @param idx
     */
    void addNWKFrameDecode(int idx);

    /**
     * @brief addNWKFrameDecode() adds decoded network frame data information in tree view
     * @param idx
     */
    void addNWKDataFrameDecode(int idx);

    /**
     * @brief addNWKFrameDecode() adds decoded network frame command information in tree view
     * @param idx
     */
    void addNWKCommandFrameDecode(int idx);

    /**
     * @brief addNWKFrameDecode() adds decoded network frame beacon information in tree view
     * @param idx
     */
    void addNWKBeaconFrameDecode(int idx);

	void showEvent(QShowEvent *event);

private:
    Ui::MainWindow* ui;
    ConfigDialog* config;
    SnifferMC1322x* sniffer;
};