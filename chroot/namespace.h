void unshare_command(const char*, char[]);

void unshare_command(const char* command, char opts[])
{
    int flags = 0;

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
            yellow("Added namespace flag");
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
        default:
            printf("\nInvalid flag supplied: %d", opts[i]);
            break;
        }
    }

    int ret = unshare(flags);
    printf("Running the unshare command, return status: %d, command %s\n", ret, command);
    if ( ret == -1)
        perror("unshare");

//    exec_command(command);
}