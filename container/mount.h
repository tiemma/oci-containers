

void mount_dir_bind(const char *, char*);
void generate_path(char[]);
void check_mount_errors();
void unmount_dir(const char *);

void check_mount_errors()
{
    int err_val = errno;
    white("\nErrors for mounts: \n");
    switch (err_val)
    {
    case ENOENT:
        red("The specified path doesn't exist\n");
        break;
    case EPERM:
        red("You don't have the required priviledges\n");
        break;
    case EINVAL:
        red("Invalid configuration specified\n");
        red("Mount target is probably in use or is not a mountable path\n");
        break;
    case EACCES:
        red("The specified configuration has no access\n");
        break;
    case EBUSY:
        red("Mountpoint target  is busy, debug!\n");
        break;
    case 0:
        yellow("No errors happened, talk about a lucky run!\n");
        break;
    default:
        red("Unknown error, debug!!!\n");
        printf("Error value: %d\n", err_val);
        fflush(stdout);
        break;
    }
    perror("Mount");
}


void mount_dir_bind(const char *path, char *mount_path)
{
    char fullpath[80];

    if (!mount_path) {
        strcpy(fullpath, ROOTFS_PATH);
        strcat(fullpath, path);
    } else {
        strcpy(fullpath, mount_path);

    }

    printf("Mounting directory %s on %s\n", path, fullpath);

    const char *opts = "mode=0700,uid=65534";
    int retval = mount(path, fullpath, NULL, MS_REC | MS_BIND | MS_PRIVATE, opts); // filesystem & data are ignored for MS_BIND
    check_mount_errors();
    white("");
    retval == 0 ? printf("Mounted directory %s on %s successfully\n", path, fullpath) : perror("Error occured during mount");
}


void unmount_dir(const char *path)
{
    char fullpath[80];
    strcpy(fullpath, ROOTFS_PATH);
    strcat(fullpath, path);
    int retval = umount2(fullpath, MNT_FORCE | MNT_DETACH);
    check_mount_errors();
    white("");
    retval == 0 ? printf("Unmounted directory %s on %s successfully\n", path, fullpath) : perror("Error occured during unmount");
}