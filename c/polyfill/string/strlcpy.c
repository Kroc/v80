static unsigned
zstrlcpy(char *buf, const char *zsrc, unsigned bufsiz)
{
    const char *psrc = zsrc;
    char *pdst = buf, *lastbuf = buf + bufsiz - 1;
    while(*psrc && pdst < lastbuf)
        *pdst++ = *psrc++;
    if(bufsiz)
        *pdst = '\0';
    while(*psrc)
        psrc++;
    return psrc - zsrc;
}
#undef strlcpy
#define strlcpy zstrlcpy
