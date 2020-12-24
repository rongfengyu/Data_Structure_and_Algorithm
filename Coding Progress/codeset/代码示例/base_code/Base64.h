/*
* file: Base64.h
* function:  
* author: root
* email: root@30wish.com
* date:2019-03-31
*/
int EncodeBase64(const unsigned char* pSrc, char* pDst, int nSrcLen, int nMaxLineLen);
int DecodeBase64(const char* pSrc, unsigned char* pDst, int nSrcLen);



