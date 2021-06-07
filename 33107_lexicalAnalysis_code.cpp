#include<bits/stdc++.h>

using namespace std;

set<string> terminal;
vector<string> tokens;
vector<string> literal_table,identifier_table;

struct field{
	string symbol;
	string type;
	int index;
};

vector<field> UST;

void init(){
	terminal.insert("{");terminal.insert("}");
	terminal.insert("[");terminal.insert("]");
	terminal.insert("(");terminal.insert(")");
	terminal.insert("+");terminal.insert("-");
	terminal.insert("*");terminal.insert("/");
	terminal.insert("++");terminal.insert("--");
	terminal.insert("=");terminal.insert("==");
	terminal.insert(">");terminal.insert("<");
	terminal.insert(">=");terminal.insert("<=");
	terminal.insert(";");terminal.insert(":");
	terminal.insert(",");terminal.insert("?");
	terminal.insert("&");
	terminal.insert("int");terminal.insert("float");
	terminal.insert("double");terminal.insert("auto");
	terminal.insert("break");
	terminal.insert("case");terminal.insert("char");
	terminal.insert("const");terminal.insert("continue");
	terminal.insert("default");terminal.insert("do");
	terminal.insert("else");terminal.insert("enum");
	terminal.insert("extern");terminal.insert("for");
	terminal.insert("goto");terminal.insert("if");
	terminal.insert("long");terminal.insert("register");
	terminal.insert("return");terminal.insert("short");
	terminal.insert("signed");terminal.insert("sizeof");
	terminal.insert("static");terminal.insert("struct");
	terminal.insert("switch");terminal.insert("typedef");
	terminal.insert("union");terminal.insert("unsigned");
	terminal.insert("void");terminal.insert("volatile");
	terminal.insert("while");
}

void make_identifier_literal(){
	for(auto x:tokens){
		if(terminal.count(x)){
			continue;
		}
		else if(x[0]=='"' && x[x.length()-1]=='"'){
			if(find(literal_table.begin(),literal_table.end(),x)==literal_table.end())
				literal_table.push_back(x);
		}
		else if(isdigit(x[0])){
			bool digit=true;
			for(auto y:x){
				if(!isdigit(y)){
					digit=false;
				}
			}
			if(digit){
				if(find(literal_table.begin(),literal_table.end(),x)==literal_table.end())
					literal_table.push_back(x);
			}
			else{
				if(find(identifier_table.begin(),identifier_table.end(),x)==identifier_table.end())
					identifier_table.push_back(x);
			}
		}	
		else{
			if(find(identifier_table.begin(),identifier_table.end(),x)==identifier_table.end())
				identifier_table.push_back(x);
		}
	}
	
	
	cout<<"\n----LITERAL TABLE----"<<endl;
	int ind=0;
	for(auto x:literal_table){
		cout<<ind++<<".	"<<x<<endl;
	}
	ind=0;
	cout<<"\n----IDENTIFIER TABLE----"<<endl;
	for(auto x:identifier_table){
		cout<<ind++<<".	"<<x<<endl;
	}
}

void make_ust(){
	for(auto x:tokens){
		if(terminal.count(x)){
			int ind=0;
			for(auto y:terminal){
				if(y==x)break;
				ind++;
			}
			UST.push_back({x,"TRM",ind});
		}
		else if(find(literal_table.begin(),literal_table.end(),x)!=literal_table.end()){
			int ind=0;
			for(auto y:literal_table){
				if(y==x){
					break;
				}
				ind++;
			}
			UST.push_back({x,"LIT",ind});
		}
		else{
			int ind=0;
			for(auto y:identifier_table){
				if(y==x){
					break;
				}
				ind++;
			}
			UST.push_back({x,"IDN",ind});
		}
	}
	cout<<"\n----UNIFORM SYMBOL TABLE----"<<endl;
	for(auto x:UST){
		cout<<x.symbol<<" 		"<<x.type<<" 		"<<x.index<<endl;
	}
}

int main(){
	init();
	FILE* fp=fopen("code.txt","r");
	
	char buffer[1000];
	int k=0;
	char c;
	bool iscomm=false;
	while((c=getc(fp))!=EOF){
		if(c=='\n'){
			buffer[k++]=' ';
			//buffer[k++]=' ';
			//buffer[k++]=' ';
			buffer[k]=0;
			k=0;
			int i=0;
			
			if(buffer[0]){
				string temp;
				bool literal=false;
				
				while(buffer[i]){
					
					if(buffer[i]=='/' && buffer[i+1]=='/')break;
					
					if(buffer[i]=='/' && buffer[i+1]=='*'){
						iscomm=true;
					}
					
					if(iscomm && buffer[i]=='*' && buffer[i+1]=='/'){
						iscomm=false;
						i+=2;
						continue;
					}
					
					if(iscomm){
						i++;continue;
					}
					
					if(buffer[i]=='"')literal=!literal;
					
					if(!literal && (buffer[i]==' ' || buffer[i]=='\t')){
						if(temp.length()){
							tokens.push_back(temp);
							temp="";
						}
						i++;
						continue;
					}
	
					temp+=buffer[i];
					
					//check only if next char is some operator.
					
					if(!(isdigit(buffer[i+1]) || isalpha(buffer[i+1]))){
						string s;s+=buffer[i+1];
						if(!literal && terminal.count(s)){
							
							/*temp='+'
							s=buffer[i+1]='+'
							++*/
							
							if(terminal.count(temp+s)){
								tokens.push_back(temp+s);
								temp="";
								i++;
							}
							else{
								tokens.push_back(temp);
								temp="";
							}
						}
					}
					//
					//check if next is a space or current is not a (digit,number).
					if(!literal && (buffer[i+1]==' ' || !(isdigit(buffer[i]) || isalpha(buffer[i])) && terminal.count(temp))){
						tokens.push_back(temp);
						temp="";
					}
					
					i++;
				}
			}
		}
		else{
			buffer[k++]=c;			
		}
	}	
	
	for(auto x:tokens){
		cout<<x<<endl;
	}

	make_identifier_literal();
	make_ust();
}
