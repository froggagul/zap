#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utmpx.h> // struct utmp#include
#include <lastlog.h>
#include <fcntl.h> // O_RDWR
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pwd.h>

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

char *strptime(const char *s, const char *format, struct tm *tm);

void getYMD(char *buf, time_t t) {
    struct tm* ptm;
    ptm = gmtime(&t);
    strftime(buf, sizeof(buf), "%d%m%y", ptm);
    // printf("The date is: %s\n", buf);
}

time_t getSeconds(char *buf) {
    struct tm ptm = { 0 };
    strptime(buf, "%d%m%y", &ptm);
    return mktime(&ptm);
}


void read_tmp(char *filename, char*filename_new) {
    int fp, fp_new;
	struct utmpx ut, ut_new;	/* Current utmp entry */
    if ((fp = open(filename,O_RDONLY)) >= 0 && (fp_new = open(filename_new,O_RDONLY)) >= 0) {
        printf("\nbefore\n");
        while(read(fp, &ut, sizeof (ut))> 0) {
            printf("ut_type: %d ut_line: %s ut_id: %s ut_user: %s ut_host: %s ut_exit: %d ut_exit_termination: %d\n", ut.ut_type, ut.ut_line, ut.ut_id, ut.ut_user, ut.ut_host, ut.ut_exit.__e_exit, ut.ut_exit.__e_termination);
            time_t seconds = ut.ut_tv.tv_sec;
            char *buf = (char*)malloc(12);
            getYMD(buf, seconds);
            printf("ut_pid : %d tv_sec: %d converted tv_sec: %s\n", ut.ut_pid, ut.ut_tv.tv_sec, buf);
        }
        printf("\n========\n");
        printf("after\n");
        while(read(fp_new, &ut, sizeof (ut))> 0) {
            printf("ut_type: %d ut_line: %s ut_id: %s ut_user: %s ut_host: %s ut_exit: %d ut_exit_termination: %d\n", ut.ut_type, ut.ut_line, ut.ut_id, ut.ut_user, ut.ut_host, ut.ut_exit.__e_exit, ut.ut_exit.__e_termination);
            time_t seconds = ut.ut_tv.tv_sec;
            char *buf = (char*)malloc(12);
            getYMD(buf, seconds);
            printf("ut_pid : %d tv_sec: %d converted tv_sec: %s\n", ut.ut_pid, ut.ut_tv.tv_sec, buf);
        }

        close(fp);
        close(fp_new);
    }
}

int is_valid() {
    int valid = 0;
}

void process_tmp(char *filename, char *filename_new, int is_replace, char *src_username, char *tgt_username, char *src_day, char *tgt_day, char *src_terminal, char *tgt_terminal, char *src_host, char *tgt_host) {
    int fp, fp_new;
	struct utmpx ut;	/* modified utmp entry */
	struct utmpx ut_base;	/* base utmp entry */

    int quit = 0;
    remove(filename_new);

    if ((fp = open(filename,O_RDWR)) >= 0 && (fp_new = open(filename_new,O_RDWR | O_CREAT, 0664)) >= 0) {
        while(read(fp, &ut, sizeof (ut))> 0) {
            time_t seconds = ut.ut_tv.tv_sec;
            char *buf = (char*)malloc(12);
            getYMD(buf, seconds);

            if (is_replace) {
                ut_base = ut;
                int write_replace = 1;
                if (strlen(src_username) != 0 && strlen(tgt_username) != 0) {
                    if (strncmp(ut.ut_user, src_username, max(sizeof(ut.ut_user), sizeof(src_username))) == 0) {
                        write_replace &= 1;
                        strncpy(ut.ut_user, tgt_username, sizeof(ut.ut_user));
                    } else {
                        write_replace &= 0;
                    }
                }
                if (strlen(src_terminal) != 0 && strlen(tgt_username) != 0) {
                    if (strncmp(ut.ut_line, src_terminal, max(sizeof(ut.ut_line), sizeof(src_terminal))) == 0) {
                        write_replace &= 1;
                        strncpy(ut.ut_line, tgt_terminal, sizeof(ut.ut_line));
                    } else {
                        write_replace &= 0;
                    }
                }
                if (strlen(src_host) != 0 && strlen(tgt_host) != 0) {
                    if (strncmp(ut.ut_host, src_host, max(sizeof(ut.ut_host), sizeof(src_host))) == 0) {
                        write_replace &= 1;
                        strncpy(ut.ut_host, tgt_host, sizeof(ut.ut_host));
                    } else {
                        write_replace &= 0;
                    }
                }
                if (strlen(src_day) != 0 && strlen(tgt_day) != 0) {
                    if (strncmp(buf, src_day, max(sizeof(buf), sizeof(src_day))) == 0) {
                        write_replace &= 1;
                        time_t tgt_seconds = getSeconds(tgt_day);
                        time_t src_seconds = getSeconds(buf);
                        seconds = seconds + (tgt_seconds - src_seconds);
                        ut.ut_tv.tv_sec = seconds;
                    } else {
                        write_replace &= 0;
                    }
                }
                if (write_replace) {
                    write (fp_new, &ut, sizeof (ut));                
                } else {
                    write (fp_new, &ut_base, sizeof (ut_base));
                }
            } else {
                int not_delete = 1;
                if (strlen(src_username) != 0) {
                    if (strncmp(ut.ut_user, src_username, max(sizeof(ut.ut_user), sizeof(src_username))) != 0) {
                        not_delete &= 1;
                    } else {
                        not_delete &= 0;
                    }
                }
                if (strlen(src_terminal) != 0) {
                    if (strncmp(ut.ut_line, src_terminal, max(sizeof(ut.ut_line), sizeof(src_terminal))) != 0) {
                        not_delete &= 1;
                    } else {
                        not_delete &= 0;
                    }
                }
                if (strlen(src_host) != 0) {
                    if (strncmp(ut.ut_host, src_host, max(sizeof(ut.ut_host), sizeof(src_host))) != 0) {
                        not_delete &= 1;
                    } else {
                        not_delete &= 0;
                    }
                }
                if (strlen(src_day) != 0) {
                    if (strncmp(buf, src_day, max(sizeof(buf), sizeof(src_day))) != 0) {
                        not_delete &= 1;
                    } else {
                        not_delete &= 0;
                    }
                }
                if (not_delete) {
                    write (fp_new, &ut, sizeof (ut));
                }
            }
        }
        close(fp);
        close(fp_new);
    }
}

int isValidIP(char *ip) {
    if(strcmp(ip, " ") == 0) {
        return 0;
    }
    int ip_len = strlen(ip);
    int numCount = 0, dotCount = 0;
    int i = 0;

    if(ip_len < 7 || ip_len > 15) {
        return 0;
    }

    for( i = 0; i < ip_len; i++) {
        if(ip[i] < '0' || ip[i] > '9') {
            if(ip[i] == '.') {
                numCount = 0;
                dotCount += 1;
            } else {
                return 0;
            }
        } else {
            numCount += 1;
            if(numCount > 3) {
                return 0;
            }
        }
    }

    if(dotCount != 3) {
        return 0;
    }
    return 1;
}

void update_lastlog(char *filename_wtmp_new, char* username, struct lastlog *ll) {
    int fp;
	struct utmpx ut;	/* modified utmp entry */
    time_t max_second = 0;

    if ((fp = open(filename_wtmp_new,O_RDWR)) >= 0) {
        while(read(fp, &ut, sizeof (ut))> 0) {
            if (strncmp(ut.ut_user, username, max(sizeof(ut.ut_user), sizeof(username))) == 0 && isValidIP(ut.ut_host)) {
                if(max_second < ut.ut_tv.tv_sec) {
                    max_second = ut.ut_tv.tv_sec;

                    ll->ll_time = max_second;
                    strncpy(ll->ll_host, ut.ut_host, sizeof(ut.ut_host));
                    strncpy(ll->ll_line, ut.ut_line, sizeof(ut.ut_line));
                }
            }
        }
    }
    if (max_second == 0) {
        ll->ll_time = max_second;
        strncpy(ll->ll_host, "", sizeof(""));
        strncpy(ll->ll_line, "", sizeof(""));
    }
}

void process_lastlog(char *filename, char *filename_new, char*filename_wtmp_new, int is_replace, char *src_username, char *tgt_username) {
    int fp, fp_new;
	struct lastlog ll;	/* Current utmp entry */

    int quit = 0;

    struct passwd *pwd;
    uid_t src_id = -1, tgt_id = -1;

    if ((pwd = getpwnam(src_username)) != NULL) {
        src_id = pwd->pw_uid;
    }
    if (is_replace) {
        if ((pwd = getpwnam(tgt_username)) != NULL) {
            tgt_id = pwd->pw_uid;
        }
    }
    uid_t cur_id = 0;
    if ((fp = open(filename,O_RDWR)) >= 0 && (fp_new = open(filename_new,O_RDWR | O_CREAT, 0664)) >= 0) {
        while(read(fp, &ll, sizeof (ll))> 0) {
            if (cur_id == src_id) {
                update_lastlog(filename_wtmp_new, src_username, &ll);
            } else if (cur_id == tgt_id) {
                update_lastlog(filename_wtmp_new, tgt_username, &ll);
            }
            cur_id += 1;
            write (fp_new, &ll, sizeof (ll));
        }
        close(fp);
        close(fp_new);
    }
}

void read_lastlog(char *filename, char *filename_new) {
    int fp;
	struct lastlog ll;	/* Current utmp entry */
    int cur_id = 0;

    if ((fp = open(filename_new,O_RDWR)) >= 0) {
        while(read(fp, &ll, sizeof (ll))> 0) {
            printf("ll_host: %s ll_line: %s ll_time: %d id: %d\n", ll.ll_host, ll.ll_line, ll.ll_time, cur_id);
            cur_id += 1;
        }
    }
}

int main(int argc, char **argv) {
    int c;
    char *src_username;
    src_username = calloc(100, sizeof(char));
    char *tgt_username;
    tgt_username = calloc(100, sizeof(char));
    char *src_day;
    src_day = calloc(100, sizeof(char));
    char *tgt_day;
    tgt_day = calloc(100, sizeof(char));
    char *src_device;
    src_device = calloc(100, sizeof(char));
    char *tgt_device;
    tgt_device = calloc(100, sizeof(char));
    char *src_host;
    src_host = calloc(100, sizeof(char));
    char *tgt_host;
    tgt_host = calloc(100, sizeof(char));
    char *filename = (char*)malloc(100);
    char *filename_new = (char*)malloc(100);
    char *filename_new_wtmp = (char*)malloc(100);
    
    int is_replace = 0;

    while ((c = getopt_long(argc, argv, "a:i:d:t:R")) != -1) {

		switch(c) {
		case 'a':
            strcpy(src_username, optarg);
            if (optind < argc && argv[optind][0] != '-') {
                strcpy(tgt_username, argv[optind]);
            }
			break;
		case 't':
            strcpy(src_device, optarg);
            if (optind < argc && argv[optind][0] != '-') {
                strcpy(tgt_device, argv[optind]);
            }
			break;
		case 'd':
            strcpy(src_day, optarg);
            if (optind < argc && argv[optind][0] != '-') {
                strcpy(tgt_day, argv[optind]);
            }
			break;
		case 'i':
            strcpy(src_host, optarg);
            if (optind < argc && argv[optind][0] != '-') {
                strcpy(tgt_host, argv[optind]);
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
        tgt_device = NULL;
    }

    printf("src_username: %s, tgt_username: %s\n", src_username, tgt_username);
    printf("src_day: %s, tgt_day: %s\n", src_day, tgt_day);
    printf("src_device: %s, tgt_device: %s\n", src_device, tgt_device);
    printf("src_host: %s, tgt_host: %s\n", src_host, tgt_host);


    strncpy(filename, "./wtmp", 7);
    strncpy(filename_new, "./wtmp_new", 11);
    strncpy(filename_new_wtmp, "./wtmp_new", 11);
    process_tmp(filename, filename_new, is_replace, src_username, tgt_username, src_day, tgt_day, src_device, tgt_device, src_host, tgt_host);
    // read_tmp(filename, filename_new);

    strncpy(filename, "./utmp", 7);
    strncpy(filename_new, "./utmp_new", 11);
    process_tmp(filename, filename_new, is_replace, src_username, tgt_username, src_day, tgt_day, src_device, tgt_device, src_host, tgt_host);

    strncpy(filename, "./lastlog", 10);
    strncpy(filename_new, "./lastlog_new", 14);
    process_lastlog(filename, filename_new, filename_new_wtmp, is_replace, src_username, tgt_username);
    // read_lastlog(filename, filename_new);

    rename("./wtmp", "./wtmp_backup");
    rename("./wtmp_new", "./wtmp");
    rename("./utmp", "./utmp_backup");
    rename("./utmp_new", "./utmp");

    free(src_username);
    free(tgt_username);
    free(src_day);
    free(tgt_day);
    free(src_device);
    free(tgt_device);
    free(src_host);
    free(tgt_host);

    free(filename);
    free(filename_new);
    free(filename_new_wtmp);

    return 0;
}