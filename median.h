#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void applyMedian(const Mat& input, Mat& output, int kernelSize, bool isColor = false) {
    int kernelRadiusX = kernelSize / 2;
    int kernelRadiusY = kernelSize / 2;

    output.create(input.size(), input.type());

    for (int y = 0; y < input.rows; ++y) {
        for (int x = 0; x < input.cols; ++x) {
            if (isColor) {
                vector<Vec3b> values;

                for (int j = -kernelRadiusY; j <= kernelRadiusY; ++j) {
                    for (int i = -kernelRadiusX; i <= kernelRadiusX; ++i) {
                        int yIndex = std::max(0, std::min(y + j, input.rows - 1));
                        int xIndex = std::max(0, std::min(x + i, input.cols - 1));
                        values.push_back(input.at<Vec3b>(yIndex, xIndex));
                    }
                }

                sort(values.begin(), values.end(), [](const Vec3b& a, const Vec3b& b) {
                    return a[0] < b[0];
                    });

                output.at<Vec3b>(y, x) = values[kernelSize * kernelSize / 2];
            }
            else {
                vector<uchar> values;

                for (int j = -kernelRadiusY; j <= kernelRadiusY; ++j) {
                    for (int i = -kernelRadiusX; i <= kernelRadiusX; ++i) {
                        int yIndex = std::max(0, std::min(y + j, input.rows - 1));
                        int xIndex = std::max(0, std::min(x + i, input.cols - 1));
                        values.push_back(input.at<uchar>(yIndex, xIndex));
                    }
                }

                sort(values.begin(), values.end());
                output.at<uchar>(y, x) = values[kernelSize * kernelSize / 2];
            }
        }
    }
}

string median(string path) {

    Mat image = imread(path, IMREAD_COLOR);
    Mat histogramized_image;

    int rows = image.rows;
    int cols = image.cols;

    Mat filteredImage;
    int kernelSize = 5;

    applyMedian(image, filteredImage, kernelSize, true);

    imshow("Original Image", image);
    imshow("Median Filtered Image", filteredImage);

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