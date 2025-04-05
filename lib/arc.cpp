#include "arc.h"

void createArchive(const std::string &arcName, const std::vector<std::string> &files, uint64_t bitsInWord) {

  FILE *arc = fopen((outPath + arcName + ".haf").c_str(), "wb");
  createInfoHeader(arc, files);

  FILE *tempWrite = fopen((outPath + arcName + ".txt").c_str(), "wb");
  FILE *file;
  for (const auto &fileName : files) {
    file = fopen((inPath + fileName).c_str(), "rb");
    copyAll(file, tempWrite);
    std::cout << fileName << " add to " + arcName << std::endl;
    fclose(file);
  }

  fclose(tempWrite);

  FILE *tempRead = fopen((outPath + arcName + ".txt").c_str(), "rb");
  encodeFile(tempRead, arc, bitsInWord);

  fclose(arc);
  fclose(tempRead);
  remove((outPath + arcName + ".txt").c_str());

  std::cout << arcName + " successfully created" << std::endl;
}

void createInfoHeader(FILE *arc, const std::vector<std::string> &files, const std::string &oldInfo) {

  std::string info = updateInfo(oldInfo, files);

  char *infoSizeBuf = toBytes(info.size());
  std::string padding(5 - strlen(infoSizeBuf), '\0');

  fwrite(infoSizeBuf, 1, strlen(infoSizeBuf), arc);
  fwrite(padding.c_str(), 1, padding.size(), arc);
  fwrite(info.c_str(), 1, info.size(), arc);
}

void extractAllFiles(const std::string &arcName, const uint64_t bitsInWord) {

  FILE *arc = fopen((outPath + arcName + ".haf").c_str(), "rb");
  std::string header = getInfo(arc);
  std::vector<std::string> info = parseInfo(header);
  size_t infoSize = info.size();

  FILE *tempWrite = fopen((outPath + arcName + ".txt").c_str(), "wb");
  decodeFile(arc, tempWrite, bitsInWord);
  fclose(tempWrite);

  FILE *tempRead = fopen((outPath + arcName + ".txt").c_str(), "rb");
  FILE *file;
  for (int i = 0; i < infoSize - 1; i += 2) {
    const char *fileSizeBuf = info[i].c_str();
    std::string fileName = info[i + 1];
    uint32_t fileSize = toInt(fileSizeBuf);

    file = fopen((outPath + fileName).c_str(), "wb");
    copyPart(tempRead, file, fileSize);

    fclose(file);
  }

  fclose(tempRead);
  remove((outPath + arcName + ".txt").c_str());
  std::cout << "all files extracted" << std::endl;
}

void extractFiles(const std::string &arcName, const std::vector<std::string> &files, uint64_t bitsInWord) {

  FILE *arc = fopen((outPath + arcName + ".haf").c_str(), "rb");
  std::string header = getInfo(arc);
  std::vector<std::string> info = parseInfo(header);
  size_t infoSize = info.size();

  FILE *tempWrite = fopen((outPath + arcName + ".txt").c_str(), "wb");
  decodeFile(arc, tempWrite, bitsInWord);
  fclose(arc);
  fclose(tempWrite);

  FILE *tempRead = fopen((outPath + arcName + ".txt").c_str(), "rb");

  FILE *file;
  size_t size = files.size();
  for (int i = 0; i < size; i++) {
    long pointer = 0;
    for (int j = 1; j < infoSize; j += 2) {
      size_t fileSize = toInt(info[j - 1].c_str());
      if (info[j] == files[i]) {
        file = fopen((outPath + info[j]).c_str(), "wb");
        fseek(tempRead, pointer, SEEK_SET);
        copyPart(tempRead, file, fileSize);
        fclose(file);

        std::cout << info[j] + " extracted" << std::endl;
        break;
      }

      pointer += (long) fileSize;
    }
  }

  fclose(tempRead);
  remove((outPath + arcName + ".txt").c_str());
}

std::vector<std::string> getListOfFiles(const std::string &arcName) {
  FILE *tempInfo = fopen((outPath + arcName + ".haf").c_str(), "rb");

  std::string header = getInfo(tempInfo);
  std::vector<std::string> info = parseInfo(header);

  std::vector<std::string> files;
  for (int i = 1; i < info.size(); i += 2) {
    files.push_back(info[i]);
  }

  fclose(tempInfo);
  return files;
}

void addNewFile(const std::string &arcName, const std::string &fileName, uint64_t bitsInWord) {

  FILE *arc = fopen((outPath + arcName + ".haf").c_str(), "rb");
  std::string info = getInfo(arc);

  FILE *tempWrite = fopen((outPath + "temp.txt").c_str(), "wb");
  decodeFile(arc, tempWrite, bitsInWord);
  fclose(arc);

  FILE *newFile = fopen((inPath + fileName).c_str(), "rb");
  copyAll(newFile, tempWrite);

  fclose(tempWrite);
  fclose(newFile);

  FILE *updArc = fopen((outPath + arcName + ".haf").c_str(), "wb");
  FILE *tempRead = fopen((outPath + "temp.txt").c_str(), "rb");
  createInfoHeader(updArc, {fileName}, info);
  encodeFile(tempRead, updArc, bitsInWord);

  fclose(updArc);
  fclose(tempRead);
  remove((outPath + "temp.txt").c_str());

  std::cout << fileName + " added to " + arcName << std::endl;
}

void deleteFile(const std::string &arcName, const std::string &fileName, uint64_t bitsInWord) {

  FILE *arc = fopen((outPath + arcName + ".haf").c_str(), "rb");
  std::string header = getInfo(arc);
  std::vector<std::string> info = parseInfo(header);
  size_t infoSize = info.size();

  FILE *tempArcWrite = fopen((outPath + arcName + ".txt").c_str(), "wb");
  decodeFile(arc, tempArcWrite, bitsInWord);
  fclose(arc);
  fclose(tempArcWrite);

  FILE *tempArcRead = fopen((outPath + arcName + ".txt").c_str(), "rb");
  FILE *tempWrite = fopen((outPath + "temp.txt").c_str(), "wb");

  std::vector<std::string> remainingFiles;
  for (int j = 1; j < infoSize; j += 2) {
    size_t fileSize = toInt(info[j - 1].c_str());
    if (info[j] != fileName) {
      copyPart(tempArcRead, tempWrite, fileSize);
      remainingFiles.emplace_back(info[j]);
    } else {
      fseek(tempArcRead, ftell(tempArcRead) + (long) fileSize, SEEK_SET);
    }
  }

  fclose(tempArcRead);
  fclose(tempWrite);

  if (remainingFiles.size() == info.size() / 2) {
    std::cout << arcName + " doesn't contain " + fileName << std::endl;
  } else {
    FILE *tempRead = fopen((outPath + "temp.txt").c_str(), "rb");
    FILE *updArc = fopen((outPath + arcName + ".haf").c_str(), "wb");
    createInfoHeader(updArc, remainingFiles);
    encodeFile(tempRead, updArc, bitsInWord);

    fclose(tempRead);
    fclose(updArc);

    std::cout << fileName + " deleted" << std::endl;
  }

  remove((outPath + arcName + ".txt").c_str());
  remove((outPath + "temp.txt").c_str());
}

void mergeArchives(const std::string &arcName1,
                   const std::string &arcName2,
                   const std::string &newArcName,
                   uint64_t bitsInWord) {

  FILE *arc1 = fopen((outPath + arcName1 + ".haf").c_str(), "rb");
  FILE *arc2 = fopen((outPath + arcName2 + ".haf").c_str(), "rb");
  std::string info1 = getInfo(arc1);
  std::string info2 = getInfo(arc2);

  FILE *tempWrite = fopen((outPath + newArcName + ".txt").c_str(), "wb");
  decodeFile(arc1, tempWrite, bitsInWord);
  decodeFile(arc2, tempWrite, bitsInWord);
  fclose(arc1);
  fclose(arc2);
  fclose(tempWrite);

  std::string newInfo = info1 + info2;

  FILE *tempRead = fopen((outPath + newArcName + ".txt").c_str(), "rb");
  FILE *newArc = fopen((outPath + newArcName + ".haf").c_str(), "wb");
  createInfoHeader(newArc, {}, newInfo);
  encodeFile(tempRead, newArc, bitsInWord);

  fclose(tempRead);
  fclose(newArc);
  remove((outPath + newArcName + ".txt").c_str());

  std::cout << arcName1 + " & " + arcName2 + " merged to " + newArcName << std::endl;
}
