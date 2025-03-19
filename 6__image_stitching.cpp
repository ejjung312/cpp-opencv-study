#include <opencv2/opencv.hpp>

cv::Mat StitchImage(std::vector<cv::Mat>& images) {
	// 이미지 스티칭
	// - 동일 장면의 사진을 자연스럽게 붙여서 한 장의 사진으로 만드는 기술
	// - 사진 이어 붙이기, 파노라마 영상
	cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create();
	cv::Mat pano;
	cv::Stitcher::Status status = stitcher->stitch(images, pano);

	if (status != cv::Stitcher::OK) {
		std::cout << "Image stitching failed." << std::endl;
		return cv::Mat();
	}

	return pano;
}

void DisplayImage(cv::Mat& pano) {
	cv::cvtColor(pano, pano, cv::COLOR_BGR2RGB);

	cv::imshow("pano", pano);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

int main() {
	cv::Mat img1 = cv::imread("pano1.jpg");
	cv::Mat img2 = cv::imread("pano2.jpg");

	std::vector<cv::Mat> images;
	images.push_back(img1);
	images.push_back(img2);
	
	cv::Mat pano = StitchImage(images);
	DisplayImage(pano);


	return 0;
}