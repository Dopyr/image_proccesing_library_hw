#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void dilateCustom(const Mat& src, Mat& dst, int kernelSize) {
    int rows = src.rows;
    int cols = src.cols;

    dst = Mat::zeros(src.size(), CV_8UC1);

    int borderSize = kernelSize / 2;

    for (int i = borderSize; i < rows - borderSize; i++) {
        for (int j = borderSize; j < cols - borderSize; j++) {
            if (src.at<uchar>(i, j) == 255) {
                for (int ki = -borderSize; ki <= borderSize; ki++) {
                    for (int kj = -borderSize; kj <= borderSize; kj++) {
                        dst.at<uchar>(i + ki, j + kj) = 255;
                    }
                }
            }
        }
    }
}

string dilation(string path) {
    Mat image, grayscaleImage;
    image = imread(path, IMREAD_COLOR);

    int rows = image.rows;
    int cols = image.cols;

    unsigned char* image_array = new unsigned char[rows * cols];

    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);
            image_array[index++] = static_cast<unsigned char>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
        }
    }

    cv::Mat bw_image(rows, cols, CV_8UC1);
    index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uchar grayValue = image_array[index];
            if (grayValue < 80) {
                bw_image.at<uchar>(i, j) = 0;
            }
            else {
                bw_image.at<uchar>(i, j) = 255;
            }
            index++;
        }
    }

    Mat dilated_image_3;
    dilateCustom(bw_image, dilated_image_3, 3);

    Mat dilated_image_5;
    dilateCustom(bw_image, dilated_image_5, 5);

    Mat dilated_image_7;
    dilateCustom(bw_image, dilated_image_7, 7);

    delete[] image_array;

    imshow("Resized Grayscale Image", bw_image);
    imshow("Manually Dilated Image (3x3)", dilated_image_3);
    imshow("Manually Dilated Image (5x5)", dilated_image_5);
    imshow("Manually Dilated Image (7x7)", dilated_image_7);

    string newPath;
    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, dilated_image_3);
    waitKey(0);

    return newPath;
}
