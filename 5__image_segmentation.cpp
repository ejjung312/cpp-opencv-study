#include <iostream>
#include <opencv2/opencv.hpp>

void SegmentImage(cv::Mat& image) {
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// �̹��� ����ȭ
	// - �Ӱ谪�� ������ �Ӱ谪���� ũ�� ���, �ƴϸ� ������
	cv::Mat thresh;
	cv::threshold(gray, thresh, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)); // ���̽� �ڵ�: kernel = np.ones((3,3), np.uint8)

	// ��������
	// - �̹����� ��翡 ���� ���� ����
	// - ����(opening): ħ��(erosion) �� ��â. ����� �����ϴ� ���
	// - �ݱ�(closing): ��â(dilate) �� ħ��
	cv::Mat opening;
	cv::morphologyEx(thresh, opening, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);

	// ��â - ��ü �ܰ��� Ȯ���Ű�� ����
	cv::Mat surebg;
	cv::dilate(opening, surebg, kernel, cv::Point(-1, -1), 3);

	// �Ÿ� ��ȯ(distance transform)
	// - �ȼ����� 0�� ������κ��� �Ÿ��� �ȼ����� 255�� ������ ǥ���ϴ� ���
	// - ��ü ������ ���븦 ã�� ������ ��Ȯ�� �ľ�
	cv::Mat dist_transform;
	cv::distanceTransform(opening, dist_transform, cv::DIST_L2, 5); // ����� 32��Ʈ �ε��Ҽ���(CV_32F)�̹Ƿ�

	// 0.7 * dist_transform.max() ���� ũ�� ���, �ƴϸ� ���������� ����
	cv::Mat surefg;
	double maxVal = 0;
	// ���̽��� dist_transform.max() �ڵ�
	// �ִ밪�� �ʿ��ϹǷ� �ּҰ��� nullptr ����
	cv::minMaxLoc(dist_transform, nullptr, &maxVal);
	cv::threshold(dist_transform, surefg, 0.7 * maxVal, 255, 0);

	// surefg�� CV_32F�̹Ƿ� ����ȭ ���� �� ������ �߻��� �� ���� => CV_8U(8��Ʈ unsigned int)
	surefg.convertTo(surefg, CV_8U);

	// ���� ������ ���� ��輱 ���� ����
	cv::Mat unknown;
	cv::subtract(surebg, surefg, unknown);

	// ���̺�
	// - ���� ����Ǿ� �ִ� ��ü �ȼ��� ������ȣ�� �Ҵ��Ͽ� ������� ���м�, ���̺��, �ٿ���ڽ�, �ȼ� ����, ���� �߽� ��ǥ ���� ��ȯ�� �� �ְ� ��
	// ���̺��
	// - ������ ���̺� �����ϸ� ������ ��ü ������ ���� ��ȣ�� �Ű��� 2���� ���� ���
	cv::Mat markers;
	int numLabels = cv::connectedComponents(surefg, markers); // ���̺�� ����

	// Watershed �˰����� 0�� ��迵������ ����ϹǷ�, ���̺� ���� 1�� �������� ���� ���̺��� 1�̻��� ������ ����
	markers = markers + 1;
	// ����� ����� Ȯ������ ���� ��迵��(unknown����)�� 0���� ����
	markers.setTo(0, unknown == 255); // ���̽� �ڵ� markers[unknown == 255] = 0; ��ȯ

	cv::watershed(image, markers);

	// Watershed�� ��輱���� ������ �κ��� �ð������� ǥ��
	cv::Mat boundaryMark;
	cv::compare(markers, -1, boundaryMark, cv::CMP_EQ); // CMP_EQ: src1 is equal to src2.
	image.setTo(cv::Scalar(255,0,255), boundaryMark);
}

void DisplaySegmentedImage(cv::Mat& image) {
	cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
	cv::imshow("Segmented Image", image);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

int main5_1() {
	cv::Mat image = cv::imread("bird1.jpg");
	cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

	SegmentImage(image);
	DisplaySegmentedImage(image);

	return 0;
}