#include<iostream>
#include<string>
#include<fstream>
using namespace std;

///������
class INEXSIST_KEY{};//keyֵ������
class KEY_OVERFLOW{};//keyֵ��������
class CHILD_OVERFLOW{};//child��������


///��������
enum nodetype { internal, leaf };//�ڵ�����
enum bro_direction { left, right };//�ֵܽڵ㷽��

const int order = 200;//B+���Ľ���

const int max_interal_order = order;//ÿ���ڲ��ڵ�Ĺؼ����Ͻ�
const int min_internal_order = order / 2;//ÿ���ڲ��ڵ�Ĺؼ����½�
const int max_leaf_order = order;//ÿ��Ҷ�ӽڵ�Ĺؼ����Ͻ�
const int min_leaf_order = order / 2;//ÿ��Ҷ�ӽڵ�Ĺؼ����½�

const int max_child = order + 1;//�����������
const int min_child = order / 2 + 1;//��С��������


///�ڵ���
template<class keytype>
class BTnode
{
public:
	BTnode();
	~BTnode();

	keytype getkey(int index);//�����±귵�ض�Ӧ��keyֵ
	int getindex(keytype k);//����keyֵ���ض�Ӧ���±�
	int getfatherindex();//���ظýڵ����丸�ڵ��е��±�

	void setkey(int index, keytype k);//�����±�ı�keyֵ
	void setchild(int index, BTnode* child);//�����±�ı�childֵ

	void split(BTnode*parent, int childindex);//��ָ���±�ĺ��ӽڵ㴦�ָ�ڵ�

	void insert(int keyIndex, keytype key, int childIndex, BTnode* childNode);//��ָ��������key���ӽڵ�

	void remove(int keyIndex, int childIndex);//ɾ��key���ӽڵ�

	void borrowFrom(BTnode* siblingNode, BTnode* parentNode, int keyIndex, bro_direction d);//���ֵܽڵ㴦��key�ͺ��ӽڵ�

private:
	bool isleaf;//�ж��Ƿ�ΪҶ�ӽڵ�

	keytype key[max_interal_order];//����ÿ���ڵ��е�key
	int keynum;//key�ĸ���

	BTnode * branch_child[max_child];//ָ��ýڵ�������ӽڵ��ָ��
	BTnode * branch_father;//ָ�򸸽ڵ�
	BTnode * bro_left;//ָ�����ֵܽڵ�
	BTnode * bro_right;//ָ�����ֵܽڵ�
};