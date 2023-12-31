#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include "matchMethods.hpp"
using namespace std;

int getCenter(cv::Mat const target, cv::Mat &targetVector) {
  int m = target.rows;
  int n = target.cols;

  if (m < 9 || n < 9) {
    cout << "error: out of bound" << endl;
    return (-1);
  }

  // get the center of the target image
  int centerR = m / 2;
  int centerCol = n / 2;
  // get the center 9x9 square of the target image
  targetVector = target(cv::Rect(centerCol - 4, centerR - 4, 9, 9));

  return (0);
}

float sumDiff(cv::Mat const v1, cv::Mat const v2) {
  if (v1.rows != v2.rows || v1.cols != v2.cols) {
    printf("error: size of feature vectors dont match\n");
    return (-1);
  }

  // calculate the sum-of-squared-differences for each pixel at three channels
  float ssd = 0.0;
  for (int i = 0; i < v1.rows; i++) {
    // get the ith row of the feature vectors
    const cv::Vec3b *r1 = v1.ptr<cv::Vec3b>(i);
    const cv::Vec3b *r2 = v2.ptr<cv::Vec3b>(i);

    for (int j = 0; j < v1.cols; j++) {
      for (int k = 0; k < 3; k++) {
        float diff = r1[j][k] - r2[j][k];
        ssd += diff * diff;
      }
    }
  }

  return (ssd);
}

int baselineMatching(cv::Mat const target, std::vector<std::pair<std::string, cv::Mat>> const imageDatabase, std::vector<std::pair<std::string, float>> &distanceArr) {
  // get the center 9x9 square of the target image
  cv::Mat targetVector;
  getCenter(target, targetVector);

  // loops over the directory of images
  for (int i = 0; i < imageDatabase.size(); i++) {
    // get the center 9x9 square of the image
    cv::Mat imageFeatureVector;
    getCenter(imageDatabase[i].second, imageFeatureVector);

    // calculate the sum-of-squared-differences between the two feature vectors
    float ssd = sumDiff(targetVector, imageFeatureVector);

    // store the filename along with the ssd value
    distanceArr.push_back(std::make_pair(imageDatabase[i].first, ssd));
  }

  return (0);
}