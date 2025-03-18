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

	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8)); // cv::Ptr<cv::CLAHE> - OpenCV의 스마트 포인터로, 자동으로 메모리를 관리
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)); // 사각형 커널 생성
	while (true) {
		cap.read(frame);

		if (frame.empty()) {
			break;
		}

		// 엣지 검출
		// -- 그레이스케일 변환. 엣지 검출 전 색상 제거
		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		// 히스토그램 평활화
		// -- 이미지 밝기 대비 개선
		clahe.get()->apply(gray, gray);

		// 블러 처리
		// -- 가우시안 블러 - 전체적인 부드러운 노이즈 제거 및 부드럽게 처리
		// -- 메디안 블러 - 엣지를 더 보존하면서 노이즈 제거
		cv::Mat blurred;
		cv::medianBlur(gray, blurred, 5);

		// 엣지 검출
		// -- 케니 엣지 검출
		//    - 최소임계값: 이 값보다 낮은 그라디언트 값은 엣지로 간주되지 않음
		//	  - 최대임계값: 이 값보다 높은 그라디언트 값은 확실한 엣지로 간주
		//cv::Mat edges;
		//cv::Canny(blurred, edges, 100, 200);

		// 자동 임계값 계산
		double v = getMedian(blurred); // 파이썬의 v = np.median(blurred) 변환 함수
		int lower = static_cast<int>(std::max(0.0, 0.66 * v));
		int upper = static_cast<int>(std::min(255.0, 1.33 * v));
		cv::Mat edges;
		cv::Canny(blurred, edges, lower, upper);

		// 엣지 연결
		// -- 모폴로지 연산을 사용하여 연결
		cv::Mat dilated;
		cv::dilate(edges, dilated, kernel); // 팽창 - 커널을 적용하여 하나라도 1이 있으면 중심 픽셀을 1로 만듬
		cv::erode(dilated, edges, kernel); // 침식 - 커널을 적용하여 하나라도 0이 있으면 중심 픽셀 제거

		// 필터(가우시안, 양방향 등)
		// -- 색상 단순화 - 색상 평탄화. 잡음 제거
		cv::Mat color;
		cv::bilateralFilter(frame, color, 9, 75, 75);

		// 엣지와 색상 결합
		cv::bitwise_not(edges, edges);
		cv::Mat edges_colored;
		cv::cvtColor(edges, edges_colored, cv::COLOR_GRAY2BGR);

		// 색상과 윤곽선 결합
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