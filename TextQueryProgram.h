#ifndef program_12_3
#define program_12_3

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>

using namespace std;

//通过指针可以避免拷贝
class QueryResult{
    friend ostream& print(ostream&, const QueryResult&);
public:
    QueryResult(string s, 
                shared_ptr<set<int> > p, 
                shared_ptr<vector<string> > f):sought(s), lines(p), file(f){}
private:
    string sought;
    shared_ptr<set<int> > lines;
    shared_ptr<vector<string> > file;
};

class TextQuery{
public:
    TextQuery(ifstream &infile):text(new vector<string>){
        string s;
        while(getline(infile, s)){
            text->push_back(s);
            //对于每一个单词，保存其行号
            int n = text->size()-1;
            istringstream line(s); //绑定一行字符串，然后以空格为分隔符把该行分隔开来
            string word;
            while(line >> word){
                shared_ptr<set<int> > &line_set = word2line[word]; //or auto
                if(!line_set){
                    line_set.reset(new set<int>);
                }
                line_set->insert(n);
            }
        }
    }

    QueryResult query(string &sought){
        //QueryResult query(const string &sought) const
        shared_ptr<set<int> > nodata(new set<int>);
        //static shared_ptr<set<int> > nodata(new set<int>); //静态对象是一种从构造开始到程序结束都存在的一种对象,是独立于栈与堆的
        auto loc = word2line.find(sought);//避免将单词添加到map里面
        if(loc == word2line.end()){
            return QueryResult(sought, nodata, text);
        }
        else return QueryResult(sought, loc->second, text);
    }
private:
    shared_ptr<vector<string> > text; //如果不初始化，就会初始化空指针
    map<string, shared_ptr<set<int> > > word2line;
};

ostream &print(ostream &os, const QueryResult &qr){
    os<<qr.sought<<"occurs"<<qr.lines->size()<<endl;
    for(auto num : *(qr.lines)){
        os<<"\t(line "<<num+1<<")"<<*(qr.file->begin()+num)<<endl;
    }
    return os;
}
#endif