#include <opencv2/opencv.hpp>

void ApplyAugmentation(cv::Mat& frame) {
	// 행렬 변환을 사용하여 이미지를 세피아 톤으로 변환
	cv::Mat sepiaKernel = (cv::Mat_<float>(3,3) <<
		0.272, 0.534, 0.131,
		0.349, 0.686, 0.168,
		0.393, 0.769, 0.189);

	cv::transform(frame, frame, sepiaKernel);

	// 변환 후의 픽셀 값이 0~255 범위를 벗어나지 않도록 제한
	// cv::max(frame, 0): 픽셀 값이 0보다 작으면 0으로 설정
	// cv::min(..., 255): 픽셀 값이 255보다 크면 255로 설정.
	cv::min(cv::max(frame, 0), 255); 
	frame.convertTo(frame, CV_8U); // cv::Mat이 float 타입일 경우 8-bit 이미지로 변환.
}

void AdjustBrightness(cv::Mat& frame) {
	// 이미지의 픽셀 값을 조정하고 스케일링한 후, 절대값을 적용하여 8비트 이미지로 변환하는 함수
	// 이미지 처리 중에 연산을 하다보면 픽셀 값이 0보다 작거나 255보다 커질 수 있다. 이럴 때 모든 픽셀 값을 안전하게 0~255 사이로 변환
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