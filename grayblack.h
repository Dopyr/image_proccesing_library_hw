#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

string grayToBlack(int step_value, string path) {
    Mat image, grayImage;
    string newPath;
    image = imread(path, IMREAD_COLOR);

    int rows = image.rows;
    int cols = image.cols;

    unsigned char* imageArray = new unsigned char[rows * cols];

    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);
            imageArray[index++] = static_cast<unsigned char>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
        }
    }

    Mat blackImage(rows, cols, CV_8UC1);
    index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uchar grayValue = imageArray[index];
            if (grayValue < step_value) {
                blackImage.at<uchar>(i, j) = 0;
            }
            else {
                blackImage.at<uchar>(i, j) = 255;
            }
            index++;
        }
    }

    delete[] imageArray;

    imshow("Normal Image", image);
    imshow("Resized Grayscale Image", blackImage);

    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, blackImage);
    waitKey(0);
    return newPath;
}
