#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<ctype.h> 
#include <regex>
#include <string>
#include <set>
#include <queue>
using namespace std;
 
void writeToFile(std::queue<string> q, string nameOfFile){
    ofstream outfile (nameOfFile);
    cout<<"\n\nOUTPUT OF THE FILE:\n\n";

   while (!q.empty()){
    cout<< q.front() << " " << endl;
    outfile << q.front() << endl;
    q.pop();
  }
   outfile.close();
}

int isKeyword(char buffer[]){
    string str(buffer);
    string keywords[32] = {"auto","break","case","char","const","continue","default",
                            "do","double","else","enum","extern","float","for","goto",
                            "if","int","long","register","return","short","signed",
                            "sizeof","static","struct","switch","typedef","union",
                            "unsigned","void","volatile","while"};
    int i, flag = 0;
    
    for(i = 0; i < 32; ++i){
         if(!keywords[i].compare(str)){
            flag = 1;
            break;
        }
    }    
    return flag;
}

int isComparison(char buffer[]){
    string str(buffer);
    string keywords[6] = {"==","!=","<",">","<=",">="};
    int i, flag = 0;
    
    for(i = 0; i < 6; ++i){
        if(!keywords[i].compare(str)){
            flag = 1;
            break;
        }
    }    
    return flag;
}

bool isRealNumber(char buffer[]){
  string str(buffer);
  regex real ("[+-]?\\d*\\.\\d+"); //, regex_constants::icase);
  regex nat ("^(0|[1-9][0-9]*)$");
  return regex_search(str, real) || regex_search(str, nat);
}

bool isVariable(char buffer[]){
    string str(buffer);
    regex var("^[a-zA-Z_$][a-zA-Z_$0-9]*$");
    return regex_search(str, var);
}


int main(int argc, char *argv[]){
    set<string> tableOfSymbols;
    queue<string> realTableOfSymbols;
    char ch, buffer[15], operators[] = "%+-*/", assign = '=', specialSymbols[] = "[]{}(),;", strBuffer[200];
    ifstream fin(argv[1]);
    int i,j=0,k=0,sumCheckSS=0,sumCheckC=0;
    
    if(!fin.is_open()){
        cout<<"Error while opening the file\n";
        exit(0);
    }
    
    while(!fin.eof()){
        ch = fin.get();         
        
          if (ch == '"') {
             ch = fin.get();
             while(ch != '"'){
                strBuffer[k++] = ch;
                ch = fin.get();
             }
               strBuffer[k] = '\0';
               k=0;
               cout<<strBuffer<<" is a String\n";
               string tmpStr(strBuffer);
               if (!tableOfSymbols.count(tmpStr + " 7"))
                        realTableOfSymbols.push(tmpStr + " 7");
               tableOfSymbols.insert(tmpStr + " 7");
         
          }

       else if(ch == '/' and fin.peek() == '/') { // Potencial Bug :v
             ch = fin.get();
             ch = fin.get();
             while(ch != '\n'){
                strBuffer[k++] = ch;
                ch = fin.get();
             }
               strBuffer[k] = '\0';
               k=0;
               cout<<strBuffer<<" is a Comment\n";
                string tmpStr(strBuffer);
                if (!tableOfSymbols.count(tmpStr + " 8"))
                        realTableOfSymbols.push(tmpStr + " 8");
               tableOfSymbols.insert(tmpStr + " 8");
          }


        else if(ch == '/' and fin.peek() == '*') { // Potencial Bug :v
             ch = fin.get();
             ch = fin.get();
             while(ch != '*' and fin.peek() != '/'){
                if(ch != '\n')
                    strBuffer[k++] = ch;
                ch = fin.get();
             }
               strBuffer[k] = '\0';
               k=0;
               cout<<strBuffer<<" is a Comment\n";
              string tmpStr(strBuffer);
              if (!tableOfSymbols.count(tmpStr + " 8"))
                        realTableOfSymbols.push(tmpStr + " 8");
              tableOfSymbols.insert(tmpStr + " 8");
          }

       else {

        for (i = 0; i < 8; ++i){          
          if(ch == specialSymbols[i]){
               sumCheckSS+=1;
               break;
             }
        }

         for(i = 0; i < 5; ++i){              
               if(ch == operators[i])
                   sumCheckC+=1;             
           }

        if(isalnum(ch) || ch == '!' || ch == '='|| ch == '<'|| ch == '>' || ch == '_' || ch == '.'){
             buffer[j++] = ch;
         } 


        else if((ch == ' ' || ch == '\n' || !isalnum(ch)) && (j != 0) && ch!='_'){
                 buffer[j] = '\0';
                 j = 0;
                     
                 if (strcmp("=", buffer) == 0){
                      cout<<"= is an assingment symbol\n"; 
                      string tmpStr(buffer);
                      if (!tableOfSymbols.count(tmpStr + " 4"))
                        realTableOfSymbols.push(tmpStr + " 4");
                      tableOfSymbols.insert(tmpStr + " 4");              
                 }
                 else if(isKeyword(buffer) == 1){
                     cout<<buffer<<" is a keyword\n";
                     string tmpStr(buffer);
                     if (!tableOfSymbols.count(tmpStr + " 0"))
                        realTableOfSymbols.push(tmpStr + " 0");
                     tableOfSymbols.insert(tmpStr + " 0");
                 }
                 else if (isComparison(buffer) == 1){
                     cout<<buffer<<" is a comparison\n"; 
                     string tmpStr(buffer);
                     if (!tableOfSymbols.count(tmpStr + " 5"))
                        realTableOfSymbols.push(tmpStr + " 5");
                     tableOfSymbols.insert(tmpStr + " 5");
                 }
                 else if (isRealNumber(buffer)){
                     cout<<buffer<<" is a constant\n";
                     string tmpStr(buffer);
                     if (!tableOfSymbols.count(tmpStr + " 6"))
                        realTableOfSymbols.push(tmpStr + " 6");
                     tableOfSymbols.insert(tmpStr + " 6");
                 }
                 else if(isVariable(buffer)) {
                     cout<<buffer<<" is an indentifier\n"; 
                     string tmpStr(buffer);
                     if (!tableOfSymbols.count(tmpStr + " 1"))
                        realTableOfSymbols.push(tmpStr + " 1");
                     tableOfSymbols.insert(tmpStr + " 1");
                 }
         } 

         if(sumCheckSS >= 1){
           sumCheckSS = 0;
           cout<<ch<<" is a special symbol\n";
           if (!tableOfSymbols.count(string(1,ch) + " 2"))
                realTableOfSymbols.push(string(1,ch) + " 2");
           tableOfSymbols.insert(string(1,ch) + " 2");
        }
       
         if(sumCheckC >= 1){
           sumCheckC = 0;
           cout<<ch<<" is a operator\n";
           if (!tableOfSymbols.count(string(1,ch) + " 3"))
                 realTableOfSymbols.push(string(1,ch) + " 3");
           tableOfSymbols.insert(string(1,ch) + " 3");
            }   
        }                    
    }    
    fin.close();    
    writeToFile(realTableOfSymbols,"tableOfSymbols.txt");
    return 0;
}
