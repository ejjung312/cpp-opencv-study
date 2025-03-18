#include "opencv2/opencv.hpp"

/*
* cv::Mat& 
* - ������ ���� �����ϸ鼭 ��ȯ�ؾ� �Ѵٸ� cv::Mat& ���
* 
* cv::Mat* 
* - ���ο� cv::Mat ��ü�� �����ϰ� ��ȯ�Ϸ��� cv::Mat* ��� - ����Ʈ ������(std::unique_ptr<cv::Mat> ��)�� ���
* - ������ ��� cv::Mat ��ü�� ��ȯ�ϴ� ���� �� �����ϰ� ȿ����
*/
cv::Mat ConvertToSketch(cv::Mat& img) {
	cv::Mat img_gray, img_inverted, img_blurred, img_inverted_blur, img_sketch;

	// �׷��̽����� �̹���
	cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

	cv::bitwise_not(img_gray, img_inverted);

	// ��ó�� - ����������, �帲ȿ��
	cv::GaussianBlur(img_inverted, img_blurred, cv::Size(21, 21), 0, 0);

	// �̹������� - �� ȿ���� ������ ������ �� �������, ��ο� ������ �� �ΰ���
	cv::bitwise_not(img_blurred, img_inverted_blur);

	// �� ���� �̹����� �ȼ� ������ ����
	cv::divide(img_gray, img_inverted_blur, img_sketch, 256.0);

	return img_sketch;
}

int main() {
	cv::Mat img = cv::imread("person1.jpg");
	if (img.empty()) {
		std::cerr << "Image load failed!" << std::endl;
		return -1;
	}

	cv::Mat img_sketch = ConvertToSketch(img);

	cv::namedWindow("img");
	cv::imshow("img", img);
	cv::imshow("img_sketch", img_sketch);

	cv::waitKey(0);

	return 0;
}