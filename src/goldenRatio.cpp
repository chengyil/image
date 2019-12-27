#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/opencv.hpp>

using namespace dlib;
using namespace std;
using namespace cv;

void drawLandmarksAndRatio(Mat &im, full_object_detection landmarks, dlib::rectangle &faceRect, float goldenRatio)
{
  char ratio_text[20];
  sprintf(ratio_text, "Golden Ratio : %.1f", goldenRatio);
  putText(im, ratio_text, Point(faceRect.left(), faceRect.top()), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 1);

  for(int i = 0; i < landmarks.num_parts(); i++)
  {
    int px = landmarks.part(i).x();
    int py = landmarks.part(i).y();

    char landmark_index[3];
    sprintf(landmark_index, "%d", i+1);

    circle(im, Point(px, py), 1, Scalar(0, 0, 255), 2, LINE_AA);
    putText(im, landmark_index, Point(px, py), FONT_HERSHEY_SIMPLEX, .3, Scalar(255, 0, 0), 1);
  }
}

float heightWidthRatio(dlib::rectangle &faceRect)
{
  float ratio = faceRect.height() / faceRect.width();
  float distance = 0.0;

  if(ratio < 1.6)
  {
    distance = 1.6 - ratio;
  }
  else
  {
    distance =  ratio - 1.6;
  }

  if(distance > 1 )
  {
    return 0.1;
  }
  else
  {
    return 1.0 - distance;
  }
}

float faceSegmentRatio(dlib::rectangle &faceRect, full_object_detection &landmarks)
{
  int right_eye_point =  46;
  int nose_point =  34;
  int top = landmarks.part(right_eye_point).y() - faceRect.top();
  int mid = landmarks.part(nose_point).y() - landmarks.part(right_eye_point).y();
  int bottom = faceRect.bottom() - landmarks.part(nose_point).y();
  float ratio = (top + mid + bottom) / 3;
  float topRatio = top / ratio;
  float midRatio = mid / ratio;
  float botRatio = bottom / ratio;
  float faceRatio = (topRatio + midRatio + botRatio) / 3;
  return faceRatio;
}

float calGoldenRatio(dlib::rectangle &faceRect, full_object_detection &landmarks)
{
  float _goldenRatio =  (heightWidthRatio(faceRect) + faceSegmentRatio(faceRect, landmarks)) / 2;
  return _goldenRatio;
}

int main(int argc, char** argv)
{
  // Get the face detector
  frontal_face_detector faceDetector = get_frontal_face_detector();

  // The landmark detector is implemented in the shape_predictor class
  shape_predictor landmarkDetector;

  // Load the landmark model
  deserialize("../config/shape_predictor_68_face_landmarks.dat") >> landmarkDetector;

  // Read Image
  string imageFilename("../fixture/images/test.jpg");
  cv::Mat im = cv::imread(imageFilename);

  // Convert OpenCV image format to Dlib's image format
  cv_image<bgr_pixel> dlibIm(im);

  // Detect faces in the image
  std::vector<dlib::rectangle> faceRects = faceDetector(dlibIm);
  cout << "Number of faces detected: " << faceRects.size() << endl;

  // Loop over all detected face rectangles
  for (int i = 0; i < faceRects.size(); i++)
  {
    // For every face rectangle, run landmarkDetector
    full_object_detection landmarks = landmarkDetector(dlibIm, faceRects[i]);
    float goldenRatio = calGoldenRatio(faceRects[i], landmarks);
    drawLandmarksAndRatio(im, landmarks, faceRects[i], goldenRatio);
  }

  // Save image
  string outputFilename("../result/result.jpg");
  cout << "Saving output image to " << outputFilename << endl;
  cv::imwrite(outputFilename, im);

  return EXIT_SUCCESS;
}
