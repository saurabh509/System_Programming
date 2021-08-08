#include<bits/stdc++.h>
using namespace std;
bool S(string);
bool A(string);
bool T(string);
/*
--GRAMMAR--
 S-->abT
 T-->S|A
 A-->cd|c|epsilon
*/
bool S(string s){
//cout<<s<<endl;
    if(s.length()>=2 && s[0]=='a' && s[1]=='b'){
        return T(s.substr(2,s.length()));
    }
    return false;
}

bool T(string s){
    return (S(s) || A(s));
}
bool A(string s){
//cout<<"A:"<<s<<endl;
    if(s.length()==2 && s[0]=='c' && s[1]=='d'){
        return A(s.substr(2,s.length()));
    }
    else if(s.length()==1 && s[0]=='c'){
        return A(s.substr(1,s.length()));
    }
    else if(s.length()==0){
        return true;
    }
    return false;
}
int main(){
    cout<<"ENTER THE NUMBER OF STRINGS TO ANALYSE:";
    int n;
    cin>>n;
    cout<<endl;
    while(n--){
        cout<<"ENTER THE STRING TO ANALYSE:";
        string s;
        cin>>s;
        if(S(s)){
            cout<<"YES,the sting belongs to language"<<endl;
        }
        else{
            cout<<"NO,the sting belongs to language"<<endl;
        }
        cout<<endl;
    }
}
