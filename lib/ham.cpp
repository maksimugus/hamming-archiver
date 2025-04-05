#include "ham.h"

std::string encodeWord(std::string &inf) {

  std::vector<bool> word = convert(inf);

  int pos = 1;
  const int numOfCB = (int) std::ceil(std::log2(word.size())) + 1;
  auto it = word.begin();
  for (int i = 0; i < numOfCB; i++) {
    word.insert(it + pos - 1, false);
    pos *= 2;
  }

  std::vector<bool> controlBits = getControlBits(word);

  pos = 1;
  for (auto cb : controlBits) {
    word[pos - 1] = cb;
    pos *= 2;
  }

  std::string result;
  for (auto bit : word) {
    result.append(std::to_string(bit));
  }

  return result;
}

std::string decodeWord(std::string &inf) {

  std::vector<bool> word = convert(inf);

  std::vector<bool> controlBits = getControlBits(word);

  int pos = 1;
  int errBit = 0;
  for (auto i : controlBits) {
    errBit += i * pos;
    pos *= 2;
  }

  if (errBit != 0) {
    word[errBit - 1] = !word[errBit - 1];
  }

  std::string result;
  pos = 1;
  size_t wordLen = word.size();
  for (int i = 0; i < wordLen; i++) {
    if (i != pos - 1) {
      result.append(std::to_string(word[i]));
    } else {
      pos *= 2;
    }
  }

  return result;
}

std::vector<bool> getControlBits(const std::vector<bool> &word) {

  size_t wordLen = word.size();
  std::vector<bool> controlBits;

  for (int pos = 1; pos < wordLen; pos *= 2) {
    bool buf = false;

    for (int i = pos - 1; i < wordLen; i += pos) {
      int end = i + pos;

      while (i < wordLen && i < end) {
        buf = buf ^ word[i];
        i++;
      }

    }

    controlBits.emplace_back(buf);
  }

  return controlBits;
}

std::vector<bool> convert(std::string &st) {
  std::vector<bool> result;

  for (char bit : st) {
    if (bit == '0') {
      result.push_back(false);
    } else {
      result.push_back(true);
    }
  }

  return result;
}