#pragma once

#include "common.h"
#include "pHash.h"

using std::vector;
using std::string;
using std::map;
using std::pair;

class ImageDeduper {
public:
  ImageDeduper(const vector<string>& imagefiles);
  vector<vector<string>> Dedup() const;

private:
  void GenerateHashes(const vector<string>& imagefiles);
  vector<vector<int>> GenerateGroups(const vector<pair<int, int>>& pairs) const;

private:
  vector<string> files;
  map<string, ulong64> hashes;
};