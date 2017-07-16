#include<iostream>
#include<string>
#include<fstream>
using namespace std;

///报错类
class INEXSIST_KEY{};//key值不存在
class KEY_OVERFLOW{};//key值超过上限
class CHILD_OVERFLOW{};//child超过上限


///常量定义
enum nodetype { internal, leaf };//节点类型
enum bro_direction { left, right };//兄弟节点方向

const int order = 200;//B+树的阶数

const int max_interal_order = order;//每个内部节点的关键码上界
const int min_internal_order = order / 2;//每个内部节点的关键码下界
const int max_leaf_order = order;//每个叶子节点的关键码上界
const int min_leaf_order = order / 2;//每个叶子节点的关键码下界

const int max_child = order + 1;//最大子树个数
const int min_child = order / 2 + 1;//最小子树个数


///节点类
template<class keytype>
class BTnode
{
public:
	BTnode();
	~BTnode();

	keytype getkey(int index);//根据下标返回对应的key值
	int getindex(keytype k);//根据key值返回对应的下标
	int getfatherindex();//返回该节点在其父节点中的下标

	void setkey(int index, keytype k);//根据下标改变key值
	void setchild(int index, BTnode* child);//根据下标改变child值

	void split(BTnode*parent, int childindex);//在指定下标的孩子节点处分割节点

	void insert(int keyIndex, keytype key, int childIndex, BTnode* childNode);//在指定处插入key或孩子节点

	void remove(int keyIndex, int childIndex);//删除key或孩子节点

	void borrowFrom(BTnode* siblingNode, BTnode* parentNode, int keyIndex, bro_direction d);//从兄弟节点处借key和孩子节点

private:
	bool isleaf;//判断是否为叶子节点

	keytype key[max_interal_order];//储存每个节点中的key
	int keynum;//key的个数

	BTnode * branch_child[max_child];//指向该节点各个儿子节点的指针
	BTnode * branch_father;//指向父节点
	BTnode * bro_left;//指向左兄弟节点
	BTnode * bro_right;//指向右兄弟节点
};