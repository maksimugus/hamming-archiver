///pclp = parsing command line parameters

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

struct Parameters {
  bool create = false;
  std::string arcName;
  bool getList = false;
  bool extract = false;
  bool appFile = false;
  std::string appFileName;
  bool delFile = false;
  std::string delFileName;
  bool merge = false;
  std::string firstArcName;
  std::string secondArcName;
  std::vector<std::string> filesToAdd;
  std::vector<std::string> filesToExtract;
  uint64_t bitsInWord;
};

Parameters getParametersFromCommandLine(int argc, char *argv[]);

