#include "tree.h"

node* newNode(Type id, const char * username){
    node* p;
    p=(node*)malloc(sizeof(node));
    if(p==NULL) return NULL;
    p->id=id;
	//p->name = username;
	strcpy(p->name, username);
    p->firstChild=NULL;
    p->nextSibling=NULL;
    return p;
}

//在树T中查找期望ID
node* SearchTree(tree T,Type x){  
    tree temp1,temp2;
    if(T == NULL)return NULL;
    if(T->id == x)return T;
    else{
		temp1 = SearchTree(T->firstChild,x);
        temp2 = SearchTree(T->nextSibling,x);
        if(temp1 != NULL) return temp1;
        return temp2;
    }
}

//将节点插入到兄弟节点
void addBrotherNode(node* bro, node* node)
{
    if(bro->nextSibling == NULL)
        bro->nextSibling = node;
    else
        addBrotherNode(bro->nextSibling, node);
}

//addNode将父子结点组对
//如果父节点的firstChild==NULL, 则firstChild = node;
//如果父节点的firstChild != NULL, 则
void addNode(tree T, int i, int j, const char *username)
{
    node* parent = SearchTree(T, i);
	if(parent == NULL)
	{
		printf("%s : SearchTree %d  error!\n", __FUNCTION__, i);
		return;
	}
	
    node* nnode = newNode(j, username);

    if(parent->firstChild == NULL)
        parent->firstChild = nnode;
    else
        addBrotherNode(parent->firstChild, nnode);
}


void delNode(tree T, int i, int j)
{
	node* parent = SearchTree(T, i);
	if(parent == NULL)
	{
		printf("%s : SearchTree %d  error!\n", __FUNCTION__, i);
		return;
	}
	
    node* dnode = SearchTree(T, j);
	if(dnode == NULL)
	{
		printf("%s : SearchTree %d  error!\n", __FUNCTION__, j);
		return;
	}
	
	//free(dnode);
}

void PrintTree(tree T){  
    if(T != NULL){
		printf("%d\n",T->id);
		PrintTree(T->firstChild);
        PrintTree(T->nextSibling);
    }
}

void printjson(tree T, string& outString)
{
	stringstream ss;
	
    if(T != NULL){
		ss << T->id;
		string s=ss.str();
		 
		outString = outString + "{\"text\":\"";
		//outString = outString + s;
		outString = outString + T->name;
		outString = outString + "\",";
		
		//outString = outString + "\"expanded\":false,";
		
		if(T->firstChild != NULL)
		{
			outString = outString + "\"expanded\":true,";
			outString = outString + "\"leaf\":false";
		}
	    else
		{
			outString = outString + "\"expanded\":false,";
			outString = outString + "\"leaf\":true";
		}
		
		if(T->firstChild != NULL)
		{
			
			outString = outString + ",\"children\":[";
			
			printjson(T->firstChild, outString);
			
			outString = outString + "]";
		}
		outString = outString + "}";
		
		if(T->nextSibling != NULL)
		{
			outString = outString + ",";
		    printjson(T->nextSibling, outString);
		}

    }
}
