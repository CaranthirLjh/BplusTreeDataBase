#include"BPlus_node.h"

///报错类定义
INEXSIST_KEY inexsist_key;//key值不存在
KEY_OVERFLOW key_overflow;//key值超过上限
CHILD_OVERFLOW child_overflow;//child超过上限

///节点类成员函数实现
template<class keytype> BTnode<keytype>::BTnode()
{
	isleaf = true;
	keynum = 0;
}

template<class keytype> BTnode<keytype>::~BTnode()
{

}

//根据下标返回对应的key值,若不存在则throw一个错误类
template<class keytype> keytype BTnode<keytype>::getkey(int index)//根据下标返回对应的key值,若不存在则throw一个错误类
{
	if (index < keynum)
	{
		return key[index];
	}
	else
	{
		throw inexsist_key;
	}
}

//根据key值返回对应的下标,若不存在则throw一个错误类
template<class keytype> int BTnode<keytype>::getindex(keytype k)//根据key值返回对应的下标,若不存在则throw一个错误类
{
	for (int i=0; i < keynum; i++)
	{
		if (key[i] == k)
		{
			return i;
		}
	}
	throw inexsist_key;
}

//根据下标改变key值，若大于最大key数则报错
template<class keytype> void BTnode<keytype>::setkey(int index, keytype k)//根据下标改变key值，若大于最大key数则报错
{
	if (index < max_interal_order)
	{
		if (index < keynum)
		{
			key[index] = k;
		}
		/*else//若在key的空指针处赋值则keynum加一
		{
			key[index] = k;
			keynum++;
		}*/
	}
	else
	{
		throw key_overflow;
	}
}

//根据下标改变child值
template<class keytype> void BTnode<keytype>::setchild(int index, BTnode* child)//根据下标改变child值,若大于最大child则报错
{
	if (index < max_child)
	{
		branch_child[index] = child;
	}
	else
	{
		throw child_overflow;
	}
}

//返回该节点在其父节点中的下标
template<class keytype> int BTnode<keytype>::getfatherindex()//返回该节点在其父节点中的下标
{
	for (int i = 0; i < branch_father->keynum; i++)
	{
		if (branch_father->branch_child[i] == this)
		{
			return i;
		}
	}
}

//在指定下标的孩子节点处分割节点
template<class keytype> void BTnode<keytype>::split(BTnode*parent, int childindex)//在指定下标的孩子节点处分割节点
{
	BTnode* newnode = new BTnode();//分离出的新的右兄弟节点
	for (int i = 0; i < min_internal_order; i++)
	{
		newnode->setkey(i, key[min_internal_order + i]);//设置新分裂节点的key值
		newnode->keynum++;
	}
	for (int i = 0; i < min_child; i++)
	{
		newnode->setchild(i, branch_child[min_child + i]);//设置新分裂节点的child
		branch_child[min_child + i] = NULL;//删除该child在原节点中的指针
	}
	keynum = max_interal_order - newnode->keynum;//更新原节点的key的数量

	((BTnode*)parent)->insert(childindex, childindex + 1, key[min_internal_order], newnode);
}

//在指定处插入key或孩子节点
template<class keytype> void BTnode<keytype>::insert(int keyIndex, keytype key, int childIndex, BTnode* childNode)//在指定处插入key或孩子节点
{
	//对key进行插入操作
	if (keyIndex >= 0 && keyIndex < max_interal_order)
	{
		for (int i = keynum - 1; i >= keyIndex; i--)
		{
			setkey(i + 1, key[i]);//将要插入位置的节点及其之后的节点向后移动一位
		}
		setkey(keyIndex, key);//插入新key
		keynum++;
	}
	//对孩子节点进行插入操作
	if (childIndex >= 0 && childIndex < max_child)
	{
		for (int i = keynum; i >= childIndex; i--)
		{
			setchild(i + 1, branch_child[i]);//将要插入位置的孩子节点及其之后的孩子节点向后移动一位
		}
		setchild(childIndex, childNode);//插入新孩子节点
	}
}

//删除key或孩子节点
template<class keytype> void BTnode<keytype>::remove(int keyIndex, int childIndex)//删除key或孩子节点
{
	bool delete_key = false;//判断是否有key被删除
	//删除key操作
	if (keyIndex >= 0 && keyIndex < max_interal_order)
	{
		delete_key = true;
		for (int i = keyIndex + 1; i < keynum; i++)
		{
			setkey(i - 1, key[i]);//将需删除key后的key全向前移动一位
		}
	}
	//删除孩子节点操作
	if (childIndex >= 0 && childIndex < max_child)
	{
		for (int i = childIndex + 1; i <= keynum; i++)
		{
			setchild(i - 1, branch_child[i]);//将需删除孩子节点后的孩子节点全向前移动一位
		}
	}
	//若有key删除则keynum减一
	if (delete_key == true)
	{
		keynum--;
	}
}

//从兄弟节点处借key和孩子节点
template<class keytype> void BTnode<keytype>::borrowFrom(BTnode* siblingNode, BTnode* parentNode, int keyIndex, bro_direction d)//从兄弟节点处借key和孩子节点
{
	switch (d)
	{
	case left://从左兄弟处借一个key和孩子节点
	{
		parentNode->setkey(keyIndex, siblingNode->key[siblingNode->keynum - 1]);//将父亲节点连接孩子节点处的key值改为孩子兄弟节点key末尾的key值
		insert(0, parentNode->key[keyIndex], 0, ((BTnode*)siblingNode)->branch_child[siblingNode->keynum]);//在该节点首处插入兄弟节点末尾处的key和孩子节点
		siblingNode->remove(keynum - 1, keynum);//删除兄弟节点中插入到该节点中的key和孩子节点
		break;
	}
	case right:
	{
		parentNode->setkey(keyIndex, siblingNode->key[0]);//将父亲节点连接孩子节点处的key值改为孩子兄弟节点key开头的key值
		insert(keynum, parentNode->key[keyIndex], keynum + 1, ((BTnode*)siblingNode)->branch_child[0]);//在该节点首处插入兄弟节点开头处的key和孩子节点
		siblingNode->remove(0, 0);//删除兄弟节点中插入到该节点中的key和孩子节点
		break;
	}
	default:
		break;
	}
}