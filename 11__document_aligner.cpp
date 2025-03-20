#include <opencv2/opencv.hpp>

cv::Mat RotateDocument(cv::Mat& image) {
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// 흰 영역(240) 이상은 255, 나머지는 검정(0)
	cv::Mat binary;
	cv::threshold(gray, binary, 240, 255, cv::THRESH_BINARY);

	// 이미지 반전. 흰 영역 => 검정 영역 / 검정 영역 => 흰 영역. 결과는 이진화 이미지
	cv::bitwise_not(binary, binary);

	// 이진화 이미지에서 값이 1인 픽셀의 위치를 true로 변환
	std::vector<cv::Point> coords;
	cv::findNonZero(binary, coords); // 파이썬 코드: coords = np.column_stack(np.where(binary > 0))

	if (coords.empty()) return image;

	// 면적이 가장 작은 직사각형을 계산
	cv::RotatedRect rect = cv::minAreaRect(coords);

	float angle = rect.angle;
	if (angle < 45) {
		angle += 90; // 현재 각도를 그대로 사용하되, 부호를 반대로 하여 수평 정렬을 수행
	}
	else {
		angle -= 90; // 90도에서 현재 각도를 빼서 수평 정렬 각도를 계산
	}

	int h = image.rows; // 파이썬 코드: (h,w) = image.shape[:2]
	int w = image.cols;

	cv::Point center(w / 2, h / 2); // 파이썬 코드: center = (w//2, h//2)

	// 회전 변환 행렬 계산
	cv::Mat M = cv::getRotationMatrix2D(center, angle, 1.0);  // 2x3 회전 변환 행렬 생성 (항상 2x3 행렬 반환). 아핀 변환 행렬

	// 새로운 회전된 이미지 크기 계산
	// cv::RotatedRect(center, image.size(), angle) -> 중심(center)을 기준으로 이미지 전체(image.size())를 angle 만큼 회전한 사각형을 정의
	// .boundingRect() -> 회전된 사각형을 포함하는 최소 크기의 사각형을 계산
	cv::Rect bbox = cv::RotatedRect(center, image.size(), angle).boundingRect();
	// M.at<double>(0, 2) -> X 방향 이동 변환 값
	// bbox.width / 2 - center.x -> X 방향 중심을 보정
	M.at<double>(0, 2) += bbox.width / 2 - center.x;
	// M.at<double>(1, 2) -> Y 방향 이동 변환 값
	// bbox.height / 2 - center.y -> Y 방향 중심을 보정
	M.at<double>(1, 2) += bbox.height / 2 - center.y;

	// 이미지 회전
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