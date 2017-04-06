#include "common.h"
#include "imagededuper.h"

using namespace std;

ImageDeduper::ImageDeduper(const vector <string> &imagefiles) {
  files = imagefiles;
  GenerateHashes(imagefiles);
}

void ImageDeduper::GenerateHashes(const vector <string> &imagefiles) {
  for(auto s : imagefiles) {
    ulong64 h;
    ph_dct_imagehash(s.c_str(), h);
    hashes[s] = h;
    cout << s << ": " << h << endl;
  }
}

vector<vector<string>> ImageDeduper::Dedup() const {
  const int nimgs = files.size();
  vector<vector<int>> dists(nimgs, vector<int>(nimgs));

  const int T = 5;

  vector<pair<int, int>> pairs;

  for(int i=0;i<nimgs;++i) {
    dists[i][i] = 0;
    for(int j=i+1;j<nimgs;++j) {
      dists[i][j] = ph_hamming_distance(hashes.at(files[i]), hashes.at(files[j]));
      if(dists[i][j] < T) {
        cout << files[i] << " vs " << files[j] << ": " << dists[i][j] << endl;
        pairs.push_back(make_pair(i, j));
      }
    }
  }

  // merge the pairs into groups
  vector<vector<int>> groups = GenerateGroups(pairs);
  vector<vector<string>> res;
  for(int i=0;i<groups.size();++i) {
    vector<string> gi;
    for(int j=0;j<groups[i].size();++j) {
      gi.push_back(files[groups[i][j]]);
    }
    res.push_back(gi);
  }

  return res;
}

vector<vector<int>> ImageDeduper::GenerateGroups(const vector<pair<int, int>> &pairs) const {
  vector<vector<int>> groups;

  vector<pair<int, int>> remaining_pairs = pairs;
  while(!remaining_pairs.empty()) {
    auto& pi = remaining_pairs.back();
    remaining_pairs.pop_back();
    vector<pair<int, int>> unprocessed_pairs;
    set<int> merged_set{pi.first, pi.second};
    for(auto pj : remaining_pairs) {
      if(merged_set.count(pj.first) || merged_set.count(pj.second)) {
        merged_set.insert(pj.first);
        merged_set.insert(pj.second);
      } else {
        unprocessed_pairs.push_back(pj);
      }
    }
    remaining_pairs = unprocessed_pairs;
    groups.push_back(vector<int>(merged_set.begin(), merged_set.end()));
  }

  return groups;
}