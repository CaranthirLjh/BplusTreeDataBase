#include"BPlus_node.h"

///�����ඨ��
INEXSIST_KEY inexsist_key;//keyֵ������
KEY_OVERFLOW key_overflow;//keyֵ��������
CHILD_OVERFLOW child_overflow;//child��������

///�ڵ����Ա����ʵ��
template<class keytype> BTnode<keytype>::BTnode()
{
	isleaf = true;
	keynum = 0;
}

template<class keytype> BTnode<keytype>::~BTnode()
{

}

//�����±귵�ض�Ӧ��keyֵ,����������throwһ��������
template<class keytype> keytype BTnode<keytype>::getkey(int index)//�����±귵�ض�Ӧ��keyֵ,����������throwһ��������
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

//����keyֵ���ض�Ӧ���±�,����������throwһ��������
template<class keytype> int BTnode<keytype>::getindex(keytype k)//����keyֵ���ض�Ӧ���±�,����������throwһ��������
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

//�����±�ı�keyֵ�����������key���򱨴�
template<class keytype> void BTnode<keytype>::setkey(int index, keytype k)//�����±�ı�keyֵ�����������key���򱨴�
{
	if (index < max_interal_order)
	{
		if (index < keynum)
		{
			key[index] = k;
		}
		/*else//����key�Ŀ�ָ�봦��ֵ��keynum��һ
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

//�����±�ı�childֵ
template<class keytype> void BTnode<keytype>::setchild(int index, BTnode* child)//�����±�ı�childֵ,���������child�򱨴�
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

//���ظýڵ����丸�ڵ��е��±�
template<class keytype> int BTnode<keytype>::getfatherindex()//���ظýڵ����丸�ڵ��е��±�
{
	for (int i = 0; i < branch_father->keynum; i++)
	{
		if (branch_father->branch_child[i] == this)
		{
			return i;
		}
	}
}

//��ָ���±�ĺ��ӽڵ㴦�ָ�ڵ�
template<class keytype> void BTnode<keytype>::split(BTnode*parent, int childindex)//��ָ���±�ĺ��ӽڵ㴦�ָ�ڵ�
{
	BTnode* newnode = new BTnode();//��������µ����ֵܽڵ�
	for (int i = 0; i < min_internal_order; i++)
	{
		newnode->setkey(i, key[min_internal_order + i]);//�����·��ѽڵ��keyֵ
		newnode->keynum++;
	}
	for (int i = 0; i < min_child; i++)
	{
		newnode->setchild(i, branch_child[min_child + i]);//�����·��ѽڵ��child
		branch_child[min_child + i] = NULL;//ɾ����child��ԭ�ڵ��е�ָ��
	}
	keynum = max_interal_order - newnode->keynum;//����ԭ�ڵ��key������

	((BTnode*)parent)->insert(childindex, childindex + 1, key[min_internal_order], newnode);
}

//��ָ��������key���ӽڵ�
template<class keytype> void BTnode<keytype>::insert(int keyIndex, keytype key, int childIndex, BTnode* childNode)//��ָ��������key���ӽڵ�
{
	//��key���в������
	if (keyIndex >= 0 && keyIndex < max_interal_order)
	{
		for (int i = keynum - 1; i >= keyIndex; i--)
		{
			setkey(i + 1, key[i]);//��Ҫ����λ�õĽڵ㼰��֮��Ľڵ�����ƶ�һλ
		}
		setkey(keyIndex, key);//������key
		keynum++;
	}
	//�Ժ��ӽڵ���в������
	if (childIndex >= 0 && childIndex < max_child)
	{
		for (int i = keynum; i >= childIndex; i--)
		{
			setchild(i + 1, branch_child[i]);//��Ҫ����λ�õĺ��ӽڵ㼰��֮��ĺ��ӽڵ�����ƶ�һλ
		}
		setchild(childIndex, childNode);//�����º��ӽڵ�
	}
}

//ɾ��key���ӽڵ�
template<class keytype> void BTnode<keytype>::remove(int keyIndex, int childIndex)//ɾ��key���ӽڵ�
{
	bool delete_key = false;//�ж��Ƿ���key��ɾ��
	//ɾ��key����
	if (keyIndex >= 0 && keyIndex < max_interal_order)
	{
		delete_key = true;
		for (int i = keyIndex + 1; i < keynum; i++)
		{
			setkey(i - 1, key[i]);//����ɾ��key���keyȫ��ǰ�ƶ�һλ
		}
	}
	//ɾ�����ӽڵ����
	if (childIndex >= 0 && childIndex < max_child)
	{
		for (int i = childIndex + 1; i <= keynum; i++)
		{
			setchild(i - 1, branch_child[i]);//����ɾ�����ӽڵ��ĺ��ӽڵ�ȫ��ǰ�ƶ�һλ
		}
	}
	//����keyɾ����keynum��һ
	if (delete_key == true)
	{
		keynum--;
	}
}

//���ֵܽڵ㴦��key�ͺ��ӽڵ�
template<class keytype> void BTnode<keytype>::borrowFrom(BTnode* siblingNode, BTnode* parentNode, int keyIndex, bro_direction d)//���ֵܽڵ㴦��key�ͺ��ӽڵ�
{
	switch (d)
	{
	case left://�����ֵܴ���һ��key�ͺ��ӽڵ�
	{
		parentNode->setkey(keyIndex, siblingNode->key[siblingNode->keynum - 1]);//�����׽ڵ����Ӻ��ӽڵ㴦��keyֵ��Ϊ�����ֵܽڵ�keyĩβ��keyֵ
		insert(0, parentNode->key[keyIndex], 0, ((BTnode*)siblingNode)->branch_child[siblingNode->keynum]);//�ڸýڵ��״������ֵܽڵ�ĩβ����key�ͺ��ӽڵ�
		siblingNode->remove(keynum - 1, keynum);//ɾ���ֵܽڵ��в��뵽�ýڵ��е�key�ͺ��ӽڵ�
		break;
	}
	case right:
	{
		parentNode->setkey(keyIndex, siblingNode->key[0]);//�����׽ڵ����Ӻ��ӽڵ㴦��keyֵ��Ϊ�����ֵܽڵ�key��ͷ��keyֵ
		insert(keynum, parentNode->key[keyIndex], keynum + 1, ((BTnode*)siblingNode)->branch_child[0]);//�ڸýڵ��״������ֵܽڵ㿪ͷ����key�ͺ��ӽڵ�
		siblingNode->remove(0, 0);//ɾ���ֵܽڵ��в��뵽�ýڵ��е�key�ͺ��ӽڵ�
		break;
	}
	default:
		break;
	}
}