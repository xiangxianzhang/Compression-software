#include <iostream>
#include <string>
#include<functional>
#include <map>
#include <vector>
#include <queue>
#include <bitset>
#include <fstream>
#include <cassert>>
#include <algorithm>
using namespace std;
//-------------------- hufuman Node ------------------------------//
typedef struct Node
{

    char ch;
    int freq;
    Node* leftchild;
    Node* rightchild;
    Node* parent;
}Node,*pNode;

struct cmp1
{
    bool operator()(const pNode a, const pNode b)
    {
        return a->freq > b->freq;//这里好坑啊！！！！如果优先级低的要建立大顶堆
    }
};

//--------------------get <string,int>  dic-----------------------
void getdic2(map<char,int>& dic,const char* addr)
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
bool cmp(const pair<char, int> &a, const pair<char, int> &b)
{
	return a.second < b.second;
}

//--------------------get lower vector----------------------------
void getvec2(map<char,int>& dic,vector<pair<char,int> >& vec)
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

//--------------------get Node queue------------------------------
void getnopriority_queue( vector<pair<char,int> >& vec, priority_queue<pNode ,vector<pNode>,cmp1 > & deq)
{
    vector<pair<char,int> >::iterator q = vec.begin();
    while(q != vec.end())
    {
        pNode p_Node = new Node;
        p_Node->ch = q->first;
        p_Node->freq = q->second;
       // cout<< p_Node->ch <<" "<< p_Node->freq<<endl;
        p_Node->leftchild = NULL;
        p_Node->rightchild = NULL;
        p_Node->parent = NULL;
        deq.push(p_Node);
        q++;
    }
    //cout<<endl;
}

//----------------------------------------------------------------
pNode findParentNode(pNode &a, pNode &b)
{
    pNode parent = new Node;
    parent->freq = a->freq + b->freq;
    parent->leftchild = a;
    parent->rightchild = b;
    a->parent = NULL;
    b->parent = NULL;
    return parent;
}

//--------------------make hafuman tree---------------------------
pNode makhfmtr( priority_queue<pNode ,vector<pNode>,cmp1 >  dep)
{
    while(dep.size() >= 2)
    {
        pNode x,y;
        x = dep.top();dep.pop();
        y = dep.top();dep.pop();
        //cout<<x->freq<<" "<<y->freq<<endl;
        dep.push(findParentNode(x,y));
    }
    dep.top()->parent = NULL;
    return dep.top();
}

//--------------------利用DFS求哈夫曼编码---------------------------
map <char,string>  hfmList2;
vector<char> V;
void gethfmList2(pNode root)
{
   if(root->leftchild == NULL && root->rightchild == NULL)
   {
       string a;
       for(int i = 0; i < V.size(); i++)
              a += V[i];
       hfmList2[root->ch] = a;
       V.pop_back();
       return;

   }

   if(root->leftchild)
   {
       V.push_back('0');
       gethfmList2(root->leftchild);

   }
   if(root->rightchild)
   {
        V.push_back('1');
        gethfmList2(root->rightchild);
   }

   if(!V.empty())
   {
       V.pop_back();
   }
}


void smallerToFile(const char* addrYuan,const char* addrMudi)
{
    using namespace std;
    ifstream fin;
    fin.open(addrYuan);
    assert(fin.is_open());
    ofstream fout;
    fout.open(addrMudi,ios_base::binary);
    char chFromFile;
    char greatch = 0;
    string str;
    while(1)
    {
        fin.get(chFromFile);
        if(!fin) break;
        str += hfmList2[chFromFile];
        while(str.size() >= 8)
        {
            string str2(str,0,8);
            //cout<<" cxvdf";
            bitset<8> aaa(str2);
            greatch = (char)aaa.to_ulong();
            fout << greatch;
            str.erase(0,8);
        }
    }
    //////////////////////////////////////////////////////////////////////////
    char youxiao = str.size();
    for(char i = youxiao; i <= 7; i++)
    {
        str.append("0");
    }
    //cout<<str;
    bitset<8> aaa(str);
    chFromFile = (char)aaa.to_ulong();
    fout << chFromFile;
    fout << youxiao;//把最后一位剩余的有效位置存起来
    fin.close();
    fout.close();
}

void dicToFile(const char* addr,const  map <char,int> dic)
{
    ofstream fout("密码本.txt");
    map<char,int>::const_iterator pos = dic.begin();
    while(pos != dic.end())
    {
        fout << pos->first << " "<< pos->second << endl;
        pos++;
    }
    fout.close();
}


int main()
{

    map <char,int> dic;
    vector<pair<char,int> > vec;
    priority_queue<pNode ,vector<pNode>,cmp1 > deq;
    getdic2(dic,"原文.txt");
    getvec2(dic,vec);
    getnopriority_queue(vec,deq);
    pNode root = makhfmtr(deq);
    gethfmList2(root);
    dicToFile("密码本",dic);
    smallerToFile("原文.txt","压缩后.txt");
    return 0;
}
