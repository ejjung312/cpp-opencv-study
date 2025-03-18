#include <opencv2/opencv.hpp>

double getMedian(cv::Mat& img) {
	std::vector<uchar> values;
	values.assign(img.datastart, img.dataend);

	std::nth_element(values.begin(), values.begin() + values.size() / 2, values.end());

	return static_cast<double>(values[values.size() / 2]);
}

int main() {
	cv::Mat frame;

	cv::VideoCapture cap;
	cap.open("person2.mp4");
	if (!cap.isOpened()) {
		std::cerr << "Video Open Error" << std::endl;
		return -1;
	}

	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8)); // cv::Ptr<cv::CLAHE> - OpenCV�� ����Ʈ �����ͷ�, �ڵ����� �޸𸮸� ����
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)); // �簢�� Ŀ�� ����
	while (true) {
		cap.read(frame);

		if (frame.empty()) {
			break;
		}

		// ���� ����
		// -- �׷��̽����� ��ȯ. ���� ���� �� ���� ����
		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// ������׷� ��Ȱȭ
		// -- �̹��� ��� ��� ����
		clahe.get()->apply(gray, gray);

		// �� ó��
		// -- ����þ� �� - ��ü���� �ε巯�� ������ ���� �� �ε巴�� ó��
		// -- �޵�� �� - ������ �� �����ϸ鼭 ������ ����
		cv::Mat blurred;
		cv::medianBlur(gray, blurred, 5);

		// ���� ����
		// -- �ɴ� ���� ����
		//    - �ּ��Ӱ谪: �� ������ ���� �׶���Ʈ ���� ������ ���ֵ��� ����
		//	  - �ִ��Ӱ谪: �� ������ ���� �׶���Ʈ ���� Ȯ���� ������ ����
		//cv::Mat edges;
		//cv::Canny(blurred, edges, 100, 200);

		// �ڵ� �Ӱ谪 ���
		double v = getMedian(blurred); // ���̽��� v = np.median(blurred) ��ȯ �Լ�
		int lower = static_cast<int>(std::max(0.0, 0.66 * v));
		int upper = static_cast<int>(std::min(255.0, 1.33 * v));
		cv::Mat edges;
		cv::Canny(blurred, edges, lower, upper);

		// ���� ����
		// -- �������� ������ ����Ͽ� ����
		cv::Mat dilated;
		cv::dilate(edges, dilated, kernel); // ��â - Ŀ���� �����Ͽ� �ϳ��� 1�� ������ �߽� �ȼ��� 1�� ����
		cv::erode(dilated, edges, kernel); // ħ�� - Ŀ���� �����Ͽ� �ϳ��� 0�� ������ �߽� �ȼ� ����

		// ����(����þ�, ����� ��)
		// -- ���� �ܼ�ȭ - ���� ��źȭ. ���� ����
		cv::Mat color;
		cv::bilateralFilter(frame, color, 9, 75, 75);

		// ������ ���� ����
		cv::bitwise_not(edges, edges);
		cv::Mat edges_colored;
		cv::cvtColor(edges, edges_colored, cv::COLOR_GRAY2BGR);

		// ����� ������ ����
		cv::Mat cartoon = cv::Mat::zeros(frame.size(), frame.type());
		cv::bitwise_and(color, edges_colored, cartoon);

		cv::imshow("cartoon", cartoon);

		if (cv::waitKey(25) == 'q') {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}