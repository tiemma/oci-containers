

void mount_dir_bind(const char *);
void generate_path(char[]);
void check_mount_errors(const char *, const char *);
void unmount_dir(const char *);

void check_mount_errors(const char *path, const char *fullpath)
{
    int err_val = errno;
    switch (err_val)
    {
    case ENOENT:
        red("The specified path doesn't exist\n\n");
        return;
    case EPERM:
        red("You don't have the required priviledges\n\n");
        return;
    case EINVAL:
        red("Invalid configuration specified\n");
        red("Mount target is probably in use or is not a mountable path\n\n");
        return;
    case EACCES:
        red("The specified configuration has no access\n\n");
        return;
    case EBUSY:
        red("Mountpoint target  is busy, debug!\n\n");
        return;
    case 0:
        yellow("No errors happened, talk about a lucky run!\n");
        return;
    default:
        red("Unknown error, debug!!!\n\n");
        return;
    }
}

void mount_dir_bind(const char *path)
{
    char fullpath[80];
    strcpy(fullpath, ROOTFS_PATH);
    strcat(fullpath, path);
    printf("Mounting directory %s on %s\n", path, fullpath);

    const char *opts = "mode=0700,uid=65534";
    int retval = mount(path, fullpath, NULL, MS_REC | MS_BIND, opts); // filesystem & data are ignored for MS_BIND
    check_mount_errors(path, fullpath);
    white("");
    retval == 0 ? printf("Mounted directory %s on %s successfully\n\n", path, fullpath) : perror("Error occured during mount");
}

void unmount_dir(const char *path)
{
    char fullpath[80];
    strcpy(fullpath, ROOTFS_PATH);
    strcat(fullpath, path);
    int retval = umount2(fullpath, MNT_FORCE | MNT_DETACH);
    check_mount_errors(path, fullpath);
    white("");
    retval == 0 ? printf("Unmounted directory %s on %s successfully\n\n", path, fullpath) : perror("Error occured during unmount");
}