#include "./constants.h"

#include <stdio.h>
#include <sys/mount.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>
#include <fcntl.h>

#include "./colors.h"
#include "./mount.h"
#include "./run.h"
#include "./chroot.h"
#include "./namespace.h"


/*
 * Compute the hash of the string so we can pass in string arguments
 */
static const unsigned long hash(const char *str)
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
        switch (hash(argument)) {

        case IMAGE:
            printf("Extracting base image for container using Docker and command: {%s}", EXTRACT_IMAGE);
            exec_command(EXTRACT_IMAGE);
            break;

        case MOUNT:
            blue("Mounting directories for container {proc sys dev}\n");
            mount_dir_bind("/proc", NULL);
            mount_dir_bind("/sys", NULL);
            mount_dir_bind("/dev", NULL);
            break;

        case UNMOUNT:
            blue("Unmounting directories for container {proc sys dev}\n");
            unmount_dir("/proc");
            unmount_dir("/sys");
            unmount_dir("/dev");
            break;

        case CHROOT:
            blue("Chrooting into environment\n");
            chroot_rootfs(ROOTFS_PATH);
            exec_command("ls");
            exec_command("apk");
            white("Running 'exit' on the next prompt to get out of container>>>>");
            exec_command("exit");
            white("\nExiting container environments\n");
            exec_command("apk");
            green("\nSike!, You can't exit out of the container cause that'd be a jail break and you'd access the main shell of the user\n");
            white("");
            break;

        case SHELL:
            blue("Chrooting into shell environment");
            chroot_rootfs(ROOTFS_PATH);
            white("");
            exec_command("exit");
            exec_command("sh");
            break;

        case VOLUME:
            blue("Demo of volumes in containers");
            exec_command("touch /dev/THIS_IS_A_VOLUME");
            break;

        case UNSHARE:
            blue("Setting up namespaces");

            // We need to make the current root directory mount private
            // This is because pivot_root will not work on a shared(MNT_SHARE) filesystem
            yellow("Mounting root directory under a private propagation subtree");
            mount("none", "/", NULL, MS_REC | MS_PRIVATE, NULL);

            check_mount_errors();

            char opts[9] = "inmuc";
            unshare_command(opts);
            break;

        case NAMESPACE:
            blue("Exec into a shell in a new container filesystem and namespace");

            // Command to mount proc and setup the init process
            const char* command = "mount -t proc proc /proc && sh";

            // Creating the newroot directory for the pivot_root
            blue("Creating directories of newroot and oldroot");
            mode_t modes = S_IRWXU | S_IRWXO;
            exec_command("rm -r /newroot");
            mkdir("/newroot", modes);
            perror("mkdir");


            // Mounting the tmpfs filesystem for the pivot_root
            // Creatign a tmpfs filesystem to ensure size limits on disk
            blue("Mounting tmpfs filesystem for pivot_root");
            mount("tmpfs", "/newroot", "tmpfs", 0, "size=500M");
            check_mount_errors();

            // Copy the filesystem to the newroot filesystem
            blue("Copying files to newroot filesystem");
            chdir(ROOTFS_PATH);
            exec_command("find . -depth -xdev -print | cpio -pd --quiet /newroot");

            // Run the pivot_root syscall
            blue("Executing the pivot_root on newroot");
            chdir("/newroot");
            pivot_root(".", "oldroot");

            // Chroot after the pivot_root
            chroot_rootfs("/");

            // Unnmount and delete the oldroot to get rid of the previous mountspace
            blue("Unmounting oldroot");
            umount2("/oldroot", MNT_DETACH);
            rmdir("/oldroot");
            perror("unmount");

            // We set the PID namespace last so we don't interfere
            // with commands that need to run on the host namespace
            blue("Setting up PID namespaces");
            unshare_command("p");
            perror("unshare_pid");

//            blue("Setting user namesspace");
//            unshare_command("U");
//            set_user_namespace();
//            perror("unshare_user");

            printf("\nExecuting command %s", command);
            // We finish our container setup by mounting proc and opening a shell session
            system(command);


            break;

        default:
            printf("Invalid argument %s specified", argument);
            break;
        }
    }
    return 0;
}
