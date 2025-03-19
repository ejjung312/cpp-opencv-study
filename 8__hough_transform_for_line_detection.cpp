#include <opencv2/opencv.hpp>

void DetectLines(cv::Mat& image) {
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// 케니 엣지 검출 - 엣지 검출
	// -- 최소임계값: 이 값보다 낮은 그라디언트 값은 엣지로 간주되지 않음
	// -- 최대임계값: 이 값보다 높은 그라디언트 값은 확실한 엣지로 간주
	cv::Mat edges;
	cv::Canny(gray, edges, 50, 200);

	// 허프 변환
	// -- 이미지에서 모양을 찾는 알고리즘
	// -- 엣지 기반의 직선 검출 알고리즘
	std::vector<cv::Vec2f> lines;
	cv::HoughLines(edges, lines, 1, CV_PI/180, 100); // 파이썬 코드: cv2.HoughLines(edges, 1, np.pi/180, self.hough_thresh_scale.get())

	if (!lines.empty()) {
		for (size_t i = 0; i < lines.size(); i++) {
			float rho = lines[i][0]; // 직선이 원점에서 떨어진 거리
			float theta = lines[i][1]; // 직선이 x축과 이루는 각도

			// 허프 변환에서 직선은 극좌표 방정식으로 표현됨 ==> p = x*cos(θ) + y*sin(θ)
			// 극좌표 형태 (rho, theta)를 직선의 두 점(x1,y1) (x2,y2)로 변환
			float a = std::cos(theta);
			float b = std::sin(theta);

			// 직선 위의 한 점 계산
			float x0 = a * rho;
			float y0 = b * rho;

			// 1000은 임의로 큰 값으로 설정하여 직선을 충분히 연장해 화면 밖까지 그릴 수 있도록
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