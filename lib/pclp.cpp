#include "pclp.h"

Parameters getParametersFromCommandLine(int argc, char *argv[]) {

  Parameters input{};

  uint8_t flag = 0; //shows in which list should be added filename

  for (int i = 1; i < argc; i++) {

    if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--create") == 0) {

      input.create = true;
      flag = 1;

    } else if (strcmp(argv[i], "-f") == 0) {

      input.arcName = argv[++i];

    } else if (argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] == 'f') {

      std::string buf(argv[i]);
      size_t length = buf.find_last_of(']') - buf.find_first_of('[') - 1;
      input.arcName = buf.substr(buf.find_first_of('[') + 1, length);

    } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--list") == 0) {

      input.getList = true;

    } else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--extract") == 0) {

      input.extract = true;
      flag = 2;

    } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--append") == 0) {

      input.appFile = true;
      input.appFileName = argv[++i];

    } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--delete") == 0) {

      input.delFile = true;
      input.delFileName = argv[++i];

    } else if (strcmp(argv[i], "-A") == 0 || strcmp(argv[i], "--concatenate") == 0) {

      input.merge = true;
      input.firstArcName = argv[++i];
      input.secondArcName = argv[++i];

    } else if (strcmp(argv[i], "-hc") == 0) {

      input.bitsInWord = std::strtoull(argv[++i], nullptr, 10);

    } else {

      if (flag == 1) {
        input.filesToAdd.emplace_back(argv[i]);
      } else if (flag == 2) {
        input.filesToExtract.emplace_back(argv[i]);
      }

    }
  }

  return input;
}
