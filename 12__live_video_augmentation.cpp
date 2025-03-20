#include <opencv2/opencv.hpp>

void ApplyAugmentation(cv::Mat& frame) {
	// ��� ��ȯ�� ����Ͽ� �̹����� ���Ǿ� ������ ��ȯ
	cv::Mat sepiaKernel = (cv::Mat_<float>(3,3) <<
		0.272, 0.534, 0.131,
		0.349, 0.686, 0.168,
		0.393, 0.769, 0.189);

	cv::transform(frame, frame, sepiaKernel);

	// ��ȯ ���� �ȼ� ���� 0~255 ������ ����� �ʵ��� ����
	// cv::max(frame, 0): �ȼ� ���� 0���� ������ 0���� ����
	// cv::min(..., 255): �ȼ� ���� 255���� ũ�� 255�� ����.
	cv::min(cv::max(frame, 0), 255); 
	frame.convertTo(frame, CV_8U); // cv::Mat�� float Ÿ���� ��� 8-bit �̹����� ��ȯ.
}

void AdjustBrightness(cv::Mat& frame) {
	// �̹����� �ȼ� ���� �����ϰ� �����ϸ��� ��, ���밪�� �����Ͽ� 8��Ʈ �̹����� ��ȯ�ϴ� �Լ�
	// �̹��� ó�� �߿� ������ �ϴٺ��� �ȼ� ���� 0���� �۰ų� 255���� Ŀ�� �� �ִ�. �̷� �� ��� �ȼ� ���� �����ϰ� 0~255 ���̷� ��ȯ
	cv::convertScaleAbs(frame, frame, 1);
}

int main12_() {
	cv::Mat frame;
	cv::VideoCapture cap;

	cap.open("person2.mp4");
	if (!cap.isOpened()) {
		return -1;
	}

	while (true) {
		cap.read(frame);

		if (frame.empty()) {
			break;
		}

		ApplyAugmentation(frame);
		AdjustBrightness(frame);

		cv::imshow("window", frame);
		
		if (cv::waitKey(25) == 'q') {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}