#include "./constants.h"

#include <err.h>
#include <stdio.h>
#include <sys/mount.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/syscall.h>

#include "./colors.h"
#include "./mount.h"
#include "./run.h"
#include "./chroot.h"
#include "./namespace.h"


static int
pivot_root(const char *new_root, const char *put_old)
{
    int ret = syscall(SYS_pivot_root, new_root, put_old);
    if ( ret < 0 ){
        printf("Failed to change root from %s to %s", put_old, new_root);
    }
    return ret;
}


const unsigned long hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        char *argument = argv[i];
        printf("%s: %lu\n", argument, hash(argument));
        switch (hash(argument))
        {
        case IMAGE:
            printf("Extracting base image for chroot using Docker and command: {%s}", EXTRACT_IMAGE);
            exec_command(EXTRACT_IMAGE);
            break;
        case MOUNT:
            blue("Mounting directories for chroot {proc sys dev}\n");
            mount_dir_bind("/proc", NULL);
            mount_dir_bind("/sys", NULL);
            mount_dir_bind("/dev", NULL);
            break;
        case UNMOUNT:
            blue("Unmounting directories for chroot {proc sys dev}\n");
            unmount_dir("/proc");
            unmount_dir("/sys");
            unmount_dir("/dev");
            break;
        case CHROOT:
            blue("Chrooting into environment\n");
            chroot_rootfs();
            exec_command("ls");
            exec_command("apk");
            white("Running 'exit' on the next prompt to get out of chroot>>>>");
            exec_command("exit");
            white("\nExiting chroot environments\n");
            exec_command("apk");
            green("\nSike!, You can't exit out of the chroot cause that'd be a jail break and you'd access the main shell of the user\n\n");
            white("");
            break;
        case SHELL:
            blue("Chrooting into shell environment");
            chroot_rootfs();
            white("");
            exec_command("exit");
            exec_command("sh");
            break;
        case VOLUME:
            blue("Demo of volumes in containers");
            exec_command("touch /dev/THIS_IS_A_VOLUME");
            break;
        case NAMESPACE:
            blue("Exec into a shell in a new namespace");

            const char* command = "/bin/bash";
            char opts[6] = "inmpuU";

            exec_command("mkdir /tmp/newroot");
            exec_command("mkdir /tmp/oldroot");
            mount_dir_bind(ROOTFS_PATH, "/tmp/newroot");
            unshare_command(command, opts);
            pivot_root("/tmp/newroot", "/tmp/oldroot");
            break;

        default:
            printf("Invalid argument %s specified", argument);
            break;
        }
    }
    return 0;
}
