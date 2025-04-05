#include "tools.h"

char *toBytes(size_t value) {
  auto *tmp = new char[5];
  tmp[0] = (char) (value);
  tmp[1] = (char) (value >> 8);
  tmp[2] = (char) (value >> 16);
  tmp[3] = (char) (value >> 24);
  tmp[4] = (char) (value >> 32);
  return tmp;
}

size_t toInt(const char value[]) {
  uint64_t tmp = 0;
  int base = 0;
  for (int i = 0; i < strlen(value); i++) {
    tmp |= (value[i] << base);
    base += 8;
  }
  return tmp;
}

std::string toBinaryString(uint8_t byte) {
  std::bitset<8> bin(byte);
  std::string res = bin.to_string();
  return res;
}

void copyAll(FILE *from, FILE *to) {
  char byte[1];

  while (!feof(from)) {
    if (fread(byte, 1, 1, from)) {
      fwrite(byte, 1, 1, to);
    }
  }
}

void copyPart(FILE *from, FILE *to, uint32_t count) {
  char byte[1];

  for (int i = 0; i < count; i++) {
    if (fread(byte, 1, 1, from)) {
      fwrite(byte, 1, 1, to);
    }
  }
}

void encodeFile(FILE *temp, FILE *arc, uint64_t bitsInWord) {

  uint8_t byte[1];
  std::string bufToCode;
  std::string bufToWrite;

  while (!feof(temp) || !bufToWrite.empty()) {
    if (fread(byte, 1, 1, temp)) {
      std::string bin = toBinaryString(byte[0]);
      std::reverse(bin.begin(), bin.end());
      bufToCode.append(bin);
    }

    size_t bufSize1 = bufToCode.size();

    if (feof(temp) && bufSize1 % bitsInWord != 0) {
      bufToCode.append(std::string(bitsInWord - bufSize1 % bitsInWord, '0'));
      bufSize1 = bufToCode.size();
    }

    if (bufSize1 >= bitsInWord) {
      uint64_t i = 0;
      while (i + bitsInWord <= bufSize1) {
        std::string word = bufToCode.substr(i, bitsInWord);
        bufToWrite.append(encodeWord(word));
        i += bitsInWord;
      }

      if (i != bufSize1) {
        bufToCode = bufToCode.substr(i);
      } else {
        bufToCode.clear();
      }
    }

    size_t bufSize2 = bufToWrite.size();

    if (feof(temp) && bufSize2 % byteSize != 0) {
      bufToWrite.append(std::string(byteSize - bufSize2 % byteSize, '0'));
      bufSize2 = bufToWrite.size();
    }

    if (bufSize2 >= byteSize) {
      int j = 0;
      while (j + byteSize <= bufSize2) {
        std::string byteBuf = bufToWrite.substr(j, byteSize);
        byte[0] = std::stoi(byteBuf, nullptr, 2);
        fwrite(byte, 1, 1, arc);
        j += byteSize;
      }

      if (j != bufSize2) {
        bufToWrite = bufToWrite.substr(j);
      } else {
        bufToWrite.clear();
      }
    }
  }
}

void decodeFile(FILE *arc, FILE *temp, uint64_t bitsInWord) {

  bitsInWord += (int) std::ceil(std::log2(bitsInWord)) + 1;
  uint8_t byte[1];
  std::string bufToDecode;
  std::string bufToWrite;

  while (!feof(arc) || !bufToWrite.empty()) {
    if (fread(byte, 1, 1, arc)) {
      bufToDecode.append(toBinaryString(byte[0]));
    }

    size_t bufSize1 = bufToDecode.size();
    if (bufSize1 >= bitsInWord) {
      uint64_t i = 0;
      while (i + bitsInWord <= bufSize1) {
        std::string word = bufToDecode.substr(i, bitsInWord);
        bufToWrite.append(decodeWord(word));
        i += bitsInWord;
      }

      if (i != bufSize1) {
        bufToDecode = bufToDecode.substr(i);
      } else {
        bufToDecode.clear();
      }
    }

    size_t bufSize2 = bufToWrite.size();

    if (bufSize2 >= byteSize) {
      int j = 0;
      while (j + byteSize <= bufSize2) {
        std::string byteBuf = bufToWrite.substr(j, byteSize);
        std::reverse(byteBuf.begin(), byteBuf.end());
        byte[0] = std::stoi(byteBuf, nullptr, 2);

        if (byte[0] != '\0') {
          fwrite(byte, 1, 1, temp);
        }

        j += byteSize;
      }

      if (j != bufSize2) {
        bufToWrite = bufToWrite.substr(j);
      } else {
        bufToWrite.clear();
      }
    }
  }
}

std::string getInfo(FILE *archive) {
  char infoSizeBuf[5];
  fread(infoSizeBuf, 1, 5, archive);
  size_t infoSize = toInt(infoSizeBuf);

  std::string info;
  info.resize(infoSize);
  fread(info.data(), 1, infoSize, archive);

  return info;
}

std::vector<std::string> parseInfo(const std::string &info) {

  std::vector<std::string> data;
  size_t infoSize = info.size();
  std::string buf;
  bool newFile = true; //indicates whether the information about the next file has begun
  size_t i = 0;

  while (i < infoSize - 1) {
    char ch = info[i];
    if (ch != '|') {
      buf += ch;
      i++;
    } else if (newFile) {
      data.push_back(buf);
      buf.clear();
      newFile = false;
      i++;
    } else {
      size_t nameSize = toInt(buf.c_str());
      data.push_back(info.substr(++i, nameSize));
      buf.clear();
      newFile = true;
      i += nameSize;
    }
  }

  return data;
}

std::string updateInfo(const std::string &oldInfo, const std::vector<std::string> &files) {
  std::string newInfo = oldInfo;

  for (const auto &fileName : files) {
    FILE *file = fopen((inPath + fileName).c_str(), "rb");
    if (!file) {
      std::cout << "file not found!" << std::endl;
      return "";
    }

    fseek(file, 0, SEEK_END);
    uint32_t fileSize = ftell(file);

    char *fileSizeBuf = toBytes(fileSize);
    char *nameSizeBuf = toBytes(fileName.size());

    fclose(file);

    newInfo.append(fileSizeBuf);
    newInfo.append("|");
    newInfo.append(nameSizeBuf);
    newInfo.append("|");
    newInfo.append(fileName);
  }

  return newInfo;
}