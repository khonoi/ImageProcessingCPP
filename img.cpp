#include <opencv2/opencv.hpp>
#include <windows.h>
#include <commdlg.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat originalImg;
Mat currentImg;
Mat controlPanel;

// ================= CHỌN FILE =================

string openFileDialog()
{
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter =
        "Image Files\0*.jpg;*.jpeg;*.png;*.bmp\0"
        "All Files\0*.*\0";

    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if(GetOpenFileName(&ofn))
        return string(fileName);

    return "";
}

string saveFileDialog()
{
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "output.jpg";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter =
        "JPEG (*.jpg)\0*.jpg\0"
        "PNG (*.png)\0*.png\0"
        "Bitmap (*.bmp)\0*.bmp\0";

    ofn.Flags = OFN_OVERWRITEPROMPT;

    if(GetSaveFileName(&ofn))
        return string(fileName);

    return "";
}

// ================= XỬ LÝ ẢNH =================

Mat flipHorizontal(Mat img)
{
    Mat result(img.rows, img.cols, img.type());

    for(int y=0;y<img.rows;y++)
        for(int x=0;x<img.cols;x++)
            result.at<Vec3b>(y,x) =
                img.at<Vec3b>(y,img.cols-1-x);

    return result;
}

Mat flipVertical(Mat img)
{
    Mat result(img.rows,img.cols,img.type());

    for(int y=0;y<img.rows;y++)
        for(int x=0;x<img.cols;x++)
            result.at<Vec3b>(y,x) =
                img.at<Vec3b>(img.rows-1-y,x);

    return result;
}

Mat zoomIn(Mat img)
{
    int newW = img.cols * 2;
    int newH = img.rows * 2;

    Mat result(newH,newW,img.type());

    for(int y=0;y<newH;y++)
    {
        for(int x=0;x<newW;x++)
        {
            int oldX = x / 2;
            int oldY = y / 2;

            result.at<Vec3b>(y,x) =
                img.at<Vec3b>(oldY,oldX);
        }
    }

    return result;
}

Mat zoomOut(Mat img)
{
    int newW = img.cols / 2;
    int newH = img.rows / 2;

    Mat result(newH,newW,img.type());

    for(int y=0;y<newH;y++)
    {
        for(int x=0;x<newW;x++)
        {
            result.at<Vec3b>(y,x) =
                img.at<Vec3b>(y*2,x*2);
        }
    }

    return result;
}

Mat rotate90(Mat img)
{
    Mat result(img.cols,img.rows,img.type());

    for(int y=0;y<img.rows;y++)
    {
        for(int x=0;x<img.cols;x++)
        {
            result.at<Vec3b>(x,img.rows-1-y) =
                img.at<Vec3b>(y,x);
        }
    }

    return result;
}

Mat grayScale(Mat img)
{
    Mat gray(img.rows,img.cols,CV_8UC1);

    for(int y=0;y<img.rows;y++)
    {
        for(int x=0;x<img.cols;x++)
        {
            Vec3b p = img.at<Vec3b>(y,x);

            int g =
                0.114*p[0] +
                0.587*p[1] +
                0.299*p[2];

            gray.at<uchar>(y,x)=g;
        }
    }

    Mat result;
    cvtColor(gray,result,COLOR_GRAY2BGR);

    return result;
}

// ================= GIAO DIỆN =================

void drawButton(Rect r, string text)
{
    rectangle(controlPanel,r,
              Scalar(220,220,220),
              FILLED);

    rectangle(controlPanel,r,
              Scalar(0,0,0),
              2);

    putText(controlPanel,
            text,
            Point(r.x+10,r.y+35),
            FONT_HERSHEY_SIMPLEX,
            0.7,
            Scalar(0,0,0),
            2);
}

void drawUI()
{
    controlPanel =
        Mat(120,1030,CV_8UC3,
            Scalar(240,240,240));

    drawButton(Rect(10,20,100,50),"Open");
    drawButton(Rect(120,20,100,50),"Zoom+");
    drawButton(Rect(230,20,100,50),"Zoom-");
    drawButton(Rect(340,20,100,50),"Flip H");
    drawButton(Rect(450,20,100,50),"Flip V");
    drawButton(Rect(560,20,100,50),"Rotate");
    drawButton(Rect(670,20,100,50),"Gray");
    drawButton(Rect(780,20,100,50),"Save");
    drawButton(Rect(890,20,100,50),"Reset");
}

void showImage()
{
    if(!currentImg.empty())
        imshow("Image", currentImg);
}

// ================= SỰ KIỆN CHUỘT =================

void mouseCallback(int event,
                   int x,
                   int y,
                   int flags,
                   void* userdata)
{
    if(event != EVENT_LBUTTONDOWN)
        return;

    if(y < 20 || y > 70)
        return;

    // OPEN
    if(x >= 10 && x <= 110)
    {
        string path = openFileDialog();

        if(path.empty())
            return;

        originalImg = imread(path);

        if(originalImg.empty())
        {
            MessageBoxA(NULL,
                        "Khong mo duoc anh!",
                        "Error",
                        MB_OK);
            return;
        }

        currentImg = originalImg.clone();
        showImage();
    }

    // ZOOM+
    else if(x >= 120 && x <= 220)
    {
        if(!currentImg.empty())
        {
            currentImg = zoomIn(currentImg);
            showImage();
        }
    }

    // ZOOM-
    else if(x >= 230 && x <= 330)
    {
        if(!currentImg.empty())
        {
            currentImg = zoomOut(currentImg);
            showImage();
        }
    }

    // FLIP H
    else if(x >= 340 && x <= 440)
    {
        if(!currentImg.empty())
        {
            currentImg = flipHorizontal(currentImg);
            showImage();
        }
    }

    // FLIP V
    else if(x >= 450 && x <= 550)
    {
        if(!currentImg.empty())
        {
            currentImg = flipVertical(currentImg);
            showImage();
        }
    }

    // ROTATE
    else if(x >= 560 && x <= 660)
    {
        if(!currentImg.empty())
        {
            currentImg = rotate90(currentImg);
            showImage();
        }
    }

    // GRAY
    else if(x >= 670 && x <= 770)
    {
        if(!currentImg.empty())
        {
            currentImg = grayScale(currentImg);
            showImage();
        }
    }

    // SAVE
    else if(x >= 780 && x <= 880)
    {
        if(currentImg.empty())
            return;

        string savePath = saveFileDialog();

        if(savePath.empty())
            return;

        imwrite(savePath,currentImg);

        MessageBoxA(NULL,
                    "Luu anh thanh cong!",
                    "Thong bao",
                    MB_OK);
    }
    // RESET
    else if(x >= 890 && x <= 990)
    {
        if(!originalImg.empty())
        {
            currentImg = originalImg.clone();
            showImage();

            MessageBoxA(
                NULL,
                "Da khoi phuc anh goc!",
                "Reset",
                MB_OK
            );
        }
    }
}

// ================= MAIN =================

int main()
{
    drawUI();

    namedWindow("Control");

    setMouseCallback("Control",
                     mouseCallback);

    while(true)
    {
        imshow("Control", controlPanel);

        int key = waitKey(30);

        if(key == 27)
            break;
    }

    destroyAllWindows();

    return 0;
}