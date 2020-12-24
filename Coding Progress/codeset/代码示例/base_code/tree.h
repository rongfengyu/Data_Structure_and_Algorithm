#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

#define Type unsigned int

//树的节点
typedef struct tree_node_t {
	char name[100];
	unsigned int id;

	struct tree_node_t *firstChild, *nextSibling;
}node, *tree;


node* newNode(Type id, const char * username);

node* SearchTree(tree T,Type x);

void addBrotherNode(node* bro, node* node);

void addNode(tree T, int i, int j, const char *username);

void delNode(tree T, int i, int j);

void PrintTree(tree T);

void printjson(tree T, string& outString);

