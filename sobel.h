#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void applySobel(const Mat& input, const int kernel[3][3], Mat& output) {
    int kernelRadiusX = 1;
    int kernelRadiusY = 1;

    output.create(input.size(), input.type());

    for (int y = kernelRadiusY; y < input.rows - kernelRadiusY; ++y) {
        for (int x = kernelRadiusX; x < input.cols - kernelRadiusX; ++x) {
            double sum = 0.0;
            for (int j = -kernelRadiusY; j <= kernelRadiusY; ++j) {
                for (int i = -kernelRadiusX; i <= kernelRadiusX; ++i) {
                    sum += input.at<uchar>(y + j, x + i) * kernel[j + kernelRadiusY][i + kernelRadiusX];
                }
            }
            output.at<uchar>(y, x) = static_cast<uchar>(abs(sum));
        }
    }
}

void findMinMax(const cv::Mat& matrix, double& min_val, double& max_val) {
    min_val = std::numeric_limits<double>::max();
    max_val = std::numeric_limits<double>::lowest();

    for (int i = 0; i < matrix.rows; ++i) {
        for (int j = 0; j < matrix.cols; ++j) {
            double current_val = static_cast<double>(matrix.at<uchar>(i, j));  
            if (current_val < min_val) {
                min_val = current_val;
            }
            if (current_val > max_val) {
                max_val = current_val;
            }
        }
    }
}

string sobel(string path) {
    Mat image = imread(path, IMREAD_COLOR);

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

    cv::Mat grayImage(rows, cols, CV_8UC1);
    index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grayImage.at<uchar>(i, j) = imageArray[index++];
        }
    }

    delete[] imageArray;

    int horizontal_sobel_kernel[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int vertical_sobel_kernel[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    Mat gradientGrayX, gradientGrayY, gradientGrayMagnitude;

    applySobel(grayImage, horizontal_sobel_kernel, gradientGrayX);
    applySobel(grayImage, vertical_sobel_kernel, gradientGrayY);

    Mat gradientXAbs = gradientGrayX.clone();
    Mat gradientYAbs = gradientGrayY.clone();

    for (int i = 0; i < gradientGrayX.rows; ++i) {
        for (int j = 0; j < gradientGrayX.cols; ++j) {
            gradientXAbs.at<uchar>(i, j) = static_cast<uchar>(abs(gradientGrayX.at<uchar>(i, j)));
            gradientYAbs.at<uchar>(i, j) = static_cast<uchar>(abs(gradientGrayY.at<uchar>(i, j)));
        }
    }

    gradientGrayMagnitude.create(gradientGrayX.size(), gradientGrayX.type());

    for (int i = 0; i < gradientGrayX.rows; ++i) {
        for (int j = 0; j < gradientGrayX.cols; ++j) {
            double result = 0.5 * gradientXAbs.at<uchar>(i, j) + 0.5 * gradientYAbs.at<uchar>(i, j);
            gradientGrayMagnitude.at<uchar>(i, j) = result > 255.0 ? 255 : (result < 0.0 ? 0 : static_cast<uchar>(result));
        }
    }
    double min_val, max_val;
    findMinMax(gradientGrayMagnitude, min_val, max_val);
    for (int i = 0; i < gradientGrayMagnitude.rows; ++i) {
        for (int j = 0; j < gradientGrayMagnitude.cols; ++j) {
            double normalized = 255.0 * (gradientGrayMagnitude.at<uchar>(i, j) - min_val) / (max_val - min_val);
            gradientGrayMagnitude.at<uchar>(i, j) = normalized > 255.0 ? 255 : (normalized < 0.0 ? 0 : static_cast<uchar>(normalized));
        }
    }

    imshow("Original Image", image);
    imshow("Grayscale Image", grayImage);
    imshow("Gray Gradient Magnitude", gradientGrayMagnitude);

    string newPath;
    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, gradientGrayMagnitude);
    waitKey(0);

    return newPath;
}
