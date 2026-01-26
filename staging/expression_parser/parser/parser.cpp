#include <iostream>
#include <cstdint>
#include <list>
#include <set>
#include <string>
#include <cmath>
#include <stack>


class parser {
    private:
    enum token_type {UNKN = 0, OPERATOR, OPEN_PAREN, CLOSE_PAREN, DIGIT, NUMBER};
    using i64 = int64_t;
    union token_val {
        char op;
        i64 num;
    };

    struct token {
        token_val value;
        bool is_num = false; //not necessary, ttype should suffice.
        token_type ttype = UNKN;
    };

    using lit_c = std::list<token>::const_iterator;
    using lit = std::list<token>::iterator;

    public:
    parser(std::string& input): input_str(input) {}

    bool evaluate()
    {
        tokenize();

        if (detect_errors() == false)
            return false;

        reduce_parens();
        reduce_operators(token_list.begin(), token_list.end());

        return true;
    }

    void print_token_list()
    {
        std::list<token>::const_iterator tit = token_list.begin();
        while (tit != token_list.end()) {
            token t = *tit;
            if (t.is_num == true)
                std::cout << t.value.num;
            else
                std::cout << t.value.op;
                
            std::cout << " ";
            tit++;
        }
        std::cout << std::endl;
    }

    //reduce_parens and reduce_operators should return an error which
    //should be the serial number of the token.
    private:
    bool detect_errors() 
    {
        lit lst_it = token_list.begin();
        std::stack<char> paren_stack;
        token_type current_state = OPERATOR;
        
        while (lst_it != token_list.end()) {
            switch (lst_it->ttype) {
                case OPEN_PAREN:
                    paren_stack.push('p');
                    break;
                case CLOSE_PAREN:
                    if (paren_stack.size() == 0)
                        return false;
                    paren_stack.pop();
                    break;
                case NUMBER:
                    if (current_state == NUMBER)
                        return false;
                    current_state = NUMBER;
                    break;
                case OPERATOR:
                    if (current_state == OPERATOR)
                        return false;
                    current_state = OPERATOR;
                    break;
                default:
                    break;
            }
            lst_it = std::next(lst_it);
        }
        
        if (paren_stack.size() != 0)
            return false;

        return true;
    }
   
    void reduce_parens()
    {
        lit lst_it = token_list.begin();
        while (lst_it != token_list.end()) {
            if (lst_it->ttype == CLOSE_PAREN) {
                lit cp = lst_it;
                lit op = lst_it;
                while (op->ttype != OPEN_PAREN)
                    op = std::prev(op);
                reduce_operators(op, cp);
                lst_it = std::next(cp);
                token_list.erase(op);
                token_list.erase(cp);
                if (lst_it == token_list.end()) 
                    break;
            } else
                lst_it = std::next(lst_it);
        }
    }

    i64 resolve_operator(i64 ln, char op, i64 rn)
    {
        std::cout << "RO " << ln << ":" << op << ":" << rn << std::endl;
        long double r;
        switch (op) {
            case '^':
                r = powl(static_cast<long double>(ln),
                         static_cast<long double> (rn));
                return static_cast<i64>(r);
            case '*':
                return ln * rn;
            case '/':
                return ln / rn;
            case '%':
                return ln % rn;
            case '+':
                return ln + rn;
            case '-':
                return ln - rn;
            default:
                return 0;
        }
    }

    void reduce_operators(lit lst_b, lit lst_e) 
    {
        for (auto& op:{'^','*','/','%','+','-'}) {
            lit tl_it = lst_b;
            while (tl_it != lst_e) {
                if ((tl_it->is_num == false) && (tl_it->value.op == op)) {
                    lit pt = std::prev(tl_it);
                    lit ct = tl_it;
                    lit nt = std::next(tl_it);
                    pt->value.num = resolve_operator(pt->value.num, op,
                                                     nt->value.num);
                    pt->is_num = true;
                    tl_it = pt;
                    token_list.erase(ct);
                    token_list.erase(nt);
                } else 
                    tl_it++;
            }
        }
    }
    
    bool is_digit(char c)
    {
        if ((c >= '0') && (c <= '9')) 
            return true;

        return false;
    }

    void tokenize() {
        std::string num_str;
        int i = 0;
        int k;
        while (i < input_str.size()) {
            token t;
            t.ttype = classify_token(input_str[i]);
            switch (t.ttype) {
                case OPERATOR:
                    if ((input_str[i] == '-') && 
                        (is_digit(input_str[i + 1]) == true)) {
                        i++;
                        break;
                    }
                    t.value.op = input_str[i];
                    token_list.push_back(t);
                    i++;
                    break;
                case OPEN_PAREN:
                    t.value.op = input_str[i];
                    token_list.push_back(t);
                    i++;
                    break;
                case CLOSE_PAREN:
                    t.value.op = input_str[i];
                    token_list.push_back(t);
                    i++;
                    break;
                case DIGIT:
                    k = i;
                    num_str = "";
                    while((i < input_str.size()) && 
                                (is_digit(input_str[i]) == true)) {
                        num_str += input_str[i++];
                    }
                    t.is_num = true;
                    t.ttype = NUMBER;
                    t.value.num = std::strtoll(num_str.c_str(), nullptr, 10);
                    if ((k > 0) && (input_str[k - 1] == '-'))
                        t.value.num *= -1;
                    token_list.push_back(t);
                    break;
                case UNKN:
                    i++;
                    break;
            }
        }
    }
   
    token_type classify_token(char c)
    {
        if ((c >= '0') && (c <= '9'))
            return DIGIT;

        if (c == '(')
            return OPEN_PAREN;

        if (c == ')')
            return CLOSE_PAREN;

        if (operator_set.find(c) != operator_set.end())
            return OPERATOR;

        return UNKN;
    }
    
    private:
    std::string input_str;
    std::list<token> token_list;
    std::set<char> operator_set = {'+', '-', '*', '/', '%', '^'};
};

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Error" << std::endl;
        return -1;
    }

    std::string istr = argv[1];
    parser p(istr);
    if (p.evaluate() == false)
        std::cout << "Error" << std::endl;
    p.print_token_list();
}
