#include <opencv2/opencv.hpp>

int main4_() {
	/*
	* �̹��� Ư¡��
	* - �̹������� ������ �����̳� ������ ������ �ٸ� �κа� ���������� Ư¡���� ����(feature points)�� ã�Ƴ�
	* - �̹������� ����ũ�ϰ� �ٸ� �κа� �����Ǵ� ���� ã�Ƴ�
	*/
	cv::Mat img1, img2;
	img1 = cv::imread("coin1.jpg");
	img2 = cv::imread("coin2.jpg");

	// Ư¡�� ���� ��ü
	cv::Ptr<cv::ORB> orb = cv::ORB::create();

	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	cv::Mat descriptors1, descriptors2;
	// Ư¡�� �����ϰ� ����� ���
	orb.get()->detectAndCompute(img1, cv::noArray(), keypoints1, descriptors1);
	orb.get()->detectAndCompute(img2, cv::noArray(), keypoints2, descriptors2);

	// ����� �� ���絵 ��� ��ü
	//cv::BFMatcher bf = cv::BFMatcher(cv::NORM_HAMMING, true); // ��ü�� ���� �����ϸ� �޹̷� ������ ������ ��
	cv::Ptr<cv::BFMatcher> bf = cv::BFMatcher::create(cv::NORM_HAMMING, true); // ����Ʈ ������ ����ؼ� BFMatcher ����. �ڵ����� �޸� ����
	// ����� �迭�� ��Ī
	std::vector<cv::DMatch> matches;
	//bf.match(descriptors1, descriptors2, matches);
	bf->match(descriptors1, descriptors2, matches);
	
	// ���̽��� matches = sorted(matches, key=lambda x:x.distance) ��ȯ
	std::sort(matches.begin(), matches.end(), [](const cv::DMatch& a, const cv::DMatch& b) {
		return a.distance < b.distance;
	});

	// �ֻ��� 50�� ��Ī ����� ��� - matches[:50]�� �ش�
	// cv2.drawMatches(img1, keypoints1, img2, keypoints2, matches[:50], None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
	std::vector<cv::DMatch> good_matches(matches.begin(), matches.begin() + std::min(50, static_cast<int>(matches.size())));

	cv::Mat img_matches;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches, 
		cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	cv::imshow("Feature Matching", img_matches);
	cv::waitKey(0);

	return 0;
}