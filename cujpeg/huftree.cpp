/*
 * tree.cpp
 *
 *  Created on: 08.12.2011
 *      Author: id23cat
 */

#include <memory.h>

#include "huftree.h"

void TREE::treeFree(treeNODE *p) {
	if (p) {
		if (p->left)
			treeFree(p->left);
		if (p->right)
			treeFree(p->right);
		free(p);
	}
}

void TREE::addList(treeNODE *p, treeLIST l) {
	if (!p->left) { // if left node does not exist
//		p->left = (treeNODE*) malloc(sizeof(treeNODE)); //create left node
		SAFE_MALLOC(p->left, 1, treeNODE);	//create left node
		//		memset(p->left, 0, sizeof(treeNODE));
		p->left->prev = p;
		//		p->left->data = l;
		curptr = p->left;
		curptr->left = NULL;
		curptr->right = NULL;
		curptr->data.codelength = 0;
		curptr->depth = p->depth + 1; // incrementing depth

		if (curptr->depth) {
			curptr->data.bitval = 0;
			UINT16 v = p->data.bitval;
			v = v << 1;
			curptr->data.bitval = v;
			//			printf("0x%x\n", curptr->data.bitval);

		}

		if (curptr->depth == l.codelength) {
			l.bitval = curptr->data.bitval;
			curptr->data = l;
			curptr = p;
			return;
		} else
			addList(curptr, l);
	} else if (!p->right) {
//		p->right = (treeNODE*) malloc(sizeof(treeNODE)); //create right node
		SAFE_MALLOC(p->right, 1, treeNODE);
		//		memset(p->left, 0, sizeof(treeNODE));
		p->right->prev = p;
		//		p->right->data = l;
		curptr = p->right;
		curptr->left = NULL;
		curptr->right = NULL;
		curptr->data.codelength = 0;
		curptr->depth = p->depth + 1; // incrementing depth


		if (curptr->depth) {
			curptr->data.bitval = 0;
			UINT16 v = p->data.bitval;
			v = (v << 1) | 1;
			curptr->data.bitval = v;
			//			printf("0x%x\n", curptr->data.bitval);
		}

		if (curptr->depth == l.codelength) {
			l.bitval = curptr->data.bitval;
			curptr->data = l;
			curptr = p;
			return;
		} else
			addList(curptr, l);
	} else {
		curptr = p->prev;
		addList(curptr, l);
	}

}

void TREE::printData(treeNODE *p) {
	p->PrintData();
	if (p->left) {
		printf("to Left:\n");
		printData(p->left);
	}
	if (p->right) {
		printf("to Right:\n");
		printData(p->right);
	}
}

TREE::TREE() {
//	tree = (treeNODE*) malloc(sizeof(treeNODE));
	SAFE_MALLOC(tree, 1, treeNODE);
	memset(tree, 0, sizeof(treeNODE));
	curptr = tree;
	tree->depth = 0;
}
TREE::~TREE() {
	treeFree(tree);
	curptr = NULL;
}

bool TREE::MovePtr(bool branch) throw (tree_fail) {
//		printf("branch_%d ", (int)branch); fflush(stdout);
//
	if (branch)
		if (curptr->right)
			curptr = curptr->right;
		else
			throw tree_fail(__FILE__, __LINE__,
					"TREE::MovePtr(): right branch doesn't exist") << int_info(
					curptr->depth) << int_info(curptr->data.code);
	else if (curptr->left)
		curptr = curptr->left;
	else
		throw tree_fail(__FILE__, __LINE__,
				"TREE::MovePtr(): left branch doesn't exist") << int_info(
				curptr->depth) << int_info(curptr->data.code);

//		curptr->PrintData();
//
	if (curptr->data.codelength)
		return true;
	return false;
}

UINT8 TREE::GetCode()throw (tree_fail)  {
	if (!curptr)
		throw tree_fail(__FILE__, __LINE__,
				"TREE::GetCode(): curptr is NULL");
	return curptr->data.code;
}
