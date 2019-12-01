void chroot_rootfs();
void check_chroot_errors();

void chroot_rootfs()
{
    chdir(ROOTFS_PATH);
    int retval = chroot(".");
    check_chroot_errors();
    white("");
    retval == 0 ? printf("Chrooted into directory %s successfully\n\n", ROOTFS_PATH) : perror("Error occured during chroot");
}

void check_chroot_errors()
{
    int err_val = errno;
    switch (err_val)
    {
    case EACCES:
        red("Search permission is denied on a component of the path prefix.\n\n");
        return;
    case EIO:
        red("An I/O error occurred.\n\n");
        return;
    case ELOOP:
        red("Too many symbolic links were encountered in resolving path.\n\n");
        return;
    case ENOENT:
        red("The file does not exist.\n\n");
        return;
    case ENOTDIR:
        red("A component of path is not a directory.\n\n");
        return;
    case EPERM:
        red("You don't have the required priviledges\n\n");
        return;
    case 0:
        yellow("No errors happened, talk about a lucky run!\n");
        return;
    default:
        red("Unknown error, debug!!!\n\n");
        return;
    }
}