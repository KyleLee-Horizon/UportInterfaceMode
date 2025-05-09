#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStatusBar>
#include <QProcess>
#include <QTimer>

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
    void refreshSerialPorts();      // 刷新串口列表
    void switchToRS232();
    void switchToRS422();
    void handleCommandResult(int exitCode);

private:
    void detectSerialPorts();       // 检测可用串口设备

    QComboBox *comboPorts;          // 设备下拉菜单
    QPushButton *btnRefresh;        // 刷新按钮
    QPushButton *btnRS232;
    QPushButton *btnRS422;
    QStatusBar *statusBar;
    QProcess process;
};
#endif // MAINWINDOW_H
