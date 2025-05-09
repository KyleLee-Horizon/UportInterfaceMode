#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 主布局
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 设备选择行
    QHBoxLayout *portLayout = new QHBoxLayout();
    comboPorts = new QComboBox(this);
    btnRefresh = new QPushButton("刷新", this);
    portLayout->addWidget(comboPorts);
    portLayout->addWidget(btnRefresh);

    // 模式切换按钮
    btnRS232 = new QPushButton("切换到 RS232", this);
    btnRS422 = new QPushButton("切换到 RS422", this);

    // 状态栏
    statusBar = new QStatusBar(this);

    // 组合布局
    mainLayout->addLayout(portLayout);
    mainLayout->addWidget(btnRS232);
    mainLayout->addWidget(btnRS422);
    setCentralWidget(centralWidget);
    setStatusBar(statusBar);

    // 初始检测设备
    detectSerialPorts();

    // 信号槽连接
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::refreshSerialPorts);
    connect(btnRS232, &QPushButton::clicked, this, &MainWindow::switchToRS232);
    connect(btnRS422, &QPushButton::clicked, this, &MainWindow::switchToRS422);
    connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this](int exitCode, QProcess::ExitStatus /*exitStatus*/) {
                handleCommandResult(exitCode);  // 仅传递 exitCode，忽略 exitStatus
            });
}

MainWindow::~MainWindow() {
    // 清理资源（Qt 对象自动管理，无需手动删除）
}

// 检测可用串口设备
void MainWindow::detectSerialPorts() {
    comboPorts->clear();
    QDir devDir("/dev");
    QStringList ports = devDir.entryList(QStringList() << "ttyMXUSB*" << "ttyUSB*", QDir::System);

    if (ports.isEmpty()) {
        statusBar->showMessage("未找到串口设备", 3000);
    } else {
        foreach (const QString &port, ports) {
            comboPorts->addItem("/dev/" + port);
        }
        statusBar->showMessage("找到 " + QString::number(ports.size()) + " 个设备", 2000);
    }
}

// 刷新设备列表
void MainWindow::refreshSerialPorts() {
    detectSerialPorts();
}

// 切换到 RS232 模式
void MainWindow::switchToRS232() {
    if (comboPorts->currentText().isEmpty()) {
        statusBar->showMessage("错误：未选择设备", 3000);
        return;
    }

    QStringList args;
    args << comboPorts->currentText() << "port" << "0x0";  // 根据硬件手册调整值
    process.start("setserial", args);
}

// 切换到 RS422 模式
void MainWindow::switchToRS422() {
    if (comboPorts->currentText().isEmpty()) {
        statusBar->showMessage("错误：未选择设备", 3000);
        return;
    }

    QStringList args;
    args << comboPorts->currentText() << "port" << "0x2";  // 根据硬件手册调整值
    process.start("setserial", args);
}

// 处理命令执行结果
void MainWindow::handleCommandResult(int exitCode) {
    if (exitCode == 0) {
        statusBar->showMessage("操作成功: " + process.arguments().join(" "), 3000);
    } else {
        QString error = process.readAllStandardError();
        statusBar->showMessage("错误: " + error, 5000);
    }
}
