#include <opencv2/opencv.hpp>

cv::Mat CartoonifyImage(const cv::Mat& img) {
	int blur_value = 5;
	int block_size = 9;
	int bilateral_filter_value = 250;

	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	// 미디언 필터 - 주변 픽셀 값을 정렬하여 중앙값으로 픽셀값 대체
	cv::medianBlur(gray, gray, blur_value);

	// 적응형 이진화 - 이미지의 영역 별로 임계처리
	cv::Mat edges;
	cv::adaptiveThreshold(gray, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, block_size, 9);

	// bilateralFilter: 양방향 필터링 함수. 엣지가 아닌 부분만 블러링하여 엣지 보존
	cv::Mat color;
	cv::bilateralFilter(img, color, 9, bilateral_filter_value, bilateral_filter_value);

	// color 이미지에서 edges 마스크로 선택된 부분만 유지된 새로운 이미지
	cv::Mat edges_color, cartoon;
	cv::cvtColor(edges, edges_color, cv::COLOR_GRAY2BGR);
	cv::bitwise_and(color, edges_color, cartoon);

	return cartoon;
}

int main() {
	cv::Mat img = cv::imread("person1.jpg");

	cv::Mat cartoon = CartoonifyImage(img);

	cv::imshow("cartoon", cartoon);

	cv::waitKey(0);

	return 0;
}