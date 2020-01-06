void chroot_rootfs();
void check_chroot_errors();

void chroot_rootfs(const char* dir)
{
    chdir(dir);
    int retval = chroot(".");
    check_chroot_errors();
    white("");
    retval == 0 ? printf("Chrooted into directory %s successfully\n", dir) : perror("Error occured during container");
}


void check_chroot_errors()
{
    int err_val = errno;
    white("\nErrors for container: \n");
    switch (err_val)
    {
    case EACCES:
        red("Search permission is denied on a component of the path prefix.\n");
        break;
    case EIO:
        red("An I/O error occurred.\n");
        break;
    case ELOOP:
        red("Too many symbolic links were encountered in resolving path.\n");
        break;
    case ENOENT:
        red("The file does not exist.\n");
        break;
    case ENOTDIR:
        red("A component of path is not a directory.\n");
        break;
    case EPERM:
        red("You don't have the required priviledges\n");
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
    perror("container");
}