#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Vec3b bilinearInterpolation(const Mat& img, double x, double y) {
    int x1 = static_cast<int>(floor(x));
    int y1 = static_cast<int>(floor(y));
    int x2 = min(x1 + 1, img.cols - 1);
    int y2 = min(y1 + 1, img.rows - 1);

    double alpha = x - x1;
    double beta = y - y1;

    Vec3b p1 = img.at<Vec3b>(y1, x1);
    Vec3b p2 = img.at<Vec3b>(y1, x2);
    Vec3b p3 = img.at<Vec3b>(y2, x1);
    Vec3b p4 = img.at<Vec3b>(y2, x2);

    Vec3b result;
    for (int i = 0; i < 3; ++i) {
        result[i] = static_cast<uchar>((1 - alpha) * (1 - beta) * p1[i] +
            alpha * (1 - beta) * p2[i] +
            (1 - alpha) * beta * p3[i] +
            alpha * beta * p4[i]);
    }

    return result;
}

string zoom(string path) {

    Mat image, zoomedIn, zoomedOut;
    string newPath;
    image = imread(path, IMREAD_COLOR);

    int rows = image.rows;
    int cols = image.cols;

    double zoom_factor = 1.3;
    int new_rows = static_cast<int>(rows * zoom_factor);
    int new_cols = static_cast<int>(cols * zoom_factor);

    zoomedIn.create(new_rows, new_cols, CV_8UC3);

    for (int i = 0; i < new_rows; i++) {
        for (int j = 0; j < new_cols; j++) {
            double original_i = i / zoom_factor;
            double original_j = j / zoom_factor;

            Vec3b& pixel = zoomedIn.at<Vec3b>(i, j);
            pixel = bilinearInterpolation(image, original_j, original_i);
        }
    }

    double zoom_factor_out = 0.5;
    int new_rows_out = static_cast<int>(rows * zoom_factor_out);
    int new_cols_out = static_cast<int>(cols * zoom_factor_out);

    zoomedOut.create(new_rows_out, new_cols_out, CV_8UC3);

    for (int i = 0; i < new_rows_out; i++) {
        for (int j = 0; j < new_cols_out; j++) {
            double original_i = i / zoom_factor_out; 
            double original_j = j / zoom_factor_out; 
            Vec3b& pixel = zoomedOut.at<Vec3b>(i, j);
            pixel = bilinearInterpolation(image, original_j, original_i);
        }
    }


    imshow("Original Image", image);
    imshow("Zoomed out image", zoomedOut);
    imshow("Zoomed in image", zoomedIn);
    moveWindow("Original Image", 0, 0);
    moveWindow("Zoomed out image", 405, 0);
    moveWindow("Zoomed in image", 810, 0);
    waitKey(0);

    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    char option;
    cout << "Zoomed out(y) or in(ann button)" << endl;
    cin >> option;

    if (option == 'y' || option == 'Y') {
        imwrite(newPath, zoomedOut);
    }
    else {
        imwrite(newPath, zoomedIn);
    }
    return newPath;
}
