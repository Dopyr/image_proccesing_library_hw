#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "gray.h"
#include "grayblack.h"
#include "zoom1.h"
#include "cliping.h"
#include "histogram.h"
#include "quantization.h"
#include "gaussianblur.h"
#include "sharpened.h"
#include "sobel.h"
#include "mean.h"
#include "median.h"
#include "contraharmonic.h"
#include "erosion.h"
#include "dilation.h"
#include "skeletonize.h"

using namespace std;
using namespace cv;

string getFileFromUser() {
    string filePath;

    cout << "Enter the file path:";
    getline(cin, filePath);

    return filePath;
}


int main() {
    string filePath = getFileFromUser();
    filePath.erase(remove(filePath.begin(), filePath.end(), '"'), filePath.end());
    setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    Mat image = imread(filePath);
    Mat temp;
    string newPath = "";
    string newImageFileName = "";
    string tempStr = "";
    size_t lastBackslashPos;

    while (true) {
        if (!image.empty()) {
            int option;
            cout << "\nChoose an option:" << endl;
            cout << "1. Preprocessing (Convert to Grayscale)" << endl;
            cout << "2. Preprocessing (Convert to Black and White)" << endl;
            cout << "3. Filtering" << endl;
            cout << "4. Morphology" << endl;
            cout << "5. Change image" << endl;
            cout << "6. Save As" << endl;
            cout << "7. Exit" << endl;

            cout << "Enter your choice: ";
            cin >> option;

            switch (option) {
            case 1:
                while (true) {
                    char subOption;
                    cout << "\na. Convert to Grayscale" << endl;
                    cout << "b. Grayscale to Black and White (Enter threshold value)" << endl;
                    cout << "c. Zoom In and Out" << endl;
                    cout << "d. Clipping (Specify region to clip)" << endl;
                    cout << "Enter your sub-option: ";
                    cin >> subOption;

                    switch (subOption) {
                    case 'a':
                        newPath = convertGray(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'b':
                        int threshold;
                        cout << "Enter the threshold value: \n";
                        cin >> threshold;
                        newPath = grayToBlack(threshold, filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'c':
                        newPath = zoom(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'd':
                        newPath = clipping(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    default:
                        cout << "Invalid sub-option. Please choose again." << endl;
                        continue;
                    }

                    char continueOption;
                    cout << "Do you want to continue from the sub-option? (y/n): ";
                    cin >> continueOption;

                    if (continueOption != 'y' && continueOption != 'Y') {
                        break;
                    }
                }
                break;

            case 2:
                while (true) {
                    char subOption2;
                    cout << "\na. Histogram Equalization" << endl;
                    cout << "b. Image Quantization" << endl;
                    cout << "Choose a sub-option: ";
                    cin >> subOption2;

                    switch (subOption2) {
                    case 'a':
                        newPath = hist(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'b':
                        newPath = quantization(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    default:
                        cout << "Invalid sub-option. Please choose again." << endl;
                    }

                    char continueOption;
                    cout << "Do you want to continue from the sub-option? (y/n): ";
                    cin >> continueOption;

                    if (continueOption != 'y' && continueOption != 'Y') {
                        break;
                    }

                }
                break;

            case 3:
                while (true) {
                    char subOption3;
                    cout << "\na. Gaussian Blur (Enter standard deviation (?) value)" << endl;
                    cout << "b. Sharpening Filter" << endl;
                    cout << "c. Edge Detection Filter" << endl;
                    cout << "d. Mean Filter" << endl;
                    cout << "e. Median Filter" << endl;
                    cout << "f. Contraharmonic Mean Filter" << endl;
                    cout << "Enter your sub-option: ";
                    cin >> subOption3;

                    switch (subOption3) {
                    case 'a':
                        double sigma;
                        cout << "Enter the standard deviation (?) value: \n";
                        cin >> sigma;
                        newPath = gauss(filePath, sigma);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'b':
                        newPath = sharpening(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'c':
                        newPath = sobel(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'd':
                        newPath = mean(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'e':
                        newPath = median(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'f':
                        newPath = contraharmonic(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    default:
                        cout << "Invalid sub-option. Please choose again." << endl;
                    }
                    char continueOption;
                    cout << "Do you want to continue from the sub-option? (y/n): ";
                    cin >> continueOption;

                    if (continueOption != 'y' && continueOption != 'Y') {
                        break;
                    }
                }
                break;

            case 4:
                while (true) {
                    char morphologyOption;
                    cout << "\na. Erosion" << endl;
                    cout << "b. Dilation" << endl;
                    cout << "c. Skeletonize" << endl;
                    cout << "Enter your sub-option: ";
                    cin >> morphologyOption;

                    switch (morphologyOption) {
                    case 'a':
                        newPath = erosion(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'b':
                        newPath = dilation(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    case 'c':
                        newPath = skeleton(filePath);
                        filePath = newPath;
                        filePath.erase(std::remove(filePath.begin(), filePath.end(), '\"'), filePath.end());
                        break;
                    default:
                        cout << "Invalid option." << endl;
                        break;
                    }
                    char continueOption;
                    cout << "Do you want to continue from the sub-option? (y/n): ";
                    cin >> continueOption;

                    if (continueOption != 'y' && continueOption != 'Y') {
                        break;
                    }
                }
                break;
            case 5:
                filePath = getFileFromUser();
                filePath.erase(remove(filePath.begin(), filePath.end(), '"'), filePath.end());
                image = imread(filePath);
                break;
            case 6:
                cout << "Enter new file name" << endl;
                cin >> newPath;
                lastBackslashPos = filePath.find_last_of('\\');
                tempStr = filePath.substr(0, lastBackslashPos + 1);
                tempStr += newPath;

                rename(filePath.c_str(), tempStr.c_str());

                filePath = getFileFromUser();
                filePath.erase(remove(filePath.begin(), filePath.end(), '"'), filePath.end());
                image = imread(filePath);               
                break;
            case 7:
                return 0;
            default:
                cout << "Invalid option. Please choose again." << endl;
            }

        }
        else {
            cout << "Failed to open the image. Please check the file path and format.\n" << endl;
            filePath = getFileFromUser();
            filePath.erase(remove(filePath.begin(), filePath.end(), '"'), filePath.end());
            setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
            image = imread(filePath);
        }
    }

    return 0;
}
