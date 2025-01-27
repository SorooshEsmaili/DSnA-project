#include <fstream>
#include <stack>
#include ".\codes\lex.cpp"
#include ".\codes\parse table.cpp"

// defining parse tree data structure
struct Node {
    std::string label;
    Node* LeftChild;
    Node* RightSibling;
    Node* Parent;
    Node(std::string lbl) : label(lbl) {} //constructor
};
std::map<TokenType, std::string> enum2str{
    {STRING, "STRING"},
    {NUMBER, "NUMBER"},
    {SYMBOL, "SYMBOL"},
    {IDENTIFIER, "IDENTIFIER"},
    {RESERVEDWORD, "RESERVEDWORD"}
};
// bonus part 1
std::string inedtifier_first_occurrence(std::string s, Node* M){
    if(!std::regex_match(s,IDENTIFIER_expr)){
        return "Error! Invalid name for an identifier.";
    }else{
        std::stack<Node*> DFS, FSD;
        DFS.push(M);
        Node* now;
        while(!DFS.empty()){
            now=DFS.top();
            DFS.pop();
            if(now->label==s){
                std::string t=now->Parent->Parent->LeftChild->label;//int or float
                t=t+" "+s+" ";
                std::vector<Node*> Q;
                Q.push_back(now->RightSibling);//<Assign>
                while(!Q.empty()){
                    now=Q[0];
                    Q.erase(Q.begin());
                    if(now->label.substr(0,1)=="<"){// is variable?
                    now=now->LeftChild;
                    while(now->label!="NIL"){
                    Q.push_back(now);
                    now=now->RightSibling;
                    }
                    }else if(now->label=="epsilon"){
                        continue;
                    }else{
                        t=t+now->label+" ";
                    }
                }
                return t;
            }else if(now->label.substr(0,1)!="<"){
                continue;
            }
            else{
                now=now->LeftChild;
                while(now->label!="NIL"){
                    FSD.push(now);
                    now=now->RightSibling;
                }
                while (!FSD.empty()){
                    DFS.push(FSD.top());
                    FSD.pop();
                }
            }

        }
       

    }
    return "No such variable is declared";
}

int main(){
    std::ifstream input("input.txt"); 
    std::string line;
    // reading the input string
    if (input.is_open()) {
        // tokenizing the input string
        std::vector<Token> tokenz;
        int ln=1;
        while (getline(input, line)) {
            std::vector<Token> newTokenz=tokenize(line,ln);
            tokenz.insert(tokenz.end(), newTokenz.begin(), newTokenz.end());
            ln++;
        }
        std::string tavaken[5]={"STRING","NUMBER", "SYMBOL","IDENTIFIER", "RESERVEDWORD"};
        // std::cout << "Tokenized string:\n";
        // for (const auto& token : tokenz) {
        // std::cout << "[" << tavaken[token.type]<< ", " << token.value << "]" << std::endl;
        // }
        // creating the token table
        std::vector<Token> tokenzSorted = tokenz;
        std::sort(tokenzSorted.begin(), tokenzSorted.end(),typecmp);
        // std::cout << "Token Table:\n";
        // for (const auto& token : tokenzSorted) {
        //     std::cout << "[" << tavaken[token.type]<< ", " << token.value <<  "\t h(k)= " << token.hash << "]" << std::endl;
        // }
        // Preparing token list, creating stack, and loading parse table
        std::map<std::string, std::string> ParseTable=parsetable();
        std::stack<std::string> st;
        // st.push("$");
        st.push("<Start>");
        // tokenz.push_back({STRING, "$",-1,-1});
        int i=0;
        Node* root = new Node("Program");
        Node* NIL = new Node("NIL");
        root->Parent=NIL;
        root->LeftChild=new Node("<Start>");
        Node* current = root->LeftChild;
        std::stack<std::string> temp;
        // parsing the input string
        while(!st.empty()){
            std::cout<<"currently in "<<current->label<<" with input: "<<tokenz[i].value<<"\n";
            if(current->label=="epsilon"){
            // if we're in an epsilon-node
                while(current->RightSibling->label=="NIL"){
                    current=current->Parent;
                }
                current=current->RightSibling;
                std::cout<<"moving to "<<current->label<<"\n";
            }
            else if(st.top()==tokenz[i].value){
                // if the current node represents a terminal other than NUMBER, STRING, IDENTIFIER
                current->LeftChild=NIL;
                // current->label=st.top();
                std::cout<<"Matched!"<<"\n";
                std::cout<<"popping "<<st.top()<<"\n";
                st.pop();
                i++;
                if(st.empty()){break;}
                while(current->RightSibling->label=="NIL"){
                    current=current->Parent;
                }
                current=current->RightSibling;
                std::cout<<"moving to "<<current->label<<"\n";
            }
            else if(st.top()==enum2str[tokenz[i].type]){
                // if the current node represents a one of { NUMBER, STRING, IDENTIFIER }
                current->LeftChild=NIL;
                // current->label=st.top();
                std::cout<<"Matched!"<<"\n";
                std::cout<<"popping "<<st.top()<<"\n";
                st.pop();
                i++;
                while(current->RightSibling->label=="NIL"){
                    current=current->Parent;
                }
                current=current->RightSibling;
                std::cout<<"moving to "<<current->label<<"\n";
            }
            else if(!ParseTable.count(st.top() + " " + tokenz[i].value) && (!ParseTable.count(st.top() + " " + enum2str[tokenz[i].type]))){
                // if the parse table has nothing for the current input symbol and st.top()
                std::cout << "Parse error 1 in line " << tokenz[i].linenumber <<"\n";
                break;
            }
            else if (ParseTable.count(st.top() + " " + tokenz[i].value )){
                // if the parse table has a rule for the current input symbol and st.top()
                std::string Rule(ParseTable[st.top() + " " + tokenz[i].value]);
                Rule=Rule+" ";
                std::cout<<"popping "<<st.top()<<"\n";
                st.pop();
                bool first_child=true;
                while(Rule.length()){
                    int j = Rule.find(" ");
                    if(first_child){
                        first_child=false;
                        current->LeftChild=new Node(Rule.substr(0,j));
                        current->LeftChild->Parent=current;
                        current=current->LeftChild;
                        std::cout<<"creating "<<current->label<<" under "<<current->Parent->label<<"\n";
                    }else{
                        current->RightSibling=new Node(Rule.substr(0,j));
                        current->RightSibling->Parent=current->Parent;
                        std::cout<<"creating "<<Rule.substr(0,j)<<" right of "<<current->label<<"\n";
                        current=current->RightSibling;
                    }
                    if(Rule.substr(0,j)!="epsilon"){
                        temp.push(Rule.substr(0,j));
                    }
                    Rule.erase(0, j+1);
                }
                current->RightSibling=new Node("NIL");
                current=current->Parent->LeftChild;
                std::cout<<"moving to "<<current->label<<"\n";
                while(!temp.empty()){
                    std::cout<<"pushing "<<temp.top()<<" into stack"<<"\n";
                    st.push(temp.top());
                    temp.pop();
                }
            }
            else if(ParseTable.count(st.top() + " " + enum2str[tokenz[i].type])){
                std::string Rule(ParseTable[st.top() + " " + enum2str[tokenz[i].type]]);
                Rule=Rule+" ";
                std::cout<<"popping "<<st.top()<<"\n";
                st.pop();
                bool first_child=true;
                while(Rule.length()){
                    int j = Rule.find(" ");
                    if(first_child){
                        first_child=false;
                        if(Rule.substr(0,j)==enum2str[tokenz[i].type]){current->LeftChild=new Node(tokenz[i].value);}
                        else{current->LeftChild=new Node(Rule.substr(0,j));}
                        current->LeftChild->Parent=current;
                        current=current->LeftChild;
                        std::cout<<"creating "<<current->label<<" under "<<current->Parent->label<<"\n";
                    }else{
                        if(Rule.substr(0,j)==enum2str[tokenz[i].type]){current->RightSibling=new Node(tokenz[i].value);}
                        else{current->RightSibling=new Node(Rule.substr(0,j));}
                        current->RightSibling->Parent=current->Parent;
                        std::cout<<"creating "<<Rule.substr(0,j)<<" right of "<<current->label<<"\n";
                        current=current->RightSibling;
                    }
                    if(Rule.substr(0,j)!="epsilon"){
                        temp.push(Rule.substr(0,j));
                    }
                    Rule.erase(0, j+1);
                }
                current->RightSibling=new Node("NIL");
                current=current->Parent->LeftChild;
                std::cout<<"moving to "<<current->label<<"\n";
                while(!temp.empty()){
                    std::cout<<"pushing "<<temp.top()<<" into stack"<<"\n";
                    st.push(temp.top());
                    temp.pop();
                }
            }
            else if(std::regex_match(st.top(),expr)){
                // if st.top() is a token other than the current input symbol
                std::cout << "Parse error 2 in  line " << tokenz[i].linenumber <<"\n";
                break;
            }
        }
        // bonus 1
        std::cout<<"first occurrence of s is:"<< inedtifier_first_occurrence("s",root->LeftChild->LeftChild->RightSibling->RightSibling);


    input.close();
    } else {
        std::cerr << "Unable to open file input.txt" << std::endl;
    }
    return 0;
};
