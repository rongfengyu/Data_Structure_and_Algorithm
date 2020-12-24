#include<iostream>  
#include<set>  
#include<string> 
#include<algorithm>
#include<vector> 

using namespace std;  

int main()  
{  
    set<string> s;  
    s.insert("eth0"); //��һ�β���5�����Բ���  
    s.insert("eth3");  
    s.insert("eth1");  
    s.insert("eth4");  
    s.insert("eth2"); //�ڶ��β���5���ظ�Ԫ�أ��������  
    set<string>::iterator it; //����ǰ�������  
    //������������е�����Ԫ��  
    for(it = s.begin(); it != s.end(); it++)  
    {  
        cout << *it << " ";  
    }  
    
    cout << endl; 
    
    set<string> s1;  
    s1.insert("eth4"); //��һ�β���5�����Բ���  
    s1.insert("eth6");  
    s1.insert("eth8");  
    s1.insert("eth7");  
    s1.insert("eth9"); //�ڶ��β���5���ظ�Ԫ�أ��������
    
    // s1 = s;
    set<string>::iterator it1; //����ǰ�������  
    //������������е�����Ԫ��  
    for(it1 = s1.begin(); it1 != s1.end(); it1++)  
    {  
        cout << *it1 << " ";  
    }  
    cout<<endl;
    
    
    //����
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
    
    
    //�1
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
    
    //�2
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
    
    
    //����
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