#include <opencv2/opencv.hpp>

int main() {
	cv::Mat frame;
	cv::VideoCapture cap;

	cap.open("ball_red.mp4");
	if (!cap.isOpened()) {
		std::cerr << "Video Open Error";
		return -1;
	}

	while (true) {
		cap.read(frame);
		if (frame.empty()) {
			break;
		}

		// HSV 변환: RGB는 빛의 변화에 민감하기 때문에 HSV(Hue, Saturation, Value)로 변환
		cv::Mat hsv;
		cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

		// 마스크 생성
		//cv::Mat lower_color{ 0,120,70 };
		//cv::Mat upper_color{ 10,255,255 };
		cv::Scalar lower_color{ 0,120,70 }; // cv::Scalar는 BGR/HSV 값 범위를 저장하는데 적합
		cv::Scalar upper_color{ 10,255,255 };
		cv::Mat mask;
		cv::inRange(hsv, lower_color, upper_color, mask);

		// 컨투어 검출
		// cv::Point - 개별 좌표점
		// std::vector<cv::Point> - 하나의 컨투어
		// std::vector<std::vector<cv::Point>> - 다중 컨투어
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		double max_area = 0;
		cv::Moments M;
		std::vector<cv::Point> max_contour; // cv::Point - 정수형 좌표 || cv::Point2f - 실수형 좌표
		for (size_t i = 0; i < contours.size(); i++) {
			//std::vector<cv::Point> contour = contours[i]; // 불필요한 복사
			std::vector<cv::Point>& contour = contours[i]; // 참조 사용
			double area = cv::contourArea(contour);

			if (area > max_area) {
				max_area = area;
				M = cv::moments(contour);
				max_contour = contour; // 변환
			}
		}

		// 무게 중심 계산
		double cx = M.m10 / M.m00;
		double cy = M.m01 / M.m00;
		cv::circle(frame, cv::Point(cx, cy), 10, cv::Scalar(255,0,0), -1);

		std::vector<cv::Point2f> points(max_contour.begin(), max_contour.end());  // cv::Point - 정수형 좌표 || cv::Point2f - 실수형 좌표
		cv::Point2f center;
		float radius;
		cv::minEnclosingCircle(points, center, radius);
		int int_radius = static_cast<int>(radius);

		// 원 둘레 그리기
		cv::circle(frame, center, int_radius, cv::Scalar(0, 255, 0), 3);

		cv::imshow("Video", frame);

		if (cv::waitKey(25) == 'q') {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}