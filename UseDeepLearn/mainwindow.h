#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <DealWithImage/dealwithimage.h>
#include <QMessageBox>
#include "showimagewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    /***关闭时调用***/
    void closeEvent(QCloseEvent *event) override;
    /***显示时调用***/
    void showEvent(QShowEvent *event) override;

    void on_pbtn_fit_image_clicked();
    void on_pbtn_load_one_image_clicked();
    void on_pbtn_analyze_image_clicked();
    void on_checkbox_show_result_image_clicked(bool checked);

    void on_pbtn_load_neual_net_clicked();

private:
    Ui::MainWindow *ui;
    /***第一次显示标志***/
    bool _frist_show;

};
#endif // MAINWINDOW_H
