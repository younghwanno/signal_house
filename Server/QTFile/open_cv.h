#ifndef OPEN_CV_H
#define OPEN_CV_H

#include <QWidget>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>



#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


#include <string>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>

#include <QFont>
#include <QGraphicsScene>


#include <QCamera>
#include <QCameraViewfinder>
#include <QMediaPlayer>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QCameraInfo>
#include <QList>
#include <QFont>
#include <QVideoWidget>
#include <QThread>


using namespace cv;


class open_cv : public QWidget
{
    private :
        cv::Mat image;
        cv::Mat result_binary_image;
        cv::Mat imageRoi;
        QFont * font1, * font2;
        QLabel *scene1, *scene2;
        QLabel *output_text_num;
        QLabel *bool_text_num;


        QVBoxLayout *mainLayout;
        QHBoxLayout *hLayout1;
        QVBoxLayout *vLayout1;
        QVBoxLayout *vLayout2;
        QHBoxLayout *hLayout2;
        QHBoxLayout *hLayout3;
        QHBoxLayout *hLayout4;

        QCamera * camera1;
        int camera_state_flag1;
        QCameraImageCapture *image_capture1;
        QMediaRecorder *video_record1;
        QVideoWidget *videoWidget;


    public :
        QCamera * get_camera();
        open_cv();
        QImage Mat2QImage(Mat &);
        Mat LoadtoImage(String);
        Mat src_to_binary(Mat &);
        bool Abstract_src(Mat &, QString);
        QString OCR_src(QString);
        QPixmap PixMatImage(Mat &);

        QString get_output_text_num();
        QString get_bool_text_num();
        void set_output_text_num(QString);
        void set_bool_text_num(QString);

        QCameraImageCapture* get_image_capture();

        void init_camera();
        QVBoxLayout *get_vLayout();

        bool image_processing();
};



#endif // OPEN_CV_H
