// Implementation of the Set ADT using a balanced BST
// COMP2521 22T2 Assignment 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Set.h"
#include "SetStructs.h"

////////////////////////////////////////////////////////////////////////
// Basic Operations
struct node* createNode(int item,struct node* l,struct node* r,struct node *parent){
	struct node* ret=malloc(sizeof(struct node));
	ret->item=item;
	ret->left=l;
	ret->right=r;
	ret->parent=parent;
	ret->flag=0;
	ret->height=0;
	return ret;
}
/**
 * Creates a new empty set
 */
Set SetNew(void) {
	// TODO
	Set s=malloc(sizeof(struct set));
	s->tree=NULL;
	s->size=0;
	return s;
}
void freeTree(struct node* rt){
	if(rt->left){
		freeTree(rt->left);
	}
	if(rt->right){
		freeTree(rt->right);
	}
	free(rt);
}
/**
 * Frees all memory associated with the given set
 */
void SetFree(Set s) {
	// TODO
	freeTree(s->tree);
	free(s);
}
void updateNodeHeight(struct node* rt){
	int height=0;
	if(rt->left==NULL){
		height=0;
	}else	
		height=rt->left->height+1;
	if(rt->right!=NULL && height<rt->right->height+1)
		height=rt->right->height+1;
	rt->height=height;
}
int getTreeHeight(struct node* rt){
	if(rt==NULL)
		return -1;
	return rt->height;
}
int isLL(struct node* rt){
	return rt && getTreeHeight(rt->left)>getTreeHeight(rt->right)+1 && getTreeHeight(rt->left->left)>getTreeHeight(rt->left->right);
}
int isLR(struct node* rt){
	return rt && getTreeHeight(rt->left)>getTreeHeight(rt->right)+1 && getTreeHeight(rt->left->left)<getTreeHeight(rt->left->right);
}
int isRL(struct node* rt){
	return rt && getTreeHeight(rt->right)>getTreeHeight(rt->left)+1 && getTreeHeight(rt->right->left)>getTreeHeight(rt->right->right);
}
int isRR(struct node* rt){
	return rt && getTreeHeight(rt->right)>getTreeHeight(rt->left)+1 && getTreeHeight(rt->right->right)>getTreeHeight(rt->right->left);
}
void setNodeParent(struct node* child,struct node* parent){
	if(child==NULL)
		return;
	child->parent=parent;
}
struct node* balanceLL(struct node* rt){
	struct node* rootNew=rt->left;
	struct node* tmp=rootNew->right;
	rootNew->right=rt;
	rt->left=tmp;
	setNodeParent(rt,rootNew);
	setNodeParent(rootNew,NULL);
	setNodeParent(tmp,rt);
	return rootNew;
}
struct node* balanceLR(struct node* rt){
	struct node* rootNew=rt->left->right;
	struct node* tmp1=rootNew->left;
	struct node* tmp2=rootNew->right;
	struct node* tmp3=rt->left;
	rootNew->left=rt->left;
	rootNew->right=rt;
	rt->left->right=tmp1;
	rt->left=tmp2;
	setNodeParent(rootNew,NULL);
	setNodeParent(tmp1,tmp3);
	setNodeParent(tmp2,rt);
	setNodeParent(rt,rootNew);
	setNodeParent(tmp3,rootNew);
	return rootNew;
}
struct node* balanceRL(struct node* rt){
	struct node* rootNew=rt->right->left;
	struct node* tmp1=rootNew->left;
	struct node* tmp2=rootNew->right;
	struct node* tmp3=rt->right;
	rootNew->left=rt;
	rootNew->right=rt->right;
	rt->right->left=tmp2;
	rt->right=tmp1;
	setNodeParent(rootNew,NULL);
	setNodeParent(tmp1,rt);
	setNodeParent(tmp2,tmp3);
	setNodeParent(tmp3,rootNew);
	setNodeParent(rt,rootNew);
	return rootNew;
}
struct node* balanceRR(struct node* rt){
	struct node* rootNew=rt->right;
	struct node* tmp=rootNew->left;
	rootNew->left=rt;
	rt->right=tmp;
	setNodeParent(rt,rootNew);
	setNodeParent(rootNew,NULL);
	setNodeParent(tmp,rt);
	return rootNew;
}
struct node* balanceTree(struct node* rt){
	if(isLL(rt))
		rt=balanceLL(rt);
	else if(isLR(rt))
		rt=balanceLR(rt);
	else if(isRR(rt))
		rt=balanceRR(rt);
	else if(isRL(rt))
		rt=balanceRL(rt);
	return rt;
}
int flag=0;
struct node* insertTree(struct node* rt,int item){
	if(rt==NULL){
		flag=1;
		return createNode(item,NULL,NULL,NULL);
	}
	if(item>rt->item){
		rt->right=insertTree(rt->right,item);
		rt->right->parent=rt;
	}else if(item<rt->item){
		rt->left=insertTree(rt->left,item);
		rt->left->parent=rt;
	}
	updateNodeHeight(rt);
	rt=balanceTree(rt);
	return rt;
}
/**
 * Inserts an item into the set
 */
void SetInsert(Set s, int item) {
	// TODO
	flag=0;
	s->tree=insertTree(s->tree,item);
	if(flag)
		s->size++;
}

/**
 * Returns the number of elements in the set
 */
int SetSize(Set s) {
	// TODO
	return s->size;
}
int searchTree(struct node* rt,int item){
	if(rt==NULL){
		return 0;
	}
	if(item==rt->item){
		return 1;
	}
	if(item>rt->item){
		return searchTree(rt->right,item);
	}
	return searchTree(rt->left,item);
}
/**
 * Returns true if the set contains the given item, and false otherwise
 */
bool SetContains(Set s, int item) {
	// TODO
	return searchTree(s->tree,item);
}
void printTree(struct node* rt){
	if(rt==NULL){
		return;
	}
	printTree(rt->left);
	if(flag==0){
		printf("%d",rt->item);
		flag=1;
	}else
		printf(",%d",rt->item);
	printTree(rt->right);
}
/**
 * Prints the given set in the format
 * {elem1, elem2, elem3}
 */
void SetShow(Set s) {
	// TODO
	flag=0;
	printf("{");
	printTree(s->tree);
	printf("}");
}

////////////////////////////////////////////////////////////////////////
// Further Operations

/**
 * Returns a new set representing the union of the two sets
 */
Set SetUnion(Set s1, Set s2) {
	// TODO
	Set ret=SetNew();
	SetCursor cursor1=SetCursorNew(s1);
	SetCursor cursor2=SetCursorNew(s2);
	while(1){
		int value=SetCursorNext(cursor1);
		if(value==UNDEFINED)
			break;
		SetInsert(ret,value);
	}
	while(1){
		int value=SetCursorNext(cursor2);
		if(value==UNDEFINED)
			break;
		SetInsert(ret,value);
	}
	return ret;
}

/**
 * Returns a new set representing the intersection of the two sets
 */
Set SetIntersection(Set s1, Set s2) {
	// TODO
	Set ret=SetNew();
	SetCursor cursor1=SetCursorNew(s1);
	while(1){
		int value=SetCursorNext(cursor1);
		if(value==UNDEFINED)
			break;
		if(SetContains(s2,value))
			SetInsert(ret,value);
	}
	return ret;
}

/**
 * Returns a new set representing the set difference s1 - s2
 */
Set SetDifference(Set s1, Set s2) {
	// TODO
	Set ret=SetNew();
	SetCursor cursor1=SetCursorNew(s1);
	while(1){
		int value=SetCursorNext(cursor1);
		if(value==UNDEFINED)
			break;
		if(!SetContains(s2,value))
			SetInsert(ret,value);
	}
	return ret;
}

/**
 * Returns true if the two sets are equal, and false otherwise
 */
bool SetEquals(Set s1, Set s2) {
	if(s1->size!=s2->size)
		return 0;
	// TODO
	SetCursor cursor1=SetCursorNew(s1);
	SetCursor cursor2=SetCursorNew(s2);
	while(1){
		int v1=SetCursorNext(cursor1);
		int v2=SetCursorNext(cursor2);
		if(v1!=v2){
			return 0;
		}
		if(v1==UNDEFINED)
			break;
	}
	return 1;
}

/**
 * Returns true if set s1 is a subset of set s2, and false otherwise
 */
bool SetSubset(Set s1, Set s2) {
	// TODO
	SetCursor cursor1=SetCursorNew(s1);
	while(1){
		int value=SetCursorNext(cursor1);
		if(value==UNDEFINED)
			break;
		if(!SetContains(s2,value))
			return 0;
	}
	return 1;
}
int findTreeFloor(struct node* rt,int item){
	if(rt==NULL)
		return UNDEFINED;
	if(item==rt->item)
		return item;
	if(rt->item>item){
		return findTreeFloor(rt->left,item);
	}else{
		int floor1=findTreeFloor(rt->right,item);
		if(floor1!=UNDEFINED)
			return floor1;
		return rt->item;
	}
}
/**
 * Finds the floor of a given item in a set
 */
int SetFloor(Set s, int item) {
	// TODO
	return findTreeFloor(s->tree,item);
}

int findTreeCeiling(struct node* rt,int item){
	if(rt==NULL)
		return UNDEFINED;
	if(item==rt->item)
		return item;
	if(rt->item<item){
		return findTreeCeiling(rt->right,item);
	}else{
		int ceiling1=findTreeCeiling(rt->left,item);
		if(ceiling1!=UNDEFINED)
			return ceiling1;
		return rt->item;
	}
}
/** 
 * Returns the ceiling of a given item in a set
 */
int SetCeiling(Set s, int item) {
	// TODO
	return findTreeCeiling(s->tree,item);
}

////////////////////////////////////////////////////////////////////////
// Cursor Operations

void clearTreeFlag(struct node* rt){
	if(!rt)
		return;
	rt->flag=0;
	clearTreeFlag(rt->left);
	clearTreeFlag(rt->right);
}
/**
 * Creates a new cursor positioned at the smallest element of the set
 */
SetCursor SetCursorNew(Set s) {
	// TODO
	clearTreeFlag(s->tree);
	SetCursor cursor=malloc(sizeof(struct cursor));
	struct node* rt=s->tree;
	while(rt->left){
		rt=rt->left;
	}
	cursor->cur=rt;
	return cursor;
}

/**
 * Frees all memory associated with the given cursor
 */
void SetCursorFree(SetCursor cur) {
	// TODO
	free(cur);
}
int isVisited(struct node* rt){
	return rt==NULL || rt->flag;
}
struct node* findSmallestNode(struct node* rt){
	//rt必须存在
	while(rt->left){
		rt=rt->left;
	}
	return rt;
}
/**
 * Returns the element at the cursor's current position, and then moves
 * the cursor to the next greatest element. If there is no next greatest
 * element, then all subsequent calls to SetCursorNext on this cursor
 * should return UNDEFINED.
 */
int SetCursorNext(SetCursor cur) {
	// TODO
	if(cur->cur==NULL)
		return UNDEFINED;
	int ret=cur->cur->item;
	cur->cur->flag=1;
	while(cur->cur && isVisited(cur->cur)){
		//当前节点已经访问。
		if(cur->cur->left &&!isVisited(cur->cur->left)){
			//左子树存在且没有遍历，去左子树的最小节点。
			cur->cur=findSmallestNode(cur->cur->left);
			break;
		}else if(cur->cur->right && !isVisited(cur->cur->right)){
			//左子树都遍历了，右子树存在且没有遍历，去右子树的最小节点。
			cur->cur=findSmallestNode(cur->cur->right);
			break;
		}else{
			//左右子树都遍历了，去上面。
			cur->cur=cur->cur->parent;
		}
	}
	//printf("%d\n",ret);
	return ret;
}

////////////////////////////////////////////////////////////////////////

