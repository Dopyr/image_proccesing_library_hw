#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void plotHistogram(const int* hist, const int* cum_hist) {
    int histSize = 256;
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = static_cast<int>((double)hist_w / histSize);

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
    Mat cumHistImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

    int max_hist = *max_element(hist, hist + histSize);
    int max_cum_hist = *max_element(cum_hist, cum_hist + histSize);

    for (int i = 0; i < histSize; i++) {
        int bin_height = cvRound((double)hist[i] / max_hist * histImage.rows);

        rectangle(histImage, Point(bin_w * i, hist_h), Point(bin_w * (i + 1), hist_h - bin_height),
            Scalar(0, 0, 255), -1, 8, 0);
    }

    for (int i = 0; i < histSize; i++) {
        int bin_height = cvRound((double)cum_hist[i] / max_cum_hist * cumHistImage.rows);

        rectangle(cumHistImage, Point(bin_w * i, hist_h),
            Point(bin_w * (i + 1), hist_h - bin_height),
            Scalar(0, 255, 0), -1, 8, 0);
    }
    Mat combined_image;
    hconcat(histImage, cumHistImage, combined_image);

    imshow("Histograms", combined_image);
    moveWindow("Histograms", 810, 0);
}

string hist(string path) {
    Mat image, histogramizedImage;
    string newPath;

    image = cv::imread(path, IMREAD_COLOR);

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

    int hist[256] = { 0 };

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            hist[grayImage.at<uchar>(i, j)]++;
        }
    }

    int cum_hist[256] = { 0 };
    cum_hist[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cum_hist[i] = cum_hist[i - 1] + hist[i];
    }

    int total_pixels = rows * cols;
    for (int i = 0; i < 256; i++) {
        cum_hist[i] = static_cast<int>((cum_hist[i] / static_cast<float>(total_pixels - 1)) * 255);
    }

    histogramizedImage = grayImage.clone();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            histogramizedImage.at<uchar>(i, j) = static_cast<uchar>(cum_hist[grayImage.at<uchar>(i, j)]);
        }
    }

    imshow("Original", image);
    imshow("Histogramized", histogramizedImage);
    plotHistogram(hist, cum_hist);
    moveWindow("Original", 0, 0);
    moveWindow("Histogramized", 405, 0);

    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, histogramizedImage);

    waitKey(0);
    return newPath;
}
