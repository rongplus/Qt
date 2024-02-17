//
//  MainWindow.cpp
//  MainWindow
//
//  Created by Younghwan Mun on 2016-08-05.
//  Copyright © 2016 Mircom Group of Companies. All rights reserved.
//
#include "SiteSurveyWindow.h"
#include "ui_mainwindow.h"
#include "SavedPreference.h"
#include "SavedFrames.h"
#include "ConfigDialog.h"
#include "SnifferMC1322x.h"
#include "Helper.h"
#include "flzonecontrollor.h"
#include <QMessageBox>
#include <QTableWidgetItem>

SiteSurveyWindow::SiteSurveyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // dialogs
    config = new ConfigDialog(this);

    // sniffer
    sniffer = new SnifferMC1322x(this);

    // connections
    connect(ui->pushButton, SIGNAL(clicked()), this  , SLOT(openSniffer()));
    //connect(ui->actionDisconnect, &QAction::triggered, this  , &SiteSurveyWindow::closeSniffer);
   // connect(ui->actionQuit      , &QAction::triggered, this  , &SiteSurveyWindow::close);
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(show_config()));
    //connect(ui->actionClearAll  , &QAction::triggered, this  , &SiteSurveyWindow::clearAll);
    //connect(ui->actionAbout     , &QAction::triggered, this  , &SiteSurveyWindow::about);

    connect(ui->ztcFrameView, &QTableWidget::cellClicked, this, &SiteSurveyWindow::selectedZTCFrameItem);
    connect(ui->macFrameView, &QTableWidget::cellClicked, this, &SiteSurveyWindow::selectedMACFrameItem);
    connect(ui->nwkInfoView , &QTableWidget::cellClicked, this, &SiteSurveyWindow::selectedNWKInfoItem);
    connect(ui->trafficView , &QTableWidget::cellClicked, this, &SiteSurveyWindow::selectedFrameTrafficItem);

    connect(config, &ConfigDialog::apply, this, &SiteSurveyWindow::updateDeviceMgrView);

    connect(sniffer, &SnifferMC1322x::notifyErrorMessage    , this, &SiteSurveyWindow::showErrorMessage);
    connect(sniffer, &SnifferMC1322x::notifyReceivedBytes   , this, &SiteSurveyWindow::receivedBytes);
    connect(sniffer, &SnifferMC1322x::notifyTransmittedBytes, this, &SiteSurveyWindow::transmittedBytes);
    connect(sniffer, &SnifferMC1322x::notifyReceivedFrame   , this, &SiteSurveyWindow::receivedFrame);
    connect(sniffer, &SnifferMC1322x::notifyTransmittedFrame, this, &SiteSurveyWindow::transmittedFrame);

    // menus
  //  ui->actionConnect->setEnabled(true);
  //  ui->actionDisconnect->setEnabled(false);
  //  ui->actionConfigure->setEnabled(true);

    // status bar
    ui->statusBar->showMessage("Application launched.", 2000);

    // Device view
    updateDeviceMgrView();

    // Raw packet view
    QTableWidget* table = ui->rawView;
    table->setColumnWidth(0, 36);   // no.
    table->setColumnWidth(1, 114);  // timestamp
    table->setColumnWidth(2, 26);   // channel
    table->setColumnWidth(3, 26);   // length
    table->setColumnWidth(4, 36);   // type
                                    // hex values

    // ZTC frame view
    table = ui->ztcFrameView;
    table->setColumnWidth(0, 36);   // no.
    table->setColumnWidth(1, 114);  // timestamp
    table->setColumnWidth(2, 26);   // channel
    table->setColumnWidth(3, 26);   // length
    table->setColumnWidth(4, 60);   // layer
    table->setColumnWidth(5, 100);  // message
    table->setColumnWidth(6, 70);   // type
                                    // information

    // MAC frame view
    table = ui->macFrameView;
    table->setColumnWidth(0, 36);   // no.
    table->setColumnWidth(1, 114);  // timestamp
    table->setColumnWidth(2, 26);   // channel
    table->setColumnWidth(3, 26);   // length
    table->setColumnWidth(4, 76);   // protocol
    table->setColumnWidth(5, 60);   // LQI
    table->setColumnWidth(6, 80);   // message
    table->setColumnWidth(7, 58);   // source pan id
    table->setColumnWidth(8, 58);   // source
    table->setColumnWidth(9, 58);   // destination
    table->setColumnWidth(10, 58);  // destination id
    table->setColumnWidth(11, 32);  // sequence
                                    // information

    // NWK info view
    table = ui->nwkInfoView;
    table->setColumnWidth(0, 36);   // no.
    table->setColumnWidth(1, 114);  // timestamp
    table->setColumnWidth(2, 26);   // channel
    table->setColumnWidth(3, 26);   // length
    table->setColumnWidth(4, 70);   // stack profile
    table->setColumnWidth(5, 30);   // protocol version
    table->setColumnWidth(6, 50);   // source
    table->setColumnWidth(7, 126);  // extended pan id
    table->setColumnWidth(8, 52);   // router capacity
    table->setColumnWidth(9, 58);   // end device capacity
    table->setColumnWidth(10, 42);  // device depth
    table->setColumnWidth(11, 50);  // update id
                                    // transmitted offset

    // Frame traffic view
    table = ui->trafficView;
    table->setColumnWidth(0, 36);   // no.
    table->setColumnWidth(1, 114);  // timestamp
    table->setColumnWidth(2, 26);   // channel
    table->setColumnWidth(3, 26);   // length
    table->setColumnWidth(4, 46);   // stack
    table->setColumnWidth(5, 46);   // layer
    table->setColumnWidth(6, 140);  // message
    table->setColumnWidth(7, 60);   // mac source
    table->setColumnWidth(8, 60);   // mac destination
    table->setColumnWidth(9, 60);   // mac sequence
                                    // information
    // Frame decode view 
    QList<int> sizes;
    sizes.push_back(200);
    ui->frameDecodeSplitter->setSizes(sizes);
    table = ui->frameHexView;
    table->setColumnWidth(0, 50);
    table->setColumnWidth(1, 175);
    table->setColumnWidth(2, 80);
    table->setFont(fixedFont());
}

SiteSurveyWindow::~SiteSurveyWindow()
{
    //delete sniffer;
    //delete config;
    //delete ui;
}

void SiteSurveyWindow::openSniffer()
{
	QSerialPort::SerialPortError retCode =  (QSerialPort::SerialPortError) sniffer->open();
    if (retCode == QSerialPort::NoError)
    {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
    }
    else
    {
        QString name = SavedPreference::instance()->serialInfo().name;
        QMessageBox::critical(this, "Sniffer Error", tr("Can't open serial port: <b>%1</b><br/>%2").arg(name).arg(toString(retCode)));
        ui->statusBar->showMessage(tr("Can't open serial port: %1 (%2)").arg(name).arg(toString(retCode)), 4000);
    }
}

void SiteSurveyWindow::closeSniffer()
{
    sniffer->close();

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
}

void SiteSurveyWindow::clearAll()
{
    ui->rawView->setRowCount(0);
    ui->ztcFrameView->setRowCount(0);
    ui->macFrameView->setRowCount(0);
    ui->nwkInfoView->setRowCount(0);
    ui->trafficView->setRowCount(0);
    ui->frameDecodeView->clear();
    ui->frameHexView->setRowCount(0);

    SavedFrames::instance()->clearAll();
}

void SiteSurveyWindow::updateDeviceMgrView()
{
    const SavedPreference::DeviceInfo& info = SavedPreference::instance()->serialInfo();

    ui->deviceView->setItem(0, 1, new QTableWidgetItem(info.name));
    ui->deviceView->setItem(1, 1, new QTableWidgetItem(tr("%1 (%2.0 Mhz)").arg(info.sChannel).arg(ZBInfo::frequency(info.channel))));
    ui->deviceView->setItem(2, 1, new QTableWidgetItem(info.description));
    ui->deviceView->setItem(3, 1, new QTableWidgetItem(info.manufacturer));
    ui->deviceView->setItem(4, 1, new QTableWidgetItem(info.serialNumber));
    ui->deviceView->setItem(5, 1, new QTableWidgetItem(info.location));
    ui->deviceView->setItem(6, 1, new QTableWidgetItem(info.vendorID));
    ui->deviceView->setItem(7, 1, new QTableWidgetItem(info.productID));
    ui->deviceView->setItem(8, 1, new QTableWidgetItem(info.sBaudRate));
    ui->deviceView->setItem(9, 1, new QTableWidgetItem(info.sDataBits));
    ui->deviceView->setItem(10, 1, new QTableWidgetItem(info.sParity));
    ui->deviceView->setItem(11, 1, new QTableWidgetItem(info.sStopBits));
    ui->deviceView->setItem(12, 1, new QTableWidgetItem(info.sFlowControl));
}

void SiteSurveyWindow::selectedZTCFrameItem(int row, int col)
{
    Q_UNUSED(col)
    updateFrameDecode(ui->ztcFrameView->item(row, 0)->text().toUInt());
}

void SiteSurveyWindow::selectedMACFrameItem(int row, int col)
{
    Q_UNUSED(col)
    updateFrameDecode(ui->macFrameView->item(row, 0)->text().toUInt());
}

void SiteSurveyWindow::selectedNWKInfoItem(int row, int col)
{
    Q_UNUSED(col)
    updateFrameDecode(ui->nwkInfoView->item(row, 0)->text().toUInt());
}

void SiteSurveyWindow::selectedFrameTrafficItem(int row, int col)
{
    Q_UNUSED(col)
    updateFrameDecode(ui->trafficView->item(row, 0)->text().toUInt());
}

void SiteSurveyWindow::showErrorMessage(int code, const QString& msg)
{
    Q_UNUSED(code)
    ui->statusBar->showMessage(msg, 4000);
}

void SiteSurveyWindow::receivedBytes(const QByteArray& bin)
{
    ZBInt32 idx = SavedFrames::instance()->addPacket(ZB_RX, toBytes(bin));
    addRawPacketItem(idx);
}

void SiteSurveyWindow::transmittedBytes(const QByteArray& bin)
{
    ZBInt32 idx = SavedFrames::instance()->addPacket(ZB_TX, toBytes(bin));
    addRawPacketItem(idx);
}

void SiteSurveyWindow::receivedFrame(const QByteArray& frmBin)
{
    ZBInt32 idx = SavedFrames::instance()->addFrame(ZB_RX, toBytes(frmBin));
    addZTCFrameItem(idx);
    addMACFrameItem(idx);
    addNWKInfoItem(idx);
    addTrafficItem(idx);
    if (ui->ztcFrameView->selectedItems().empty() &&
        ui->macFrameView->selectedItems().empty() &&
        ui->nwkInfoView->selectedItems().empty())
        updateFrameDecode(idx);
}

void SiteSurveyWindow::transmittedFrame(const QByteArray& frmBin)
{
    ZBInt32 idx = SavedFrames::instance()->addFrame(ZB_TX, toBytes(frmBin));
    addZTCFrameItem(idx);
    addMACFrameItem(idx);
    addNWKInfoItem(idx);
    addTrafficItem(idx);
    if (ui->ztcFrameView->selectedItems().empty() &&
        ui->macFrameView->selectedItems().empty() &&
        ui->nwkInfoView->selectedItems().empty())
        updateFrameDecode(idx);
}

void SiteSurveyWindow::addRawPacketItem(int idx)
{
    const SavedFrames::Saved& saved = SavedFrames::instance()->packet(idx);
    if (saved.bin.empty())
        return;

    QString sTime = tr("%1 (%2)").arg(toString(saved.time)).arg(toString(saved.delta, "s.zzz"));
    QString sType = saved.type == ZB_RX ? "RX" : "TX";
    QList<QString> list =
    {
        toNum(idx),                         // no.
        sTime,                              // timestamp
        toNum(saved.channel),               // channel
        toNum(saved.bin.size()),            // length
        sType,                              // type
        toHex(saved.bin)                    // hex values
    };

    QTableWidget* table = ui->rawView;
    int row = table->rowCount();
    table->insertRow(row);
    for (int i = 0; i < list.size(); i++)
        table->setItem(row, i, new QTableWidgetItem(list[i]));

    // decorate
    table->item(row, 0)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // no.
    table->item(row, 1)->setTextAlignment(Qt::AlignCenter);                     // timestamp
    table->item(row, 2)->setTextAlignment(Qt::AlignCenter);                     // channel
    table->item(row, 3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // length
    table->item(row, 4)->setTextAlignment(Qt::AlignCenter);                     // type
    table->item(row, 5)->setFont(fixedFont());                                  // hex values
    if (saved.type == ZB_TX)
        for (int i = 0; i < table->columnCount(); i++)
            table->item(row, i)->setForeground(Qt::red);
    table->resizeRowsToContents();

    // scroll
    table->scrollToBottom();

#if ZB_DEBUG
    qDebug() << tr("Raw %1 %2 %3: %4").arg(list[0]).arg(sTime).arg(sType).arg(list[5]);
#endif
}

void SiteSurveyWindow::addZTCFrameItem(int idx)
{
    const ZTCFrame& frame = SavedFrames::instance()->ztcFrame(idx);
    if (frame.empty())
        return;

    const SavedFrames::Saved& saved = SavedFrames::instance()->frame(idx);
    const ZTCFrameInfo& info = frame.info();

    QColor color = Qt::black;
    QString sTime = tr("%1 (%2)").arg(toString(saved.time)).arg(toString(saved.delta, "s.zzz"));
    QString sInfo = info.attribute().c_str();
    if (info.type() == "Request")
    {
        color = QColor(Qt::red);
    }
    else if (info.type() == "Confirm")
    {
        color = QColor(Qt::darkGreen);
        if (info.attribute().empty())
            sInfo.append(info.status().c_str());
        else
            sInfo.append(tr(": %1").arg(info.status().c_str()));
    }
    else
    {
        ZTCDataFrame dataFrame = frame;
        if (!dataFrame.empty())
            sInfo = tr("%1 LQI: %2").arg(toString(dataFrame.timeStamp())).arg(toNumHex(dataFrame.linkQuality()));
        else if (info.layer() == "Unknown")  // For Debug
            sInfo = toHex(frame.payload());
    }

    QList<QString> list =
    {
        toNum(idx),                         // no.
        sTime,                              // timestamp
        toNum(saved.channel),               // channel
        toNum(frame.size()),                // length
        info.layer().c_str(),               // layer
        info.message().c_str(),             // message
        info.type().c_str(),                // type
        sInfo                               // information
    };

    // add items
    QTableWidget* table = ui->ztcFrameView;
    int row = table->rowCount();
    table->insertRow(row);
    for (int i = 0; i < list.size(); i++)
        table->setItem(row, i, new QTableWidgetItem(list[i]));

    // decorate
    table->item(row, 0)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // no.
    table->item(row, 1)->setTextAlignment(Qt::AlignCenter);                     // timestamp
    table->item(row, 2)->setTextAlignment(Qt::AlignCenter);                     // channel
    table->item(row, 3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // length
    table->item(row, 4)->setTextAlignment(Qt::AlignCenter);                     // layer
    for (int i = 0; i < table->columnCount(); i++)
        table->item(row, i)->setForeground(color);
    if (list[2] == "Unknown")
        table->item(row, 5)->setFont(fixedFont());

    // scroll
    if (table->selectedItems().empty())
        table->scrollToBottom();

#if ZB_DEBUG
    qDebug() << tr("Frm %1 %2 %3: -- %4 --").arg(list[0]).arg(list[1]).arg(ZB_RX ? "RX" : "TX").arg(toHex(frame.bin()));
#endif
}

void SiteSurveyWindow::addMACFrameItem(int idx)
{
    const ZTCDataFrame& dataFrame = SavedFrames::instance()->ztcDataFrame(idx);
    if (dataFrame.empty())
        return;

    const MACFrame& frame = dataFrame;
    const SavedFrames::Saved& saved = SavedFrames::instance()->frame(idx);
    const MACFrame::FrameControl& ctl = frame.frameControl();
    const MACFrame::Addressing& addr = frame.addressing();

    QColor color = Qt::black;
    QString sTime = tr("%1 (%2)").arg(toString(saved.time)).arg(toString(saved.delta, "s.zzz"));
    QString sDstPan, sDstAddr;
    QString sSrcPan, sSrcAddr;

    switch (ctl.dstMode)
    {
    case MAC_ADDRESS_MODE_GROUP:
    case MAC_ADDRESS_MODE_16_BIT:
        sDstPan = toHex(addr.dstPanID);
        sDstAddr = addr.dstAddr16 == 0xFFFF ? "Broadcast" : toHex(addr.dstAddr16);
        break;

    case MAC_ADDRESS_MODE_64_BIT:
        sDstPan = toHex(addr.dstPanID);
        sDstAddr = toHex(addr.dstAddr64);
        break;
    }

    switch (ctl.srcMode)
    {
    case MAC_ADDRESS_MODE_GROUP:
    case MAC_ADDRESS_MODE_16_BIT:        
        sSrcPan = ctl.intraPAN ? "Intra" : toHex(addr.srcPanID);
        sSrcAddr = toHex(addr.srcAddr16);
        break;

    case MAC_ADDRESS_MODE_64_BIT:
        sSrcPan = ctl.intraPAN ? "Intra" : toHex(addr.srcPanID);
        sSrcAddr = toHex(addr.srcAddr64);
        break;
    }

    QString sProtocol = frame.protocol().c_str();  
    QString sInfo;

    switch (ctl.type)
    {
        case MAC_FRAME_BEACON:
        {
            const MACBeaconFrame& bcnFrame = frame;
            const NWKLayerInfo& nwkFrame = bcnFrame;
            if (!nwkFrame.empty())
            {
                sProtocol = nwkFrame.protocol().c_str();
                sInfo = tr("%1, Src: %2, EPID: %3").arg(bcnFrame.type().c_str()).arg(sSrcAddr).arg(toHex(nwkFrame.extendedPanID()));
            }
            else
                sInfo = toHex(bcnFrame.payload());
        }   break;

        case MAC_FRAME_COMMAND:
        {
            const MACCommandFrame& cmdFrame = frame;
            sInfo = cmdFrame.commandInfo().c_str();
            if (!cmdFrame.payload().empty())
                sInfo += ": " + toHex(cmdFrame.payload());
            color = QColor(Qt::red);
        }   break;

        case MAC_FRAME_ACKNOWLEDGE:
            sInfo = toHex(MACAckFrame(frame).payload());
            color = QColor(Qt::darkGreen);
            break;

        case MAC_FRAME_DATA:
        {
            const MACDataFrame& datFrame = frame;
            const NWKFrame& nwkFrame = datFrame;
            if (!nwkFrame.empty())
            {
                sProtocol = nwkFrame.protocol().c_str();
                const NWKCommandFrame& cmdFrame = nwkFrame;
                if (!cmdFrame.empty())
                {
                    sInfo = cmdFrame.commandInfo().c_str();
                    switch (cmdFrame.command())
                    {
                        case NWK_FRAME_COMMAND_ROUTE_REQUEST: {
                            const NWKCommandFrame::RouteRequest& cmd = cmdFrame.routeRequest();
                            sInfo += tr(", Src: %1, Dst: %2").arg(sSrcAddr).arg(toHex(cmd.dstAddr16));
                        }   break;

                        case NWK_FRAME_COMMAND_NETWORK_STATUS: {
                            const NWKCommandFrame::NetworkStatus& cmd = cmdFrame.networkStatus();
                            sInfo += tr(", %1, Dst: %2").arg(cmd.statusInfo.c_str()).arg(toHex(cmd.dstAddr16));
                        }   break;

                        case NWK_FRAME_COMMAND_LEAVE: {
                            const NWKCommandFrame::Leave& cmd = cmdFrame.leave();
                            sInfo = tr(", Rejoin: %1, Request/Indication: %2, Remove Children: %3").arg(toBool(cmd.rejoin)).arg(toBool(cmd.request)).arg(toBool(cmd.removeChildren));
                        }   break;

                        case NWK_FRAME_COMMAND_LINK_STATUS: {
                            const NWKCommandFrame::LinkStatus& cmd = cmdFrame.linkStatus();
                            sInfo = tr(", Count: %1").arg(toNum(cmd.linkCnt));
                            for (ZBByte i = 0; i < cmd.linkCnt; i++)
                            {
                                const NWKCommandFrame::Link& link = cmd.links[i];
                                sInfo += tr(", %1 (%2:%3)").arg(toHex(link.neighbor)).arg(toNum(link.incoming)).arg(toNum(link.outgoing));
                            }
                        }   break;
                    }
                }
            }
            else
            {
                sInfo = tr("%1, Src: %2, Dst: %3").arg(datFrame.type().c_str()).arg(sSrcAddr).arg(sDstAddr);
            }
            color = QColor(Qt::blue);
        }   break;
    }
    QList<QString> list =
    {
        toNum(idx),                             // no.
        sTime,                                  // timestamp
        toNum(saved.channel),                   // channel
        toNum(frame.size()),                    // length
        sProtocol,                              // protocol
        toNumHex(dataFrame.linkQuality()),      // link quality
        frame.type().c_str(),                   // message
        sSrcPan,                                // source pan id
        sSrcAddr,                               // source
        sDstPan,                                // destination pan id
        sDstAddr,                               // destination
        toNum(frame.sequence()),                // sequence
        sInfo                                   // payload
    };

    QTableWidget* table = ui->macFrameView;
    int row = table->rowCount();
    table->insertRow(row);
    for (int i = 0; i < list.size(); i++)
        table->setItem(row, i, new QTableWidgetItem(list[i]));

    // decorate
    table->item(row, 0)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // no.
    table->item(row, 1)->setTextAlignment(Qt::AlignCenter);                     // timestamp
    table->item(row, 2)->setTextAlignment(Qt::AlignCenter);                     // channel
    table->item(row, 3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // length
    table->item(row, 5)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // link quality
    table->item(row, 7)->setTextAlignment(Qt::AlignCenter);                     // source pan id
    table->item(row, 8)->setTextAlignment(Qt::AlignCenter);                     // source
    table->item(row, 9)->setTextAlignment(Qt::AlignCenter);                     // destination pan id
    table->item(row, 10)->setTextAlignment(Qt::AlignCenter);                    // destination
    table->item(row, 11)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);  // sequence
    for (int i = 0; i < table->columnCount(); i++)
        table->item(row, i)->setForeground(color);

    // scroll
    if (table->selectedItems().empty())
        table->scrollToBottom();
}

void SiteSurveyWindow::addNWKInfoItem(int idx)
{
    const MACBeaconFrame& bcnFrame = SavedFrames::instance()->macBeaconFrame(idx);
    if (bcnFrame.empty())
        return;

    const NWKLayerInfo& nwkInfo = bcnFrame;
    if (nwkInfo.empty())
        return;

    const SavedFrames::Saved& saved = SavedFrames::instance()->frame(idx);

    QString sTime = tr("%1 (%2)").arg(toString(saved.time)).arg(toString(saved.delta, "s.zzz"));
    QList<QString> list =
    {
        toNum(idx),                                 // index
        sTime,                                      // timestamp
        toNum(saved.channel),                       // channel
        toNum(nwkInfo.size()),                      // length
        nwkInfo.profile().c_str(),                  // stack profile
        toNum(nwkInfo.protocolVer()),               // protocol version
        toHex(bcnFrame.addressing().srcAddr16),     // source
        toHex(nwkInfo.extendedPanID()),             // extended pan id
        toBool(nwkInfo.routerCapacity()),           // router capacity
        toBool(nwkInfo.endDeviceCapacity()),        // end device capacity
        toNum(nwkInfo.deviceDepth()),               // device depth
        toNum(nwkInfo.updateID()),                  // update id
        toHex24(nwkInfo.transmitOffset())           // transmit offset
    };

    QTableWidget* table = ui->nwkInfoView;
    int row = table->rowCount();
    table->insertRow(row);
    for (int i = 0; i < list.size(); i++)
        table->setItem(row, i, new QTableWidgetItem(list[i]));

    // decorate
    table->item(row, 0)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // no.
    table->item(row, 1)->setTextAlignment(Qt::AlignCenter);                     // timestamp
    table->item(row, 2)->setTextAlignment(Qt::AlignCenter);                     // channel
    table->item(row, 3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // length
    table->item(row, 4)->setTextAlignment(Qt::AlignCenter);                     // stack profile
    table->item(row, 5)->setTextAlignment(Qt::AlignCenter);                     // protocol version
    table->item(row, 6)->setTextAlignment(Qt::AlignCenter);                     // source
    table->item(row, 7)->setTextAlignment(Qt::AlignCenter);                     // extended pan id
    table->item(row, 8)->setTextAlignment(Qt::AlignCenter);                     // router capacity
    table->item(row, 9)->setTextAlignment(Qt::AlignCenter);                     // end device capacity
    table->item(row, 10)->setTextAlignment(Qt::AlignCenter);                    // device depth
    table->item(row, 11)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);  // update id

    // scroll
    if (table->selectedItems().empty())
        table->scrollToBottom();
}

void SiteSurveyWindow::addTrafficItem(int idx)
{
    const ZTCFrame& frame = SavedFrames::instance()->ztcFrame(idx);
    if (frame.empty())
        return;

    const SavedFrames::Saved& saved = SavedFrames::instance()->frame(idx);

    QColor color(Qt::black);

    QString sTime = tr("%1 (%2)").arg(toString(saved.time)).arg(toString(saved.delta, "s.zzz"));
    QString sStack = "ZigBee";
    QString sLayer;
    QString sMsg;
    QString sMacSeq;
    QString sMacSrc;
    QString sMacDst;
    QString sInfo;

    const ZTCDataFrame& ztcDataFrame = frame;
    if (ztcDataFrame.empty())
    {
        const ZTCFrameInfo& info = frame.info();
        sLayer = frame.layer().c_str();
        sMsg = tr("%1 %2").arg(info.message().c_str()).arg(info.type().c_str());
        sInfo = info.status().c_str();
        sMacSeq = "-";
        sMacSrc = "-";
        sMacDst = "-";

        if (info.type() == "Request")
            color = QColor(Qt::red);
        else if (info.type() == "Confirm")
            color = QColor(Qt::darkGreen);
    }
    else
    {
        const MACFrame& macFrame = ztcDataFrame;
        if (!macFrame.empty())
        {
            const MACFrame::FrameControl& ctl = macFrame.frameControl();
            const MACFrame::Addressing& addr = macFrame.addressing();

            sLayer = macFrame.layer().c_str();
            sMsg = MACFrameInfo::type(ctl.type).c_str();
            sMacSeq = toNum(macFrame.sequence());

            switch (ctl.srcMode)
            {
                case MAC_ADDRESS_MODE_GROUP:
                case MAC_ADDRESS_MODE_16_BIT:
                    sMacSrc = toHex(addr.srcAddr16);
                    break;
                case MAC_ADDRESS_MODE_64_BIT:
                    sMacSrc = toHex(addr.srcAddr64);
                    break;
            }

            switch (ctl.dstMode)
            {
                case MAC_ADDRESS_MODE_GROUP:
                case MAC_ADDRESS_MODE_16_BIT:
                    sMacDst = toHex(addr.dstAddr16);
                    break;
                case MAC_ADDRESS_MODE_64_BIT:
                    sMacDst = toHex(addr.dstAddr64);
                    break;
            }

            switch (ctl.type)
            {
                case MAC_FRAME_BEACON:
                {
                    const MACBeaconFrame& bcnFrame = macFrame;
                    const NWKLayerInfo& nwkFrame = bcnFrame;
                    if (!nwkFrame.empty())
                    {
                        sLayer = nwkFrame.layer().c_str();
                        sInfo = tr("Src: %1, EPID: %2").arg(sMacSrc).arg(toHex(nwkFrame.extendedPanID()));
                    }
                    else
                        sInfo = tr("Payload: %1").arg(toHex(bcnFrame.payload()));
                }   break;

                case MAC_FRAME_ACKNOWLEDGE:
                    color = QColor(Qt::darkGreen);
                    break;

                case MAC_FRAME_DATA: {
                    const MACDataFrame& datFrame = macFrame;
                    const NWKFrame& nwkFrame = datFrame;
                    if (!nwkFrame.empty())
                    {
                        sLayer = nwkFrame.layer().c_str();
                        const NWKCommandFrame& cmdFrame = nwkFrame;
                        if (!cmdFrame.empty())
                        {
                            sMsg = cmdFrame.commandInfo().c_str();
                            switch (cmdFrame.command())
                            {
                                case NWK_FRAME_COMMAND_ROUTE_REQUEST: {
                                    const NWKCommandFrame::RouteRequest& cmd = cmdFrame.routeRequest();
                                    sInfo = tr("Src: %1, Dst: %2").arg(sMacSrc).arg(toHex(cmd.dstAddr16));
                                    if (cmd.hasDstIEEE)
                                        sInfo += tr(" (%1)").arg(toHex(cmd.dstAddr64));
                                }   break;

                                case NWK_FRAME_COMMAND_NETWORK_STATUS: {
                                    const NWKCommandFrame::NetworkStatus& cmd = cmdFrame.networkStatus();
                                    sInfo = tr("%1, Dst: %2").arg(cmd.statusInfo.c_str()).arg(toHex(cmd.dstAddr16));
                                }   break;

                                case NWK_FRAME_COMMAND_LEAVE: {
                                    const NWKCommandFrame::Leave& cmd = cmdFrame.leave();
                                    sInfo = tr("Rejoin: %1, Request/Indication: %2, Remove Children: %3").arg(toBool(cmd.rejoin)).arg(toBool(cmd.request)).arg(toBool(cmd.removeChildren));
                                }   break;

                                case NWK_FRAME_COMMAND_LINK_STATUS: {
                                    const NWKCommandFrame::LinkStatus& cmd = cmdFrame.linkStatus();
                                    sInfo = tr("Count: %1").arg(toNum(cmd.linkCnt));
                                    for (ZBByte i = 0; i < cmd.linkCnt; i++)
                                    {
                                        const NWKCommandFrame::Link& link = cmd.links[i];
                                        sInfo += tr(", %1 (%2:%3)").arg(toHex(link.neighbor)).arg(toNum(link.incoming)).arg(toNum(link.outgoing));
                                    }
                                }   break;
                            }
                        }
                        else
                        {
                            sInfo = tr("Src: %2, Dst: %3").arg(sMacSrc).arg(sMacDst);
                        }
                    }
                    color = QColor(Qt::blue);
                }   break;

                case MAC_FRAME_COMMAND: {
                    const MACCommandFrame& cmdFrame = macFrame;
                    color = QColor(Qt::red);
                    sMsg = cmdFrame.commandInfo().c_str();
                    if (!cmdFrame.payload().empty())
                        sInfo = tr("Payload: %1").arg(toHex(cmdFrame.payload()));
                }   break;
            }
        }
    }

    QList<QString> list =
    {
        toNum(idx),                         // no.
        sTime,                              // timestamp
        toNum(saved.channel),               // channel
        toNum(frame.size()),                // length
        sStack,                             // stack
        sLayer,                             // layer
        sMsg,                               // message
        sMacSrc,                            // mac source
        sMacDst,                            // mac destination
        sMacSeq,                            // mac sequence
        sInfo                               // information
    };

    QTableWidget* table = ui->trafficView;
    int row = table->rowCount();
    table->insertRow(row);
    for (int i = 0; i < list.size(); i++)
        table->setItem(row, i, new QTableWidgetItem(list[i]));

    // decorate
    table->item(row, 0)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // no.
    table->item(row, 1)->setTextAlignment(Qt::AlignCenter);                     // timestamp
    table->item(row, 2)->setTextAlignment(Qt::AlignCenter);                     // channel
    table->item(row, 3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // length
    table->item(row, 4)->setTextAlignment(Qt::AlignCenter);                     // stack
    table->item(row, 5)->setTextAlignment(Qt::AlignCenter);                     // layer
    table->item(row, 7)->setTextAlignment(Qt::AlignCenter);                     // mac source
    table->item(row, 8)->setTextAlignment(Qt::AlignCenter);                     // mac destination
    table->item(row, 9)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);   // mac sequence
    for (int i = 0; i < table->columnCount(); i++)
        table->item(row, i)->setForeground(color);

    // scroll
    if (table->selectedItems().empty())
        table->scrollToBottom();
}

void SiteSurveyWindow::updateFrameDecode(int idx)
{
    QTreeWidget* tree = ui->frameDecodeView;
    tree->clear();

    addZTCFrameDecode(idx);
    addZTCDataFrameDecode(idx);

    addMACFrameDecode(idx);
    addMACBeaconFrameDecode(idx);
    addMACDataFrameDecode(idx);
    addMACAckFrameDecode(idx);
    addMACCommandFrameDecode(idx);

    addNWKFrameDecode(idx);
    addNWKDataFrameDecode(idx);
    addNWKCommandFrameDecode(idx);
    addNWKBeaconFrameDecode(idx);

    tree->expandToDepth(0);
    //tree->scrollToBottom();

    updateFrameHexView(idx);
}

void SiteSurveyWindow::updateFrameHexView(int idx)
{
    QTableWidget* table = ui->frameHexView;
    table->setRowCount(0);

    const ZTCFrame& frame = SavedFrames::instance()->ztcFrame(idx);
    if (frame.empty())
        return;

    const ZBBytes& bin = frame.bin();
    int len = bin.size();
    for (int i = 0; i < len; i += 8)
    {
        int row = table->rowCount();
        table->insertRow(row);

        const ZBBytes& subBin = sub(bin, i, i+8 < len ? 8 : -1);
        table->setItem(row, 0, new QTableWidgetItem(toHex(ZBInt16(i))));
        table->setItem(row, 1, new QTableWidgetItem(toHex(subBin)));
        table->setItem(row, 2, new QTableWidgetItem(toAscii(subBin)));
    }
}

void SiteSurveyWindow::addZTCFrameDecode(int idx)
{
    const ZTCFrame& frame = SavedFrames::instance()->ztcFrame(idx);
    if (frame.empty())
        return;

    const SavedFrames::Saved& saved = SavedFrames::instance()->frame(idx);
    const SavedPreference::DeviceInfo& device = SavedPreference::instance()->serialInfo();

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1 %2 Frame %3: %4").arg(frame.protocol().c_str()).arg(frame.layer().c_str()).arg(toNum(idx)).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    QList<QString> list =
    {
        tr("Channel: %1 (%2.0 Mhz)").arg(device.sChannel).arg(ZBInfo::frequency(device.channel)),
        tr("Time: %1").arg(toString(saved.time)),
        tr("OpCode: %1").arg(toHex(frame.opCode())),
    };
    for (QString s : list)
        root->addChild(new QTreeWidgetItem(QStringList(s)));
}

void SiteSurveyWindow::addZTCDataFrameDecode(int idx)
{
    const ZTCDataFrame& frame = SavedFrames::instance()->ztcDataFrame(idx);
    if (frame.empty())
        return;

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1: %3").arg(frame.name().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    QList<QString> list =
    {
        tr("Link Quality: %1").arg(toNumHex(frame.linkQuality())),
        tr("Timestamp: %1 sec").arg(toString(frame.timeStamp())),
        tr("Data Payload: %1").arg(toHex(frame.payload()))
    };
    for (QString s : list)
        root->addChild(new QTreeWidgetItem(QStringList(s)));
}

void SiteSurveyWindow::addMACFrameDecode(int idx)
{
    const MACFrame& frame = SavedFrames::instance()->macFrame(idx);
    if (frame.empty())
        return;

    const MACFrame::FrameControl& ctl = frame.frameControl();
    const MACFrame::Addressing& addr = frame.addressing();

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1 %2: %3").arg(frame.protocol().c_str()).arg(frame.layer().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    QList<QString> list =
    {
        tr("Frame Control Field: %1, Frame Type: %2, Destination Addressing Mode: %3, Source Addressing Mode: %4")
            .arg(toHex(ctl.spec)).arg(frame.type().c_str()).arg(frame.dstAddrMode().c_str()).arg(frame.srcAddrMode().c_str()),
        tr("Sequence Number: %1 (%2)").arg(uint(frame.sequence())).arg(toHex(frame.sequence()))
    };

    switch (ctl.dstMode)
    {
    case MAC_ADDRESS_MODE_INDIRECT:
        break;
    case MAC_ADDRESS_MODE_GROUP:
    case MAC_ADDRESS_MODE_16_BIT:
        list.push_back(tr("Destination PAN Identifier: %1").arg(toHex(addr.dstPanID)));
        list.push_back(tr("Destination Address: %1").arg(toHex(addr.dstAddr16)));
        break;
    case MAC_ADDRESS_MODE_64_BIT:
        list.push_back(tr("Destination PAN Identifier: %1").arg(toHex(addr.dstPanID)));
        list.push_back(tr("Destination Address: %1").arg(toHex(addr.dstAddr64)));
        break;
    }
    switch (ctl.srcMode)
    {
    case MAC_ADDRESS_MODE_INDIRECT:
        break;
    case MAC_ADDRESS_MODE_GROUP:
    case MAC_ADDRESS_MODE_16_BIT:
        if (!ctl.intraPAN)
            list.push_back(tr("Source PAN Identifier: %1").arg(toHex(addr.srcPanID)));
        list.push_back(tr("Source Address: %1").arg(toHex(addr.srcAddr16)));
        break;
    case MAC_ADDRESS_MODE_64_BIT:
        if (!ctl.intraPAN)
            list.push_back(tr("Source PAN Identifier: %1").arg(toHex(addr.srcPanID)));
        list.push_back(tr("Source Address: %1").arg(toHex(addr.srcAddr64)));
        break;
    }
    for (QString s : list)
        root->addChild(new QTreeWidgetItem(QStringList(s)));

    // frame control
    QList<QString> listCtl =
    {
        tr(".... .... .... .%1 - Frame Type: %2 (%3)").arg(toBit(ctl.type, 3)).arg(frame.type().c_str()).arg(toHex(ctl.type)),
        tr(".... .... .... %1... - Security Enabled: %2").arg(toBit(ctl.security)).arg(toBool(ctl.security)),
        tr(".... .... ...%1 .... - Frame Pending: %2").arg(toBit(ctl.pending)).arg(toBool(ctl.pending)),
        tr(".... .... ..%1. .... - Acknowledge Request: %2").arg(toBit(ctl.ackRequest)).arg(toBool(ctl.ackRequest)),
        tr(".... .... .%1.. .... - Intra-PAN: %2").arg(toBit(ctl.intraPAN)).arg(toBool(ctl.intraPAN)),
        tr(".... ..%1 %2... .... - Reserved: %3").arg(toBit(ctl.reserved, 2)).arg(toBit(ctl.reserved2)).arg(toHex(ZBByte(ctl.reserved | (ctl.reserved2 << 1)))),
        tr(".... %1.. .... .... - Destination Addressing Mode: %2 (%3)").arg(toBit(ctl.dstMode, 2)).arg(frame.dstAddrMode().c_str()).arg(toHex(ctl.dstMode)),
        tr("..%1 .... .... .... - 802.15.14 MAC Frame Version: 802.15.4 MAC %2 Frame(%3)").arg(toBit(ctl.version, 2)).arg(QString::number(2003+ctl.version)).arg(toHex(ctl.version)),
        tr("%1.. .... .... .... - Source Addressing Mode: %2 (%3)").arg(toBit(ctl.srcMode, 2)).arg(frame.srcAddrMode().c_str()).arg(toHex(ctl.srcMode))
    };
    for (QString s : listCtl)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(s));
        item->setFont(0, fixedFont());
        root->child(0)->addChild(item);
    }
}

void SiteSurveyWindow::addMACBeaconFrameDecode(int idx)
{
    const MACBeaconFrame& frame = SavedFrames::instance()->macBeaconFrame(idx);
    if (frame.empty())
        return;

    const MACBeaconFrame::SuperFrame& superFrm = frame.superFrame();
    const MACBeaconFrame::GTS& gts = frame.gts();
    const MACBeaconFrame::PendingAddress& pendAddr = frame.pendingAddress();

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1: %2").arg(frame.name().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    QList<QString> list =
    {
        tr("Superframe Specification: %1").arg(toHex(superFrm.spec)),
        tr("GTS Fields: %1").arg(toHex(gts.spec + gts.descriptors)),
        tr("Pending Address Fields: Short Count = %1 Extended Count = %2").arg(toNum(pendAddr.addr16Cnt)).arg(toNum(pendAddr.addr64Cnt)),
        tr("Beacon Payload: %1").arg(toHex(frame.payload()))
    };
    if (superFrm.associationPermit)
        list[0].append(tr(", %1").arg("Association Permit"));
    for (QString s : list)
        root->addChild(new QTreeWidgetItem(QStringList(s)));

    // superframe specification
    QList<QString> listSuperfrm =
    {
        tr(".... .... .... %1 - Beacon Interval: %2 ms").arg(toBit(superFrm.beaconInterval, 4)).arg(uint(superFrm.beaconInterval)),
        tr(".... .... %1 .... - Superframe Interval: %2 ms").arg(toBit(superFrm.superFrameInterval, 4)).arg(uint(superFrm.superFrameInterval)),
        tr(".... %1 .... .... - Final CAP Slot: %2").arg(toBit(superFrm.finalCapSlot, 4)).arg(toHex(superFrm.finalCapSlot)),
        tr("...%1 .... .... .... - Battery Life Extension: %2").arg(toBit(superFrm.batteryExtension)).arg(toBool(superFrm.batteryExtension)),
        tr("..%1. .... .... .... - Reseved: %2").arg(toBit(superFrm.reserved)).arg(toBool(superFrm.reserved)),
        tr(".%1.. .... .... .... - PAN Coordinator: %2").arg(toBit(superFrm.panCoordinator)).arg(toBool(superFrm.panCoordinator)),
        tr("%1... .... .... .... - Association Permit: %2").arg(toBit(superFrm.associationPermit)).arg(toBool(superFrm.associationPermit))
    };
    for (QString s : listSuperfrm)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(s));
        item->setFont(0, fixedFont());
        root->child(0)->addChild(item);
    }

    // gts fields
    QList<QString> listGTS = {tr("GTS Specification: %1").arg(toHex(gts.spec))};
    if (!gts.descriptors.empty())
        listGTS.push_back(tr("Descriptors: %1").arg(toHex(gts.descriptors)));
    for (QString s : listGTS)
        root->child(1)->addChild(new QTreeWidgetItem(QStringList(s)));
    QList<QString> listGTSSpec =
    {
        tr(".... .%1 - Descriptor Count: %2").arg(toBit(gts.descCnt, 3)).arg(uint(gts.descCnt)),
        tr(".%1 %2... - Reserved: %3").arg(toBit(gts.reserved, 3)).arg(toBit(gts.reserved2)).arg(toHex(ZBByte(ZBByte(gts.reserved2 << 1)|gts.reserved))),
        tr("%1... .... - Coordinator accepts GTS requests: %2").arg(toBit(gts.permit)).arg(toBool(gts.permit))
    };
    for (QString s : listGTSSpec)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(s));
        item->setFont(0, fixedFont());
        root->child(1)->child(0)->addChild(item);
    }

    // pendging address fields
    QList<QString> listPendingAddr = {tr("Pending Address Specification: %1").arg(toHex(pendAddr.spec))};
    for (int i = 0; pendAddr.addr16Cnt; i++)
        listGTS.push_back(tr("Short Address %1: %2").arg(i).arg(toHex(pendAddr.addr16s[i])));
    for (int i = 0; pendAddr.addr64Cnt; i++)
        listGTS.push_back(tr("Extended Address %1: %2").arg(i).arg(toHex(pendAddr.addr64s[i])));
    for (QString s : listPendingAddr)
        root->child(2)->addChild(new QTreeWidgetItem(QStringList(s)));
    QList<QString> listPendAddrSpec =
    {
        tr(".... .%1 - Number of Short Address Pending: %2").arg(toBit(pendAddr.addr16Cnt, 3)).arg(uint(pendAddr.addr16Cnt)),
        tr(".... %1... - Reserved: %2").arg(toBit(pendAddr.reserved)).arg(toHex(pendAddr.reserved)),
        tr(".%1 .... - Number of Extended Address Pending: %2").arg(toBit(pendAddr.addr64Cnt, 3)).arg(uint(pendAddr.addr64Cnt)),
        tr("%1... .... - Reserved: %2").arg(toBit(pendAddr.reserved2)).arg(toHex(pendAddr.reserved2)),
    };
    for (QString s : listPendAddrSpec)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(s));
        item->setFont(0, fixedFont());
        root->child(2)->child(0)->addChild(item);
    }
}

void SiteSurveyWindow::addMACDataFrameDecode(int idx)
{
    const MACDataFrame& frame = SavedFrames::instance()->macDataFrame(idx);
    if (frame.empty())
        return;

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1: %2").arg(frame.name().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    root->addChild(new QTreeWidgetItem(QStringList(tr("Data Payload: %1").arg(toHex(frame.payload())))));
}

void SiteSurveyWindow::addMACAckFrameDecode(int idx)
{
    const MACAckFrame& frame = SavedFrames::instance()->macAckFrame(idx);
    if (frame.empty())
        return;

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1").arg(frame.type().c_str())));
    ui->frameDecodeView->addTopLevelItem(root);
}

void SiteSurveyWindow::addMACCommandFrameDecode(int idx)
{
    const MACCommandFrame& frame = SavedFrames::instance()->macCommandFrame(idx);
    if (frame.empty())
        return;

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1: 0x%2").arg(frame.name().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    QList<QString> list = {tr("Command Idenifier: %1 (%2)").arg(frame.commandInfo().c_str()).arg(toHex(frame.command()))};    
    if (!frame.payload().empty())
        list.push_back(tr("Command Payload: %1").arg(toHex(frame.payload())));
    for (QString s : list)
        root->addChild(new QTreeWidgetItem(QStringList(s)));
}

void SiteSurveyWindow::addNWKFrameDecode(int idx)
{
    const NWKFrame& frame = SavedFrames::instance()->nwkFrame(idx);
    if (frame.empty())
        return;

    const NWKFrame::FrameControl& ctl = frame.frameControl();

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1 %2: %3").arg(frame.protocol().c_str()).arg(frame.layer().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    QList<QString> list =
    {
        tr("Frame Control Field: %1, Frame Type: %2, Discover Route: %3")
            .arg(toHex(ctl.spec)).arg(frame.type().c_str()).arg(frame.discoverRoute().c_str()),
        tr("Destination Address: %1").arg(toHex(frame.dstAddr())),
        tr("Source Address: %1").arg(toHex(frame.srcAddr())),
        tr("Radius: %1").arg(toNumHex(frame.radius())),
        tr("Sequence Number: %1").arg(toNumHex(frame.sequence()))
    };

    if (ctl.dstIEEEAddr16)
        list.push_back(tr("Destination IEEE Address : %1").arg(toHex(frame.dstIEEEAddr())));
    if (ctl.srcIEEEAddr64)
        list.push_back(tr("Source IEEE Address : %1").arg(toHex(frame.srcIEEEAddr())));
    if (ctl.multicast)
        list.push_back(tr("Multicast Control : %1").arg(toHex(frame.multicast())));

    if (ctl.srcRoute)
    {
        const NWKFrame::RouteSubFrame& subFrm = frame.srcRouteSubFrame();
        list.push_back(tr("Source Route Sub-Frame : %1").arg(toHex(subFrm.spec)));
    }

    for (QString s : list)
        root->addChild(new QTreeWidgetItem(QStringList(s)));

    // frame control
    QList<QString> listCtl =
    {
        tr(".... .... .... ..%1 - Frame Type: %2 (%3)").arg(toBit(ctl.type, 2)).arg(frame.type().c_str()).arg(toHex(ctl.type)),
        tr(".... .... ..%1 %2.. - Protocol Version: %3").arg(toBit(ctl.protocolVer>>2, 2)).arg(toBit(ctl.protocolVer&0x3, 2)).arg(toNum(ctl.protocolVer)),
        tr(".... .... %1.. .... - Discover Route: %2 (%3)").arg(toBit(ctl.discoverRoute, 2)).arg(frame.discoverRoute().c_str()).arg(toHex(ctl.discoverRoute)),
        tr(".... ...%1 .... .... - Multicast: %2").arg(toBit(ctl.multicast)).arg(ctl.multicast? "True" : "Unicast or Broadcast"),
        tr(".... ..%1. .... .... - Security Enabled: %2").arg(toBit(ctl.security)).arg(toBool(ctl.security)),
        tr(".... .%1.. .... .... - Source Route Included: %2").arg(toBit(ctl.srcRoute)).arg(toBool(ctl.srcRoute)),
        tr(".... %1... .... .... - Destination IEEE Address Included: %2").arg(toBit(ctl.dstIEEEAddr16)).arg(toBool(ctl.dstIEEEAddr16)),
        tr("...%1 .... .... .... - Source IEEE Address Included: %2").arg(toBit(ctl.srcIEEEAddr64)).arg(toBool(ctl.srcIEEEAddr64)),
        tr("..%1. .... .... .... - Device Initiator: %2").arg(toBit(ctl.devInitiator)).arg(toBool(ctl.devInitiator)),
        tr("%1.. .... .... .... - Reserved: %2").arg(toBit(ctl.reserved, 2)).arg(toHex(ctl.reserved))
    };
    for (QString s : listCtl)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(s));
        item->setFont(0, fixedFont());
        root->child(0)->addChild(item);
    }
}

void SiteSurveyWindow::addNWKDataFrameDecode(int idx)
{
    const NWKDataFrame& frame = SavedFrames::instance()->nwkDataFrame(idx);
    if (frame.empty())
        return;

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1: %2").arg(frame.name().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);
}

void SiteSurveyWindow::addNWKCommandFrameDecode(int idx)
{
    const NWKCommandFrame& frame = SavedFrames::instance()->nwkCommandFrame(idx);
    if (frame.empty())
        return;

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1: %2").arg(frame.name().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(tr("Command Idenifier: %1 (%2)").arg(frame.commandInfo().c_str()).arg(toHex(frame.command()))));
    root->addChild(item);

    switch (frame.command())
    {
        case NWK_FRAME_COMMAND_ROUTE_REQUEST: {
            const NWKCommandFrame::RouteRequest cmd = frame.routeRequest();

            QString cmdOpt = tr("Command Options: %1").arg(toHex(cmd.opt));
            if (cmd.hasDstIEEE)
                cmdOpt += ", Destination IEEE Adress Included";
            cmdOpt += tr(", Many-to-One Discovery: %1").arg(cmd.manyToOneInfo.c_str());
            QTreeWidgetItem* item2 = new QTreeWidgetItem(QStringList(cmdOpt));
            root->addChild(item2);

            QList<QString> listOpt =
            {
                tr(".... .%1 = Reserved: %2").arg(toBit(cmd.rsvd, 3)).arg(toHex(cmd.rsvd)),
                tr("...%1 %2... = Many to One Discovery: %3 (%4)").arg(toBit(cmd.manyToOne>>1)).arg(toBit(cmd.manyToOne&01)).arg(cmd.manyToOneInfo.c_str()).arg(toHex(cmd.manyToOne)),
                tr("..%1. .... = Destination IEEE Address Included: %2").arg(toBit(cmd.hasDstIEEE)).arg(toBool(cmd.hasDstIEEE)),
                tr(".%1.. .... = Multicast Route Request: %2").arg(toBit(cmd.multicastReq)).arg(toBool(cmd.multicastReq)),
                tr("%1... .... = Reserved: %2").arg(toBit(cmd.rsvd2)).arg(toHex(cmd.rsvd2))
            };
            for (QString s : listOpt)
            {
                QTreeWidgetItem* subItem = new QTreeWidgetItem(QStringList(s));
                subItem->setFont(0, fixedFont());
                item2->addChild(subItem);
            }

            QList<QString> listCmd =
            {
                tr("Route Request Identifier: %1").arg(toNumHex(cmd.reqId)),
                tr("Route Destination Address : %1").arg(toHex(cmd.dstAddr16)),
                tr("Path Cost : %1").arg(toNum(cmd.pathCost))
            };
            if (cmd.hasDstIEEE)
                listCmd.push_back(tr("Route Destination IEEE Address : %1").arg(toHex(cmd.dstAddr64)));
            for (QString s : listCmd)
                root->addChild(new QTreeWidgetItem(QStringList(s)));
        }   break;

        case NWK_FRAME_COMMAND_ROUTE_REPLY: {
            const NWKCommandFrame::RouteReply cmd = frame.routeReply();

            QTreeWidgetItem* item2 = new QTreeWidgetItem(QStringList(tr("Command Options: %1").arg(toHex(cmd.opt))));
            root->addChild(item2);

            QList<QString> listOpt =
            {
                tr(".%1 %2 = Reserved: %3").arg(toBit((cmd.rsvd>>4)&0x7, 3)).arg(toBit(cmd.rsvd&0xF, 4)).arg(toNum(cmd.rsvd)),
                tr("%1... .... = Route Repair: %2").arg(toBit(cmd.repair)).arg(toBool(cmd.repair))
            };
            for (QString s : listOpt)
            {
                QTreeWidgetItem* subItem = new QTreeWidgetItem(QStringList(s));
                subItem->setFont(0, fixedFont());
                item2->addChild(subItem);
            }

            QList<QString> listCmd =
            {
                tr("Route Request Identifier: %1").arg(toHex(cmd.reqId)),
                tr("Originator Address : %1").arg(toHex(cmd.srcAddr)),
                tr("Responder Address : %1").arg(toHex(cmd.dstAddr)),
                tr("Path Cost : %1").arg(toNum(cmd.pathCost))
            };
            for (QString s : listCmd)
                root->addChild(new QTreeWidgetItem(QStringList(s)));
        }   break;

        case NWK_FRAME_COMMAND_NETWORK_STATUS: {
            const NWKCommandFrame::NetworkStatus cmd = frame.networkStatus();

            QList<QString> listCmd =
            {
                tr("Status Code: %1 (%2)").arg(cmd.statusInfo.c_str()).arg(toHex(cmd.status)),
                tr("Destination Address: %1").arg(toHex(cmd.dstAddr16))
            };
            for (QString s : listCmd)
                root->addChild(new QTreeWidgetItem(QStringList(s)));
        }   break;

        case NWK_FRAME_COMMAND_LEAVE: {
            const NWKCommandFrame::Leave& cmd = frame.leave();

            QTreeWidgetItem* item2 = new QTreeWidgetItem(QStringList(tr("Command Options: %1").arg(toHex(cmd.opt))));
            root->addChild(item2);

            QList<QString> listOpt =
            {
                tr("...%1 %2 = Reserved: %3").arg(toBit(cmd.rsvd>>4)).arg(toBit(cmd.rsvd&0xF, 4)).arg(toNum(cmd.rsvd)),
                tr("..%1. .... = Rejoin: %2").arg(toBit(cmd.rejoin)).arg(toBool(cmd.rejoin)),
                tr(".%1.. .... = Request/Indication: %2").arg(toBit(cmd.request)).arg(toBool(cmd.request)),
                tr("%1... .... = Remove Children: %2").arg(toBit(cmd.removeChildren)).arg(toBool(cmd.removeChildren))
            };
            for (QString s : listOpt)
            {
                QTreeWidgetItem* subItem = new QTreeWidgetItem(QStringList(s));
                subItem->setFont(0, fixedFont());
                item2->addChild(subItem);
            }
        }   break;

        case NWK_FRAME_COMMAND_LINK_STATUS: {
            const NWKCommandFrame::LinkStatus& cmd = frame.linkStatus();

            QTreeWidgetItem* item2 = new QTreeWidgetItem(QStringList(tr("Command Options: %1").arg(toHex(cmd.opt))));
            root->addChild(item2);

            QList<QString> listOpt =
            {
                tr("...%1 %2 = Link Count: %3").arg(toBit(cmd.linkCnt>>4)).arg(toBit(cmd.linkCnt&0xF, 4)).arg(toNum(cmd.linkCnt)),
                tr("..%1. .... = First Frame: %2").arg(toBit(cmd.firstFrm)).arg(toBool(cmd.firstFrm)),
                tr(".%1.. .... = Last Frame: %2").arg(toBit(cmd.lastFrm)).arg(toBool(cmd.lastFrm)),
                tr("%1... .... = Reserved: %2").arg(toBit(cmd.rsvd)).arg(toHex(cmd.rsvd))
            };
            for (QString s : listOpt)
            {
                QTreeWidgetItem* subItem = new QTreeWidgetItem(QStringList(s));
                subItem->setFont(0, fixedFont());
                item2->addChild(subItem);
            }

            QTreeWidgetItem* item3 = new QTreeWidgetItem(QStringList(tr("Link List: %1").arg(toHex(frame.payload(), 2))));
            root->addChild(item3);

            for (ZBByte i = 0; i < cmd.linkCnt; i++)
            {
                const NWKCommandFrame::Link& entry = cmd.links[i];

                QTreeWidgetItem* subItem = new QTreeWidgetItem(QStringList(tr("Link %1: %2").arg(toNum(i)).arg(toHex(entry.bin))));
                item3->addChild(subItem);

                QTreeWidgetItem* ssubItem1 = new QTreeWidgetItem(QStringList(tr("Neighbor Network Address: %1").arg(toHex(entry.neighbor))));
                subItem->addChild(ssubItem1);

                QTreeWidgetItem* ssubItem2 = new QTreeWidgetItem(QStringList(tr("Status: %1").arg(toHex(entry.status))));
                subItem->addChild(ssubItem2);

                // link status
                QList<QString> listStatus =
                {
                    tr(".... .%1 = Incoming Cost: %2").arg(toBit(entry.incoming, 3)).arg(toNum(entry.incoming)),
                    tr(".... %1... = Reserved: %2").arg(toBit(entry.rsvd1)).arg(toHex(entry.rsvd1)),
                    tr(".%1 .... = Outgoing Cost: %2").arg(toBit(entry.outgoing, 3)).arg(toNum(entry.outgoing)),
                    tr("%1... .... = Reserved: %2").arg(toBit(entry.rsvd2)).arg(toHex(entry.rsvd2))
                };
                for (QString s : listStatus)
                {
                    QTreeWidgetItem* sssubItem = new QTreeWidgetItem(QStringList(s));
                    sssubItem->setFont(0, fixedFont());
                    ssubItem2->addChild(sssubItem);
                }
            }
        }   break;

        case NWK_FRAME_COMMAND_ROUTE_RECORD:
        case NWK_FRAME_COMMAND_REJOIN_REQUEST:
        case NWK_FRAME_COMMAND_REJOIN_RESPONSE:
            root->addChild(new QTreeWidgetItem(QStringList(tr("Command Payload: %1").arg(toHex(frame.payload())))));
    }
}

void SiteSurveyWindow::addNWKBeaconFrameDecode(int idx)
{
    const NWKLayerInfo& frame = SavedFrames::instance()->nwkLayerInfo(idx);
    if (frame.empty())
        return;

    QTreeWidgetItem* root = new QTreeWidgetItem(QStringList(tr("%1: %2").arg(frame.name().c_str()).arg(toHex(frame.hdr()))));
    ui->frameDecodeView->addTopLevelItem(root);

    QList<QString> list =
    {
        tr("Protocol ID: %1").arg(toNumHex(frame.protocolID())),
        tr(".... %1 = Stack Profile: %2 (%3)").arg(toBit(frame.stackProfile(), 4)).arg(frame.profile().c_str()).arg(toHex(frame.stackProfile())),
        tr("%1 .... = Protocol Version: %2").arg(toBit(frame.protocolVer(), 4)).arg(toNumHex(frame.protocolVer())),
        tr(".... ..%1 = Reserved: %2").arg(toBit(frame.rsvd(), 2)).arg(toHex(frame.rsvd())),
        tr(".... .%1.. = Router Capacity: %2").arg(toBit(frame.routerCapacity())).arg(toBool(frame.routerCapacity())),
        tr(".%1 %2... = Device Depth: %3").arg(toBit(frame.deviceDepth()>>1, 3)).arg(toBit(frame.deviceDepth()&0x01)).arg(toNum(frame.deviceDepth())),
        tr("%1... .... = End Device Capacity: %2").arg(toBit(frame.endDeviceCapacity())).arg(toBool(frame.endDeviceCapacity())),
        tr("Extended PAN ID: %1").arg(toHex(frame.extendedPanID())),
        tr("Tx Offset: %1 (%2)").arg(toNum(frame.transmitOffset())).arg(toHex24(frame.transmitOffset())),
        tr("Update ID: %1").arg(toNumHex(frame.updateID()))
    };
    for (int i = 0; i < list.size(); i++)
    {
        QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(list[i]));
        if (i >= 1 && i <= 6)
            item->setFont(0, fixedFont());
        root->addChild(item);
    }
}

void SiteSurveyWindow::showEvent(QShowEvent * event)
{
	QMainWindow::showEvent(event);
	if (ui->widget->Floor())
		return;
	FLFloor * floor = new FLFloor(0);

	FLZoneControllor* zc = new FLZoneControllor(0);
	zc->Init();
	zc->InitDefaultIO();
	floor->addZC(zc);

	ui->widget->setFloor(floor);

	
	qDebug() << "FLZigbeeTest view show event";
}

void SiteSurveyWindow::about()
{
    QString title = tr("About Site Survey Tool");
    QString text = tr("The <b>Site Survey Tool</b> shows the wireless connections "
                      "between ZigBee devices in a building."
                      "<br/><br/>Copyright © 2016 <b>Mircom Group of Companies</b>. All rights reserved.");
    QMessageBox::about(this, title, text);
}


void SiteSurveyWindow::show_config()
{
	config->show();
}
