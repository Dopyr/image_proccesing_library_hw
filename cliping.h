#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

string newPath;

struct UserData {
    Point start;
    Point end;
    Mat image;
    string pathStruct;
};

void onMouseClick(int event, int x, int y, int flags, void* userdata) {
    UserData* data = static_cast<UserData*>(userdata);

    if (event == EVENT_LBUTTONDOWN) {
        data->start = cv::Point(x, y);
    }
    else if (event == EVENT_LBUTTONUP) {
        data->end = cv::Point(x, y);

        int rows, cols;

        rows = abs(data->start.y - data->end.y);
        cols = abs(data->start.x - data->end.x);

        int y_n = 0;
        Mat new_image(rows, cols, CV_8UC3);
        for (int y = min(data->start.y, data->end.y); y < max(data->start.y, data->end.y); y++) {
            int x_n = 0;
            for (int x = min(data->start.x, data->end.x); x < max(data->start.x, data->end.x); x++) {
                new_image.at<Vec3b>(y_n, x_n) = data->image.at<cv::Vec3b>(y, x);
                x_n += 1;
            }
            y_n += 1;
        }

        imshow("Modified Image", new_image);
        imwrite(data->pathStruct, new_image);
    }
}


string clipping(string path) {

    Mat image = imread(path, IMREAD_COLOR);
    string newPath;

    int rows = image.rows;
    int cols = image.cols;

    imshow("Original Image", image);

    UserData data;
    data.image = image;

    size_t lastBackslashPos = path.find_last_of('\\');
    newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
        path.substr(lastBackslashPos + 1);


    if (path.find("new_") != std::string::npos) {
        data.pathStruct = path;
        cout << data.pathStruct << endl;
    }
    else {
        data.pathStruct = newPath;
        cout << data.pathStruct << endl;
    }
    setMouseCallback("Original Image", onMouseClick, &data);
    waitKey(0);

    string dataNewPath = data.pathStruct;
    return dataNewPath;
}