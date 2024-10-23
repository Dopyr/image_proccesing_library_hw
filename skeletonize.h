#include <opencv2/opencv.hpp>

void convertToGray(const cv::Mat& input, cv::Mat& output) {
    output.create(input.size(), CV_8UC1);

    for (int i = 0; i < input.rows; ++i) {
        for (int j = 0; j < input.cols; ++j) {
            cv::Vec3b pixel = input.at<cv::Vec3b>(i, j);
            output.at<uchar>(i, j) = static_cast<uchar>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
        }
    }
}

void applyThreshold(const cv::Mat& input, cv::Mat& output, uchar threshold) {
    output.create(input.size(), CV_8UC1);

    for (int i = 0; i < input.rows; ++i) {
        for (int j = 0; j < input.cols; ++j) {
            output.at<uchar>(i, j) = (input.at<uchar>(i, j) > threshold) ? 255 : 0;
        }
    }
}

void thinningIteration(cv::Mat& img, int iter) {
    if (img.channels() != 1 || img.depth() != CV_8U || img.rows <= 3 || img.cols <= 3) {
        cerr << "Error: Invalid image dimensions or type." << std::endl;
    }

    cv::Mat marker = cv::Mat::zeros(img.size(), CV_8UC1);

    int nRows = img.rows;
    int nCols = img.cols;

    if (img.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    int x, y;
    uchar* pAbove;
    uchar* pCurr;
    uchar* pBelow;
    uchar* nw, * no, * ne;
    uchar* we, * me, * ea;
    uchar* sw, * so, * se;

    uchar* pDst;

    pAbove = NULL;
    pCurr = img.ptr<uchar>(0);
    pBelow = img.ptr<uchar>(1);

    for (y = 1; y < img.rows - 1; ++y) {
        pAbove = pCurr;
        pCurr = pBelow;
        pBelow = img.ptr<uchar>(y + 1);

        pDst = marker.ptr<uchar>(y);

        no = &(pAbove[0]);
        ne = &(pAbove[1]);
        me = &(pCurr[0]);
        ea = &(pCurr[1]);
        so = &(pBelow[0]);
        se = &(pBelow[1]);

        for (x = 1; x < img.cols - 1; ++x) {
            nw = no;
            no = ne;
            ne = &(pAbove[x + 1]);
            we = me;
            me = ea;
            ea = &(pCurr[x + 1]);
            sw = so;
            so = se;
            se = &(pBelow[x + 1]);

            int A = (*no == 0 && *ne == 1) + (*ne == 0 && *ea == 1) +
                (*ea == 0 && *se == 1) + (*se == 0 && *so == 1) +
                (*so == 0 && *sw == 1) + (*sw == 0 && *we == 1) +
                (*we == 0 && *nw == 1) + (*nw == 0 && *no == 1);
            int B = *no + *ne + *ea + *se + *so + *sw + *we + *nw;
            int m1 = iter == 0 ? (*no * *ea * *so) : (*no * *ea * *we);
            int m2 = iter == 0 ? (*ea * *so * *we) : (*no * *so * *we);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                pDst[x] = 1;
        }
    }

    img &= ~marker;
}

void bitwiseDifference(const cv::Mat& src1, const cv::Mat& src2, cv::Mat& dst) {
    CV_Assert(src1.size() == src2.size());
    CV_Assert(src1.type() == src2.type());

    dst.create(src1.size(), src1.type());

    for (int i = 0; i < src1.rows; ++i) {
        for (int j = 0; j < src1.cols; ++j) {
            dst.at<uchar>(i, j) = src1.at<uchar>(i, j) ^ src2.at<uchar>(i, j);
        }
    }
}

int customCountNonZero(const cv::Mat& img) {
    int count = 0;

    for (int i = 0; i < img.rows; ++i) {
        for (int j = 0; j < img.cols; ++j) {
            if (img.at<uchar>(i, j) != 0) {
                count++;
            }
        }
    }

    return count;
}

void thinning(cv::Mat& src, cv::Mat& dst) {
    dst = src.clone();
    dst /= 255;

    cv::Mat prev = cv::Mat::zeros(dst.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningIteration(dst, 0);
        thinningIteration(dst, 1);
        bitwiseDifference(dst, prev, diff);
        dst.copyTo(prev);
    } while (customCountNonZero(diff) > 0);

    dst *= 255;
}

string skeleton(string path) {
    cv::Mat src = cv::imread(path);

    cv::Mat bw;
    convertToGray(src, bw);
    applyThreshold(bw, bw, 10);

    thinning(bw, bw);

    imshow("Original Image", src);
    imshow("Skeletonized Image", bw);

    string newPath;
    if (path.find("new_") != std::string::npos) {
        newPath = path;
    }
    else {
        size_t lastBackslashPos = path.find_last_of('\\');
        newPath = path.substr(0, lastBackslashPos + 1) + "new_" +
            path.substr(lastBackslashPos + 1);
    }

    imwrite(newPath, bw);
    waitKey(0);

    return newPath;
}