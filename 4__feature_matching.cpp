#include <opencv2/opencv.hpp>

int main4_() {
	/*
	* 이미지 특징점
	* - 이미지에서 고유한 패턴이나 구조를 가지며 다른 부분과 구별가능한 특징점인 지점(feature points)을 찾아냄
	* - 이미지에서 유니크하고 다른 부분과 구별되는 점을 찾아냄
	*/
	cv::Mat img1, img2;
	img1 = cv::imread("coin1.jpg");
	img2 = cv::imread("coin2.jpg");

	// 특징점 검출 객체
	cv::Ptr<cv::ORB> orb = cv::ORB::create();

	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	// 특징점 검출하고 기술자 계산
	orb.get()->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
	orb.get()->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);

	// 기술자 간 유사도 계산 객체
	//cv::BFMatcher bf = cv::BFMatcher(cv::NORM_HAMMING, true); // 객체를 직접 생성하면 메미로 관리가 복잡해 짐
	cv::Ptr<cv::BFMatcher> bf = cv::BFMatcher::create(cv::NORM_HAMMING, true); // 스마트 포인터 사용해서 BFMatcher 생성. 자동으로 메모리 관리
	// 기술자 배열간 매칭
	std::vector<cv::DMatch> matches;
	//bf.match(descriptors1, descriptors2, matches);
	bf->match(descriptors1, descriptors2, matches);
	
	// 파이썬의 matches = sorted(matches, key=lambda x:x.distance) 변환
	std::sort(matches.begin(), matches.end(), [](const cv::DMatch& a, const cv::DMatch& b) {
		return a.distance < b.distance;
	});

	// 최상위 50개 매칭 결과만 사용 - matches[:50]에 해당
	// cv2.drawMatches(img1, keypoints1, img2, keypoints2, matches[:50], None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
	std::vector<cv::DMatch> good_matches(matches.begin(), matches.begin() + std::min(50, static_cast<int>(matches.size())));

	cv::Mat img_matches;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches, 
		cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	cv::imshow("Feature Matching", img_matches);
	cv::waitKey(0);

	return 0;
}