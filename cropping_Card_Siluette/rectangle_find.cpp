#include"rectangle_find.h"

//Constants
//It's the bigger rectangle calculated in the image
RotatedRect bigger_rect;
//Canny_output is the resulting variable from the canny algorithm
Mat canny_output;
//these are variables for calculate the time between the
//two function
double freque;
int64 starte;

void st_timer() {
    freque = getTickFrequency();
    starte = getTickCount();
}

double sp_timer() {
    return double(getTickCount() - starte) * 1000 / freque;
}

//@param first float to switch
//@param second float to switch
//A function that switch variables
void switch_variables(float &first, float &second){
  float tmp = first;
  first = second;
  second = tmp;
}

//@param first float to switch
//@param second float to switch
//A function that switch variables
void switch_variables(int &first, int &second){
  int tmp = first;
  first = second;
  second = tmp;
}

//Finds the countors of an image
//@param image
//@return vector of vector of points
vector<vector<Point>> find_contours(Mat image) {
	blur(image,image,Size(3,3));
	Canny(image, canny_output, thresh_min, thresh_max);
	vector<vector<Point> > contours;
	findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	return contours;
}

float margine_error=0.3;//1/REAL_L_C;//==1mm

bool isSimilar(float x, float y){
	if( x < y ){
		switch_variables(x,y);
	}
	float rateo = x/y;
	if( rateo/C_RATEO_HL > 1-margine_error && rateo/C_RATEO_HL < 1+margine_error ){
		return true;
	}
	else return false;
}



//Finds the bigger rectangle in the vector of points
//@param vector of vector of points of contours
//@return The points that has the bigger rectangle
RotatedRect find_bigger_rectangle( Mat image ){
	vector<vector<Point>> contours = find_contours( image );
	//Crate the rectangle that circoscrive the contour
	//Under the size of minRect discards that rectangle
	Size2f minRect_sizes;
	if(image.size().height<image.size().width){
		minRect_sizes.height = (float)image.size().height/10;
		minRect_sizes.width  = (float)image.size().height/10;
	}
	else{
		minRect_sizes.height= (float)image.size().width/10;
		minRect_sizes.width = (float)image.size().width/10;
}
	RotatedRect minRect =RotatedRect(Point2f(-1,-1),minRect_sizes,(float)0.0);
	for( int i = 0; i < contours.size(); i++ ){
		RotatedRect rect_cont = minAreaRect( contours[i] );
		//cout<<"size of "<<i+1<<"^ rect:"<<rect_cont.size<<endl;
		if( minRect.size.height<rect_cont.size.height&&minRect.size.width<rect_cont.size.width||
		minRect.size.height<rect_cont.size.width&&minRect.size.width<rect_cont.size.height){
			if( rect_cont.center.x > 0 && rect_cont.center.y > 0 ){
				//a control to make sure that the rectangle has the right proportios
				if(isSimilar( rect_cont.size.width, rect_cont.size.height ))
					minRect = rect_cont;
			}
		}
	}
	return minRect;
}

//THIS FUNCTION IS SPECIFICALLY FOR ROTATE_RECT
//@param image to crop
//@param rotated rectangle
//@return cropped image
Mat crop_rectangle(Mat image,RotatedRect rrect){
	Rect cropped_rect(rrect.center.x-rrect.size.width/2,rrect.center.y-rrect.size.height/2,rrect.size.width,rrect.size.height);
	Rect cropped_rect2(rrect.center.x-rrect.size.height/2,rrect.center.y-rrect.size.width/2,rrect.size.height,rrect.size.width);

	if(rrect.size.height>rrect.size.width){
		return image(cropped_rect);
	}
	else{
		return image(cropped_rect2);
	}
}

//Rotate the image of a card if the rectangle isn't streight
//@param The card image
//@param The scued rectangle
//@return The card image straight
Mat rotate_rect(Mat card,RotatedRect rrect){
	Mat ret;
	//Get the center of the card and rotate the image in rispect of that angle
	Mat srect;
	if(rrect.size.height>rrect.size.width){
		srect = getRotationMatrix2D(rrect.center, rrect.angle, 1.0);
	}
	else{
		srect = getRotationMatrix2D(rrect.center, rrect.angle-90, 1.0);
	}
	//the box where the card will go (only used for size?)
	Rect2f bbox = rrect.boundingRect2f();
	warpAffine(card, ret, srect,card.size());
	return ret;
}


int main(int argv, char *argc[]){
	if(argv!=3){
		cerr<<"Error in command line call."<<endl;
		cerr<<"The call needs 2 arguments."<<endl;
		exit(0);
	}
	Mat original_img=imread(argc[1],IMREAD_GRAYSCALE);
	Mat img_sub=imread(argc[2],IMREAD_GRAYSCALE);

	if(! img_sub.data ){
        	cout <<  "Image"<<argc[2] <<" not found."<<endl ;
		return -1;
	}
	if(! original_img.data ){
		cerr<<"File "<<argc[1]<<" not found."<<endl;
		return -1;
	}
	if(img_sub.empty()){
		cerr<<"File "<<argc[2]<<" not found."<<endl;
	}

	RotatedRect rot_rect = find_bigger_rectangle(img_sub);
	if(rot_rect.center.x<0||rot_rect.center.y<0){
		cerr<<"Didn't find a rectangle."<<endl;
		exit(0);
	}
//	cout<<rot_rect.center<<endl;
	Mat rotated_image=rotate_rect(original_img,rot_rect);
//	imshow("Starting_image",img_sub);
//	imshow("Card_rotated",rotated_image);

	Mat final_image=crop_rectangle(rotated_image,rot_rect);
//	waitKey(0);
//	imshow("Card_rotated",final_image);
//	waitKey(0);
	imwrite("card.png",final_image);
	return 0;
}
