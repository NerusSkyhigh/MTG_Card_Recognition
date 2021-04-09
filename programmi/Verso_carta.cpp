#include "verse_and_set.h"

//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/common/2.jpg";
const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/uncommon/7.jpg";
//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/rare/1.jpg";
//const char* path_image = "C:/Users/User/Desktop/cards_db/Amonkhet/mythic/124.jpg";
const char* window_src = "Card selected";

#define SHOWOUTPUT true
#define SHOWSTARTINGIMAGE false
#define SHOWEXRACTS true
int main(int argc, char* argv[]) {
    Mat img;
    if (argc < 2) {
        img = imread(path_image, IMREAD_COLOR);

    }
    else {
        img = imread(argv[1], IMREAD_COLOR);
    }
    if (img.empty()) {
        cerr << "Can't read the image" << endl;
        return EXIT_FAILURE;
    }
    if (SHOWOUTPUT) {
        if (SHOWSTARTINGIMAGE) {
            namedWindow(window_src, WINDOW_KEEPRATIO);
            imshow(window_src, img);
            waitKey(0);
        }
        if (SHOWEXRACTS) {
            namedWindow("DX", WINDOW_AUTOSIZE);
            namedWindow("SX", WINDOW_AUTOSIZE);
        }
    }
    verse_and_set(img);
    return EXIT_SUCCESS;
}














//px differenza (trov-templ)^2
