#include <openssl/aes.h>
#include <openssl/evp.h>
#include <iostream>
#include <string.h>

using namespace std;


//g++ -o test_cfb128 test_cfb128.c -lssl -lcrypto


#define CERT_CRYPT_KEY				"30c72b05705d5c46f412af8cbed55a30"	
#define CERT_CRYPT_IV				"667b02a85c61c786def4521b060265e8"	

unsigned char* str2hex(char *str) 
{
	unsigned char *ret = NULL;
	int str_len = strlen(str);
	int i = 0;
	//assert((str_len%2) == 0);
	ret = (unsigned char *)malloc(str_len/2);
	for (i =0;i < str_len; i = i+2 ) 
	{
		sscanf(str+i,"%2hhx",&ret[i/2]);
	}
	return ret;
}

void encrpyt_buf(unsigned char *raw_buf, unsigned char *encrpy_buf, int len )
{
	AES_KEY aes;
	unsigned char *ekey = str2hex(CERT_CRYPT_KEY);
	unsigned char *eiv = str2hex(CERT_CRYPT_IV);
	memset(&aes, 0, sizeof(AES_KEY));
	AES_set_encrypt_key((const unsigned char *)ekey,128,&aes);//加解密都是用的这个函数

	int num = 0;
	AES_cfb128_encrypt(raw_buf, encrpy_buf, len, &aes, eiv, &num, AES_ENCRYPT);
	
	free(ekey);
	free(eiv);
}

void decrpyt_buf(unsigned char *raw_buf, unsigned char *encrpy_buf, int len ) 
{
	AES_KEY aes;
	unsigned char *ekey = str2hex(CERT_CRYPT_KEY);
	unsigned char *eiv = str2hex(CERT_CRYPT_IV);
	memset(&aes, 0, sizeof(AES_KEY));
	AES_set_encrypt_key((const unsigned char *)ekey,128,&aes);//加解密都是用的这个函数

	int num = 0;
	AES_cfb128_encrypt(raw_buf, encrpy_buf, len, &aes, eiv, &num, AES_DECRYPT);
	
	free(ekey);
	free(eiv);
}


int main()
{

    int bytes_read;
    unsigned char indata[1024] = {0};
    unsigned char outdata[1024] = {0};
    unsigned char decryptdata[1024] = {0};
	
	strcpy((char *)indata, "1234567812345678abcdefghijklmnopq");
	
	
	//encrpyt_buf
	bytes_read = strlen((char *)indata);
	cout << "bytes_read:" << bytes_read << endl;

	encrpyt_buf(indata, outdata, bytes_read);
	
	int i =0;
	for(i=0; i<bytes_read; i++){
		printf("%x ", indata[i]);
	}
	cout << endl;
	
	i =0;
	for(i=0; i<bytes_read; i++){
		printf("%x ", outdata[i]);
	}
	cout << endl;
	
	//decrpyt_buf

	decrpyt_buf(outdata, decryptdata, bytes_read);
	for(i=0; i<bytes_read; i++){
		printf("%x ", decryptdata[i]);
	}
	cout << endl;

    return 0;
}
