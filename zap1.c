#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int c;
    char *src_username = NULL;
    char *tgt_username = NULL;
    char *src_day = NULL;
    char *tgt_day = NULL;
    char *src_terminal = NULL;
    char *tgt_terminal = NULL;
    
    int is_replace = 0;

    while ((c = getopt_long(argc, argv, "a:d:t:R")) != -1) {

		switch(c) {
		case 'a':
            src_username = optarg;
            if (optind < argc && argv[optind][0] != '-') {
                tgt_username = argv[optind];
            }
			break;
		case 't':
            src_terminal = optarg;
            if (optind < argc && argv[optind][0] != '-') {
                tgt_terminal = argv[optind];
            }
			break;
		case 'd':
            src_day = optarg;
            if (optind < argc && argv[optind][0] != '-') {
                tgt_day = argv[optind];
            }
			break;
		case 'R':
            is_replace = 1;
			break;
        case '?':
            break;
        default:
            printf ("?? getopt returned character code 0%o ??\n", c);
		}
	}
    printf("R: %d\n", is_replace);
    if (!is_replace) {
        tgt_username = NULL;
        tgt_day = NULL;
        tgt_terminal = NULL;
    }
    printf("src_username: %s, tgt_username: %s\n", src_username, tgt_username);
    printf("src_day: %s, tgt_day: %s\n", src_day, tgt_day);
    printf("src_terminal: %s, tgt_terminal: %s\n", src_terminal, tgt_terminal);
}