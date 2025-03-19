#include <opencv2/opencv.hpp>

void DetectLines(cv::Mat& image) {
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// �ɴ� ���� ���� - ���� ����
	// -- �ּ��Ӱ谪: �� ������ ���� �׶���Ʈ ���� ������ ���ֵ��� ����
	// -- �ִ��Ӱ谪: �� ������ ���� �׶���Ʈ ���� Ȯ���� ������ ����
	cv::Mat edges;
	cv::Canny(gray, edges, 50, 200);

	// ���� ��ȯ
	// -- �̹������� ����� ã�� �˰���
	// -- ���� ����� ���� ���� �˰���
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(edges, lines, 1, CV_PI/180, 100); // ���̽� �ڵ�: cv2.HoughLines(edges, 1, np.pi/180, self.hough_thresh_scale.get())

	if (!lines.empty()) {
		for (size_t i = 0; i < lines.size(); i++) {
			float rho = lines[i][0]; // ������ �������� ������ �Ÿ�
			float theta = lines[i][1]; // ������ x��� �̷�� ����

			// ���� ��ȯ���� ������ ����ǥ ���������� ǥ���� ==> p = x*cos(��) + y*sin(��)
			// ����ǥ ���� (rho, theta)�� ������ �� ��(x1,y1) (x2,y2)�� ��ȯ
			float a = std::cos(theta);
			float b = std::sin(theta);

			// ���� ���� �� �� ���
			float x0 = a * rho;
			float y0 = b * rho;

			// 1000�� ���Ƿ� ū ������ �����Ͽ� ������ ����� ������ ȭ�� �۱��� �׸� �� �ֵ���
			int x1 = int(x0 + 1000 * (-b));
			int y1 = int(y0 + 1000 * (a));
			int x2 = int(x0 - 1000 * (-b));
			int y2 = int(y0 - 1000 * (a));

			cv::line(image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 2);
		}
	}

	cv::imshow("Detected Lines", image);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

int main() {
	cv::Mat image = cv::imread("line.jpg");

	DetectLines(image);

	return 0;
}