#include <stdio.h>
#include <sys/mount.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#define _GNU_SOURCE
#define __USE_GNU

#include <sched.h>

#include "./constants.h"
#include "./colors.h"
#include "./mount.h"
#include "./image.h"
#include "./chroot.h"
#include "./namespace.h"



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
            mount_dir_bind("/proc");
            mount_dir_bind("/sys");
            mount_dir_bind("/dev");
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
        default:
            printf("Invalid argument %s specified", argument);
            break;
        }
    }
    return 0;
}
