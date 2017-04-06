#pragma once

#include "common.h"

namespace aam {
  std::vector<std::string> ReadFileByLine(const std::string &filename);
  std::vector<std::pair<std::string, std::string>> ParseSettingsFile(const std::string& settings_filename);
  std::pair<QImage,  cv::Mat> LoadImagePointsPair(const std::string& image_filename, const std::string& points_filename);
  std::vector<cv::Vec3i> LoadTriangulation(const std::string& filename);
}
