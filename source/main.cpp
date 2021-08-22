#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <fstream>

#include "../include/common/common.h"

int main(int argc, char *argv[]) {
	int opt = 0;
	bool print_usage = false;
	char *src = nullptr, *output = nullptr, *irfile = nullptr;

	// solve command line option without check
  	for (int ch; (ch = getopt(argc, argv, "Sl:o:O:h")) != -1;) {
		switch (ch) {
			case 'l':
				irfile = strdup(optarg);
				break;
			case 'o':
				output = strdup(optarg);
				break;
			case 'O':
				opt = atoi(optarg);
				opt = opt > 0 ? opt : 0;
				break;
			case 'h':
				print_usage = true;
				break;
			default:
				break;
		}
	}

	//get src file
	if (optind <= argc) {
		src = argv[optind];
	}

	//src file not exit or get help
	if (src == nullptr || print_usage) {
		fprintf(stderr, "Usage: %s [-l irfile] [-S] [-o output_file] input_file [-O level]\n", argv[0]);
		return !print_usage && SYSTEM_ERROR;
	}

	//check input file
	int fd = open(src, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "failed to open %s\n", src);
		return SYSTEM_ERROR;
	}

	//open input file
	struct stat st {};
	fstat(fd, &st);
	char *input = (char *)mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	//run lexer

	return 0;
}

