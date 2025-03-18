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

		// HSV ��ȯ: RGB�� ���� ��ȭ�� �ΰ��ϱ� ������ HSV(Hue, Saturation, Value)�� ��ȯ
		cv::Mat hsv;
		cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

		// ����ũ ����
		//cv::Mat lower_color{ 0,120,70 };
		//cv::Mat upper_color{ 10,255,255 };
		cv::Scalar lower_color{ 0,120,70 }; // cv::Scalar�� BGR/HSV �� ������ �����ϴµ� ����
		cv::Scalar upper_color{ 10,255,255 };
		cv::Mat mask;
		cv::inRange(hsv, lower_color, upper_color, mask);

		// ������ ����
		// cv::Point - ���� ��ǥ��
		// std::vector<cv::Point> - �ϳ��� ������
		// std::vector<std::vector<cv::Point>> - ���� ������
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		double max_area = 0;
		cv::Moments M;
		std::vector<cv::Point> max_contour; // cv::Point - ������ ��ǥ || cv::Point2f - �Ǽ��� ��ǥ
		for (size_t i = 0; i < contours.size(); i++) {
			//std::vector<cv::Point> contour = contours[i]; // ���ʿ��� ����
			std::vector<cv::Point>& contour = contours[i]; // ���� ���
			double area = cv::contourArea(contour);

			if (area > max_area) {
				max_area = area;
				M = cv::moments(contour);
				max_contour = contour; // ��ȯ
			}
		}

		// ���� �߽� ���
		double cx = M.m10 / M.m00;
		double cy = M.m01 / M.m00;
		cv::circle(frame, cv::Point(cx, cy), 10, cv::Scalar(255,0,0), -1);

		std::vector<cv::Point2f> points(max_contour.begin(), max_contour.end());  // cv::Point - ������ ��ǥ || cv::Point2f - �Ǽ��� ��ǥ
		cv::Point2f center;
		float radius;
		cv::minEnclosingCircle(points, center, radius);
		int int_radius = static_cast<int>(radius);

		// �� �ѷ� �׸���
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