#include <iostream>
#include <map>
#include <string>

std::map<std::string, std::string> parsetable() {
    std::map<std::string, std::string> dictionary;
    // Parse Table
    /* dictionary[stackTop inputSymbol] = "Y1 ... Yk"
    represents stackTop ----> Y1 ... Yk when we are processing inputSymbol*/
    dictionary["<Start> #include"]        = "<S> <N> <M>";
    dictionary["<S> #include"]            = "#include < iostream > <S>";
    dictionary["<P> <"]                   = "epsilon";
    dictionary["<K> <"]                   = "epsilon";
    dictionary["<P> >"]                   = "epsilon";
    dictionary["<K> >"]                   = "epsilon";
    dictionary["<Start> using"]           = "<S> <N> <M>";
    dictionary["<S> using"]               = "epsilon";
    dictionary["<N> using"]               = "using namespace std ;";
    dictionary["<Z> ;"]                   = ";";
    dictionary["<P> ;"]                   = "epsilon";
    dictionary["<Assign> ;"]              = "epsilon";
    dictionary["<F> ;"]                   = "epsilon";
    dictionary["<H> ;"]                   = "epsilon";
    dictionary["<Start> int"]             = "epsilon";
    dictionary["<S> int"]                 = "epsilon";
    dictionary["<N> int"]                 = "epsilon";
    dictionary["<M> int"]                 = "int main ( ) { <T> <V> }";
    dictionary["<T> int"]                 = "<Id> <T>";
    dictionary["<Id> int"]                = "int <L>";
    dictionary["<P> )"]                   = "epsilon";
    dictionary["<T> }"]                   = "epsilon";
    dictionary["<V> }"]                   = "epsilon";
    dictionary["<T> return"]              = "epsilon";
    dictionary["<V> return"]             = "return 0 ;";
    dictionary["<T> float"]               = "<Id> <T>";
    dictionary["<Id> float"]              = "float <L>";
    dictionary["<L> IDENTIFIER"]          = "IDENTIFIER <Assign> <Z>";
    dictionary["<T> IDENTIFIER"]          = "<L> <T>";
    dictionary["<Operation> IDENTIFIER"]  = "IDENTIFIER <P>";
    dictionary["<W> IDENTIFIER"]          = "IDENTIFIER";
    dictionary["<Expression> IDENTIFIER"] = "<Operation> <K> <Operation>";
    dictionary["<C> IDENTIFIER"]          = "IDENTIFIER";
    dictionary["<Z> ,"]                   = ", IDENTIFIER <Assign> <Z>";
    dictionary["<P> ,"]                   = "epsilon";
    dictionary["<Assign> ,"]              = "epsilon";
    dictionary["<Operation> NUMBER"]      = "NUMBER <P>";
    dictionary["<W> NUMBER"]              = "NUMBER";
    dictionary["<Expression> NUMBER"]     = "<Operation> <K> <Operation>";
    dictionary["<C> NUMBER"]              = "NUMBER";
    dictionary["<P> +"]                   = "<O> <W> <P>";
    dictionary["<O> +"]                   = "+";
    dictionary["<P> -"]                   = "<O> <W> <P>";
    dictionary["<O> -"]                   = "-";
    dictionary["<P> *"]                   = "<O> <W> <P>";
    dictionary["<O> *"]                   = "*";
    dictionary["<Assign> ="]              = "= <Operation>";
    dictionary["<P> =="]                  = "epsilon";
    dictionary["<K> =="]                  = "==";
    dictionary["<P> >="]                  = "epsilon";
    dictionary["<K> >="]                  = ">=";
    dictionary["<P> <="]                  = "epsilon";
    dictionary["<K> <="]                  = "<=";
    dictionary["<P> !="]                  = "epsilon";
    dictionary["<K> !="]                  = "!=";
    dictionary["<T> while"]               = "<Loop> <T>";
    dictionary["<Loop> while"]            = "while ( <Expression> ) { <T> }";
    dictionary["<T> cin"]                 = "<Input> <T>";
    dictionary["<Input> cin"]             = "cin >> IDENTIFIER <F> ;";
    dictionary["<F> >>"]                  = ">> IDENTIFIER <F>";
    dictionary["<T> cout"]                = "<Output> <T>";
    dictionary["<Output> cout"]           = "cout << <C> <H> ;";
    dictionary["<H> <<"]                  = "<< <C> <H>";
    dictionary["<C> STRING"]              = "STRING";

    return dictionary;
}