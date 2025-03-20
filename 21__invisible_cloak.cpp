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
		* H-����
		* ������: 0~60
		* �����: 61~120
		* ���: 121~180
		* �þ�: 181~240
		* �Ķ���: 241~300
		* ����Ÿ: 301~360
		* - ����(0��), �ʷ�(120��), �Ķ�(240��)
		* 
		* S-ä��
		* - ������ ȸ����
		* - 0: ȸ��, 255: ����
		* 
		* V-��
		* - ������ ��⳪ ����
		* - 0: ��ο�, 255: ����
		*/

		// cv::Scalar�� OpenCV���� ���� ������ ǥ���ϴ� �⺻���� ���. cv::Scalar(B,G,R) ������ ����
		cv::Mat mask1;
		cv::Scalar lower_red1(0, 120, 50); 
		cv::Scalar upper_red2(10, 255, 255);
		cv::inRange(hsv, lower_red1, upper_red2, mask1);

		cv::Mat mask2;
		cv::Scalar lower_red3(170, 120, 70);
		cv::Scalar upper_red4(180, 255, 255);
		cv::inRange(hsv, lower_red3, upper_red4, mask2);

		// ������ ù ��° ������ �� ��° ������ ���ϴ� ��� �ȼ��� �ϳ��� ����ũ�� ��ħ
		mask1 = mask1 + mask2;

		// ������ ����ũ���� �ܰ��� ����
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(mask1, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		// �� ����ũ ����
		cv::Mat filled_mask = cv::Mat::zeros(mask1.size(), mask1.type());

		// �ܰ��� ���� ä���
		cv::drawContours(filled_mask, contours, -1, 255, cv::FILLED);

		// ��������
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