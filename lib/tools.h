#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include "lib/ham.h"

const std::string inPath = "../../input/";
const std::string outPath = "../../output/";
const uint8_t byteSize = 8;

char *toBytes(size_t value);

size_t toInt(const char value[]);

std::string toBinaryString(uint8_t byte);

void copyAll(FILE *from, FILE *to);

void copyPart(FILE *from, FILE *to, uint32_t count);

void encodeFile(FILE *temp, FILE *arc, uint64_t bitsInWord);

void decodeFile(FILE *arc, FILE *temp, uint64_t bitsInWord);

std::string getInfo(FILE *archive);

std::vector<std::string> parseInfo(const std::string &info);

std::string updateInfo(const std::string &newInfo, const std::vector<std::string> &files);
