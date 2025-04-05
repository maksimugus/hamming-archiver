#include <string>
#include <vector>
#include <iostream>
#include <cstring>
#include <utility>
#include "lib/tools.h"

void createArchive(const std::string &arcName, const std::vector<std::string> &files, uint64_t bitsInWord);

void createInfoHeader(FILE *arc, const std::vector<std::string> &files, const std::string &oldInfo = "");

void extractAllFiles(const std::string &arcName, uint64_t bitsInWord);

void extractFiles(const std::string &arcName, const std::vector<std::string> &files, uint64_t bitsInWord);

std::vector<std::string> getListOfFiles(const std::string &arcName);

void addNewFile(const std::string &arcName, const std::string &fileName, uint64_t bitsInWord);

void deleteFile(const std::string &arcName, const std::string &fileName, uint64_t bitsInWord);

void mergeArchives(const std::string &arcName1,
                   const std::string &arcName2,
                   const std::string &newArcName,
                   uint64_t bitsInWord);