#include "goldenRatio.h"
#include <dlib/opencv.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace dlib;
using namespace std;
using namespace cv;

cv::Rect dlib2CvRect(dlib::rectangle &r) {
  return cv::Rect(
      cv::Point2i(r.left(),r.top()),
      cv::Point2i(r.right() + 1,r.bottom() + 1)
      );
}

dlib::rectangle cv2DlibRect(cv::Rect &r) {
  return dlib::rectangle(
      (long)r.tl().x,
      (long)r.tl().y,
      (long)r.br().x - 1,
      (long)r.br().y - 1
      );
}

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

float faceWidth(dlib::rectangle &faceRect, full_object_detection &landmarks) 
{
  int leftJawPoint = 2;
  int rightJawPoint = 16;
  cout << "left " << landmarks.part(leftJawPoint).x() << ", " << landmarks.part(leftJawPoint).y() << endl;
  cout << "right " << landmarks.part(rightJawPoint).x() << ", " << landmarks.part(rightJawPoint).y() << endl;
  return sqrt(pow(landmarks.part(rightJawPoint).y() - landmarks.part(leftJawPoint).y(), 2.0) +
    pow(landmarks.part(rightJawPoint).x() - landmarks.part(leftJawPoint).x(), 2.0));
}

float faceHeight(dlib::rectangle &faceRect, full_object_detection &landmarks) 
{
  int bottomJawPoint = 9;
  cout << "top " << faceRect.top() << endl;
  cout << "bottom " << landmarks.part(bottomJawPoint).x() << ", " << landmarks.part(bottomJawPoint).y() << endl;
  return landmarks.part(bottomJawPoint).y() - faceRect.top();
}

float heightWidthRatio(dlib::rectangle &faceRect, full_object_detection &landmarks)
{
  float height =faceHeight(faceRect, landmarks);
  float width = faceWidth(faceRect, landmarks);

  // TODO
  // Debug Remove later
  cout << "width :" << width << endl;
  cout << "height :" << height  << endl;

  float ratio = height / width;

  float distance = abs(ratio - 1.6);
  cout << "distance :" << distance  << endl;

  float answer = 1.0 - (distance / 1.6);

  cout << "answer :" << answer  << endl;

  return answer;
}

float faceSegmentRatio(dlib::rectangle &faceRect, full_object_detection &landmarks)
{
  int right_eye_point =  46;
  int nose_point =  34;
  int bottomJawPoint = 9;
  int top = landmarks.part(right_eye_point).y() - faceRect.top();
  int mid = landmarks.part(nose_point).y() - landmarks.part(right_eye_point).y();
  int bottom = landmarks.part(bottomJawPoint).y() - landmarks.part(nose_point).y();
  float ratio = (top + mid + bottom) / 3;
  float topRatio = top / ratio;
  float midRatio = mid / ratio;
  float botRatio = bottom / ratio;
  float faceRatio = (topRatio + midRatio + botRatio) / 3;
  return faceRatio;
}

float calGoldenRatio(dlib::rectangle &faceRect, full_object_detection &landmarks)
{
  float _goldenRatio =  (heightWidthRatio(faceRect, landmarks) + faceSegmentRatio(faceRect, landmarks)) / 2;
  return _goldenRatio;
}

void calculate(char* filename)
{
  // The landmark detector is implemented in the shape_predictor class
  shape_predictor landmarkDetector;

  // Load the landmark model
  deserialize("../config/shape_predictor_68_face_landmarks.dat") >> landmarkDetector;

  // Read Image
  string imagePath("../fixture/images/");
  cv::Mat im = cv::imread(imagePath + filename);
  cv::Mat gray;
  
  if (im.channels() > 1) {
    cvtColor(im, gray, COLOR_BGR2GRAY);
  } else {
    gray = im.clone();
  }

  equalizeHist(gray, gray);

  std::vector<cv::Rect> faces;
  faces.clear();

  cv::CascadeClassifier face_cascade;
  face_cascade.load("../config/haarcascade.xml");

  // Taking the biggest face if possible
  face_cascade.detectMultiScale(
      gray,
      faces,
      1.4,
      3,
      CASCADE_SCALE_IMAGE + CASCADE_FIND_BIGGEST_OBJECT
      );
  
  // Convert OpenCV image format to Dlib's image format
  cv_image<bgr_pixel> dlibIm(im);

  if(faces.size() == 0)
  {
    cout << "Cannot detect any faces." << endl;
    return;
  }

  cout << "Number of cv faces detected: " << faces.size() << endl;

  // TODO
  // Taking the first face
  cv::Rect face = faces[0];
  cout << "height 1: " << face.height << endl;
  cout << "width 1: " << face.width << endl;
  cv::rectangle(im, face, Scalar(0,255,0));
  dlib::rectangle dlibFace = cv2DlibRect(face);
  cout << "height 2: " << dlibFace.height() << endl;
  full_object_detection landmarks = landmarkDetector(dlibIm, dlibFace);
  float goldenRatio = calGoldenRatio(dlibFace, landmarks);
  cout << "Golden Ratio : " << goldenRatio << endl;
  drawLandmarksAndRatio(im, landmarks, dlibFace, goldenRatio);

  string outputFilename("../result/");
  cout << "Saving output image to " << (outputFilename + filename) << endl;
  cv::imwrite(outputFilename + filename, im);
}
