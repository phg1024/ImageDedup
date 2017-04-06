#include "ioutils.h"

using namespace std;

namespace imagededuper {
  vector<string> ReadFileByLine(const string &filename) {
    ifstream fin(filename);
    vector<string> lines;
    while (fin) {
      string line;
      std::getline(fin, line);
      if (!line.empty())
        lines.push_back(line);
    }
    return lines;
  }

  vector<pair<string, string>> ParseSettingsFile(const string& filename) {
    vector<string> lines = ReadFileByLine(filename);

    vector<pair<string, string>> image_points_filenames(lines.size());
    std::transform(lines.begin(), lines.end(), image_points_filenames.begin(),
                   [](const string &line) {
                     vector<string> parts;
                     boost::algorithm::split(parts, line,
                                             boost::algorithm::is_any_of(" "),
                                             boost::algorithm::token_compress_on);
                     auto parts_end = std::remove_if(parts.begin(), parts.end(),
                                                     [](const string &s) {
                                                       return s.empty();
                                                     });
                     assert(std::distance(parts.begin(), parts_end) == 2);
                     return make_pair(parts.front(), parts.back());
                   });
    return image_points_filenames;
  }

  cv::Mat ReadPoints(const string& filename) {
    ifstream fin(filename);
    int npoints;
    fin >> npoints;
    cout << "reading " << npoints << " points ..." << endl;
    cv::Mat pts(npoints, 2, CV_64FC1);
    for(int i=0;i<npoints;++i) {
      double x, y;
      fin >> x >> y;
      pts.at<double>(i, 0) = x;
      pts.at<double>(i, 1) = y;
    }

    return pts;
  }

  pair<QImage, cv::Mat> LoadImagePointsPair(
    const string& image_filename,
    const string& points_filename
  ) {
    QImage img(image_filename.c_str());
    cout << "image size: " << img.width() << "x" << img.height() << endl;

    auto pts = ReadPoints(points_filename);
    cout << "number of points: " << pts.rows << endl;

    // Convert points to 0-based coordinates
    pts -= 1;

    return make_pair(img, pts);
  }

  vector<cv::Vec3i> LoadTriangulation(const string& filename) {
    vector<string> lines = ReadFileByLine(filename);

    vector<cv::Vec3i> triangles(lines.size());
    std::transform(lines.begin(), lines.end(), triangles.begin(),
                   [](const string &line) {
                     vector<string> parts;
                     boost::algorithm::split(parts, line,
                                             boost::algorithm::is_any_of(" \t"),
                                             boost::algorithm::token_compress_on);
                     auto parts_end = std::remove_if(parts.begin(), parts.end(),
                                                     [](const string &s) {
                                                       return s.empty();
                                                     });
                     assert(std::distance(parts.begin(), parts_end) == 3);
                     return cv::Vec3i(stoi(parts[0]), stoi(parts[1]), stoi(parts[2]));
                   });
    return triangles;
  }
}
