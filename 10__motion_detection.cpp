#include <opencv2/opencv.hpp>

int main10_1() {
	cv::Mat frame;
	cv::VideoCapture cap;

	cap.open("person3.mp4");
	if (!cap.isOpened()) {
		return -1;
	}

	cap.read(frame);
	cv::Mat prev_frame, prev_gray;
	cv::cvtColor(frame, prev_frame, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(prev_frame, prev_gray, cv::Size(21, 21), 0);

	while (true) {
		cap.read(frame);

		if (frame.empty()) {
			break;
		}

		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// 블러 처리
		// -- 가우시안 블러: 전체적인 부드러운 노이즈 제거 및 부드러운 처리
		cv::GaussianBlur(gray, gray, cv::Size(21, 21), 0);

		// 두 이미지 간 절대 차이 계산
		// 연속된 프레임 간의 차이를 계산해 움직임을 감지
		cv::Mat delta_frame;
		cv::absdiff(prev_gray, gray, delta_frame);

		// 이미지 이진화
		// -- 임계값을 설정해 임계값보다 크면 흰색, 아니면 검정색
		cv::Mat thresh;
		cv::threshold(delta_frame, thresh, 25, 255, cv::THRESH_BINARY);

		// 팽창 - 객체 외곽을 확대시키는 연산
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::dilate(thresh, thresh, kernel, cv::Point(-1, -1), 2); // thresh = cv2.dilate(thresh, None, iterations=2) 

		// 외곽선 검출
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		for (size_t i = 0; i < contours.size(); i++) {
			// 외곽선 감싸는 영역의 면적
			if (cv::contourArea(contours[i]) < 500) {
				continue;
			}

			cv::Rect rect = cv::boundingRect(contours[i]);
			cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
		}

		cv::imshow("Motion Detection", frame);
		if (cv::waitKey(25) == 'q') {
			break;
		}

		//prev_gray = gray; // 얕은 복사
		gray.copyTo(prev_gray); // 깊은 복사
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}