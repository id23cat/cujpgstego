/*
 * tree.h
 *
 *  Created on: 08.12.2011
 *      Author: id23cat
 */

#ifndef TREE_H_
#define TREE_H_
#include <boost/dynamic_bitset.hpp>
#include <stdio.h>
#include "huftree.h"

typedef unsigned char UINT8;
typedef unsigned short UINT16;

struct treeLIST {
	UINT8 code;
	UINT8 codelength;
//	boost::dynamic_bitset<UINT16> val;
	UINT16 bitval;
	void PrintData() {
		printf("\tLIST: code=%x, length=%d, bitset=%x[ ", code, codelength, bitval);
		for(int i=0; i<codelength; i++)
			printf("%x ", (UINT16)(bitval>>i)&1);
		printf("]\n");
	};
};

struct treeNODE {
	treeNODE *prev;
	treeNODE *left;
	treeNODE *right;
	treeLIST data;
	UINT8 depth;
	void PrintData() {
		printf("depth %d:\n", depth);
		data.PrintData();
	};
};

class TREE {
	treeNODE *tree;
	treeNODE *curptr;
	void treeFree(treeNODE *p);
	void addList(treeNODE *p, treeLIST l);
	void printData(treeNODE *p);
public:
	TREE();
	~TREE();
	void AddList(treeLIST l) {
		addList(curptr, l);
	};
	void PrintData(){
		printData(tree);
	};
	void ResetCurrentPointer(){
		curptr = tree;
	}

	bool MovePtr(bool branch);		// return true if curptr points to the list
	UINT8 GetCode();				// return code from node that currently pionted by curptr
};

#endif /* TREE_H_ */
