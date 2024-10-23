#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void applyMean(const Mat& input, Mat& output, int kernelSize, bool isColor = false) {
    int kernelRadiusX = kernelSize / 2;
    int kernelRadiusY = kernelSize / 2;

    output.create(input.size(), input.type());

    for (int y = kernelRadiusY; y < input.rows - kernelRadiusY; ++y) {
        for (int x = kernelRadiusX; x < input.cols - kernelRadiusX; ++x) {
            if (isColor) {
                Vec3f sum(0, 0, 0);

                for (int j = -kernelRadiusY; j <= kernelRadiusY; ++j) {
                    for (int i = -kernelRadiusX; i <= kernelRadiusX; ++i) {
                        sum += input.at<Vec3b>(y + j, x + i);
                    }
                }

                Vec3b average = static_cast<Vec3b>(sum / (kernelSize * kernelSize));

                average[0] = saturate_cast<uchar>(average[0]);
                average[1] = saturate_cast<uchar>(average[1]);
                average[2] = saturate_cast<uchar>(average[2]);

                output.at<Vec3b>(y, x) = average;
            }
            else {
                int sum = 0;

                for (int j = -kernelRadiusY; j <= kernelRadiusY; ++j) {
                    for (int i = -kernelRadiusX; i <= kernelRadiusX; ++i) {
                        sum += input.at<uchar>(y + j, x + i);
                    }
                }

                uchar average = static_cast<uchar>(sum / (kernelSize * kernelSize));

                average = saturate_cast<uchar>(average);
                output.at<uchar>(y, x) = average;
            }
        }
    }
}

string mean(string path) {

    Mat image = imread(path, IMREAD_COLOR);
    Mat histogramized_image;

    int rows = image.rows;
    int cols = image.cols;

    Mat filteredImage, filteredGrayImage;
    int kernelSize = 5;


    applyMean(image, filteredImage, kernelSize, true);

    imshow("Original Image", image);
    imshow("Colored Mean Filtered Image", filteredImage);
 
    string newPath;
    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, filteredImage);
    waitKey(0);

    return newPath;
}