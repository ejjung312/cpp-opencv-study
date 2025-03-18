#include "opencv2/opencv.hpp"

/*
* cv::Mat& 
* - 원본을 직접 수정하면서 반환해야 한다면 cv::Mat& 사용
* 
* cv::Mat* 
* - 새로운 cv::Mat 객체를 생성하고 반환하려면 cv::Mat* 사용 - 스마트 포인터(std::unique_ptr<cv::Mat> 등)를 고려
* - 포인터 대신 cv::Mat 자체를 반환하는 것이 더 안전하고 효율적
*/
cv::Mat ConvertToSketch(cv::Mat& img) {
	cv::Mat img_gray, img_inverted, img_blurred, img_inverted_blur, img_sketch;

	// 그레이스케일 이미지
	cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

	cv::bitwise_not(img_gray, img_inverted);

	// 블러처리 - 노이즈제거, 흐림효과
	cv::GaussianBlur(img_inverted, img_blurred, cv::Size(21, 21), 0, 0);

	// 이미지반전 - 블러 효과가 강조된 영역은 더 밝아지고, 어두운 영역은 더 부각됨
	cv::bitwise_not(img_blurred, img_inverted_blur);

	// 두 개의 이미지를 픽셀 단위로 나눔
	cv::divide(img_gray, img_inverted_blur, img_sketch, 256.0);

	return img_sketch;
}

int main() {
	cv::Mat img = cv::imread("person1.jpg");
	if (img.empty()) {
		std::cerr << "Image load failed!" << std::endl;
		return -1;
	}

	cv::Mat img_sketch = ConvertToSketch(img);

	cv::namedWindow("img");
	cv::imshow("img", img);
	cv::imshow("img_sketch", img_sketch);

	cv::waitKey(0);

	return 0;
}