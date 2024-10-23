#ifndef PI
#define PI 3.141592653589793238462643383279502884L
#endif

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

void plotHistogram(const int* hist, const int* cum_hist);

using namespace std;
using namespace cv;

void applyGaussian(const Mat& input, const double kernel[5][5], Mat& output, int channel) {
    int kernelRadiusX = 2;
    int kernelRadiusY = 2;

    output.create(input.size(), input.type());

    for (int y = kernelRadiusY; y < input.rows - kernelRadiusY; ++y) {
        for (int x = kernelRadiusX; x < input.cols - kernelRadiusX; ++x) {
            double sum = 0.0;
            for (int j = -kernelRadiusY; j <= kernelRadiusY; ++j) {
                for (int i = -kernelRadiusX; i <= kernelRadiusX; ++i) {
                    sum += input.at<Vec3b>(y + j, x + i)[channel] * kernel[j + kernelRadiusY][i + kernelRadiusX];
                }
            }
            output.at<Vec3b>(y, x)[channel] = static_cast<uchar>(abs(sum));
        }
    }
}

void generateGaussianKernel(double kernel[5][5], double sigma) {
    double sum = 0.0;
    int kernelRadiusX = 2;
    int kernelRadiusY = 2;

    for (int j = -kernelRadiusY; j <= kernelRadiusY; ++j) {
        for (int i = -kernelRadiusX; i <= kernelRadiusX; ++i) {
            kernel[j + kernelRadiusY][i + kernelRadiusX] =
                exp(-(i * i + j * j) / (2.0 * sigma * sigma)) / (2.0 * PI * sigma * sigma);
            sum += kernel[j + kernelRadiusY][i + kernelRadiusX];
        }
    }

    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i < 5; ++i) {
            kernel[j][i] /= sum;
        }
    }
}


string gauss(string path, double ratio) {
    Mat image = imread(path, IMREAD_COLOR);

    int rows = image.rows;
    int cols = image.cols;

    double sigma = ratio;
    double gaussianKernel[5][5];
    generateGaussianKernel(gaussianKernel, sigma);

    Mat blurred_image(rows, cols, CV_8UC3);
    for (int channel = 0; channel < 3; ++channel) {
        applyGaussian(image, gaussianKernel, blurred_image, channel);
    }

    imshow("Original Image", image);
    imshow("Colored blurred image", blurred_image);

    string newPath;
    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, blurred_image);
    waitKey(0);

    return newPath;
}