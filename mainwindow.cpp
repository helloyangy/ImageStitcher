#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 设置接受拖拽
    setAcceptDrops(true);
    setWindowTitle("微信公众号封面生成工具"); // 设置窗口标题

    // 创建按钮
    loadButton1 = new QPushButton("加载封面图片1", this);
    loadButton2 = new QPushButton("加载封面图片2", this);
    loadButton3 = new QPushButton("加载封面图片3", this);
    stitchButton = new QPushButton("合成封面图片", this);
    saveButton = new QPushButton("保存合成封面图片", this);

    // 设置按钮样式
    QString buttonStyle = "QPushButton {"
                          "background-color: #4CAF50; " // 默认背景色
                          "color: white; " // 文本颜色
                          "border: none; " // 无边框
                          "padding: 10px; " // 内边距
                          "font-size: 16px; " // 字体大小
                          "border-radius: 5px;" // 圆角
                          "} "
                          "QPushButton:hover {"
                          "background-color: #45a049; " // 悬停时背景色
                          "}";

    loadButton1->setStyleSheet(buttonStyle);
    loadButton2->setStyleSheet(buttonStyle);
    loadButton3->setStyleSheet(buttonStyle);
    stitchButton->setStyleSheet(buttonStyle);
    saveButton->setStyleSheet(buttonStyle);

    // 创建用于显示图片的 QLabel
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(600, 400);
    imageLabel->setAlignment(Qt::AlignCenter); // 居中对齐

    // 显示图片路径的 QLabel
    pathLabel1 = new QLabel(this);
    pathLabel2 = new QLabel(this);
    pathLabel3 = new QLabel(this);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(loadButton1);
    layout->addWidget(pathLabel1);
    layout->addWidget(loadButton2);
    layout->addWidget(pathLabel2);
    layout->addWidget(loadButton3);
    layout->addWidget(pathLabel3);
    layout->addWidget(stitchButton);
    layout->addWidget(saveButton);
    layout->addWidget(imageLabel);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // 连接按钮与槽函数
    connect(loadButton1, &QPushButton::clicked, this, &MainWindow::loadImage1);
    connect(loadButton2, &QPushButton::clicked, this, &MainWindow::loadImage2);
    connect(loadButton3, &QPushButton::clicked, this, &MainWindow::loadImage3);
    connect(stitchButton, &QPushButton::clicked, this, &MainWindow::stitchImages);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
}

MainWindow::~MainWindow() {}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction(); // 接受拖放操作
    } else {
        event->ignore(); // 忽略不支持的拖放
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    // 处理拖放事件
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        foreach (const QUrl &url, mimeData->urls()) {
            QString fileName = url.toLocalFile();
            if (fileName.endsWith(".png") || fileName.endsWith(".jpg") || fileName.endsWith(".bmp")) {
                if (image1.isNull()) {
                    image1.load(fileName);
                    pathLabel1->setText("封面图片1路径: " + fileName);
                } else if (image2.isNull()) {
                    image2.load(fileName);
                    pathLabel2->setText("封面图片2路径: " + fileName);
                } else if (image3.isNull()) {
                    image3.load(fileName);
                    pathLabel3->setText("封面图片3路径: " + fileName);
                } else {
                    QMessageBox::warning(this, "错误", "最多只能加载三张封面图片。");
                }
            }
        }
    }
}

void MainWindow::loadImage1()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择封面图片 1", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty() && image1.load(fileName)) {
        pathLabel1->setText("封面图片1路径: " + fileName);
    } else {
        QMessageBox::warning(this, "错误", "加载封面图片1失败，请重试。");
    }
}

void MainWindow::loadImage2()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择封面图片 2", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty() && image2.load(fileName)) {
        pathLabel2->setText("封面图片2路径: " + fileName);
    } else {
        QMessageBox::warning(this, "错误", "加载封面图片2失败，请重试。");
    }
}

void MainWindow::loadImage3()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择封面图片 3", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty() && image3.load(fileName)) {
        pathLabel3->setText("封面图片3路径: " + fileName);
    } else {
        QMessageBox::warning(this, "错误", "加载封面图片3失败，请重试。");
    }
}

void MainWindow::stitchImages()
{
    if (image1.isNull() || image2.isNull() || image3.isNull()) {
        QMessageBox::warning(this, "错误", "请加载三张封面图片！");
        return;
    }

    stitchedImage = stitchThreeImages(image1, image2, image3);
    imageLabel->setPixmap(QPixmap::fromImage(stitchedImage).scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

QImage MainWindow::stitchThreeImages(const QImage &img1, const QImage &img2, const QImage &img3)
{
    const double targetAspectRatio = 2.35;

    int totalWidth = img1.width() + img2.width() + img3.width();
    int targetHeight = static_cast<int>(totalWidth / targetAspectRatio);

    QImage stitchedImage(totalWidth, targetHeight, QImage::Format_ARGB32);
    stitchedImage.fill(Qt::transparent);

    QPainter painter(&stitchedImage);
    int currentX = 0;

    painter.drawImage(currentX, (targetHeight - img1.height()) / 2, img1);
    currentX += img1.width();
    painter.drawImage(currentX, (targetHeight - img2.height()) / 2, img2);
    currentX += img2.width();
    painter.drawImage(currentX, (targetHeight - img3.height()) / 2, img3);

    return stitchedImage;
}

void MainWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存合成封面图片", "", "Images (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty() && !stitchedImage.isNull()) {
        stitchedImage.save(fileName);
    } else {
        QMessageBox::warning(this, "错误", "保存合成封面图片失败，请重试。");
    }
}
