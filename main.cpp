#include <iostream>

#include "pHash.h"

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/program_options.hpp"

int main(int argc, char** argv) {
  namespace fs = boost::filesystem;
  namespace po = boost::program_options;

  po::options_description desc("Options");
  desc.add_options()
    ("settings_file", po::value<string>()->required(), "Input settings file");

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

  

  return 0;
}