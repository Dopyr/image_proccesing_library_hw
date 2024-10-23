#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat image, grayscaleImage;

    string image_path = "C:/Users/Dogukan/Downloads/lena.jpg";
    image = imread(image_path, IMREAD_COLOR);

    if (image.empty()) {
        cout << "Error: Couldn't read the image." << endl;
        return -1;
    }

    int rows = image.rows;
    int cols = image.cols;

    unsigned char* image_array = new unsigned char[rows * cols];

    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);
            image_array[index++] = static_cast<unsigned char>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
        }
    }

    int bit_value;
    cout << "Enter new bit value for image" << endl;
    cin >> bit_value;

    if (bit_value < 1 || bit_value > 8) {
        cout << "Invalid number of quantization levels. Exiting." << endl;
        delete[] image_array;
        return -1;
    }

    int bit_pow = bit_value * bit_value;
    int step_size = 256 / bit_pow;

    cv::Mat new_image(rows, cols, CV_8UC1);
    index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uchar gray_value = image_array[index];

            int quantized_value = (gray_value / step_size) * step_size;
            new_image.at<uchar>(i, j) = static_cast<uchar>(quantized_value);
            index++;
        }
    }

    delete[] image_array;

    imshow(" Grayscale Image", new_image);
    waitKey(0);

    return 0;
}
