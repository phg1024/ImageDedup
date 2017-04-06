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

  /*
  def create_exclude_set(sim_pairs):
    exclude_set = []

    while len(sim_pairs) > 0:
        current_set = sim_pairs[0]
        test_pairs = sim_pairs[1:]
        unprocessed_set = []
        merged_set = set(current_set)
        for a, b in test_pairs:
            if a in merged_set or b in merged_set:
                merged_set.update(a)
                merged_set.update(b)
            else:
                unprocessed_set.append((a, b))

        sim_pairs = unprocessed_set
        exclude_set.extend(list(merged_set)[1:])

    return exclude_set
   */

  return groups;
}