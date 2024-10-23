#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

string quantization(string path) {
    Mat image;
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

    Mat grayImage(rows, cols, CV_8UC1);
    index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grayImage.at<uchar>(i, j) = imageArray[index++];
        }
    }

    int tonal_levels;
    cout << "Enter the number of tonal levels for quantization: ";
    cin >> tonal_levels;

    if (tonal_levels < 1 || tonal_levels > 8) {
        cout << "Invalid number of tonal levels. Exiting." << endl;
        delete[] imageArray;
    }

    int levels = 256 / tonal_levels;
    Mat quantized_image(rows, cols, CV_8UC1);
    index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            quantized_image.at<uchar>(i, j) = static_cast<uchar>((imageArray[index] / levels) * levels);
            index++;
        }
    }

    delete[] imageArray;

    imshow("Original Image", grayImage);
    imshow("Grayscale Image", quantized_image);

    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, quantized_image);

    waitKey(0);
    return newPath;
}
