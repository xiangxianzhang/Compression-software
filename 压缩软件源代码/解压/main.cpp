#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <utility> //pair
#include <bitset>	 //位运算
#include <fstream>
#include <cassert>>//assert
#include <algorithm>//sort
using namespace std;
//-------------------- hufuman node ------------------------------//
typedef struct node
{

    char ch;
    int freq;
    node* leftchild;
    node* rightchild;
    node* parent;
}node,*pnode;

struct cmp2
{
    bool operator()(const pnode a, const pnode b)
    {
        return a->freq > b->freq;
    }
};

//--------------------get <string,int>  dic-----------------------
void getdic(map<char,int>& dic,const char* addr)
{
    char ch;
    ifstream fin;
    fin.open(addr);
    assert(fin.is_open());
    while(fin.get(ch))
    {
        dic[ch]++;
    }
    fin.close();
}

//----------------------------------------------------------------
bool cmp(const pair<char,int> &a,const pair<char,int> &b)
{
    return a.second < b.second;
}
//--------------------get lower vector----------------------------
void getvec(map<char,int>& dic,vector<pair<char,int> >& vec)
{
    using namespace std;
    map<char,int>::iterator p = dic.begin();
    while(p != dic.end())
    {
        vec.push_back(pair<char,int>(p->first,p->second));
        p++;
    }
    sort(vec.begin(),vec.end(),cmp);
}

//--------------------get node queue------------------------------
void getnopriority_queue(const vector<pair<char,int> >& vec, priority_queue<pnode ,vector<pnode>,cmp2 > & deq)
{
    vector<pair<char,int> >::const_iterator q = vec.begin();//pair用const_iterator
    while(q != vec.end())
    {
        pnode p_node = new node;
        p_node->ch = q->first;
        p_node->freq = q->second;
        p_node->leftchild = NULL;
        p_node->rightchild = NULL;
        p_node->parent = NULL;
        deq.push(p_node);
        q++;
    }
}

//----------------------------------------------------------------
pnode findParentNode(pnode &a, pnode &b)
{
    pnode parent = new node;
    parent->freq = a->freq + b->freq;
    parent->leftchild = a;
    parent->rightchild = b;
    a->parent = parent;
    b->parent = parent;
    return parent;
}

//--------------------make hafuman tree---------------------------
pnode makhfmtr( priority_queue<pnode ,vector<pnode>,cmp2 >  dep)
{
    while(dep.size() >= 2)
    {
        pnode x,y;
        x = dep.top();dep.pop();
        y = dep.top();dep.pop();
        dep.push(findParentNode(x,y));
    }
    dep.top()->parent = NULL;
    return dep.top();
}

//-------------------------解压缩---------------------------------------
void jiemiToFile(const char* addrYuan,const char* addrMudi,node* hfmhead)
{
	using namespace std;
	ifstream fin;

	ofstream fout;
	fout.open(addrMudi);

	char ch;
	char quan;
	char youxiao;
	node* ptr = hfmhead;

	fin.open(addrYuan,ios_base::binary);
	assert(fin.is_open());
	fin.seekg(-1,ios_base::end);
	ifstream::pos_type pos;
	fin.get(youxiao);
    fin.seekg(-2,ios_base::end);

	pos = fin.tellg();
	fin.seekg(0,ios_base::beg);

	int i = 0;
	while(1){
		if(fin.tellg() == pos) break;
		fin.get(ch);
		bitset<8> aaa(ch);
		for(i = 7;i >= 0;i--){
			if(0 == aaa[i]){
				ptr = ptr->leftchild;
				if(NULL == ptr->leftchild){
					fout << ptr->ch;
					ptr = hfmhead;
				}
			}else if(1 == aaa[i]){
				ptr = ptr->rightchild;
				if(NULL == ptr->leftchild){
					fout << ptr->ch;
					ptr = hfmhead;
				}
			}
		}
	}
	fin.get(ch);
	bitset<8> last(ch);
	for(i = 7;i >= (8-youxiao);i--){
		if(0 == last[i]){
			ptr = ptr->leftchild;
			if(NULL == ptr->leftchild){
				fout << ptr->ch;
				ptr = hfmhead;
			}
		}else if(1 == last[i]){
			ptr = ptr->rightchild;
			if(NULL == ptr->leftchild){
				fout << ptr->ch;
				ptr = hfmhead;
			}
		}
	}
	fin.close();
	fout.close();
}


//--------------------------------------------------------------------------
void dicFromFile(const char* addr,map<char,int> &dic)
{
	ifstream fin("密码本.txt");
	char ch;
	int num;
	while(1){
		fin.get(ch);
		if(!fin) break;
		fin >> num;
		dic[ch] = num;
		fin.get(ch);
	}
	fin.close();
}

int main()
{
	map <char,int> dic;
	vector<pair<char,int> > vec;
    priority_queue<pnode ,vector<pnode>,cmp2 > deq;
    dicFromFile("密码本.txt",dic);
    getvec(dic,vec);
    getnopriority_queue(vec,deq);
    node* head = makhfmtr(deq);
	jiemiToFile("压缩后.txt","解压后.txt",head);
	return 0;
}
