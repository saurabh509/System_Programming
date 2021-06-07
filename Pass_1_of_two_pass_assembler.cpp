#include<bits/stdc++.h>
using namespace std;

struct field{
    int machine_code;
    string Class;
    int no_of_op;
};

struct symbol{
    string name;
    int address;
    int size;
};

struct literal{
	int sr; 
    string literal;
    int address;
};

bool isoperator(char c){
    return (c=='+' || c=='-' || c=='*' || c=='/');
}

int LC=0,PTP=0,LTP=0,STP=0,line_num=0,literal_p=1;
string int_code;
unordered_map<string,field> m;
unordered_map<string,int> registers;
vector<symbol> symbol_table;
vector<literal> literal_table;
vector<int> pool_table;

void init(unordered_map<string,field> &m){
    m["STOP"]={0,"IS",0};
    m["ADD"]={1,"IS",2};
    m["SUB"]={2,"IS",2};
    m["MULT"]={3,"IS",2};
    m["MOVER"]={4,"IS",2};
    m["MOVEM"]={5,"IS",2};
    m["COMP"]={6,"IS",2};
    m["BC"]={7,"IS",2};
    m["DIV"]={8,"IS",2};
    m["READ"]={9,"IS",1};
    m["PRINT"]={10,"IS",1};
    m["LOAD"]={11,"IS",1};
    m["START"]={1,"AD",1};
    m["END"]={2,"AD",1};
    m["LTORG"]={3,"AD",1};
    m["ORIGIN"]={4,"AD",1};
    m["EQU"]={5,"AD",1};
    m["DC"]={1,"DL",1};
    m["DS"]={2,"DL",1};
    
    registers["AREG"]=1;
    registers["BREG"]=2;
    registers["CREG"]=3;
    registers["DREG"]=4;
    //LC=100;
}

void get_words(){

    FILE* fp=fopen("f.txt","r");
    char c;
    /*while((c=getchar())!='*'){
        fputc(c,fp);
    }
    
    fclose(fp);*/
    
    //fp=fopen("f.txt","r");
    
    char buffer[100];
    int k=0;
    
    while((c=fgetc(fp))!=EOF){
        if(c=='\n'){
            line_num++;
            buffer[k]=0;
            k=0;
            int word_c=0;
            char ans[100];
            int p=0;
            int i=0;
            int f=0;
            while(buffer[i]){
                if(buffer[i]==' ' || buffer[i]=='\t'){
                    if(f){
                        ans[p]=' ';
                        p++;
                    }
                    while(buffer[i]==' ' || buffer[i]=='\t')i++; 
                }
                else{
                    ans[p]=buffer[i];
                    f=1;
                    p++;i++;
                }
            }
            i=0;
            ans[p]=0;
            
            if(ans[0]==' ')word_c--;
            if(ans[p-1]==' ')word_c--;  
            string temp;
            
            vector<string> words;
            
            while(ans[i]){
                if(ans[i]==' '){
                    words.push_back(temp);
                    temp="";
                }
                else temp+=ans[i];
                i++;
            }
            if(temp.length()){
                words.push_back(temp);
            }
            
            
            vector<string> temp_words;
            for(int i=0;i<words.size();i++){
                if(words[i]!="="){
                    temp_words.push_back(words[i]);
                }
                else{
                    temp_words.push_back("="+words[i+1]);
                    i++;
                }
            }
            words=temp_words;
                        
            
            //cout<<ans<<"  ";
            for(auto x:words){
                cout<<x<<"  ";
            }
            
            if(words.size()==4){
            	bool f=false;
                if(symbol_table.size()>0){
                    for(auto y:symbol_table){
                        if(y.name==words[0]){
                            f=true;
                            break;
                        }
                    }
                }
                if(!f){
                    symbol_table.push_back({words[0],LC,1});
                    auto it=find(words.begin(),words.end(),words[0]);
                    words.erase(it);
                }
                /*for(int STP=0;STP<symbol_table.size();STP++){
                    if(symbol_table[STP].name==words[0]){
                        int_code+="(ST "+ to_string(STP+1) +")";    
                    }
                }*/
            }
            else if(words.size()==3){
            	bool f=false;
                if(symbol_table.size()>0){
                    for(auto y:symbol_table){
                        if(y.name==words[0]){
                            f=true;
                            break;
                        }
                    }
                }
                if(!f && words[1]!="EQU" && words[1]!="DS" && words[1]!="DC" && m.find(words[0])==m.end()){
                    symbol_table.push_back({words[0],LC,1});
                    auto it=find(words.begin(),words.end(),words[0]);
                    words.erase(it);
                }
            }
            
            //cout<<words.size()<<" ";
            int store=1;
            bool isorigin=false;
            
            for(auto x:words){
            	bool only_num=true;
            	for(auto y:x){
            		if(y>='0' && y<='9'){
            			
            		}
            		else only_num=false;
            	}
            	
            	if(only_num){
            		int_code+="(C "+x+")";
            		//cout<<"  "<<LC++<<"  "<<int_code<<endl;
            		//int_code="";
            		//continue;
            	}
            	else{
                //special cases
		            if(line_num==1){
		                LC=0;
		                store=stoi(words.back());
		            }
		            
		            if(x=="END"){
		            	//LC++;
		            	bool ispooldone=false;
						for(int LTP=0;LTP<literal_table.size();LTP++){
							if(literal_table[LTP].address==-1){
								if(!ispooldone){
									pool_table.push_back(literal_table[LTP].sr);
									ispooldone=true;
								}
								literal_table[LTP].address=LC++;
								cout<<literal_table[LTP].literal<<" ";
							}
						}
		            	cout<<"(AD 2)"<<endl;
		            	return;
		            }
		            
		            if(x=="LTORG"){
		                //process literals 
		                bool ispooldone=false;
		                for(int LTP=0;LTP<literal_table.size();LTP++){
		                    if(literal_table[LTP].address==-1){
		                    	if(!ispooldone){
		                    		pool_table.push_back(literal_table[LTP].sr);
		                    		ispooldone=true;
		                    	}
		                    	literal_table[LTP].address=LC++;
		                    	cout<<literal_table[LTP].literal<<" ";
		                    }
		                }
		                if(ispooldone)
		                	LC--;
		                
		            }
		            if(x=="ORIGIN"){
		            	isorigin=true;
		            	int i=0;
		            	string temp;
		            	bool allnums=true;
		            	
		            	for(i=0;i<words[1].size();i++){
		            		if(!isdigit(words[1][i])){
		            			allnums=false;
		            			break;
		            		}
		            	}
		            	int add=0;
		            	if(allnums){
		            		//LC=stoi(words[1]);
		            		add=stoi(words[1]);
		            	}
		            	else{
				            for(i=0;i<words[1].length() && !isoperator(words[1][i]);i++){
				            	temp+=words[1][i];
				            }
				            //int add=0;
				            //cout<<temp<<endl;
				            for(int STP=0;STP<symbol_table.size();STP++){
				            	if(symbol_table[STP].name==temp){
				            		add=symbol_table[STP].address;
				            	}
				            }
				            //LC=add;
				            temp="";
				            if(i<words[1].size()){
						        if(words[1][i]=='+'){
						        	i++;
						        	for(;i<words[1].length();i++){
						        		temp+=words[1][i];
						        	}
						        	add+=stoi(temp);
						        	//LC+=stoi(temp);
						        }
						        else if(words[1][i]=='-'){
						        	i++;
						        	for(;i<words[1].length();i++){
						        		temp+=words[1][i];
						        	}
						        	//LC-=stoi(temp);
						        	add-=stoi(temp);
						        }
						        
						        //cout<<"HI:"<<add<<endl;
						        symbol_table.push_back({words[1],add,-1});
				            }
		                }
		                store=add;
		            }
		            
		            if(x=="EQU"){
		            	int i=0;
		            	string temp;
		            	
		            	for(i=0;i<words[2].length() && !isoperator(words[2][i]);i++){
				            temp+=words[2][i];
				        }
			            int add=0;
			            //cout<<temp<<endl;
			            for(int STP=0;STP<symbol_table.size();STP++){
			            	if(symbol_table[STP].name==temp){
			            		add=symbol_table[STP].address;
			            	}
			            }
			            
			            temp="";
			            if(i<words[2].size()){
					        if(words[2][i]=='+'){
					        	i++;
					        	for(;i<words[2].length();i++){
					        		temp+=words[2][i];
					        	}
					        	add+=stoi(temp);
					        }
					        else if(words[2][i]=='-'){
					        	i++;
					        	for(;i<words[2].length();i++){
					        		temp+=words[2][i];
					        	}
					        	add-=stoi(temp);
					        }
					        
					        //cout<<"HI:"<<add<<endl;
					        //symbol_table.push_back({words[2],add,-1}); 
			            }
			            for(int STP=0;STP<symbol_table.size();STP++){
				        	if(symbol_table[STP].name==words[0]){
	                        	symbol_table[STP].address=add; 
	                        	break; 
	                    	}
				        }
				        store=0;
				        
		            }
		            
		            if(x=="DS"){
		            	//cout<<x<<endl;
		            	auto it=find(words.begin(),words.end(),"DS");
		            	it--;
		            	bool done=false;
		                for(int STP=0;STP<symbol_table.size();STP++){
		                    if(symbol_table[STP].name==*it){
		                        symbol_table[STP].address=LC; 
		                        done=true;  
		                        store=stoi(words[2]);
		                        break; 
		                    }
		                }
		            }
		            
		            if(x=="DC"){
		            	auto it=find(words.begin(),words.end(),"DC");
		            	it--;
		            	bool done=false;
		                for(int STP=0;STP<symbol_table.size();STP++){
		                    if(symbol_table[STP].name==*it){
		                        symbol_table[STP].address=LC; 
		                        done=true;  
		                        break; 
		                    }
		                }
		            }
		            
		            if(m.find(x)!=m.end()){
		                temp=m[x].Class+" "+to_string(m[x].machine_code);
		                int_code+="("+temp+")";     
		            }
		            else if(registers.find(x)!=registers.end()){
		                int_code+="(R "+to_string(registers[x])+")";
		            }
		            else if(x.find("=")<x.length()){
		                bool f=false;
		                if(literal_table.size()>0){
		                    for(auto y:literal_table){
		                        if(y.literal==x && y.address==-1){
		                            f=true;
		                            break;
		                        }
		                    }
		                }
		                if(!f){
		                    literal_table.push_back({literal_p++,x,-1});
		                    for(int LTP=literal_table.size()-1;LTP>=0;LTP--){
				                if(literal_table[LTP].literal==x){
				                    int_code+="(LT "+ to_string(LTP+1) +")"; 
				                    break;   
				                }
				            }
		                }
		                else{
				            for(int LTP=0;LTP<literal_table.size();LTP++){
				                if(literal_table[LTP].literal==x){
				                    int_code+="(LT "+ to_string(LTP+1) +")";    
				                }
				            }
		                }
		            }
		            else{
		                //cout<<x<<endl;
		                bool f=false;
		                if(symbol_table.size()>0){
		                    for(auto y:symbol_table){
		                        if(y.name==x){
		                            f=true;
		                            break;
		                        }
		                    }
		                }
		                if(!f){
		                    symbol_table.push_back({x,-1,-1});
		                }
		                for(int STP=0;STP<symbol_table.size();STP++){
		                    if(symbol_table[STP].name==x){
		                        int_code+="(ST "+ to_string(STP+1) +")";    
		                    }
		                }
		            }
                }
            }
            //cout<<words.size()<<" ";
            cout<<"  	"<<LC<<"  	"<<int_code<<endl;
            if(isorigin){
            	LC=store;
            	isorigin=false;
            }
            else LC+=store;
            int_code="";
            
        }
        else{
            if(isdigit(c) || c=='=' || isalpha(c) || c==' ' || c=='\t' || isoperator(c)){
                buffer[k]=c;
                k++;
            }
            else{
                buffer[k]=' ';
                k++;
            }
        }
    }
    
    fclose(fp);     
}

int main(){ 
    init(m);
    get_words();
    cout<<endl;
    
    cout<<"LITERAL TABLE"<<endl;
    for(auto x:literal_table){
    	cout<<x.sr<<". "<<x.literal<<" "<<x.address<<endl;
    }
    cout<<endl;
    cout<<"SYMBOL TABLE"<<endl;
    for(auto x:symbol_table){
    	cout<<x.name<<"  "<<x.address<<endl;
    }
    cout<<endl;
    cout<<"POOL TABLE"<<endl;
    for(int i=0;i<pool_table.size();i++){
    	cout<<i+1<<" "<<" #"<<pool_table[i]<<endl;
    }
}


