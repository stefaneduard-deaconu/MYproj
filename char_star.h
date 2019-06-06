#ifndef CHAR_STAR_H_INCLUDED
#define CHAR_STAR_H_INCLUDED

int leng(char *a)
{
    int i = 0;
    while (a[i])
        i++;
    return i;
}
void cpy(char *a, char *b)
{
    int i = 0;
    do
        a[i] = b[i];
    while (b[i++] != 0);
}
bool ncmp(char *a, char *b, int n)
{
    int i = 0;
    while (i < n && a[i] == b[i])
        i++;
    if (i == n)
        return true;
    return false;
}
int cmp(char *a, char *b)
{
    int i = 0;
    while (a[i] == b[i] && a[i])
        i++;
    if (a[i] == b[i])
        return 0;
    if (a[i] > b[i])
        return 1;
    return -1;
}

#endif // CHAR_STAR_H_INCLUDED
