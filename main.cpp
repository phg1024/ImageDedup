#include <iostream>

#include "common.h"

#include "ioutils.h"
#include "imagededuper.h"

using namespace std;
using namespace imagededuper;

int main(int argc, char** argv) {
  namespace fs = boost::filesystem;
  namespace po = boost::program_options;

  po::options_description desc("Options");
  desc.add_options()
    ("settings_file", po::value<string>()->required(), "Input settings file")
    ("output", po::value<string>()->required(), "Output settings file");

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if(vm.count("help")) {
      cout << desc << endl;
      return 1;
    }

    // nothing to do after successful parsing command line arguments

  } catch(po::error& e) {
    cerr << "Error: " << e.what() << endl;
    cerr << desc << endl;
    return 1;
  }

  const string settings_filename = vm["settings_file"].as<string>();
  fs::path settings_filepath(settings_filename);

  vector<pair<string, string>> image_points_filenames = ParseSettingsFile(settings_filename);
  vector<string> imagefiles;
  for(auto& p : image_points_filenames) {
    fs::path image_filename = settings_filepath.parent_path() / fs::path(p.first);
    fs::path pts_filename = settings_filepath.parent_path() / fs::path(p.second);
    cout << "[" << image_filename << ", " << pts_filename << "]" << endl;
    imagefiles.push_back(image_filename.string());
  }

  ImageDeduper deduper(imagefiles);
  auto res = deduper.Dedup();

  ofstream fout(vm["output"].as<string>());

  cout << "Result:" << endl;
  for(auto x : res) {
    //cout << x << endl;
    string img_i = fs::path(x).filename().string();
    string pts_i = fs::path(x).stem().string() + ".pts";
    cout << img_i << " " << pts_i << endl;
    fout << img_i << " " << pts_i << endl;
  }

  return 0;
}
