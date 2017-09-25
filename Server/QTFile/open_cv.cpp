#include "open_cv.h"
#include <QDebug>
#include <vector>
#include <cstdlib>

using namespace cv;

QImage open_cv::Mat2QImage(Mat &src)
{
    Mat des;
    cvtColor(src, des,CV_BGR2RGB);

    QImage dest((const uchar *) des.data, des.cols ,des.rows, des.step, QImage::Format_RGB888);
    dest.bits();

    return dest;
}


Mat open_cv::LoadtoImage(String imagepath)
{
    Mat image;
    image = imread(imagepath, IMREAD_COLOR);

    if(!image.data)
    {
        qDebug() << "Image open Fail!";
        exit;
    }

    return image;
}



Mat open_cv::src_to_binary(Mat &image)
{
    Mat gray_image, element, opened, tophat, result_binary_image;

    cvtColor(image, gray_image, CV_RGB2GRAY);
    element = Mat(101,101,CV_8U,cv::Scalar(1));

    morphologyEx(gray_image,opened,MORPH_OPEN,element);   ////////이미지 윤곽선 추출을 위한 열림연산
    subtract(gray_image, opened, tophat);                      ///////////원본 이미지에서 열림연산을 통한 배경 이미지를 빼서 윤곽선 추출
    threshold(tophat, result_binary_image,127,255,THRESH_BINARY);  //////////// 윤곽선 추출한 이미지를 이진화

    return result_binary_image;
}


bool open_cv::Abstract_src(Mat &result_binary_image, QString open_cv_path)
{
    bool ret = false;
    Mat imageRoi;
    Mat img_labels, stats, centroid;
    int numofLabels;
    int area, left, top, width, height;

    numofLabels = connectedComponentsWithStats(result_binary_image,img_labels,stats,centroid,8,CV_32S);  /////////// 이진화 한 이미지를 레이블링 하여 비슷한 윤곽끼리 연결함


    for(int j=0; j<numofLabels; j++)
    {
        area = stats.at<int>(j,CC_STAT_AREA);
        left = stats.at<int>(j,CC_STAT_LEFT);
        top  = stats.at<int>(j, CC_STAT_TOP);
        width = stats.at<int>(j, CC_STAT_WIDTH);
        height  = stats.at<int>(j, CC_STAT_HEIGHT);

        if((width >= 3*height))
        {
            if((width >= 0.28*result_binary_image.rows) && (height >= 0.05*result_binary_image.cols))
            {
                rectangle(result_binary_image,Point(left,top),Point(left+width,top+height), Scalar(255,0,0),1);  //////////if문에 적합한 조건의 레이블를 찾아서 사각형으로 표시
                imageRoi = result_binary_image(cv::Rect(left,top, width+1, height+1)); ////////// 표시한 사각형 부분만 따로 잘라냄

                qDebug() << "어브스트랙 성공!!";
                imwrite(open_cv_path.toStdString(),imageRoi);
                ret = true;
                break;
            }
        }
    }

    return ret;
}




QString open_cv::OCR_src(QString open_cv_path)
{
    bool start_falg = false;
    char *outText;
    char text[20] = {0};

    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

    if (api->Init(NULL, "kor"))
    {
        qDebug() << "Could not initialize tesseract.\n";
        exit(1);
    }

    Pix *piximage = pixRead(open_cv_path.toStdString().c_str());  //이미지파일 불러옴
    api->SetImage(piximage);

    // OCR result
    outText = api->GetUTF8Text();
    int i= 0, j = 0;;

    //qDebug() << outText;

       // Destroy used object and release memory

    if(strlen(outText) > 0)
    {
        while(outText[i] != '\n')
        {

            if(outText[i] > 0 && outText[i] > 9 && !start_falg)
            {
                start_falg = true;
            }

            if(start_falg)
            {
                text[j++] = outText[i];
                if(j == 11)break;
            }

            i++;
        }
    }
    else
    {
        strcpy(text,"invalid");
    }

            api->End();
            delete [] outText;
            pixDestroy(&piximage);

   return QString(text).replace(" ","");
}



QPixmap open_cv::PixMatImage(Mat &image)
{
    QImage qimg = Mat2QImage(image);
    QPixmap buf = QPixmap::fromImage(qimg);
    buf = buf.scaled(qimg.width(),qimg.height());

    return buf;
}



open_cv::open_cv()
{
    hLayout1 = new QHBoxLayout();
    vLayout1 = new QVBoxLayout();
    vLayout2 = new QVBoxLayout();
    hLayout2 = new QHBoxLayout();
    hLayout3 = new QHBoxLayout();
    hLayout4 = new QHBoxLayout();

    scene1 = new QLabel();
    scene2 = new QLabel();
    output_text_num = new QLabel();
    bool_text_num = new QLabel();

    hLayout1->addItem(vLayout1);
    hLayout1->addItem(vLayout2);

    vLayout2->addItem(hLayout2);
    vLayout2->addItem(hLayout3);
    vLayout2->addItem(hLayout4);


    vLayout1->addWidget(scene1);


    QFont *font = new QFont("Arial",9);
    QLabel *image_num = new QLabel("사진 번호 : ");
    image_num->setFont(*font);

    hLayout2->addWidget(scene2);
    hLayout3->addWidget(output_text_num);
    this->init_camera();

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(hLayout1);
}


QCameraImageCapture* open_cv::get_image_capture()
{
    return this->image_capture1;
}


bool open_cv::image_processing()
{

    bool ret = false;
    Mat image, image2, image3, drawing;  //  Make images.
    Rect rect, temp_rect;  //  Make temporary rectangles.
    double alpha = 1.5;
    int beta = 50;
    std::vector<std::vector<Point> > contours;  //  Vectors for 'findContours' function.
    std::vector<Vec4i> hierarchy;

    double ratio, delta_x, delta_y, gradient;  //  Variables for 'Snake' algorithm.
    int select, plate_width, count, friend_count = 0, refinery_count = 0;

    image = imread("/home/csy/test/images/still_shot2.jpg");  //  Load an image file.
    image.copyTo(image2);  //  Copy to temporary images.
    image.copyTo(image3);  //  'image2' - to preprocessing, 'image3' - to 'Snake' algorithm.

    cvtColor(image2, image2, CV_BGR2GRAY);  //  Convert to gray image.
    Canny(image2, image2, 100, 300, 3);  //  Getting edges by Canny algorithm.

    //  Finding contours.
    cv::findContours(image2, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point());
    std::vector<std::vector<Point> > contours_poly(contours.size());
    std::vector<Rect> boundRect(contours.size());
    std::vector<Rect> boundRect2(contours.size());

    //  Bind rectangle to every rectangle.
    for(int i = 0; i< contours.size(); i++)
    {
        approxPolyDP(Mat(contours[i]), contours_poly[i], 1, true);
        boundRect[i] = boundingRect(Mat(contours_poly[i]));
    }

    drawing = Mat::zeros(image2.size(), CV_8UC3);

    for(int i = 0; i< contours.size(); i++){

        ratio = (double) boundRect[i].height / boundRect[i].width;

        if((ratio <= 2.5) && (ratio >= 0.5) && (boundRect[i].area() <= 700)&& (boundRect[i].area() >= 100))
        {
            drawContours(drawing, contours, i, Scalar(0,255,255), 1, 8, hierarchy, 0, Point());
            rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(255,0,0), 1, 8, 0);

            boundRect2[refinery_count] = boundRect[i];
            refinery_count += 1;
        }
    }

    boundRect2.resize(refinery_count);  //  Resize refinery rectangle array.

    for(int i=0; i<boundRect2.size(); i++)
    {
        for(int j=0; j<(boundRect2.size()-i); j++)
        {
            if(boundRect2[j].tl().x > boundRect2[j+1].tl().x)
            {
                temp_rect = boundRect2[j];
                boundRect2[j] = boundRect2[j+1];
                boundRect2[j+1] = temp_rect;
            }
        }
    }

    for(int i = 0; i< boundRect2.size(); i++)
    {
        rectangle(image3, boundRect2[i].tl(), boundRect2[i].br(), Scalar(0,255,0), 1, 8, 0);
        count = 0;

        //  Snake moves to right, for eating his freind.
        for(int j=i+1; j<boundRect2.size(); j++)
        {
            delta_x = abs(boundRect2[j].tl().x - boundRect2[i].tl().x);

            if(delta_x > 150)  //  Can't eat snake friend too far ^-^.
                break;

            delta_y = abs(boundRect2[j].tl().y - boundRect2[i].tl().y);
            //  If delta length is 0, it causes a divide-by-zero error.

            if(delta_x == 0)
            {
                delta_x = 1;
            }

            if(delta_y == 0)
            {
                delta_y = 1;
            }

            gradient = delta_y / delta_x;  //  Get gradient.

            if(gradient < 0.25)
            {
                count += 1;
            }
        }

            //  Find the most full snake.
            if(count > friend_count)
            {
                select = i;  //  Save most full snake number.
                friend_count = count;  //  Renewal number of friends hunting.
                rectangle(image3, boundRect2[select].tl(), boundRect2[select].br(), Scalar(255,0,0), 1, 8, 0);
                plate_width = delta_x;  //  Save the last friend ate position.  

                qDebug() << friend_count;
            }                           //  It's similar to license plate width, Right?
    }


    imshow("Rectangles on original", image3);

        if(friend_count > 0 &&
                boundRect2[select].tl().x - 5 > 0 && boundRect2[select].tl().y - 5 > 0 &&
                boundRect2[select].tl().x + 150 < image.rows &&
                boundRect2[select].tl().y + 30 * 0.3< image.cols )
        {

            image = image(Rect(boundRect2[select].tl().x - 5,
                               boundRect2[select].tl().y -5,
                               150,
                               30
                               ));

            image = src_to_binary(image);
            imwrite("/home/csy/test/images/1-1.JPG", image);
            imshow("image3",image);
            QString ocr = OCR_src("/home/csy/test/images/1-1.JPG");
            qDebug() << ocr;

            if(ocr != "invalid" && ocr.length() == 7)
            {
                this->output_text_num->setText(ocr);
                ret = true;
           }
        }       
   return ret;
}



QString open_cv::get_output_text_num()
{
    return this->output_text_num->text();
}

QString open_cv::get_bool_text_num()
{
    return this->bool_text_num->text();
}

void open_cv::set_output_text_num(QString temp)
{
    this->output_text_num->setText(temp);
}

void open_cv::set_bool_text_num(QString temp)
{
    this->bool_text_num->setText(temp);
}


QVBoxLayout * open_cv::get_vLayout()
{
    return this->vLayout1;
}

QCamera* open_cv::get_camera()
{
    return this->camera1;
}


void open_cv::init_camera()
{
        QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
        camera1 = NULL;

        foreach (const QCameraInfo &cameraInfo, cameras)
        {
            if (cameraInfo.deviceName() == "/dev/video0")
            {
                camera1 = new QCamera(cameraInfo, this);
            }
        }

        if(camera1 == NULL)
        {
            QString camera_invalid = "/home/csy/test/images/camera_invalid2.png";
            QLabel *temp = new QLabel();
            temp->setPixmap(QPixmap(camera_invalid).scaled(250,130));
            vLayout2->addWidget(temp);
        }
        else
        {
            image_capture1 = new QCameraImageCapture(this->camera1);
            video_record1 = new QMediaRecorder(this->camera1);

            QAudioEncoderSettings audioSettings;
            audioSettings.setQuality(QMultimedia::HighQuality);
            video_record1->setAudioSettings(audioSettings);

            QVideoEncoderSettings videoSettings = video_record1->videoSettings();

            videoSettings.setQuality(QMultimedia::HighQuality);
            video_record1->setVideoSettings(videoSettings);
            camera_state_flag1 = 0;

            videoWidget = new QVideoWidget();
            videoWidget->setFixedSize(250,130);
            this->vLayout1->addWidget(videoWidget);
            camera1->setViewfinder(videoWidget);
       }

}

