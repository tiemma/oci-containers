//Linux library specific definitions
#define _GNU_SOURCE
#define __USE_GNU

// Container constants
#define ROOTFS_PATH "./rootfs"
#define EXTRACT_IMAGE "rm -r ./rootfs; mkdir rootfs; docker export $(docker create alpine) | tar -C rootfs -xvf -; mkdir rootfs/oldroot"

// Command hashes
#define IMAGE 210716097704
#define MOUNT 210720935288
#define UNMOUNT 229485381690395
#define CHROOT 6953391102356
#define SHELL 210727781757
#define VOLUME 6954142767101
#define NAMESPACE 249899181367082546
#define UNSHARE 229485388532699

// Color combinations
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define ENDC "\033[0m"

