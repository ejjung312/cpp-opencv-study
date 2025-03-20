#include <opencv2/opencv.hpp>

cv::Mat CartoonifyImage(const cv::Mat& img) {
	int blur_value = 5;
	int block_size = 9;
	int bilateral_filter_value = 250;

	cv::Mat gray;
	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

	// �̵�� ���� - �ֺ� �ȼ� ���� �����Ͽ� �߾Ӱ����� �ȼ��� ��ü
	cv::medianBlur(gray, gray, blur_value);

	// ������ ����ȭ - �̹����� ���� ���� �Ӱ�ó��
	cv::Mat edges;
	cv::adaptiveThreshold(gray, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, block_size, 9);

	// bilateralFilter: ����� ���͸� �Լ�. ������ �ƴ� �κи� �����Ͽ� ���� ����
	cv::Mat color;
	cv::bilateralFilter(img, color, 9, bilateral_filter_value, bilateral_filter_value);

	// color �̹������� edges ����ũ�� ���õ� �κи� ������ ���ο� �̹���
	cv::Mat edges_color, cartoon;
	cv::cvtColor(edges, edges_color, cv::COLOR_GRAY2BGR);
	cv::bitwise_and(color, edges_color, cartoon);

	return cartoon;
}

int main() {
	cv::Mat img = cv::imread("person1.jpg");

	cv::Mat cartoon = CartoonifyImage(img);

	cv::imshow("cartoon", cartoon);

	cv::waitKey(0);

	return 0;
}