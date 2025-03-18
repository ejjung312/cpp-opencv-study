#include <opencv2/opencv.hpp>

int main3() {
	cv::Mat frame;
	cv::VideoCapture cap;
	
	cap.open("person3.mp4");
	if (!cap.isOpened()) {
		std::cerr << "Video Open Failed" << std::endl;
		return -1;
	}

	while (true) {
		cap.read(frame);

		if (frame.empty()) {
			break;
		}

		// 그레이스케일 처리
		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// 색상 반전
		// -- 흑백 이미지의 밝기 반전
		cv::Mat inverted;
		cv::bitwise_not(gray, inverted);

		// 블러 처리
		// -- 커널 크기가 작아지면 세밀한 선, 크게 하면 부드러운 선
		cv::Mat blurred;
		cv::GaussianBlur(inverted, blurred, cv::Size(21, 21), 0);

		// 블러 이미지 합성
		cv::Mat inverted_blurred;
		cv::bitwise_not(blurred, inverted_blurred);

		// 스케치 필터 적용
		cv::Mat sketch;
		cv::divide(gray, inverted_blurred, sketch, 256);

		cv::imshow("sketch", sketch);

		if (cv::waitKey(25) == 'q') {
			break;
		}
	}

	return 0;
}