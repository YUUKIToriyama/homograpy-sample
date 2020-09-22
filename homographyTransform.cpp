#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat inputImage, inputImage_copy;
vector<Point2f> vertices;
Scalar myColor = {255, 255, 0, 0.7};
int num;
void eventCallback(int event, int x, int y, int flags, void* userdata);
Mat doHomographyTransformation(int WIDTH, int HEIGHT);


int main(int argc, char** argv) {
	// 引数が与えられていない場合終了する
	if (argc <= 1) {
		cout << "Usage: ./homographyTransform [input_file]" << endl;
		return -1;
	}
	// 引数からファイル名を読み取り変数inputImageに読み出す
	// inputImage_copyはそのコピー。後に用いる
	string fileName = argv[1];
	inputImage = imread(fileName, IMREAD_COLOR);
	inputImage_copy = inputImage.clone();
	
	namedWindow("main", WINDOW_NORMAL);
	int keyCode;
	while (true) {
		imshow("main", inputImage);
		setMouseCallback("main", eventCallback, NULL);
		keyCode = waitKey(1);
		if (keyCode != -1) {
			break;
		}
	}
	return 0;
}

void eventCallback(int event, int x, int y, int flags, void* userdata) {
	// 左ボタンをクリックすると頂点を選択できる
	if (event == EVENT_LBUTTONDOWN) {
		if (4 > num) {
			// クリックした位置にマーカーを描画
			circle(inputImage, Point2f(x,y), 10, myColor, 2, LINE_8);
			// 四角形の頂点の座標をベクタに格納
			vertices.push_back(Point2f(x,y));
			// 2点目からはその点とその前の点とを結ぶ線分を描く
			if (num != 0) {
				line(inputImage, vertices[num-1], vertices[num], myColor, 2);
			}
			// 4点指定したら最初の点と4点目を結び四角形にする
			if (num == 3) {
				line(inputImage, vertices[3], vertices[0], myColor, 2);
				//fillPoly(inputImage, vertices, myColor, 8, 0);
				// ホモグラフィ変換
				Mat result = doHomographyTransformation(500, 500);
				imshow("result", result);
				imwrite("result.png", result);
				waitKey(0);
			}
			num = num + 1;
		}
	}
	// 右ボタンをクリックするとリセットできる
	if (event == EVENT_RBUTTONDOWN) {
		inputImage = inputImage_copy.clone();
		num = 0;
		vertices.clear();
	}
}

Mat doHomographyTransformation(int WIDTH, int HEIGHT) {
	vector<Point2f> v = {
		Point2f(0, 0),
		Point2f(WIDTH, 0),
		Point2f(WIDTH, HEIGHT),
		Point2f(0, HEIGHT)
	};
	Mat homographyMatrix = getPerspectiveTransform(vertices, v);
	Mat output = Mat::zeros(WIDTH, HEIGHT, 8);
	warpPerspective(inputImage_copy, output, homographyMatrix, output.size());
	return output;
}
