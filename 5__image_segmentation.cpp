#include <iostream>
#include <opencv2/opencv.hpp>

void SegmentImage(cv::Mat& image) {
	cv::Mat gray;
	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

	// 이미지 이진화
	// - 임계값을 설정해 임계값보다 크면 흰색, 아니면 검정색
	cv::Mat thresh;
	cv::threshold(gray, thresh, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)); // 파이썬 코드: kernel = np.ones((3,3), np.uint8)

	// 모폴로지
	// - 이미지의 모양에 대한 정보 집중
	// - 열기(opening): 침식(erosion) 후 팽창. 노이즈를 제거하는 용오
	// - 닫기(closing): 팽창(dilate) 후 침식
	cv::Mat opening;
	cv::morphologyEx(thresh, opening, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);

	// 팽창 - 객체 외곽을 확대시키는 연산
	cv::Mat surebg;
	cv::dilate(opening, surebg, kernel, cv::Point(-1, -1), 3);

	// 거리 변환(distance transform)
	// - 픽셀값이 0인 배경으로부터 거리를 픽셀값이 255인 영역에 표현하는 방법
	// - 물체 영역의 뼈대를 찾아 영역을 정확히 파악
	cv::Mat dist_transform;
	cv::distanceTransform(opening, dist_transform, cv::DIST_L2, 5); // 출력이 32비트 부동소수점(CV_32F)이므로

	// 0.7 * dist_transform.max() 보다 크면 흰색, 아니면 검정색으로 변경
	cv::Mat surefg;
	double maxVal = 0;
	// 파이썬의 dist_transform.max() 코드
	// 최대값만 필요하므로 최소값은 nullptr 설정
	cv::minMaxLoc(dist_transform, nullptr, &maxVal);
	cv::threshold(dist_transform, surefg, 0.7 * maxVal, 255, 0);

	// surefg는 CV_32F이므로 이진화 연산 시 오류가 발생할 수 있음 => CV_8U(8비트 unsigned int)
	surefg.convertTo(surefg, CV_8U);

	// 배경과 전경을 빼서 경계선 영역 정의
	cv::Mat unknown;
	cv::subtract(surebg, surefg, unknown);

	// 레이블링
	// - 서로 연결되어 있는 객체 픽셀에 고유번호를 할당하여 영역기반 모양분석, 레이블맵, 바운딩박스, 픽셀 개수, 무게 중심 좌표 등을 반환할 수 있게 함
	// 레이블맵
	// - 영상의 레이블링 수행하면 각각의 객체 영역에 고유 번호가 매겨진 2차원 정수 행렬
	cv::Mat markers;
	int numLabels = cv::connectedComponents(surefg, markers); // 레이블맵 생성

	// Watershed 알고리즘은 0을 경계영역으로 사용하므로, 레이블 값을 1씩 증가시켜 기존 레이블을 1이상의 값으로 만듬
	markers = markers + 1;
	// 전경과 배경이 확실하지 않은 경계영역(unknown영역)을 0으로 설정
	markers.setTo(0, unknown == 255); // 파이썬 코드 markers[unknown == 255] = 0; 변환

	cv::watershed(image, markers);

	// Watershed가 경계선으로 구분한 부분을 시각적으로 표시
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