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
    printf(ENDC);
}

void nrm(const char *text)
{
    printf("\n%s%s\n", KNRM, text);
    printf(ENDC);
}

void green(const char *text)
{
    printf("\n%s>>>>>>%s\n", KGRN, text);
    printf(ENDC);
}

void yellow(const char *text)
{
    printf("\n%s%s\n", KYEL, text);
    printf(ENDC);
}

void blue(const char *text)
{
    printf("\n%s%s\n", KBLU, text);
    printf(ENDC);
}

void white(const char *text)
{
    printf("\n%s%s\n", KWHT, text);
    printf(ENDC);
}

void cyan(const char *text)
{
    printf("\n%s%s\n", KCYN, text);
    printf(ENDC);
}

void magento(const char *text)
{
    printf("\n%s%s\n", KMAG, text);
    printf(ENDC);
}