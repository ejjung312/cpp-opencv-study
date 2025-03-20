#include <opencv2/opencv.hpp>

int main() {
	cv::Mat frame = cv::imread("coin1.jpg");

	cv::Mat gray;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

	cv::Mat blurred;
	cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

	// �Ӱ谪���� ũ�� ���, �ƴϸ� ������
	cv::Mat binary_image;
	cv::threshold(blurred, binary_image, 50, 255, cv::THRESH_BINARY);

	// ħ��
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
	cv::Mat eroded_image;
	cv::erode(binary_image, eroded_image, kernel, cv::Point(-1, -1), 2);

	// ��
	cv::GaussianBlur(eroded_image, eroded_image, cv::Size(9, 9), 0);

	// �Ӱ�ġ
	cv::threshold(eroded_image, eroded_image, 50, 255, cv::THRESH_BINARY);

	cv::Mat labels;
	int numLabels = cv::connectedComponents(eroded_image, labels);

	std::vector<int> coin_labels;
	// ������ ������ 10���� ū �� ��ȯ
	for (int i = 1; i < numLabels; i++) {
		// Ư�� �� i�� �ش��ϴ� ����ũ ����
		cv::Mat mask = (labels == i);
		mask.convertTo(mask, CV_8U, 255);

		// ������ ã��
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		for (const auto& contour : contours) {
			if (cv::contourArea(contour) > 10) {
				coin_labels.push_back(i);
				break; // �� ���� ������ 10 �̻��̸� �߰��ϰ� ���� �󺧷� �̵�
			}
		}
	}

	cv::Mat output_image;
	frame.copyTo(output_image);

	/*
	output_image = image.copy()
    for label in coin_labels:
        coords = np.column_stack(np.where(labels == label))
        for coord in coords:
            cv2.circle(output_image, tuple(coord[::-1]), 5, (0,255,0), -1)
	*/
	for (int label : coin_labels) {
		std::vector<cv::Point> coords;

		// Ư�� ���� ���� ��ǥ ã��
		for (int y = 0; y < labels.rows; y++) {
			for (int x = 0; x < labels.cols; x++) {
				if (labels.at<int>(y, x) == label) {
					coords.emplace_back(x, y); // (x, y) ������� ����
				}
			}
		}

		for (const auto& coord : coords) {
			cv::circle(output_image, coord, 5, cv::Scalar(0, 255, 0), -1);
		}
	}

	cv::imshow("Coin Counter", output_image);
	
	cv::waitKey(0);

	return 0;
}