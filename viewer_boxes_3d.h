#ifndef VIEWER_BOXES_3D_H
#define VIEWER_BOXES_3D_H

#include <QWidget>
#include "qcuboidmesh.h"
#include "qphongmaterial.h"
#include <QGuiApplication>
#include <QInputDialog>
#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>
#include <QComboBox>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>
#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>
#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include "single_SKU.h"
#include <QtGlobal>
#include <QMessageBox>
#include <QLabel>
#include <QtGui>
#include <QCoreApplication>
#include <QAxObject>
#include <QDebug>
#include <QFileDialog>
#include <QApplication>
#include <QDir>
#include <Qt3DRender/QRenderStateSet>
#include <Qt3DRender/QDepthTest>
#include <QPixmap>
#include <QProcess>
#include "sql_key_edit_dialog.h"

namespace Ui {
class viewerBoxes3D;
}

class viewerBoxes3D : public QWidget
{
    Q_OBJECT

public:
    explicit viewerBoxes3D(QWidget *parent = nullptr);
    ~viewerBoxes3D();
    Qt3DExtras::Qt3DWindow* view;
    Qt3DCore::QEntity* rootEntity;
    void setSelectedData(QList<QStringList> result_list, QList<QStringList> box_list);

protected:
    void closeEvent(QCloseEvent *event) override {
        emit returnChooseMode();
        event->ignore();
    }
signals:
    void showLayout(QVector<SingleSKUBpp::LayoutResult> layout,double goods_weight,double goods_height);
    void returnHome();
    void returnChooseMode();
    void readSqlSignal();
    void getDataSignal(QString Sku_Code,int Tray_Length,int Tray_Width,int Tray_MaxHeight,int Tray_MaxWeight,int Tray_MaxNumber);
private:
    void showLayOutResult(Qt3DCore::QEntity* rootEntity, Qt3DExtras::Qt3DWindow* view, QVector<SingleSKUBpp::LayoutResult> layout,double goods_weight,double goods_height);
    Ui::viewerBoxes3D *ui = nullptr;
    void onButtonClicked(Qt3DCore::QEntity* rootEntity, Qt3DExtras::Qt3DWindow* view, int model, double length, double width, double height, double gweight);
    void onReadButtonClicked(QWidget* container, Qt3DCore::QEntity* rootEntity, Qt3DExtras::Qt3DWindow* view);
    void onLineEditEditingFinished(double &parameter, QLineEdit *lineEdit);
    sqlKeyEditDialog *key_edit_dialog = nullptr;
};

#endif // VIEWER_BOXES_3D_H
