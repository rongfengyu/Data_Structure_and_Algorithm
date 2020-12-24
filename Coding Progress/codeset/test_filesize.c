#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <dirent.h>
#include <assert.h>
#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/pkcs12.h>

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
	AES_set_encrypt_key((const unsigned char *)ekey,128,&aes);
	AES_cbc_encrypt(raw_buf,encrpy_buf,len,&aes,eiv,AES_ENCRYPT);
	free(ekey);
	free(eiv);
}

void decrpyt_buf(unsigned char *raw_buf, unsigned char *encrpy_buf, int len ) 
{
	AES_KEY aes;
	unsigned char *ekey = str2hex(CERT_CRYPT_KEY);
	unsigned char *eiv = str2hex(CERT_CRYPT_IV);
	memset(&aes, 0, sizeof(AES_KEY));
	AES_set_decrypt_key((const unsigned char *)ekey,128,&aes);
	AES_cbc_encrypt(raw_buf,encrpy_buf,len,&aes,eiv,AES_DECRYPT);
	free(ekey);
	free(eiv);
}

static int get_file_size(const char *filename)
{
	int fd = 0;
	struct stat istat;
	
	fd = open(filename, O_RDONLY, S_IREAD);
	if( fd < 0 ) {
		printf("get_file_size: Unable to open file\n");
		return -1;
	} 
	
	fstat(fd, &istat);
	close(fd);
	
	printf("file szie = %d bytes\n", istat.st_size);
	
	return istat.st_size;
}

int crypt_file(const char *infile,const char *outfile,int encypt)
{
	int ret = -1;
	int file_size = 0;
	unsigned int len = 0;
	unsigned int pad_size = 0;
	unsigned char pad_buf[16] = {0};
	unsigned char in_buf[1024] = {0};
	unsigned char out_buf[1024] = {0};
	FILE *in,*out;

	if (!infile || !outfile)
	{
        return ret;
    }

	in = fopen(infile,"r+");
	out = fopen(outfile,"w+");

	if(AES_ENCRYPT == encypt)
	{
		file_size = get_file_size(infile);
		if(file_size <= 0)
		{
			return ret;
		}
	
		pad_size = 16 - file_size%16;
		pad_buf[0] = pad_size;
	
		if(pad_buf[0] != 16)
		{
			len = fread(in_buf,1,sizeof(pad_buf) - pad_size,in);
			if(len != (sizeof(pad_buf) - pad_size))
			{
				return -1;
			}
			memcpy(pad_buf + pad_size,in_buf,len);
		}

		//LOG_PRINT("1. encrypt pad_buf[0]=%d\n",pad_buf[0]);
		//hexdump(pad_buf,16);
		encrpyt_buf(pad_buf,out_buf,sizeof(pad_buf));
		//LOG_PRINT("2. encrypt out_buf[0]=%d\n",out_buf[0]);
		//hexdump(out_buf,16);

		memset(pad_buf,0,sizeof(pad_buf));
		decrpyt_buf(out_buf, pad_buf, sizeof(pad_buf));	
		//LOG_PRINT("2.1 decrypt pad_buf[0]=%d len=%d\n",pad_buf[0],sizeof(pad_buf));
		//hexdump(pad_buf,16);
		
		int write_cn = fwrite(out_buf,1,sizeof(pad_buf),out);
		if( write_cn < 0) 
		{
			//LOG_PRINT("write output file error\n");
			return -1;
		}

		memset(in_buf,0,sizeof(in_buf));
		memset(out_buf,0,sizeof(out_buf));
		
		while((len = fread(in_buf,1,sizeof(in_buf),in)) > 0)
		{
			encrpyt_buf(in_buf,out_buf,len);
			fwrite(out_buf,1,len,out);
			
			memset(in_buf,0,sizeof(in_buf));
			memset(out_buf,0,sizeof(out_buf));
		}
	}
	else
	{
		len = fread(pad_buf,1,sizeof(pad_buf),in);
		printf("3. decrypt out_buf[0]=%d len=%d\n",pad_buf[0],len);
		//hexdump(pad_buf,16);
		decrpyt_buf(pad_buf,out_buf,len);
		//LOG_PRINT("4. decrypt out_buf[0]=%d len=%d\n",out_buf[0],len);
		//hexdump(out_buf,16);
		pad_size = out_buf[0];
		if(pad_size > 16)
		{
			return -1;
		}
	
		if(pad_size < 16)
		{
			int write_cn = fwrite(out_buf + pad_size,1,sizeof(pad_buf) - pad_size,out);
			if( write_cn < 0) 
			{
				//LOG_PRINT("write output file error\n");
				return -1;
			}
		}
		
		file_size = get_file_size(infile);
		if(file_size <= 0 || file_size%0x10)
		{
			return -1;
		}
		
		file_size -= pad_size;
		printf("file true size = %d!\n", file_size);
		unsigned char *out_buff = NULL;
		out_buff = malloc(sizeof(unsigned char) * (file_size+1));
		memset(out_buff,0,sizeof(unsigned char) * (file_size+1));
		unsigned int out_len = 0;
		
		if(pad_size < 16)
		{
			memcpy(out_buff + out_len, out_buf + pad_size, sizeof(pad_buf) - pad_size);
			out_len += sizeof(pad_buf) - pad_size;
			printf("file out_len = %d!\n", out_len);
		}
		
		memset(in_buf,0,sizeof(in_buf));
		memset(out_buf,0,sizeof(out_buf));
		
		while((len = fread(in_buf,1,sizeof(in_buf),in)) > 0)
		{
			//ret = decrypt(default_ase_key, in_buf, len, out_buf);
			decrpyt_buf(in_buf,out_buf,len);
			fwrite(out_buf,1,len,out);
			
			memcpy(out_buff + out_len, out_buf, len);
			out_len += len;
			printf("file out_len = %d!\n", out_len);
			
			memset(in_buf,0,sizeof(in_buf));
			memset(out_buf,0,sizeof(out_buf));
		}
		
		FILE *fd = fmemopen(out_buff, file_size, "r"); 
		if(fd == NULL)  
		{  
			printf("get file error!\n");  
			return -1;  
		}
		
		OpenSSL_add_all_algorithms();
		ERR_load_crypto_strings();
	
		PKCS12 *p12= d2i_PKCS12_fp(fd, NULL);      
		if (!p12) {      
			fprintf(stderr, "Error reading PKCS#12 file\n");   
			ERR_print_errors_fp(stderr);  
			return -1;   
		}
		
		//取pkey对象、X509证书、证书链
		EVP_PKEY *pkey=NULL;     
		X509 *x509=NULL;
		STACK_OF(X509) *ca = NULL;
		if (!PKCS12_parse(p12, "30wish", &pkey, &x509, &ca)) {         
			fprintf(stderr, "Error parsing PKCS#12 file\n");       
			ERR_print_errors_fp(stderr);
			return -1;
		}

		PKCS12_free(p12);
		fclose(fd);
		free(out_buff);
		
		
		
		//
		X509_STORE *ca_store = NULL;
		ca_store = X509_STORE_new();
		
		X509_STORE_CTX *csc = NULL;
		csc = X509_STORE_CTX_new();
		
		int ca_num = sk_X509_num(ca);
		printf("X509 ca num = %d\n", ca_num);
		int ca_i = 0;
		for(ca_i=0; ca_i<ca_num; ca_i++)
		{
			X509_STORE_add_cert(ca_store, sk_X509_value(ca, ca_i));
		}
		
		X509_STORE_CTX_init(csc, ca_store, x509, NULL);
		
		//证书校验，包括证书有效期校验在内，X509_cmp_time
		int ret_ver = X509_verify_cert(csc);
		printf("X509 verify ret = %d\n", ret_ver);
		if ( ret_ver != 1 )
		{
			fprintf(stderr, "X509_verify_cert fail, ret = %d, error id = %d, %s\n",
					ret_ver, csc->error, X509_verify_cert_error_string(csc->error));
		}
		X509_STORE_CTX_free(csc);
		X509_STORE_free(ca_store);

		long ver = X509_get_version(x509);
		printf("X509 version = %lu\n", ver);

		ASN1_INTEGER *bs = X509_get_serialNumber(x509);
		long sn=ASN1_INTEGER_get(bs);
		printf("X509 serial number = %lu\n", sn);

		X509_NAME *pSubName = X509_get_subject_name(x509);
		if (!pSubName)
		{
			return -1;
		}
		
		int n = X509_NAME_entry_count (pSubName);
		printf("X509 NAME entry num = %d\n", n);
		
		char	*str = NULL;
		str = X509_NAME_oneline(pSubName,0,0);
		printf("X509 NAME subject oneline = %s\n", str);
		
		
		
		
		
		
		
		
		
		
		
		
	}

	fclose(in);
	fclose(out);
	


	return 0;
}


int main()
{
	//int file_size = 0;
	//file_size = get_file_size("/etc/security/SecHome/private/device.p12");
	//if(file_size%0x10)
	//{
	//	printf("file size error!\n");
	//	return -1;
	//}
	
	crypt_file("/root/device.p12", "/root/decrypt.p12", 0);//decrypt
	
	
		
	return 0;
}