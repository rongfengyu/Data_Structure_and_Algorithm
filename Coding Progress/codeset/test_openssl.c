#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>  
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/safestack.h>
#include <openssl/pkcs12.h>
#include <openssl/x509v3.h>
#include <openssl/bn.h>
#include <openssl/asn1.h>
#include <openssl/x509_vfy.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/objects.h>

//make CROSS_COMPILE=mips64el-loongson3a-n32-linux-gnu- PREFIX=/home/KIDE/nfsroot
//mips64el-loongson3a-linux-gnu-gcc -I/home/KIDE/nfsroot/usr/include -fpic -L/home/KIDE/nfsroot/lib64 -lssl -lcrypto -o test_opnessl test_opnessl.c

/*
有效期：证书的整体有效起止时间
导入证书时，校验证书是否合法，直接使用库函数进行校验，不用分离字符串再比较

主体subject：OU——CF卡编号，CN——管理口MAC地址

X509_get_subject_name
X509_NAME_oneline
X509_NAME_get_text_by_NID

SAN扩展信息：特征库与其有效起止时间




有效期
openssl库函数X509_verify_cert会自动验证证书是否在有效时间内


openssl库函数X509_cmp_time比较时间，ASN1_TIME与time_t
X509_cmp_time(const ASN1_TIME *ctm, time_t *cmp_time)
*/

typedef struct san_detail_st
{
	char libname[10];
	int lib_start_time;
	int lib_end_time;
} san_detail;

typedef struct cert_san_st
{
	int size;
	san_detail san_d[10];
} cert_san;

typedef struct _FeatureDBInfo
{
	char name[64];
	char license_start_time[32];
	char license_end_time[32];
	char now_version[64];
	char last_version[64];
	char des[256];
}FeatureDBInfo;

BIO *bio_err=NULL;

int bio_test()
{
	/*
	创建并返回一个底层描写叙述符为fd，关闭标志为close_flag的文件描写叙述符类型的BIO。
	事实上，该函数依次调用了BIO_s_fd、BIO_new和BIO_set_fd完毕了该功能。该函数假设调
	用失败返回NULL
	*/
	BIO *out=NULL;

	out = BIO_new_fd(fileno(stdout), BIO_NOCLOSE);

	BIO_printf(out, "Hello World\n");

	BIO_free(out);
	
	return 0;
}

static int print_time(X509* xs)
{
	//形如20090101000000Z的为Generalized Time
	//形如090101000000Z的为UTC Time
	//GENERALIZEDTIME is similar to UTCTIME except the year is represented as YYYY.
	//使用openssl命令制作证书时，参数 startdata 或者 enddata，使用Generalized Time格式输入时间，有利于时间格式转换与比较
	//（其实，两种格式都支持,YYMMDDHHMMSSZ or YYYYMMDDHHMMSSZ）
	ASN1_GENERALIZEDTIME *tm = NULL;
	tm = X509_get_notBefore(xs);

	if (tm == NULL)
	{
		return -1;
	}
	
	int i = 0;
	
	const char *v = NULL;
	v=(const char *)tm->data;
	
	printf("ASN1_GENERALIZEDTIME: ");
	for(i = 0; i < tm->length; i++)
		printf("%c", v[i]);
	
	printf("\n");
	
}

static int checkValidDate(X509* xs)
{
	int ret;
	ret=X509_cmp_time(X509_get_notBefore(xs), NULL);

	if (ret == 0)
	{
		return X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD;
	}
	if (ret > 0)
	{
		return X509_V_ERR_CERT_NOT_YET_VALID;
	}

	ret=X509_cmp_time(X509_get_notAfter(xs), NULL);
	if (ret == 0)
	{
		return X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD;
	}

	if (ret < 0)
	{
		return X509_V_ERR_CERT_HAS_EXPIRED;
	}
	return X509_V_OK;
}

//得到证书的SAN扩展信息
int parse_san(STACK_OF(X509_EXTENSION) *exts, char *san_des)
{
#define EXTNAME_LEN 20

	int num_of_exts = 0;
	if (exts) 
	{       
		num_of_exts = sk_X509_EXTENSION_num(exts);
	}	
	printf("num_of_exts = %d\n", num_of_exts);
	
	int i = 0;
	for (i=0; i < num_of_exts; i++) 
	{
		X509_EXTENSION *ex = sk_X509_EXTENSION_value(exts, i);
		//IFNULL_FAIL(ex, "unable to extract extension from stack");
		ASN1_OBJECT *obj = X509_EXTENSION_get_object(ex);
		//IFNULL_FAIL(obj, "unable to extract ASN1 object from extension");
	 
		BIO *ext_bio = BIO_new(BIO_s_mem());
		//IFNULL_FAIL(ext_bio, "unable to allocate memory for extension value BIO");
		if (!X509V3_EXT_print(ext_bio, ex, 0, 0)) {
			M_ASN1_OCTET_STRING_print(ext_bio, ex->value);
		}
	 
		BUF_MEM *bptr;
		BIO_get_mem_ptr(ext_bio, &bptr);
		BIO_set_close(ext_bio, BIO_NOCLOSE);
	 
		// remove newlines
		int lastchar = bptr->length;
		if (lastchar > 1 && (bptr->data[lastchar-1] == '\n' || bptr->data[lastchar-1] == '\r')) {
			bptr->data[lastchar-1] = (char) 0;
		}
		if (lastchar > 0 && (bptr->data[lastchar] == '\n' || bptr->data[lastchar] == '\r')) {
			bptr->data[lastchar] = (char) 0;
		}
	 
		BIO_free(ext_bio);
	 
		unsigned nid = OBJ_obj2nid(obj);	
		if (nid == NID_undef) {
			// no lookup found for the provided OID so nid came back as undefined.
			char extname[EXTNAME_LEN];
			OBJ_obj2txt(extname, EXTNAME_LEN, (const ASN1_OBJECT *) obj, 1);
			printf("extension name is %s\n", extname);
		} else {
			// the OID translated to a NID which implies that the OID has a known sn/ln
			const char *c_ext_name = OBJ_nid2ln(nid);
			//IFNULL_FAIL(c_ext_name, "invalid X509v3 extension name");
			printf("extension name is %s\n", c_ext_name);
		}
		
		printf("extension length is %u\n", bptr->length);
		printf("extension value is %s\n", bptr->data);//注意文件EOF的影响

		strncpy(san_des, bptr->data, bptr->length);//注意文件EOF的影响
		printf("extension value x is %s\n", san_des);
		printf("extension value x is 138|%x|,139|%x|,140|%x|,141|%x|\n", san_des[138],san_des[139],san_des[140],san_des[141]);
	}
	
	return 0;
}

static int getCertExpirationInfo(char* cert_des, FeatureDBInfo* info)
{
	int i = 0;//解析出的lib个数
	char seps[] = "+";
    char *token;
	char subs[32][64] = {0};
	if (strlen(cert_des) > 6)
	{
		token = strtok(&cert_des[6], seps);
		while( token != NULL )
		{ 
			memcpy(subs[i++], token, strlen(token));
			//printf("[%d]subs=<%s>\n", i, subs[i-1]);
			token = strtok( NULL, seps );
		}	

		int j = 0;
		for(;j<i;j++)
		{
			char tmp_buf[2][32] = {0}; 
		    //printf("sub=%s\n",subs[j]);
			sscanf(subs[j],"%[^@]%*c%[^-]%*c%s",info[j].name,tmp_buf[0],tmp_buf[1]);

			memcpy(info[j].license_start_time,tmp_buf[0],8);
			memcpy(info[j].license_end_time,tmp_buf[1],8);
		}
	}
	
	return i;
}

char * PKCS12_get_object(char* certFile, char* pwd)
{
	OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
	//取PKCS12对象
	FILE* fp;
	if (!(fp = fopen(certFile, "rb"))) 
	{ 
		fprintf(stderr, "Error opening file %s\n", certFile);        
		return NULL;     
	}

	PKCS12 *p12= d2i_PKCS12_fp(fp, NULL);  
	fclose (fp);    
	if (!p12) {      
		fprintf(stderr, "Error reading PKCS#12 file\n");   
		ERR_print_errors_fp(stderr);  
		return NULL;   
	} 

	//取pkey对象、X509证书、证书链
	EVP_PKEY *pkey=NULL;     
	X509 *x509=NULL;
	STACK_OF(X509) *ca = NULL;
	if (!PKCS12_parse(p12, pwd, &pkey, &x509, &ca)) {         
		fprintf(stderr, "Error parsing PKCS#12 file\n");       
		ERR_print_errors_fp(stderr);
		return NULL;
	}

	PKCS12_free(p12);
	
	//验证是否证书是不是自签
	int is_self_signed = 0;
	if(X509_check_issued(x509, x509) == X509_V_OK)
	{
		is_self_signed = 1;
	}
	printf("is_self_signed = %d\n", is_self_signed);
	
	
	//dump_file
	//FILE *fp;
	const char *file_name = "./dump_file";
	if (!(fp = fopen(file_name, "w"))) {
        fprintf(stderr, "Error opening file %s\n", file_name);
        exit(1);
    }
    if (pkey) {
        fprintf(fp, "***Private Key***\n");
        PEM_write_PrivateKey(fp, pkey, NULL, NULL, 0, NULL, NULL);
    }
    if (x509) {
        fprintf(fp, "***User Certificate***\n");
        PEM_write_X509_AUX(fp, x509);
    }
    if (ca && sk_X509_num(ca)) {
        fprintf(fp, "***Other Certificates***\n");
		int tmp_i =0;
        for (tmp_i = 0; tmp_i < sk_X509_num(ca); tmp_i++) 
            PEM_write_X509_AUX(fp, sk_X509_value(ca, tmp_i));
    }
	fclose(fp);
		
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
		return NULL;
	}
	
	int n = X509_NAME_entry_count (pSubName);
	printf("X509 NAME entry num = %d\n", n);
	
	char	*str = NULL;
	str = X509_NAME_oneline(pSubName,0,0);
	printf("X509 NAME subject oneline = %s\n", str);
	
	char	buff[256];
	memset(buff,0,sizeof(buff));
	char	*pch = NULL;
	if( (pch = strstr(str,"CN=")) == NULL)
		strncpy(buff,str,17);//MAC
	else
	{
		pch +=3;
		strncpy(buff,pch,17);
		pch=NULL;
	}
	
	printf("X509 NAME oneline = %s\n", buff);

	//valid date
	int ret = checkValidDate(x509);
	printf("X509 checkValidDate ret = %d\n", ret);
	
	//print_time
	ret = print_time(x509);
	printf("X509 print_time ret = %d\n", ret);
	
	//BIO print cert info  将整个证书内容打印到stdout
	//bio_err = BIO_new_fd(fileno(stdout), BIO_NOCLOSE);
	//X509_print(bio_err,x509);
	
	//SAN扩展信息：特征库与其有效起止时间
	int num_of_exts = 0;
	STACK_OF(X509_EXTENSION) *exts = x509->cert_info->extensions;//获取全部扩展个数	
	/////
	char san_des[1024] = {0};
	FeatureDBInfo lib_db[16];
	parse_san(exts, san_des);
	int lib_cnt = getCertExpirationInfo(san_des, lib_db);
	printf("X509 san lib num = %d\n", lib_cnt);
	
	int lib_cnt_i=0;
	for(lib_cnt_i=0; lib_cnt_i<lib_cnt; lib_cnt_i++)
	{
		printf("X509 san lib[%d] libname: %s, start_time:%s, end_time:%s\n", lib_cnt_i, lib_db[lib_cnt_i].name, lib_db[lib_cnt_i].license_start_time, lib_db[lib_cnt_i].license_end_time);
	}
	
	//释放
	sk_X509_pop_free(ca, X509_free);
    X509_free(x509);
    EVP_PKEY_free(pkey);

	return NULL;
}



int main()
{
	bio_test();
	
	char *p12file="/root/device.p12";
	PKCS12_get_object(p12file, "30wish");
	
	return 0;
}

