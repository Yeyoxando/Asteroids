#include <stdlib.h>
#include <stdio.h>

struct BTree{
	int data;
	BTree* left;
	BTree* right;
};

void create_btree(BTree** barbol){
	*barbol=NULL;
}

BTree* insert_btree(int data, BTree* left, BTree* right){
	BTree* newTree;
	create_btree(&newTree);
	newTree = (BTree*) calloc(1, sizeof(BTree));
	
	(*newTree).data = data;
	(*newTree).left = left;
	(*newTree).right = right;
	
	return newTree;
}

void ShowTree(BTree* tree, int tab){
	if((*tree).right != NULL) ShowTree((*tree).right, tab + 1);
	for(int i = 0; i < tab; i++){
		printf("   ");
	}
	printf("%03d\n", (*tree).data);
	if((*tree).left != NULL) ShowTree((*tree).left, tab + 1);
}

void inorder(BTree* tree){
	if((*tree).left != NULL) inorder((*tree).left);
	printf("% 3d ", (*tree).data);
	if((*tree).right != NULL) inorder((*tree).right);
}

void preorder(BTree* tree){
	printf("% 3d ", (*tree).data);
	if((*tree).left != NULL) preorder((*tree).left);
	if((*tree).right != NULL) preorder((*tree).right);
}

void postorder(BTree* tree){
	if((*tree).left != NULL) postorder((*tree).left);
	if((*tree).right != NULL) postorder((*tree).right);
	printf("% 3d ", (*tree).data);
}