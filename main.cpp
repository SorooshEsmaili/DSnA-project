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
// bonus 2: invalid declaration
std::vector<int> invalid_declaration(std::vector<Token> T){
    std::vector<int> err;
    for (const auto& token : T) {
        if(token.value=="int" || token.value=="float"){
            if(token.type==RESERVEDWORD){
                if(T[token.hash+1].type==IDENTIFIER){
                    if(T[token.hash+2].value=="="){
                        if(!std::regex_match(T[token.hash+3].value, NUMBER_expr)){
                            err.push_back(1), err.push_back(token.linenumber);
                            return err;
                        }
                    }
                }
            }
        }
        else if(token.value=="std" && T[token.hash+1].value!=";"){
            err.push_back(2), err.push_back(token.linenumber);
            return err;
        }
        else if(token.value=="return" && T[token.hash+2].value!=";"){
            err.push_back(2), err.push_back(token.linenumber);
            return err;
        }
        else if(token.value=="cin"){
            int jump=1;
            while(1){
            if(T[token.hash+jump].value==";"){
                break;
            }else if(T[token.hash+jump].value==">>"){
                jump=jump+2;
            }else{
                err.push_back(2), err.push_back(T[token.hash+jump].linenumber);
            return err;
            }
            }
        }
        else if(token.value=="cout"){
            int jump=1;
            while(1){
            if(T[token.hash+jump].value==";"){
                break;
            }else if(T[token.hash+jump].value=="<<"){
                jump=jump+2;
            }else{
                err.push_back(2), err.push_back(T[token.hash+jump].linenumber);
                return err;
            }
            }
        }
        else if(token.type==IDENTIFIER){
            int jump=0;
            while(1){
                if(T[token.hash+jump+1].value==";"){
                    break;
                }else if(T[token.hash+jump+2].type==IDENTIFIER || T[token.hash+jump-2].value=="while"){
                    break;
                }else if(T[token.hash+jump+2].type==NUMBER){
                    jump=jump+2;
                }else {
                    err.push_back(2), err.push_back(T[token.hash+jump].linenumber);
                    return err;
                }
            }
        }
    }
    err.push_back(0);
    return err;


}
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
    std::ofstream log("log.txt");
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
        log << "Tokenized string:\n";
        for (const auto& token : tokenz) {
        log << "[" << tavaken[token.type]<< ", " << token.value << "]" << std::endl;
        }
        log << "\n\n\n";
        //bonus 2
        std::vector<int> isvalid = invalid_declaration(tokenz);
        if(isvalid[0]==1){
            log << "Invalid declaration of a variable in line "<< isvalid[1];
            return 0;
        }else if(isvalid[0]==2){
            log << "semicolon missing in line "<< isvalid[1];
            return 0;
        }else{ log << "all variable declarations are valid!" << "\n\n\n";}
        // creating the token table
        std::vector<Token> tokenzSorted = tokenz;
        std::sort(tokenzSorted.begin(), tokenzSorted.end(),typecmp);
        log << "Token Table:\n";
        for (const auto& token : tokenzSorted) {
            log << "[" << tavaken[token.type]<< ", " << token.value <<  "\t h(k)= " << token.hash << "]" << std::endl;
        }
        log << "\n\n\n";
        // Preparing token list, creating stack, and loading parse table
        std::map<std::string, std::string> ParseTable=parsetable();
        std::stack<std::string> st;
        // st.push("$");
        st.push("<Start>");
        tokenz.push_back({STRING, "$",-1,-10});
        int i=0;
        Node* root = new Node("Program");
        Node* NIL = new Node("NIL");
        root->Parent=NIL;
        root->LeftChild=new Node("<Start>");
        Node* current = root->LeftChild;
        std::stack<std::string> temp;
        // parsing the input string
        log << "tree geneeration";
        while(!st.empty()){
            if(tokenz[i].value=="$"){
                log << "parser ran out of input before finishing";
                return 0;
            }
            log <<"currently in "<<current->label<<" with input: "<<tokenz[i].value<<"\n";
            if(current->label=="epsilon"){
            // if we're in an epsilon-node
                while(current->RightSibling->label=="NIL"){
                    current=current->Parent;
                }
                current=current->RightSibling;
                log <<"moving to "<<current->label<<"\n";
            }
            else if(st.top()==tokenz[i].value){
                // if the current node represents a terminal other than NUMBER, STRING, IDENTIFIER
                current->LeftChild=NIL;
                // current->label=st.top();
                log <<"Matched!"<<"\n";
                log <<"popping "<<st.top()<<"\n";
                st.pop();
                i++;
                if(st.empty()){break;}
                while(current->RightSibling->label=="NIL"){
                    current=current->Parent;
                }
                current=current->RightSibling;
                log <<"moving to "<<current->label<<"\n";
            }
            else if(st.top()==enum2str[tokenz[i].type]){
                // if the current node represents a one of { NUMBER, STRING, IDENTIFIER }
                current->LeftChild=NIL;
                // current->label=st.top();
                log <<"Matched!"<<"\n";
                log <<"popping "<<st.top()<<"\n";
                st.pop();
                i++;
                while(current->RightSibling->label=="NIL"){
                    current=current->Parent;
                }
                current=current->RightSibling;
                log <<"moving to "<<current->label<<"\n";
            }
            else if(!ParseTable.count(st.top() + " " + tokenz[i].value) && (!ParseTable.count(st.top() + " " + enum2str[tokenz[i].type]))){
                // if the parse table has nothing for the current input symbol and st.top()
                log  << "Parse error 1 in line " << tokenz[i].linenumber <<"\n";
                break;
            }
            else if (ParseTable.count(st.top() + " " + tokenz[i].value )){
                // if the parse table has a rule for the current input symbol and st.top()
                std::string Rule(ParseTable[st.top() + " " + tokenz[i].value]);
                Rule=Rule+" ";
                log <<"popping "<<st.top()<<"\n";
                st.pop();
                bool first_child=true;
                while(Rule.length()){
                    int j = Rule.find(" ");
                    if(first_child){
                        first_child=false;
                        current->LeftChild=new Node(Rule.substr(0,j));
                        current->LeftChild->Parent=current;
                        current=current->LeftChild;
                        log <<"creating "<<current->label<<" under "<<current->Parent->label<<"\n";
                    }else{
                        current->RightSibling=new Node(Rule.substr(0,j));
                        current->RightSibling->Parent=current->Parent;
                        log <<"creating "<<Rule.substr(0,j)<<" right of "<<current->label<<"\n";
                        current=current->RightSibling;
                    }
                    if(Rule.substr(0,j)!="epsilon"){
                        temp.push(Rule.substr(0,j));
                    }
                    Rule.erase(0, j+1);
                }
                current->RightSibling=new Node("NIL");
                current=current->Parent->LeftChild;
                log <<"moving to "<<current->label<<"\n";
                while(!temp.empty()){
                    log <<"pushing "<<temp.top()<<" into stack"<<"\n";
                    st.push(temp.top());
                    temp.pop();
                }
            }
            else if(ParseTable.count(st.top() + " " + enum2str[tokenz[i].type])){
                std::string Rule(ParseTable[st.top() + " " + enum2str[tokenz[i].type]]);
                Rule=Rule+" ";
                log <<"popping "<<st.top()<<"\n";
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
                        log <<"creating "<<current->label<<" under "<<current->Parent->label<<"\n";
                    }else{
                        if(Rule.substr(0,j)==enum2str[tokenz[i].type]){current->RightSibling=new Node(tokenz[i].value);}
                        else{current->RightSibling=new Node(Rule.substr(0,j));}
                        current->RightSibling->Parent=current->Parent;
                        log <<"creating "<<Rule.substr(0,j)<<" right of "<<current->label<<"\n";
                        current=current->RightSibling;
                    }
                    if(Rule.substr(0,j)!="epsilon"){
                        temp.push(Rule.substr(0,j));
                    }
                    Rule.erase(0, j+1);
                }
                current->RightSibling=new Node("NIL");
                current=current->Parent->LeftChild;
                log <<"moving to "<<current->label<<"\n";
                while(!temp.empty()){
                    log <<"pushing "<<temp.top()<<" into stack"<<"\n";
                    st.push(temp.top());
                    temp.pop();
                }
            }
            else if(std::regex_match(st.top(),expr)){
                // if st.top() is a token other than the current input symbol
                log << "Parse error 2 in  line " << tokenz[i].linenumber <<"\n";
                break;
            }
        }
        if(tokenz[i].value!="$"){
                log << "parser finished but input remains";
                return 0;
        }
        log << "\n\n\n";
        // bonus 1
        log <<"first occurrence of s is:"<< inedtifier_first_occurrence("s",root->LeftChild->LeftChild->RightSibling->RightSibling);


    input.close();
    } else {
        std::cerr << "Unable to open file input.txt" << std::endl;
    }
    return 0;
};
