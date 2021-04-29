#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmpx.h> // struct utmp#include
#include <lastlog.h>
#include <fcntl.h> // O_RDWR
#include <sys/stat.h>
#include <string.h>

#define UCHUNKSIZE	16384	/* How much we read at once. */

void process_wtmp(char *filename) {
    int fp;
	struct utmpx ut;	/* Current utmp entry */
	time_t begintime;
    time_t lastdown;
    struct stat st;

    int quit = 0;


    if ((fp = open(filename,O_RDWR)) >= 0) {
        while(read(fp, &ut, sizeof (ut))> 0) {
            printf("ut_type: %d ut_line: %s ut_id: %s ut_user: %s ut_host: %s ut_exit: %d ut_exit_termination: %d\n", ut.ut_type, ut.ut_line, ut.ut_id, ut.ut_user, ut.ut_host, ut.ut_exit.__e_exit, ut.ut_exit.__e_termination);
            time_t seconds = ut.ut_tv.tv_sec;
            printf("tv_sec: %d \n", ut.ut_tv.tv_sec);
        }
        close(fp);
    }

}

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

    process_wtmp("./wtmp");
}