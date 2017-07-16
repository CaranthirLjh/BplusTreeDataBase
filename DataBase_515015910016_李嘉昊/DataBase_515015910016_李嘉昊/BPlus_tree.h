#include<iostream>
#include<string>
#include<fstream>
#include"BPlus_node.h"
using namespace std;

template<class keytype>
class BPlusTree
{
public:
	BPlusTree();
	~BPlusTree();

private:
	BTnode<keytype>*root;//指向根结点的指针
	BTnode<keytypr>*leaf_head;//指向叶子节点头节点的指针

};
