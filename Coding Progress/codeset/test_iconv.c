//gcc -m32 -o test_iconv test_iconv.c -L/usr/local/lib -liconv

#include <iconv.h>
#include <stdio.h>
#include <stdlib.h>

int iconv_code(const char *fromcode, const char *tocode, char *in, char *out)
{
    char *sin, *sout;
    size_t lenin, lenout;
    int ret;
    iconv_t c_pt;

    if ((c_pt = iconv_open(tocode, fromcode)) == (iconv_t) (-1)){
        out = in;
        return -1;
    }

    lenin = strlen(in) + 1;
    char* oldPasserName = malloc(sizeof(char) * lenin);
    strncpy(oldPasserName, in, strlen(in));
    oldPasserName[lenin-1] = '\0';

    lenout = lenin * 10;
    char* newPasserName = malloc(sizeof(char) * lenout);

    sin = oldPasserName;
    sout = newPasserName;
	
    ret = iconv(c_pt, &sin, &lenin, &sout, &lenout);
    if (ret == -1)
    {
        out = in;
        iconv_close(c_pt);
        free(oldPasserName);
        free(newPasserName);
        return -1;
    }
    iconv_close(c_pt);
    strcpy(out, newPasserName);
    free(oldPasserName);
    free(newPasserName);
	
    return 0;
}

int main()
{
	char to[10] ={0};
	
	int ret = iconv_code("UTF-8", "GBK", "防火墙", to);
	printf("ret = %d\n", ret);
	
	printf("to = %s\n", to);
	
	return 0;
}