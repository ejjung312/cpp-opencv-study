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

		// �׷��̽����� ó��
		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// ���� ����
		// -- ��� �̹����� ��� ����
		cv::Mat inverted;
		cv::bitwise_not(gray, inverted);

		// �� ó��
		// -- Ŀ�� ũ�Ⱑ �۾����� ������ ��, ũ�� �ϸ� �ε巯�� ��
		cv::Mat blurred;
		cv::GaussianBlur(inverted, blurred, cv::Size(21, 21), 0);

		// �� �̹��� �ռ�
		cv::Mat inverted_blurred;
		cv::bitwise_not(blurred, inverted_blurred);

		// ����ġ ���� ����
		cv::Mat sketch;
		cv::divide(gray, inverted_blurred, sketch, 256);

		cv::imshow("sketch", sketch);

		if (cv::waitKey(25) == 'q') {
			break;
		}
	}

	return 0;
}