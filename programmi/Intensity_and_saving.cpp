#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

char *pathToDesk="C:/Users/User/Desktop/";
char *nameOutput="Output";

Mat invertingImage(Mat input) {
    cout<<"Type:"<<input.type()<<endl;
    Mat ret;
    ret.create(input.size(), input.type());
    for (int i = 0;i < input.cols ;i++) {
        for (int j = 0;j < input.rows;j++) {
            Vec3b intensity = input.at<Vec3b>(Point(i, j));
            ret.at<Vec3b>(Point(i, j)).val[0] = 255 - intensity.val[0];
            ret.at<Vec3b>(Point(i, j)).val[1] = 255 - intensity.val[1];
            ret.at<Vec3b>(Point(i, j)).val[2] = 255 - intensity.val[2];
        }
    }
    return ret;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Servono percorso dell'immagine e un numero per scalare\n";
        return -1;
    }
    char* path = argv[1];
    int scal = atoi(argv[2]);
    Mat src = imread(path, IMREAD_COLOR);
    if (src.empty()) {
        cerr << "This bitch empty!\n";
    }
    namedWindow("Input", WINDOW_AUTOSIZE);
    namedWindow("Output", WINDOW_AUTOSIZE);
    imshow("Input", src);
    Mat iminvert = invertingImage(src);
    imshow("Output", iminvert);
    waitKey();
    imwrite("C:/Users/User/Desktop/Output",iminvert);

    return 0;
}
