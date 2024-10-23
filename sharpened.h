#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void applySharpening(Mat& input, const double kernel[3][3], Mat& output, int channel) {
    int kernelRadiusX = 1;
    int kernelRadiusY = 1;

    output.create(input.size(), input.type());

    for (int y = kernelRadiusY; y < input.rows - kernelRadiusY; ++y) {
        for (int x = kernelRadiusX; x < input.cols - kernelRadiusX; ++x) {
            double sum = 0.0;
            for (int j = -kernelRadiusY; j <= kernelRadiusY; ++j) {
                for (int i = -kernelRadiusX; i <= kernelRadiusX; ++i) {
                    sum += input.at<Vec3b>(y + j, x + i)[channel] * kernel[j + kernelRadiusY][i + kernelRadiusX];
                }
            }
            output.at<Vec3b>(y, x)[channel] = saturate_cast<uchar>(sum);
        }
    }
}

string sharpening(string path) {
    Mat image = imread(path, IMREAD_COLOR);
    Mat histogramized_image;

    int rows = image.rows;
    int cols = image.cols;

    const double sharpening_kernel[3][3] = {
        {0.0, -1.0, 0.0},
        {-1.0, 5.0, -1.0},
        {0.0, -1.0, 0.0}
    };

    Mat sharpened_image = image.clone();
    for (int channel = 0; channel < 3; ++channel) {
        applySharpening(image, sharpening_kernel, sharpened_image, channel);
    }
    
    imshow("Original Image", image);
    imshow("Sharpened Image", sharpened_image);

    string newPath;
    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, sharpened_image);
    waitKey(0);

    return newPath;
}