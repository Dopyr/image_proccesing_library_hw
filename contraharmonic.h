#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

double computeHarmonicMean(int x, int y, const Mat& input, int patch_size) {
    double sum = 0;
    int count = 0;

    for (int i = -patch_size; i <= patch_size; i++) {
        for (int j = -patch_size; j <= patch_size; j++) {
            if (x + i >= 0 && x + i < input.rows && y + j >= 0 && y + j < input.cols) {
                double pixel_value = static_cast<double>(input.at<uchar>(x + i, y + j));
                if (pixel_value > 0) {
                    sum += 1 / pixel_value;
                    count++;
                }
            }
        }
    }

    if (count == 0) {
        return static_cast<double>(input.at<uchar>(x, y));
    }
    else {
        return 1 / (sum / count);
    }
}

void applyContraHarmonicMeanFilter(Mat& input, int patch_size) {
    Mat result = input.clone();

    for (int x = patch_size; x < input.rows - patch_size; x++) {
        for (int y = patch_size; y < input.cols - patch_size; y++) {
            result.at<uchar>(x, y) = saturate_cast<uchar>(computeHarmonicMean(x, y, input, patch_size));
        }
    }

    input = result;
}

Mat convertToGrayscale(const Mat& colorImage) {
    int rows = colorImage.rows;
    int cols = colorImage.cols;

    Mat grayscaleImage(rows, cols, CV_8UC1);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Vec3b colorPixel = colorImage.at<Vec3b>(i, j);
            grayscaleImage.at<uchar>(i, j) = static_cast<uchar>(0.299 * colorPixel[2] + 0.587 * colorPixel[1] + 0.114 * colorPixel[0]);
        }
    }

    return grayscaleImage;
}

string contraharmonic(string path) {
    Mat image = imread(path);
    Mat gray_image = convertToGrayscale(image);

    int rows = gray_image.rows;
    int cols = gray_image.cols;

    int patch_size = 2;  

    applyContraHarmonicMeanFilter(gray_image, patch_size);

    Mat sharpenedGrayImage, sharpenedHistImage;


    imshow("Original Image", image);
    imshow("Contraharmonic Gray Image", gray_image);

    string newPath;
    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, gray_image);
    waitKey(0);

    return newPath;

    return 0;
}
