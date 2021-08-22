#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <fstream>

int main(int argc, char *argv[]) {
	int opt = 0;
	bool print_usage = false;
	char *src = nullptr, *output = nullptr, *irfile = nullptr;

  	for (int ch; (ch = getopt(argc, argv, "Sl:o:O:h")) != -1;) {
		printf("%c\n",ch);
		switch (ch) {
			case 'S':
				//do nothing
				break;
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

	if (optind <= argc) {
		src = argv[optind];
	}

	return 0;
}

