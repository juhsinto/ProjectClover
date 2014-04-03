#if !defined SOBELEDERR
#define SOBELEDERR

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class SobelDerrivative {

	private:
		int depth;
		int xDerrivative;
		int yDerrivative;
		cv::Mat sobel_horizontal;
		cv::Mat sobel_vertical;

  public:
		SobelDerrivative() : depth(CV_8UC1) , xDerrivative(0), yDerrivative(0) {}

	void setDepth(int input_depth) {
		depth = input_depth;
	}

	void setDerrivativeX(int derr) {
		xDerrivative = derr;
	}

	void setDerrivativeY(int derr) {
		yDerrivative = derr;
	}

	cv::Mat getSobelDerrivativeX(cv::Mat& input_image) {
		Sobel( input_image, sobel_horizontal, depth, xDerrivative, yDerrivative);

		return sobel_horizontal;
	}

	cv::Mat getSobelDerrivativeY(cv::Mat& input_image) {
		Sobel( input_image, sobel_vertical, depth, xDerrivative, yDerrivative);

		return sobel_vertical;
	}
};

#endif
