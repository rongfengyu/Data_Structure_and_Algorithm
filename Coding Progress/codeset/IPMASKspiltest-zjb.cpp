//============================================================================
// Name        : spilttest.cpp
// Author      : zjb
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include<cstring>

#include<sys/socket.h>
#include<arpa/inet.h>
#include <string.h>
#include<string>
#include <netinet/in.h>

using namespace std;

char s[]="1.1.1.1-1.1.1.23/24,192.168.0.24/255.255.255.0,192.168.0.10-192.168.0.20/255.255.255.0,10.0.0.2/255.255.255.0";
char eth[]="eth0";

struct ipmask
{
	char* ip;
	char* mask;
};
struct ipmask IPMASK[5];


int ipmaskcfg(struct ipmask *p,char *ethname)
{
	//for MASK
	size_t posm;
	string ptrm=string(p->mask);
	posm=ptrm.find(".");
	if(posm!=std::string::npos)//is dot
	{
		unsigned int mask=ntohl(inet_addr((p->mask)));//unsigned int==32bit
		if(0xffffffff<=mask || mask<=0)
		{
			cout<<"please check mask !"<<endl;
			return 1;
		}
		else if(((~mask+1)&(~mask))==0)
		{
			cout<<"valid dot mask !"<<endl;
		}
		else//Continuous "1" check
		{
			cout<<"error mask !"<<endl;
			return 1;
		}
	}
	else//is not dot,prefix
	{
		int mask = atoi(p->mask);
		if(mask>32 || mask<=0)
		{
			cout<<"error mask !"<<endl;
			return 1;
		}
	}

	//for IP
	size_t pos;
	string ptr=string(p->ip);
	pos=ptr.find("-");
	if(pos!=std::string::npos)//multi-IP
	{
		cout<<"multi-IP "<<ptr<<" !"<<endl;

		//start IP
		string ipstart=ptr.substr(0,pos);
		unsigned int ipstartip=ntohl(inet_addr(ipstart.c_str()));
		//end IP
		string ipend=ptr.substr(pos+1,ptr.length()-pos-1);
		unsigned int ipendip=ntohl(inet_addr(ipend.c_str()));

		//compare IP
		if(0xffffffff<=ipstartip || 0xffffffff<=ipendip)
		{
			cout<<"error: ip out of range!"<<endl;
			return 1;
		}
		if(ipstartip>ipendip)
		{
			cout<<"error: ipstartip>ipendip !"<<endl;
			return 1;
		}
		else
		{
			cout<<"ipstartip<=ipendip  "<<ipendip-ipstartip+1<<endl;
			//
			for(unsigned int i=ipstartip;i<=ipendip;i++)
			{
				in_addr inaddr;
				inaddr.s_addr=htonl(i);
				cout<<"ip addr add "<<(inet_ntoa(inaddr))<<"/"<<p->mask<<" dev "<<ethname<<endl;
			}
		}


	}
	else
	{
		cout<<"sigleIP "<<ptr<<" !"<<endl;
		unsigned int ipip=ntohl(inet_addr(ptr.c_str()));
		if(0xffffffff<=ipip || ipip<=0)
		{
			cout<<"error ip out of range!"<<endl;
			return 1;
		}
		else
		{
			in_addr inaddrs;
			inaddrs.s_addr=htonl(ipip);
			cout<<"ip addr add "<<(inet_ntoa(inaddrs))<<"/"<<p->mask<<" dev "<<ethname<<endl;
		}
	}
	return 0;
}


int main(void) {
	char *buf=s;
	char *Ptr[10];
	int i=0;

	while((Ptr[i]=strtok(buf,",/"))!=NULL)
	{
		switch(i%2)
		{
		case 0:
			IPMASK[i/2].ip=Ptr[i];
			break;
		case 1:
			IPMASK[i/2].mask=Ptr[i];
			break;
		}
		i++;
		buf=NULL;
//		Ptr[i]=strtok(NULL,",/");
	}

	int count=i/2;//count the number of IPcfg segment
	i=0;

	cout<<"************"<<count<<endl;

	for( i=0;i<count;i++)
	{
		if(!ipmaskcfg(&IPMASK[i],eth))
			cout<<i<<"success!"<<endl;
		else
			return 1;
	}

	return EXIT_SUCCESS;
}



