void red(const char *);
void nrm(const char *);
void green(const char *);
void yellow(const char *);
void blue(const char *);
void white(const char *);
void cyan(const char *);
void magento(const char *);

void red(const char *text)
{
    printf("%s%s\n", KRED, text);
}

void nrm(const char *text)
{
    printf("%s%s\n", KNRM, text);
}

void green(const char *text)
{
    printf("%s>>>>>>%s\n", KGRN, text);
}

void yellow(const char *text)
{
    printf("%s%s\n", KYEL, text);
}

void blue(const char *text)
{
    printf("%s%s\n", KBLU, text);
}

void white(const char *text)
{
    printf("%s%s\n", KWHT, text);
}

void cyan(const char *text)
{
    printf("%s%s\n", KCYN, text);
}

void magento(const char *text)
{
    printf("%s%s\n", KMAG, text);
}