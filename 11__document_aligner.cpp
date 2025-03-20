#include <opencv2/opencv.hpp>

cv::Mat RotateDocument(cv::Mat& image) {
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// �� ����(240) �̻��� 255, �������� ����(0)
	cv::Mat binary;
	cv::threshold(gray, binary, 240, 255, cv::THRESH_BINARY);

	// �̹��� ����. �� ���� => ���� ���� / ���� ���� => �� ����. ����� ����ȭ �̹���
	cv::bitwise_not(binary, binary);

	// ����ȭ �̹������� ���� 1�� �ȼ��� ��ġ�� true�� ��ȯ
	std::vector<cv::Point> coords;
	cv::findNonZero(binary, coords); // ���̽� �ڵ�: coords = np.column_stack(np.where(binary > 0))

	if (coords.empty()) return image;

	// ������ ���� ���� ���簢���� ���
	cv::RotatedRect rect = cv::minAreaRect(coords);

	float angle = rect.angle;
	if (angle < 45) {
		angle += 90; // ���� ������ �״�� ����ϵ�, ��ȣ�� �ݴ�� �Ͽ� ���� ������ ����
	}
	else {
		angle -= 90; // 90������ ���� ������ ���� ���� ���� ������ ���
	}

	int h = image.rows; // ���̽� �ڵ�: (h,w) = image.shape[:2]
	int w = image.cols;

	cv::Point center(w / 2, h / 2); // ���̽� �ڵ�: center = (w//2, h//2)

	// ȸ�� ��ȯ ��� ���
	cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);  // 2x3 ȸ�� ��ȯ ��� ���� (�׻� 2x3 ��� ��ȯ). ���� ��ȯ ���

	// ���ο� ȸ���� �̹��� ũ�� ���
	// cv::RotatedRect(center, image.size(), angle) -> �߽�(center)�� �������� �̹��� ��ü(image.size())�� angle ��ŭ ȸ���� �簢���� ����
	// .boundingRect() -> ȸ���� �簢���� �����ϴ� �ּ� ũ���� �簢���� ���
	cv::Rect bbox = cv::RotatedRect(center, image.size(), angle).boundingRect();
	// M.at<double>(0, 2) -> X ���� �̵� ��ȯ ��
	// bbox.width / 2 - center.x -> X ���� �߽��� ����
	M.at<double>(0, 2) += bbox.width / 2 - center.x;
	// M.at<double>(1, 2) -> Y ���� �̵� ��ȯ ��
	// bbox.height / 2 - center.y -> Y ���� �߽��� ����
	M.at<double>(1, 2) += bbox.height / 2 - center.y;

	// �̹��� ȸ��
	cv::Mat rotated;
	cv::warpAffine(image, rotated, M, bbox.size(), cv::INTER_CUBIC, cv::BORDER_REPLICATE);

	return rotated;
}

int main11_() {
	cv::Mat image = cv::imread("document.png");

	cv::Mat aligned_image = RotateDocument(image);

	cv::imshow("aligner", aligned_image);

	cv::waitKey(0);

	return 0;
}