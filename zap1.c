#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmpx.h> // struct utmp#include
#include <lastlog.h>
#include <fcntl.h> // O_RDWR
#include <sys/stat.h>
#include <string.h>

void read_tmp(char *filename, char*filename_new) {
    int fp, fp_new;
	struct utmpx ut, ut_new;	/* Current utmp entry */
    if ((fp = open(filename,O_RDONLY)) >= 0 && (fp_new = open(filename_new,O_RDONLY)) >= 0) {
        printf("\nbefore\n");
        while(read(fp, &ut, sizeof (ut))> 0) {
            printf("ut_type: %d ut_line: %s ut_id: %s ut_user: %s ut_host: %s ut_exit: %d ut_exit_termination: %d\n", ut.ut_type, ut.ut_line, ut.ut_id, ut.ut_user, ut.ut_host, ut.ut_exit.__e_exit, ut.ut_exit.__e_termination);
            time_t seconds = ut.ut_tv.tv_sec;
            printf("ut_pid : %d tv_sec: %d \n", ut.ut_pid, ut.ut_tv.tv_sec);
        }
        printf("\n========\n");
        printf("after\n");
        while(read(fp_new, &ut, sizeof (ut))> 0) {
            printf("ut_type: %d ut_line: %s ut_id: %s ut_user: %s ut_host: %s ut_exit: %d ut_exit_termination: %d\n", ut.ut_type, ut.ut_line, ut.ut_id, ut.ut_user, ut.ut_host, ut.ut_exit.__e_exit, ut.ut_exit.__e_termination);
            time_t seconds = ut.ut_tv.tv_sec;
            printf("ut_pid : %d tv_sec: %d \n", ut.ut_pid, ut.ut_tv.tv_sec);
        }

        close(fp);
        close(fp_new);
    }
}

void process_tmp(char *filename, char *filename_new, int is_replace, char *src_username, char *tgt_username, char *src_day, char *tgt_day, char *src_terminal, char *tgt_terminal) {
    int fp, fp_new;
	struct utmpx ut;	/* Current utmp entry */

    int quit = 0;
    remove(filename_new);

    if ((fp = open(filename,O_RDWR)) >= 0 && (fp_new = open(filename_new,O_RDWR | O_CREAT, 0664)) >= 0) {
        while(read(fp, &ut, sizeof (ut))> 0) {
            time_t seconds = ut.ut_tv.tv_sec;
            if (is_replace) {
                if (strncmp(ut.ut_user, src_username, sizeof(ut.ut_user)) == 0) {
                    strncpy(ut.ut_user, tgt_username, sizeof(ut.ut_user));
                }
                write (fp_new, &ut, sizeof (ut));
            } else {
                if (strncmp(ut.ut_user, src_username, sizeof(ut.ut_user)) != 0) {
                    write (fp_new, &ut, sizeof (ut));
                }
            }
        }
        close(fp);
        close(fp_new);
    }
}

int main(int argc, char **argv) {
    int c;
    char *src_username = (char*)malloc(100);
    char *tgt_username = (char*)malloc(100);
    char *src_day = (char*)malloc(100);
    char *tgt_day = (char*)malloc(100);
    char *src_terminal = (char*)malloc(100);
    char *tgt_terminal = (char*)malloc(100);
    char *filename = (char*)malloc(100);
    char *filename_new = (char*)malloc(100);
    
    int is_replace = 0;

    while ((c = getopt_long(argc, argv, "a:d:t:R")) != -1) {

		switch(c) {
		case 'a':
            strcpy(src_username, optarg);
            if (optind < argc && argv[optind][0] != '-') {
                strcpy(tgt_username, argv[optind]);
            }
			break;
		case 't':
            strcpy(src_terminal, optarg);
            if (optind < argc && argv[optind][0] != '-') {
                strcpy(tgt_terminal, argv[optind]);
            }
			break;
		case 'd':
            strcpy(src_day, optarg);
            if (optind < argc && argv[optind][0] != '-') {
                strcpy(tgt_day, argv[optind]);
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

    printf("src_username: %s, tgt_username: %s\n", src_username, tgt_username);
    printf("src_day: %s, tgt_day: %s\n", src_day, tgt_day);
    printf("src_terminal: %s, tgt_terminal: %s\n", src_terminal, tgt_terminal);

    strncpy(filename, "./wtmp", 7);
    strncpy(filename_new, "./wtmp_new", 11);
    process_tmp(filename, filename_new, is_replace, src_username, tgt_username, src_day, tgt_day, src_terminal, tgt_terminal);
    read_tmp(filename, filename_new);
    // strncpy(filename, "./utmp", 7);
    // strncpy(filename_new, "./utmp_new", 11);
    // process_tmp(filename, filename_new, is_replace, src_username, tgt_username, src_day, tgt_day, src_terminal, tgt_terminal);

    free(src_username);
    free(tgt_username);
    free(src_day);
    free(tgt_day);
    free(src_terminal);
    free(tgt_terminal);
    free(filename);
    free(filename_new);

    return 0;
}