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
	BTnode<keytype>*root;//ָ�������ָ��
	BTnode<keytypr>*leaf_head;//ָ��Ҷ�ӽڵ�ͷ�ڵ��ָ��

};
