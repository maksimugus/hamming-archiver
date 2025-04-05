#include "lib/arc.h"
#include "lib/pclp.h"

int main(int argc, char *argv[]) {

  Parameters params = getParametersFromCommandLine(argc, argv);

  if (params.create) {
    createArchive(params.arcName, params.filesToAdd, params.bitsInWord);
  }

  if (params.getList) {
    std::vector<std::string> list = getListOfFiles(params.arcName);
    if (list.empty()) {
      std::cout << "archive is empty" << std::endl;
    } else {
      for (auto &fileName : list) {
        std::cout << fileName << std::endl;
      }
    }
  }

  if (params.extract) {
    if (params.filesToExtract.empty()) {
      extractAllFiles(params.arcName, params.bitsInWord);
    } else {
      extractFiles(params.arcName, params.filesToExtract, params.bitsInWord);
    }
  }

  if (params.appFile) {
    addNewFile(params.arcName, params.appFileName, params.bitsInWord);
  }

  if (params.delFile) {
    deleteFile(params.arcName, params.delFileName, params.bitsInWord);
  }

  if (params.merge) {
    mergeArchives(params.firstArcName, params.secondArcName, params.arcName, params.bitsInWord);
  }

  return 0;
}