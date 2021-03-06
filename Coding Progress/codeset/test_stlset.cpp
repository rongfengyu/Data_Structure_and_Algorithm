#include<iostream>  
#include<set>  
#include<string> 
#include<algorithm>
#include<vector> 

using namespace std;  

int main()  
{  
    set<string> s;  
    s.insert("eth0"); //第一次插入5，可以插入  
    s.insert("eth3");  
    s.insert("eth1");  
    s.insert("eth4");  
    s.insert("eth2"); //第二次插入5，重复元素，不会插入  
    set<string>::iterator it; //定义前向迭代器  
    //中序遍历集合中的所有元素  
    for(it = s.begin(); it != s.end(); it++)  
    {  
        cout << *it << " ";  
    }  
    
    cout << endl; 
    
    set<string> s1;  
    s1.insert("eth4"); //第一次插入5，可以插入  
    s1.insert("eth6");  
    s1.insert("eth8");  
    s1.insert("eth7");  
    s1.insert("eth9"); //第二次插入5，重复元素，不会插入
    
    // s1 = s;
    set<string>::iterator it1; //定义前向迭代器  
    //中序遍历集合中的所有元素  
    for(it1 = s1.begin(); it1 != s1.end(); it1++)  
    {  
        cout << *it1 << " ";  
    }  
    cout<<endl;
    
    
    //并集
    cout<<endl;
    cout <<"set_union:"<< endl;
    set<string> s_union;
    set_union(s.begin(),s.end(),s1.begin(),s1.end(),insert_iterator<set<string> >(s_union, s_union.begin()));
    string merge ="";
    for(it = (s_union.begin() ); it != s_union.end(); it++)  
    {  
        cout << *it << " ";  
        merge += *it + ",";
    } 
    cout<<endl;
    cout<<"merge1 = "<<merge.substr(0,merge.length() - 1)<<endl;
    
    
    //差集1
     cout<<endl;
    cout <<"set_difference1:"<< endl;
    set<string> s_difference;
    set_difference(s.begin(),s.end(),s1.begin(),s1.end(),insert_iterator<set<string> >(s_difference, s_difference.begin()));
    merge ="";
    for(it = (s_difference.begin() ); it != s_difference.end(); it++)  
    {  
        cout << *it << " ";  
        merge += *it + ",";
    } 
    cout<<endl;
    cout<<"merge2.1 = "<<merge.substr(0,merge.length() - 1)<<endl;
    
    //差集2
    cout<<endl;
    cout <<"set_difference2:"<< endl;
    s_difference.clear();
    set_difference(s1.begin(),s1.end(),s.begin(),s.end(),insert_iterator<set<string> >(s_difference, s_difference.begin()));
    merge ="";
    for(it = (s_difference.begin() ); it != s_difference.end(); it++)  
    {  
        cout << *it << " ";  
        merge += *it + ",";
    } 
    cout<<endl;
    cout<<"merge2.2 = "<<merge.substr(0,merge.length() - 1)<<endl;
    
    
    //交集
    cout<<endl;
    cout <<"set_intersection:"<< endl;
    set<string> s_intersection;
    merge = "";
    set_intersection(s.begin(),s.end(),s1.begin(),s1.end(),insert_iterator<set<string> >(s_intersection, s_intersection.begin()));
    for(it = (s_intersection.begin() ); it != s_intersection.end(); it++)
    {  
        cout << *it << " ";  
        merge += *it + ",";
    } 
    cout<<endl;
    cout<<"merge3 = "<<merge.substr(0,merge.length() - 1)<<endl;
    return 0;  
}  