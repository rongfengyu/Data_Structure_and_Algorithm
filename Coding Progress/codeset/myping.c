/*     
*    Name: myping 
*     Desc: ping��������Ŀ����������ICMP����,���鱾��������Զ�̵�Ŀ�������Ƿ����� 
*     From: κ��@CSDN https://blog.csdn.net/weibo1230123/article/details/79891018
*  
*/     /*ICMP����ʹ��ԭʼ�׽��ֽ������,Ҫ�ֶ�����IP��ͷ����ICMP��ͷ������У��*/  
/***********������********************************************* myping.c*/  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <netinet/ip.h>  
#include <netinet/ip_icmp.h>  
#include <unistd.h>  
#include <signal.h>  
#include <arpa/inet.h>  
#include <errno.h>  
#include <sys/time.h>  
#include <stdio.h>  
#include <string.h> /* bzero */  
#include <netdb.h>  
#include <pthread.h>  
//���淢�Ͱ���״ֵ̬  
typedef struct pingm_pakcet{  
    struct timeval tv_begin;     //����ʱ��  
    struct timeval tv_end;       //���յ���ʱ��  
    short seq;                   //���к�  
    int flag;          //1����ʾ�Ѿ����͵���û�н��յ���Ӧ��0����ʾ���յ���Ӧ  
}pingm_pakcet;  
static pingm_pakcet *icmp_findpacket(int seq);  
static unsigned short icmp_cksum(unsigned char *data, int len);  
static struct timeval icmp_tvsub(struct timeval end, struct timeval begin);  
static void icmp_statistics(void);  
static void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv,int length);  
static int icmp_unpack(char *buf,int len);  
static void *icmp_recv(void *argv);  
static void icmp_sigint(int signo);  
static void icmp_usage();    
static pingm_pakcet pingpacket[128];  
#define K 1024  
#define BUFFERSIZE 72                            //���ͻ������Ĵ�С  
static unsigned char send_buff[BUFFERSIZE];        
static unsigned char recv_buff[2*K];             //��ֹ������������ô�һЩ  
static struct sockaddr_in dest;                  //Ŀ�ĵ�ַ  
static int rawsock = 0;                          //���ͺͽ����߳���Ҫ��socket������  
static pid_t pid;                                //����PID  
static int alive = 0;                            //�Ƿ���յ��˳��ź�  
static short packet_send = 0;                    //�Ѿ����͵����ݰ�����  
static short packet_recv = 0;                    //�Ѿ����յ����ݰ�����  
static char dest_str[80];                        //Ŀ�������ַ���  
static struct timeval tv_begin, tv_end, tv_interval;  
 
//2.���㷢�ͺͽ��յ�ʱ��
static void icmp_usage()  
{  
    //ping��IP��ַ��������  
    printf("ping aaa.bbb.ccc.ddd\n");  
}  
/*�ն��źŴ�����SIGINT*/  
static void icmp_sigint(int signo)  
{  
    alive = 0;  
    gettimeofday(&tv_end,NULL);  
    tv_interval = icmp_tvsub(tv_end, tv_begin);    
    return;  
} 
 
//3.ͳ�����ݽ��
/*ͳ�����ݽ������****************************************** 
��ӡȫ��ICMP���͵Ľ���ͳ�ƽ��*/  
static void icmp_statistics(void)  
 {  
   long time = (tv_interval.tv_sec * 1000) + (tv_interval.tv_usec/1000);  
   printf("--- %s ping statistics ---\n", dest_str);  
   printf("%d packets transmitted, %d received, %d%c packet loss, time %ld ms\n",   
       packet_send,packet_recv,(packet_send-packet_recv)*100/packet_send,'%',time);  
}  
 /*************���������еı�ʶ����*********************** 
 ���Һ��ʵİ���λ�� 
��seqΪ1ʱ����ʾ���ҿհ� 
����ֵ��ʾ����seq��Ӧ�İ�*/  
 static pingm_pakcet *icmp_findpacket(int seq)  
 {  
    int i;  
    pingm_pakcet *found = NULL;  
    //���Ұ���λ��  
    if(seq == -1){  
       for(i=0;i<128;i++){  
            if(pingpacket[i].flag == 0){  
              found = &pingpacket[i];  
               break;  
            }  
        }  
    }  
    else if(seq >= 0){  
        for(i =0 ;i< 128;i++){  
            if(pingpacket[i].seq == seq){  
                found = &pingpacket[i];  
                break;  
            }  
        }  
    }  
    return found;  
 } 
 
//4.У��ͺ���
/*************У��ͺ���***************************** 
TCP/IPЭ��ջʹ�õ�У���㷨�ǱȽϾ����,��16λ�����ݽ����ۼӼ���,�����ؼ�����, 
 
CRC16У��ͼ���icmp_cksum 
����: 
    data:���� 
       len:���ݳ��� 
����ֵ: 
    ������,short���� 
*/  
static unsigned short icmp_cksum(unsigned char *data, int len)  
{  
    int sum = 0;   //������  
    int odd = len & 0x01;  //�Ƿ�Ϊ����  
    /*�����ݰ���2�ֽ�Ϊ��λ�ۼ�����*/  
    while(len & 0xfffe){  
        sum += *(unsigned short*)data;  
        data += 2;  
        len -= 2;  
    }  
    /*�ж��Ƿ�Ϊ����������,��ICMP��ͷΪ�������ֽ�,��ʣ�����һ���ֽ�*/  
    if(odd){  
        unsigned short tmp = ((*data)<<8)&0xff00;  
       sum += tmp;  
    }  
    sum = (sum >> 16) + (sum & 0xffff);   //�ߵ�λ���  
    sum += (sum >> 16);                    //�����λ����  
  
    return ~sum;                           //����ȡ��ֵ  
} 
 
//5.ICMPͷ��У�����Ͳ��
/**********����ICMPͷ��У��********************/  
//����ICMP��ͷ  
static void icmp_pack(struct icmp *icmph, int seq, struct timeval *tv, int length)  
{  
    unsigned char i = 0;  
    //���ñ�ͷ  
    icmph->icmp_type = ICMP_ECHO;   //ICMP��������  
    icmph->icmp_code = 0;           //code��ֵΪ0  
    icmph->icmp_cksum = 0;          //�Ƚ�cksum��ֵ��Ϊ0�������Ժ��cksum����  
    icmph->icmp_seq = seq;          //���������к�  
    icmph->icmp_id = pid & 0xffff;  //��дPID  
    for(i=0; i< length; i++)  
        icmph->icmp_data[i] = i;   //����У���  
    icmph->icmp_cksum = icmp_cksum((unsigned char*)icmph, length);  
}  
  
/*��ѹ���յ��İ�������ӡ��Ϣ*/  
static int icmp_unpack(char *buf, int len)  
{  
    int i,iphdrlen;  
    struct ip *ip = NULL;  
    struct icmp *icmp = NULL;  
    int rtt;  
  
    ip = (struct ip *)buf;            //IP��ͷ  
    iphdrlen = ip->ip_hl * 4;         //IPͷ������  
    icmp = (struct icmp *)(buf+iphdrlen);  //ICMP�εĵ�ַ  
    len -= iphdrlen;  
    //�жϳ����Ƿ�ΪICMP��  
    if(len < 8){  
        printf("ICMP packets\'s length is less than 8\n");  
        return -1;  
    }  
    //ICMP����ΪICMP_ECHOREPLY����Ϊ�����̵�PID  
    if((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == pid)){  
        struct timeval tv_interval,tv_recv,tv_send;  
        //�ڷ��ͱ���в����Ѿ����͵İ�������seq  
        pingm_pakcet *packet = icmp_findpacket(icmp->icmp_seq);  
        if(packet == NULL)  
            return -1;  
        packet->flag = 0;          //ȡ����־  
        tv_send = packet->tv_begin;  //��ȡ�����ķ���ʱ��  
  
        gettimeofday(&tv_recv,NULL);  //��ȡ��ʱ�䣬����ʱ���  
        tv_interval = icmp_tvsub(tv_recv,tv_send);  
       rtt = tv_interval.tv_sec * 1000 + tv_interval.tv_usec/1000;  
        /*��ӡ������� 
          ICMP�εĳ��� 
          ԴIP��ַ 
          �������к� 
          TTL 
          ʱ��� 
        */  
        printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%d ms\n",   
            len,inet_ntoa(ip->ip_src),icmp->icmp_seq,ip->ip_ttl,rtt);  
        packet_recv ++;              //���հ�������1  
    }  
    else {  
        return -1;  
    }  
}
  
//6.����ʱ����
/************����ʱ���time_sub************************ 
������ 
    end:���յ�ʱ�� 
    begin:��ʼ���͵�ʱ�� 
����ֵ: 
    ʹ�õ�ʱ�� 
*/  
static struct timeval icmp_tvsub(struct timeval end, struct timeval begin)  
{  
    struct timeval tv;  
    //�����ֵ  
    tv.tv_sec = end.tv_sec - begin.tv_sec;  
    tv.tv_usec = end.tv_usec - begin.tv_usec;  
    //������յ�ʱ���usecֵС�ڷ���ʱ��usec,��uesc���λ  
    if(tv.tv_usec < 0){  
        tv.tv_sec --;  
        tv.tv_usec += 1000000;  
    }  
  
    return tv;  
} 
 
//7.���ͱ��ĺ���
//**********���ͱ���***************************  
static void *icmp_send(void *argv)  
{  
    //�������ʼ�������ݵ�ʱ��  
    gettimeofday(&tv_begin, NULL);  
    while(alive){  
        int size = 0;  
        struct timeval tv;  
        gettimeofday(&tv, NULL);     //��ǰ���ķ���ʱ��  
        //�ڷ��Ͱ�״̬�������ҵ�һ������λ��  
        pingm_pakcet *packet = icmp_findpacket(-1);  
        if(packet){  
            packet->seq = packet_send;  
            packet->flag = 1;  
            gettimeofday(&packet->tv_begin,NULL);  
        }  
        icmp_pack((struct icmp *)send_buff,packet_send,&tv, 64);  
        //�������  
        size = sendto(rawsock, send_buff,64,0,(struct sockaddr *)&dest, sizeof(dest));  
        if(size < 0){  
            perror("sendto error");  
            continue;  
        }  
       packet_send ++;  
       //ÿ��1s����һ��ICMP���������  
        sleep(1);  
    }  
}
  
//8.����Ŀ�������Ļظ�����
/***********����pingĿ�������Ļظ�***********/ 
static void *icmp_recv(void *argv)  
{  
    //��ѯ�ȴ�ʱ��  
    struct timeval tv;  
    tv.tv_usec = 200;  
    tv.tv_sec = 0;  
    fd_set readfd;  
    //��û���źŷ���һֱ��������  
    while(alive){  
       int ret = 0;  
        FD_ZERO(&readfd);  
        FD_SET(rawsock,&readfd);  
        ret = select(rawsock+1,&readfd,NULL,NULL,&tv);  
        switch(ret)  
        {  
            case -1:  
                //������  
                break;  
            case 0:  
                //��ʱ  
                break;  
            default :  
                {  
                    //�յ�һ����  
                    int fromlen = 0;  
                    struct sockaddr from;  
                    //��������  
                    int size = recv(rawsock,recv_buff,sizeof(recv_buff),0);  
                    if(errno == EINTR){  
                        perror("recvfrom error");  
                        continue;  
                    }  
                                        //���  
                    ret = icmp_unpack(recv_buff,size);  
                    if(ret == 1){  
                       continue;  
                    }  
               }  
                break;  
        }  
    }  
} 
 
//9.����ICMPͷ��(�����в���Ҫ,����ֻ����Ϊ�˽�)
/**********����ICMP���ͱ��ĵ�ͷ��********************************* 
   ���������ICMP���� 
   */  
/*struct icmp 
{ 
    u_int8_t icmp_type;   //��Ϣ���� 
    u_int8_t icmp_code;   //��Ϣ���͵����� 
    u_int16_t icmp_cksum;   //У��� 
    union 
    { 
        struct ih_idseq    //��ʾ���ݱ� 
        { 
            u_int16_t icd_id;  //���ݱ�ID 
            u_int16_t icd_seq;  //���ݱ������ 
        }ih_idseq;   
    }icmp_hun; 
#define icmp_id icmp_hun.ih_idseq.icd_id; 
#define icmp_seq icmp_hun.ih_idseq.icd_seq; 
    union 
    { 
        u_int8_t id_data[1];    //���� 
    }icmp_dun; 
#define icmp_data icmp_dun.id_data; 
}; */
  
//10.������
//������  
int main(int argc, char const *argv[])  
{  
    struct hostent *host = NULL;  
    struct protoent *protocol = NULL;  
    char protoname[] = "icmp";  
    unsigned long inaddr = 1;  
    int size = 128*K;  
  
    if(argc < 2)                     //�����Ƿ�������ȷ  
    {  
        icmp_usage();  
        return -1;  
    }  
                                          //��ȡЭ������  
    protocol = getprotobyname(protoname);  
    if(protocol == NULL)  
    {  
        perror("getprotobyname()");  
        return -1;  
    }  
                                          //����Ŀ�ĵ�ַ�ַ���  
    memcpy(dest_str, argv[1],strlen(argv[1])+1);  
    memset(pingpacket, 0, sizeof(pingm_pakcet) * 128);  
                                           //socket��ʼ��  
   rawsock = socket(AF_INET, SOCK_RAW, protocol->p_proto);  
    if(rawsock < 0){  
        perror("socket");  
       return -1;  
    }  
  
    pid = getuid();                       //Ϊ�������߳����𣬼���pid  
                                          //������ջ���������ֹ���հ�������  
    setsockopt(rawsock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));  
    bzero(&dest, sizeof(dest));  
                                          //��ȡĿ�ĵ�ַ��IP��ַ  
    dest.sin_family = AF_INET;  
                                          //�����Ŀ�ĵ�ַΪ�ַ���IP��ַ  
    inaddr = inet_addr(argv[1]);  
    if(inaddr == INADDR_NONE){             //�������DNS��ַ  
       host = gethostbyname(argv[1]);  
        if(host == NULL){
			printf("Gethostname error, %s\n", strerror(errno));
            perror("gethostbyname");  
            return -1;  
        }  
                                            //����ַ���Ƶ�dest  
       memcpy((char *)&dest.sin_addr, host->h_addr, host->h_length);  
    }                                       //IP��ַ�ַ���  
    else {  
       memcpy((char *)&dest.sin_addr, &inaddr,sizeof(inaddr));  
    }  
                                           //��ӡ��ʾ  
    inaddr = dest.sin_addr.s_addr;  
    printf("PING %s (%ld.%ld.%ld.%ld) 56(84) bytes of data.\n",   
        dest_str,(inaddr&0x000000ff)>>0,(inaddr&0x0000ff00)>>8,(inaddr&0x00ff0000)>>16,(inaddr&0xff000000)>>24);  
                                           //��ȡ�ź�SIGINT,��icmp_sigint�ҽ���  
    signal(SIGINT,icmp_sigint);  
  
    /*�������ݲ����ջ�Ӧ 
    ���������̣߳�һ�����ڷ����ݣ���һ�����ڽ�����Ӧ���ݣ�������ȴ������߳�������Ϻ��ٽ��� 
    ��һ�������Խ������ͳ�Ʋ���ӡ 
    */  
    alive = 1;                                     //��ʼ��������  
    pthread_t send_id, recv_id;                    //���������̣߳����ڷ��ͺͽ���  
    int err = 0;  
    err = pthread_create(&send_id, NULL, icmp_send, NULL); //����  
    if(err <  0){  
        return -1;  
    }  
    err = pthread_create(&recv_id, NULL, icmp_recv, NULL); //����  
    if(err < 0){  
        return -1;  
    }  
                                  //�ȴ��߳̽���  
    pthread_join(send_id, NULL);  
    pthread_join(recv_id, NULL);  
                                  //������ӡͳ�ƽ��  
    close(rawsock);  
    icmp_statistics();  
    return 0;  
}