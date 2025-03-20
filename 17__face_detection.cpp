#include <opencv2/opencv.hpp>

int main() {
	cv::Mat frame;
	cv::VideoCapture cap("person4.mp4");
	if (!cap.isOpened()) {
		return -1;
	}

	cv::CascadeClassifier cascade;
	cascade.load("haarcascade_frontalface_default.xml");

	std::vector<cv::Rect> faces;
	double scale_factor = 1.1;
	int min_neighbors = 5;
	int min_size = 30;

	while (true) {
		cap.read(frame);

		cv::Mat gray;
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		cascade.detectMultiScale(gray, faces, scale_factor, min_neighbors, 0, cv::Size(min_size, min_size));

		for (const auto& face : faces) {
			cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
		}

		cv::imshow("face detection", frame);

		if (cv::waitKey(25) == 'q') {
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}