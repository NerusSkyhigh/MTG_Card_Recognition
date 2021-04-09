

#ifndef __VERSE_AND_SET_H__
#define __VERSE_AND_SET_H__


const char* templ_probable_sim_path = "C:/Users/User/Desktop/templ_ric_verso.png";

Mat card;
Mat templ;
Mat dx, sx;
//const char* png_dir = "C:/Users/User/Desktop/Mtg_set_symbols_jpg/*.jpg";
//const char* png_dir = "C:/Users/User/Desktop/Mtg_set_symbols_with_exeptios/*.png";
const char* png_dir = "C:/Users/User/Desktop/Mtg_set_symbols/*.png";
const int match_method = 0;//TM_SQDIFF
#define CONST_RADIUS 16
#define CONST_RADIUS_LON0H 2.5

int radius_h;
int radius_l;
int scaling = 24;
vector<string> fn;
vector<double> mins_dx, mins_sx;
#define DIFFWITHWHITE true
#define SHOWOUTPUT true
#define PERCENTAGE false
#define SHOWSTARTINGIMAGE false
#define SHOWEXRACTS true
#define BLACKTOWHITE true


int perse = 100;
//const double CONST_HIGHT_SYM = 0.589743;
const double CONST_HIGHT_SYM = 0.592;
//const double CONST_LENGHT_SYM = 0.863095238;
const double CONST_LENGHT_SYM = 0.835;
const char* window_src = "Card selected";





Mat copy_rectangle(Mat imm, Point start, int cols, int rows);
void verse_and_set(Mat img);
double somm_diff(Mat trov);
double somm_diff(Mat trov, Mat templ);
void find_symbol_center(Point ret[2]);
void bound_control(int& point, int lim);
void recursive_testing(int i);
string find_verse_set() ;
void get_templ(const string path);

void match_templ(Mat extract, vector<double>& mins);


#endif
