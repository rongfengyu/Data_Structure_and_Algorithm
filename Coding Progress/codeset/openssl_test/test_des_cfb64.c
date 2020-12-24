#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <openssl/des.h>

int main(int argc,char **argv)
{
	des_key_schedule ks;
	des_cblock iv;
	int num,enc,textsize;
	FILE *s_fp,*d_fp;
	static char keystr[8];
	unsigned char *inchar,*outchar;
	
	unsigned char *origchar = "abcdefghijklmnopqrstuvwxyz";
	textsize = strlen(origchar);
	
	printf("textsize = %d\n", textsize);
	
	inchar=(unsigned char*)malloc(textsize);
	strncpy(inchar,origchar,(textsize));
	
	outchar=(unsigned char*)malloc(textsize);
	unsigned char *decchar=(unsigned char*)malloc(textsize);
	
	enc=1;

	strncpy(keystr,"nisc1234",sizeof(keystr));
	des_set_key((des_cblock *)keystr,ks);
	
	num=0;
	memset(iv,0,8);
	des_cfb64_encrypt(inchar,outchar,textsize,ks,(des_cblock *)iv,&num,enc);
	
	int i=0;
	for(i=0; i<textsize; i++){
		printf("%02x", inchar[i]);
	}
	printf("\n");
	for(i=0; i<textsize; i++){
		printf("%02x", outchar[i]);
	}
	printf("\n");
	
	enc=0;

	strncpy(keystr,"nisc1234",sizeof(keystr));
	des_set_key((des_cblock *)keystr,ks);
	
	num=0;
	memset(iv,0,8);
	des_cfb64_encrypt(outchar,decchar,textsize,ks,(des_cblock *)iv,&num,enc);
	
	
	for(i=0; i<textsize; i++){
		printf("%02x", decchar[i]);
	}
	printf("\n");
	
}