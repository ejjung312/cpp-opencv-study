#include <opencv2/opencv.hpp>

int main() {
	cv::Mat frame;
	cv::VideoCapture cap("person6.mp4");

	if (!cap.isOpened()) {
		return -1;
	}

	cap.read(frame);

	cap.set(cv::CAP_PROP_POS_FRAMES, 0);

	cv::Mat background = cv::Mat::zeros(frame.size(), frame.type());

	while (true) {
		cap.read(frame);

		if (frame.empty()) break;

		cv::Mat hsv;
		cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        /*
		* H-색조
		* 빨간색: 0~60
		* 노란색: 61~120
		* 녹색: 121~180
		* 시안: 181~240
		* 파란색: 241~300
		* 마젠타: 301~360
		* - 빨강(0°), 초록(120°), 파랑(240°)
		* 
		* S-채도
		* - 색상의 회색양
		* - 0: 회색, 255: 선명
		* 
		* V-값
		* - 색상의 밝기나 강도
		* - 0: 어두움, 255: 밝음
		*/

		// cv::Scalar는 OpenCV에서 색상 범위를 표현하는 기본적인 방식. cv::Scalar(B,G,R) 순서로 저장
		cv::Mat mask1;
		cv::Scalar lower_red1(0, 120, 50); 
		cv::Scalar upper_red2(10, 255, 255);
		cv::inRange(hsv, lower_red1, upper_red2, mask1);

		cv::Mat mask2;
		cv::Scalar lower_red3(170, 120, 70);
		cv::Scalar upper_red4(180, 255, 255);
		cv::inRange(hsv, lower_red3, upper_red4, mask2);

		// 빨간색 첫 번째 범위와 두 번째 범위에 속하는 모든 픽셀을 하나의 마스크로 합침
		mask1 = mask1 + mask2;

		// 빨간색 마스크에서 외곽선 감지
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(mask1, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		// 빈 마스크 생성
		cv::Mat filled_mask = cv::Mat::zeros(mask1.size(), mask1.type());

		// 외곽선 내부 채우기
		cv::drawContours(filled_mask, contours, -1, 255, cv::FILLED);

		// 모폴로지
		cv::morphologyEx(mask1, mask1, cv::MORPH_OPEN, cv::Mat::ones(cv::Size(3, 3), CV_8U));
		cv::morphologyEx(mask1, mask1, cv::MORPH_DILATE, cv::Mat::ones(cv::Size(3, 3), CV_8U));

		cv::bitwise_not(mask1, mask2);

		cv::Mat res1, res2;
		cv::bitwise_and(frame, frame, res1, mask2);
		cv::bitwise_and(background, res2, mask1);

		cv::Mat finalOutput;
		cv::addWeighted(res1, 1, res2, 1, 0, finalOutput);

		cv::imshow("magic", finalOutput);

		if (cv::waitKey(25) == 'q') {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}