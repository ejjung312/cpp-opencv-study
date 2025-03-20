#include <opencv2/opencv.hpp>

int main14_() {
	cv::Mat frame;
	cv::VideoCapture cap("person2.mp4");
	if (!cap.isOpened()) {
		return -1;
	}

	// 배경 추출
	cv::Ptr<cv::BackgroundSubtractorMOG2> back_sub = cv::createBackgroundSubtractorMOG2();

	int total_frames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
	int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
	int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	double fps = cap.get(cv::CAP_PROP_FPS);
	cv::VideoWriter out("out.mp4", cv::VideoWriter::fourcc('m', 'p', '4', 'v'), fps, cv::Size(frame_width, frame_height));

	cv::Mat fg_mask, fg_mask_bgr;
	while (true) {
		cap.read(frame);
		if (frame.empty()) break;

		back_sub->apply(frame, fg_mask);

		// 흑백 -> BGR 변환 후 저장
		cv::cvtColor(fg_mask, fg_mask_bgr, cv::COLOR_GRAY2BGR);
		out.write(fg_mask_bgr);

		cv::imshow("Foreground Mask", fg_mask);

		if (cv::waitKey(25) == 'q') {
			break;
		}
	}

	cap.release();
	out.release();
	cv::destroyAllWindows();

	return 0;
}