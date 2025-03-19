#include <opencv2/opencv.hpp>

cv::Mat StitchImage(std::vector<cv::Mat>& images) {
	// �̹��� ��ƼĪ
	// - ���� ����� ������ �ڿ������� �ٿ��� �� ���� �������� ����� ���
	// - ���� �̾� ���̱�, �ĳ�� ����
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