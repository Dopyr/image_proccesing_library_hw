#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

double zoomFactor = 1.0;
Point cursorPos(-1, -1);

void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_MOUSEWHEEL) {
        if (getMouseWheelDelta(flags) > 0) {
            zoomFactor *= 2.0;
        }
        else {
            zoomFactor /= 2.0;
        }

        cursorPos.x = x;
        cursorPos.y = y;

        Mat* image = static_cast<Mat*>(userdata);

        int rows = image->rows;
        int cols = image->cols;

        int roi_height = static_cast<int>(rows / zoomFactor);
        int roi_width = static_cast<int>(cols / zoomFactor);

        int start_i = max(0, cursorPos.y - roi_height / 2);
        int start_j = max(0, cursorPos.x - roi_width / 2);

        int end_i = min(rows - 1, start_i + roi_height);
        int end_j = min(cols - 1, start_j + roi_width);

        Mat zoomed_image(rows, cols, image->type(), Scalar(0, 0, 0));
        (*image)(Range(start_i, end_i), Range(start_j, end_j)).copyTo(zoomed_image(Range(start_i, end_i), Range(start_j, end_j)));

        imshow("Zoomed Image", zoomed_image);
    }
}

int main() {
    cv::Mat image;
    string image_path = "C:/Users/Dogukan/Downloads/gamma1.jpg";
    image = cv::imread(image_path, cv::IMREAD_COLOR);

    if (image.empty()) {
        cerr << "Error loading image" << endl;
        return -1;
    }

    namedWindow("Zoomed Image", WINDOW_NORMAL);
    imshow("Zoomed Image", image);
    setMouseCallback("Zoomed Image", onMouse, &image);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
