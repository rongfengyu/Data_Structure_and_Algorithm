#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define HSIZE	256
#define DSIZE	256
#define HLIST_SIZE_MAX		(4*1024*1024)

struct list_t{
	//struct list_t *pre;
	struct list_t *next;
	time_t create;
	time_t update;
	unsigned char data[DSIZE];
};
struct hlist_t{
	int count;
	int size;
	struct list_t *htable[HSIZE];
};

int hlist_init(struct hlist_t *h)
{
	int i=0;
	if(!h)
		return -1;
	h->count = 0;
	h->size = HLIST_SIZE_MAX;
	for(i=0;i<HSIZE;i++)
		h->htable[i]=NULL;
	return 0;
}
int hlist_release(struct hlist_t *h)
{
	int i=0;
	struct list_t *l = NULL;
	if(!h)
		return -1;
	for(i=0;i<HSIZE;i++){
		while(h->htable[i]){
			l=h->htable[i];
			h->htable[i]=l->next;
			l->next = NULL;
			if(l)
				free(l);
		}
		h->htable[i] = NULL;
	}
	h->count = 0;
	h->size = 0;
	return 0;
}
int hlist_is_exist(struct hlist_t *h, int (*compare)(struct list_t *,void *,int len),
								void *d, int len,int key)
{
	struct list_t *l = NULL;
	if(!h||!compare||!d||key<0||key>=HSIZE)
		return -1;
	l = h->htable[key];
	while(l){
		if(!compare(l,d,len))
			return 0;
		l= l->next;
	}
	return -1;
}

int hlist_add(struct hlist_t *h,int (*compare)(struct list_t *,void *,int),void *d,int len,int key)
{
	struct list_t *l = NULL;
	if(!h || !d ||key<0||key>=HSIZE)
		return -1;
	l = h->htable[key];
	while(l){
		if(!compare(l,d,len))
			return -2;
		l= l->next;
	}
	if(!(l=(struct list_t *)malloc(sizeof(struct list_t))))
		return -1;

	l->create = time(NULL);
	l->update = time(NULL);
	memcpy(l->data,d,len);

	l->next = h->htable[key];
	h->htable[key] = l;
	h->count++;
	
	return 0;
}

int hlist_update(struct hlist_t *h,int (*compare)(struct list_t *,void *,int), 
						int (*update)(struct list_t*,void *,int),void *d,int len,int key)
{
	struct list_t *l = NULL;
	if(!h || !d ||key<0||key>=HSIZE)
		return -1;
	l = h->htable[key];
	while(l){
		if(!compare(l,d,len)){
			update(l,d,len);
			l->update = time(NULL);
			return 0;
		}
		l= l->next;
	}
	return -1;
}

int hlist_del(struct hlist_t *h, int (*compare)(struct list_t *,void *,int),
								void *d, int len,int key)
{
	struct list_t *pre = NULL;
	struct list_t *list = NULL;
	if(!h||!compare||!d||key<0||key>=HSIZE)
		return -1;
	pre = h->htable[key];
	list = h->htable[key];
	while(list){
		if(!compare(list,d,len)){
			if(list == h->htable[key])
				h->htable[key] = list->next;
			else
				pre->next = list->next;			
			list->next = NULL;
			if(list)
				free(list);
			h->count--;
			return 0;
		}
		pre = list;
		list= list->next;
	}	
	return -1;
}

struct list_t *hlist_find(struct hlist_t *h, int (*compare)(struct list_t *,void *,int),
								void *d, int len,int key)
{
	struct list_t *l = NULL;
	if(!h||!compare||!d||key<0||key>=HSIZE)
		return NULL;
	l = h->htable[key];
	while(l){
		if(!compare(l,d,len))
			return l;
		l= l->next;
	}
	
	return NULL;
}

int hlist_check_valid(struct hlist_t *h,int create_max,int update_max,struct list_t **output)
{
	int i=0;
	time_t now;
	struct list_t *pre = NULL;
	struct list_t *list = NULL;
	struct list_t *out = NULL;
	if(!h||create_max<=0||update_max<=0||!output)
		return -1;
	now = time(NULL);
	printf("now=%d\n",now);
	for(i=0;i<HSIZE;i++){
		pre = h->htable[i];
		list = h->htable[i];
		while(list){
			if((abs(now - list->create)>create_max)||(abs(now - list->update)>update_max)){
				if((pre == list)&&(list!=h->htable[i]))
					continue;
				if(list == h->htable[i]){
					h->htable[i] = list->next;
					pre = h->htable[i];
				}else
					pre->next = list->next;			
				list->next = NULL;
				h->count--;

				list->next = out;
				out = list;

				list = pre;
				continue;
 			}
			pre = list;
			list= list->next;
		}
	}
	*output = out;
	return 0;
}
void hlist_print(struct hlist_t *h)
{
	int i=0;
	struct list_t *l = NULL;
	if(!h)
		return;
	printf("count=%d\n",h->count);
	for(i=0;i<HSIZE;i++){
		l = h->htable[i];
		while(l){
			printf("hash=%d,create=%d,update=%d\n",i,l->create,l->update);
			l = l->next;
		}
	}

	return;
}

#ifdef _HASH_TEST_
int compare(struct list_t *l,void *d,int len)
{
	if(!l||!d||len<=0||len>=DSIZE)
		return -1;
	
	return memcmp(l->data,d,len);
}

int update(struct list_t *l,void *d,int len)
{
	if(!l||!d||len<=0||len>=DSIZE)
		return -1;
	memcpy(l->data,d,len);
	return 0;
}
int hash(void * d, int len)
{
	if(!d||len<=0||len>=DSIZE)
		return -1;
	return ((*(char *)d)*len%HSIZE);
}
int main(int argc, char *argv[])
{
	struct hlist_t hdata;
	struct list_t *list = NULL;
	hlist_init(&hdata);
	hlist_add(&hdata, compare, "aaaaaa", 6, 1);
	hlist_add(&hdata, compare, "aaaaaaaa", 8, 1);
	sleep(1);
	hlist_add(&hdata, compare, "bbbbbbbb", 8, 3);
	sleep(2);
	hlist_add(&hdata, compare, "bbbbbbbbb", 9, 3);
	hlist_update(&hdata, compare,update, "bbbbbbbb", 8, 3);
	sleep(1);
	hlist_update(&hdata, compare,update, "aaaaaa", 6, 1);
	sleep(1);
	hlist_update(&hdata, compare,update, "ccccc", 5, 2);
	sleep(1);
	hlist_print(&hdata);
	hlist_del(&hdata, compare,"aaaaaa", 6, 1);
	hlist_print(&hdata);
	hlist_check_valid(&hdata, 5, 3, &list);
	while(list){
		printf("create=%d,update=%d\n",list->create,list->update);
		list = list->next;
	}
	hlist_release(&hdata);
    return 0;
}
#endif
