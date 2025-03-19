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

		// �� ó��
		// -- ����þ� ��: ��ü���� �ε巯�� ������ ���� �� �ε巯�� ó��
		cv::GaussianBlur(gray, gray, cv::Size(21, 21), 0);

		// �� �̹��� �� ���� ���� ���
		// ���ӵ� ������ ���� ���̸� ����� �������� ����
		cv::Mat delta_frame;
		cv::absdiff(prev_gray, gray, delta_frame);

		// �̹��� ����ȭ
		// -- �Ӱ谪�� ������ �Ӱ谪���� ũ�� ���, �ƴϸ� ������
		cv::Mat thresh;
		cv::threshold(delta_frame, thresh, 25, 255, cv::THRESH_BINARY);

		// ��â - ��ü �ܰ��� Ȯ���Ű�� ����
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::dilate(thresh, thresh, kernel, cv::Point(-1, -1), 2); // thresh = cv2.dilate(thresh, None, iterations=2) 

		// �ܰ��� ����
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		for (size_t i = 0; i < contours.size(); i++) {
			// �ܰ��� ���δ� ������ ����
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

		//prev_gray = gray; // ���� ����
		gray.copyTo(prev_gray); // ���� ����
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}