#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 重写 dragEnterEvent 和 dropEvent 方法
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void loadImage1();          // 加载图片1
    void loadImage2();          // 加载图片2
    void loadImage3();          // 加载图片3
    void stitchImages();        // 拼接图片
    void saveImage();           // 保存拼接后的图片

private:
    QImage stitchThreeImages(const QImage &img1, const QImage &img2, const QImage &img3); // 拼接三张图片

    // UI 元素
    QPushButton *loadButton1;     // 加载按钮1
    QPushButton *loadButton2;     // 加载按钮2
    QPushButton *loadButton3;     // 加载按钮3
    QPushButton *stitchButton;     // 拼接按钮
    QPushButton *saveButton;       // 保存按钮

    QLabel *imageLabel;            // 显示图片的 QLabel
    QLabel *pathLabel1;            // 显示图片路径的 QLabel
    QLabel *pathLabel2;            // 显示图片路径的 QLabel
    QLabel *pathLabel3;            // 显示图片路径的 QLabel

    QImage image1;                 // 图片1
    QImage image2;                 // 图片2
    QImage image3;                 // 图片3
    QImage stitchedImage;          // 拼接后的图片
};

#endif // MAINWINDOW_H
