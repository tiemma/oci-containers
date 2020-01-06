void exec_command(const char *);

void exec_command(const char *command)
{
    white("");
    printf("\nExecuting command: %s\nOutput:\n", command);
    FILE *fp;
    char line[130];
    fp = popen(command, "r");

    if (fp == NULL)
    {
        perror("Error executing command");
    }
    while (fgets(line, sizeof line, fp))
    {
        blue(line);
    }
    pclose(fp);
    perror("Exec command");
    printf("\n");
}