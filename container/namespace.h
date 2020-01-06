void unshare_command(char[]);
void pivot_root(const char*, const char*);


void pivot_root(const char *new_root, const char *put_old)
{
    int ret = syscall(SYS_pivot_root, new_root, put_old);
    if ( ret != 0 ){
        printf("Failed to change root from %s to %s\n", put_old, new_root);
    }
    perror("pivot_root");
}

void unshare_command(char opts[])
{
    int flags = 0;
    blue("\nAdding namespace flags\n");

    for (int i = 0; i < (int)strlen(opts); i++)
    {
        switch (opts[i])
        {
        case 'i':
            flags |= CLONE_NEWIPC;
            yellow("Added IPC flag");
            break;
        case 'm':
            flags |= CLONE_NEWNS;
            yellow("Added mount flag");
            break;
        case 'n':
            flags |= CLONE_NEWNET;
            yellow("Added network flag");
            break;
        case 'p':
            flags |= CLONE_NEWPID;
            yellow("Added PID flag");
            break;
        case 'u':
            flags |= CLONE_NEWUTS;
            yellow("Added UTS flag");
            break;
        case 'U':
            flags |= CLONE_NEWUSER;
            yellow("Added user flag");
            break;
        case 'c':
            flags |= CLONE_NEWCGROUP;
            yellow("Added cgroup flag");
            break;
        default:
            printf("\nInvalid flag supplied: %d", opts[i]);
            break;
        }
    }

    int ret = unshare(flags);
    printf("Running the unshare command, return status: %d\n", ret);
    printf("New PID after unshare is %i\n", getpid());
    perror("unshare");

}


static void
proc_setgroups_write(pid_t child_pid, char *str)
{
    char setgroups_path[PATH_MAX];
    int fd;

    snprintf(setgroups_path, PATH_MAX, "/proc/%ld/setgroups",
             (long) child_pid);

    fd = open(setgroups_path, O_RDWR);
    if (fd == -1) {

        /* We may be on a system that doesn't support
           /proc/PID/setgroups. In that case, the file won't exist,
           and the system won't impose the restrictions that Linux 3.19
           added. That's fine: we don't need to do anything in order
           to permit 'gid_map' to be updated.

           However, if the error from open() was something other than
           the ENOENT error that is expected for that case,  let the
           user know. */

        if (errno != ENOENT)
            fprintf(stderr, "ERROR: open %s: %s\n", setgroups_path, strerror(errno));
        return;
    }

    if (write(fd, str, strlen(str)) == -1)
        fprintf(stderr, "ERROR: write %s: %s\n", setgroups_path, strerror(errno));

    close(fd);
}


static void
update_map(char *mapping, char *map_file)
{
    int fd, j;
    size_t map_len;     /* Length of 'mapping' */

    /* Replace commas in mapping string with newlines */

    map_len = strlen(mapping);
    for (j = 0; j < map_len; j++)
        if (mapping[j] == ',')
            mapping[j] = '\n';

    fd = open(map_file, O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "ERROR: open %s: %s\n", map_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (write(fd, mapping, map_len) != map_len) {
        fprintf(stderr, "ERROR: write %s: %s\n", map_file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void set_user_namespace(){

    char *uid_map, *gid_map, *uid_map_command, *map_buf, *map_path;
    int map_zero;

    pid_t pid = getpid();
    uid_t prev_uid = getuid();
    gid_t prev_gid = getgid();

//    unshare_command("U");
//    perror("unshare");
//
//    uid_t new_uid = getuid();
//    gid_t new_gid = getgid();

    uid_t new_uid = 65534;
    gid_t new_gid = 65534;

    printf("Transitioned from UID: %d to %d after user namespace unsharing \n", prev_uid, new_uid);
    printf("Transitioned from GID: %d to %d after user namespace unsharing \n", prev_gid, new_gid);


//            asprintf(&uid_map_command, "echo '%d %d %ld' > /proc/%d/gid_map", prev_gid, new_gid, 4294967295, pid);
//            printf("Running command: %s", uid_map_command);
//            exec_command(uid_map_command);


    if (gid_map != NULL || map_zero) {
        proc_setgroups_write(pid, "deny");

        asprintf(&map_path, "/proc/%ld/gid_map", (long) pid);
        if (map_zero) {
            asprintf(&map_buf, "0 %ld 1", (long) new_gid);
            gid_map = map_buf;
        }
        update_map(gid_map, map_path);
    }

    if (uid_map != NULL || map_zero) {
        asprintf(&map_path, "/proc/%ld/uid_map", (long) pid);
        if (map_zero) {
            asprintf(&map_buf, "0 %ld 1", (long) new_uid);
            uid_map = map_buf;
        }
        update_map(uid_map, map_path);
    }
}
