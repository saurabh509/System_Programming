#include<bits/stdc++.h>
using namespace std;

struct symbol{
    string name;
    int address;
};

struct literal{
    string literal;
    int address;
};

struct intcode{
	int lc;
	vector<pair<string,string>> v;
};

vector<symbol> symbol_table;
vector<literal> literal_table;
vector<intcode> intcode_table;

int LC_end=0;

void process_symbol_table(){

	FILE* fp=fopen("symbol.txt","r");
	char c;	 
	char buffer[1000];
	int k=0;
	while((c=fgetc(fp))!=EOF){
	 	if(c=='\n'){
	 		buffer[k]=0;
	 		k=0;
	 		int i=0;
	 		string sym;
	 		int add=0;
	 		
	 		if(buffer[0]){
		 		while(buffer[i]!=' '){
		 			sym+=buffer[i];
		 			i++;
		 		}
		 		while(buffer[i]==' ')i++;
		 		while(buffer[i]){
		 			add=(add*10+(buffer[i]-'0'));
		 			i++;
		 		}
		 		symbol_table.push_back({sym,add});
		 		buffer[0]=0;
	 		}
	 	}
	 	else if(isalpha(c) || isdigit(c) || c==' ' || c=='=' || c=='+' || c=='-'){
			buffer[k++]=c;
		}
	}

	fclose(fp);
}

void process_literal_table(){
	FILE* fp=fopen("literal.txt","r");
	char c;	 
	char buffer[1000];
	int k=0;
	while((c=fgetc(fp))!=EOF){
	 	if(c=='\n'){
	 		buffer[k]=0;
	 		k=0;
	 		int i=0;
	 		string lit;
	 		int add=0;
	 		
	 		if(buffer[0]){
		 		while(buffer[i]!=' '){
		 			lit+=buffer[i];
		 			i++;
		 		}
		 		while(buffer[i]==' ')i++;
		 		while(buffer[i]){
		 			add=(add*10+(buffer[i]-'0'));
		 			i++;
		 		}
		 		literal_table.push_back({lit,add});
		 		buffer[0]=0;
	 		}
	 	}
	 	else if(isalpha(c) || isdigit(c) || c==' ' || c=='='){
			buffer[k++]=c;
		}
	}

	fclose(fp);
}

void process_int_code(){
	FILE* fp=fopen("intcode.txt","r");
	char c;	 
	char buffer[1000];
	int k=0;
	while((c=fgetc(fp))!=EOF){
	 	if(c=='\n'){
	 		buffer[k]=0;
	 		k=0;
	 		int i=0;
	 		int add=0;
	 		intcode I;
	 		if(buffer[0]){
	 			string temp;
	 			while(buffer[i]!='('){
	 				if(buffer[i]!=' ')temp+=buffer[i];
	 				i++; 
	 			}
	 			int lc=-1;
	 			//cout<<temp<<endl;
	 			if(temp[0]!='-')lc=stoi(temp);
	 			
	 			I.lc=lc;
	 			pair<string,string> p;
	 			while(buffer[i]){
	 				if(buffer[i]=='(')temp="";
	 				else if(buffer[i]==')'){
	 					p.second=temp;
	 					I.v.push_back(p);
	 					temp="";
	 				}
	 				else if(buffer[i]==' '){
	 					p.first=temp;
	 					temp="";
	 				}
	 				else{
	 					temp+=buffer[i];
	 				}
	 				i++;
	 			}
	 			intcode_table.push_back(I);
				cout<<I.lc<<" ";
				for(auto x:I.v){
					cout<<"("<<x.first<<" "<<x.second<<")";
				} 
				//cout<<endl;
				
				
				//WRITE processing code for pass 2
				
				bool isIS=false;
				bool isLTORG=false;
				bool isDC=false;
				
				string spaces="     ";
				
				for(auto x:I.v){
					if(x.first=="IS"){
						isIS=true;
						for(int s=0;s<5-I.v.size();s++){
							cout<<spaces;
						}
						cout<<I.lc<<") "<<"+";
						for(auto x:I.v){
							if(x.first=="IS" && x.second.length()==1){
								cout<<"0"+x.second<<" ";
							}
							else if(x.first=="IS"){
								cout<<x.second<<" ";
							}
							else if(x.first=="R"){
								cout<<"0"+x.second<<" ";
							}
							else if(x.first=="ST"){
								cout<<symbol_table[stoi(x.second)-1].address<<" ";
								//cout<<stoi(x.second)-1<<" ";
							}
							else if(x.first=="LT"){
								cout<<literal_table[stoi(x.second)-1].address<<" ";
								//cout<<stoi(x.second)-1<<" ";
							}
							//cout<<endl;
						}
						break;
					}
				}	
				//if(isIS)continue;
				
				for(auto x:I.v){
					if(x.first=="AD" && x.second=="3"){
						isLTORG=true;
						for(int s=0;s<5-I.v.size();s++){
							cout<<spaces;
						}
						cout<<I.lc<<") ";
						for(auto x:I.v){
							if(x.first=="LT"){
cout<<literal_table[stoi(x.second)-1].literal[0]<<"'"<<literal_table[stoi(x.second)-1].literal[1]<<"'"<<" ";
							}
						}
						break;
					}
				}
				
				for(auto x:I.v){
					if(x.first=="DL" && x.second=="1"){
						isDC=true;
						for(int s=0;s<5-I.v.size();s++){
							cout<<spaces;
						}
						cout<<I.lc<<") ";
						for(auto x:I.v){
							if(x.first=="C"){
								cout<<"'"<<x.second<<"'"<<" ";
							}
						}
						break;
					}
				}
				LC_end=max(LC_end,I.lc);
				for(auto x:I.v){
					if(x.first=="AD" && x.second=="2"){
						LC_end++;
						for(auto x:literal_table){
							
							if(x.address>=LC_end){
								for(int s=0;s<5-I.v.size();s++){
									cout<<spaces;
								}
								cout<<LC_end<<") ";
								cout<<x.literal[0]<<"'"<<x.literal[1]<<"'"<<" ";
							}
						}
						break;
					}
				}
				
				cout<<endl;
	 		}
	 	}
	 	else if(isalpha(c) || isdigit(c) || c==' ' || c=='=' || c=='-' || c==')' || c=='(' || c=='\t'){
			buffer[k++]=c;
		}
	}

	fclose(fp);
}

int main(){
	process_literal_table(); 
	process_symbol_table();
	process_int_code();
	
	cout<<endl;
	cout<<"---SYMBOL TABLE---"<<endl;
	for(auto x:symbol_table){
		cout<<x.name<<" "<<x.address<<endl;
	}
	cout<<endl;
	cout<<"---LITERAL TABLE---"<<endl;
	for(auto x:literal_table){
		cout<<x.literal<<" "<<x.address<<endl;
	}
}


