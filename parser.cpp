//
// Created by Jannis Grimm on 9/17/22.
// This is Project 1 for CSE340 with Bazzi
// The program takes an input text which consists of a token name, its corresponding grammar, and an input string.
// Lexical analysis is then performed on the inout.
//

#include "parser.h"
#include "lexer.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <set>
#include <algorithm>

using namespace std;
string tokenName = "";

vector<Token> list;
vector<Token> returnList;
vector<NameAndREG> nameAndREGList;

//Method printing out if there has been a syntax error
void Parser::syntax_error() {
    cout << "SYNTAX ERROR IN EXPRESSION OF " << tokenName << "\n";
    exit(1);
}

//Method printing out if there has been a snytax error
void Parser::snytax_error() {
    cout << "SNYTAX ERORR\n";
    exit(1);
}

//Method taking the expected type and returning the token if there is no syntax error
Token Parser::expectSyntax(TokenType expected_type) {
    Token t = lexer.GetToken();

    if (t.token_type != expected_type) {
        syntax_error();
    }
    return t;
}

//Method taking the expected type and returning the token if there is no snytax error
Token Parser::expectSnytax(TokenType expected_type) {
    Token t = lexer.GetToken();

    if (t.token_type != expected_type) {
        snytax_error();
    }
    return t;
}

//Method called from the parse_token() and returns the *REG recursively
REG * Parser::parse_expr() {

    Token t;
    t = lexer.peek(1);

    if (t.token_type == CHAR) {

        expectSnytax(CHAR);

        //REGs
        REG_node * startingN = new REG_node();
        REG_node * acceptN = new REG_node();

        startingN->first_label = t.lexeme[0];
        startingN->first_neighbor = acceptN;

        REG * reg = new REG();

        reg->start = startingN;
        reg->accept = acceptN;

        return reg;
    }

    else if (t.token_type == UNDERSCORE) {

        expectSnytax(UNDERSCORE);

        //REGs
        REG_node * startingN = new REG_node();
        REG_node * acceptN = new REG_node();

        startingN->first_label = '_';
        startingN->first_neighbor = acceptN;

        REG * reg = new REG();

        reg->start = startingN;
        reg->accept = acceptN;

        return reg;
    }

    else if (t.token_type == LPAREN) {

        expectSyntax(LPAREN);

        REG * reg = parse_expr();
        expectSyntax(RPAREN);

        Token t2;
        t2 = lexer.peek(1); //Checking for what follows

        //THREE POSSIBLE CASES

        if (t2.token_type == STAR) {
            expectSnytax(STAR);

            //REGs
            REG_node * start = new REG_node();
            REG_node * accept = new REG_node();

            start->first_label = '_';
            start->first_neighbor = reg->start;

            start->second_label = '_';
            start->second_neighbor = accept;

            reg->accept->first_label = '_';
            reg->accept->first_neighbor = reg->start;

            reg->accept->second_label = '_';
            reg->accept->second_neighbor = accept;

            REG * starExpr = new REG();

            starExpr->accept = accept;
            starExpr->start = start;

            return starExpr;

        }

        else if (t2.token_type == OR) {

            expectSnytax(OR);
            expectSyntax(LPAREN);
            REG * reg2 = parse_expr();
            expectSyntax(RPAREN);

            REG_node *startingN = new REG_node();
            REG_node * acceptingN = new REG_node();

            startingN->first_label = '_';
            startingN->second_label = '_';
            startingN->first_neighbor = reg->start;
            startingN->second_neighbor = reg2->start;

            reg->accept->first_label = '_';
            reg->accept->first_neighbor = acceptingN;
            reg2->accept->first_label = '_';
            reg2->accept->first_neighbor = acceptingN;

            REG * comExpr = new REG();

            comExpr->start = startingN;
            comExpr->accept = acceptingN;

            return comExpr;

        }

        else if (t2.token_type == DOT) {

            expectSnytax(DOT);
            expectSyntax(LPAREN);
            REG * reg2 = parse_expr();
            expectSyntax(RPAREN);

            //REGs
            REG_node *startingN1;
            REG_node *acceptingN1;

            startingN1 = reg->start;

            reg->accept->first_label = '_';
            reg->accept->first_neighbor = reg2->start;

            acceptingN1 = reg2->accept;

            REG * comExpr = new REG();

            comExpr->start = startingN1;
            comExpr->accept = acceptingN1;

            return comExpr;

        }

        else {
            syntax_error();
            return NULL;
        }

    }

    else {
        syntax_error();
        return NULL;
    } //Syntax error

}

//Method called from the parse_token_list() and calls methods according to grammar (one of them is the REG construction)
void Parser::parse_token() {

    Token t = expectSnytax(ID);
    tokenName = t.lexeme;
    list.push_back(t);

    //Add REG and its token name to a struct vector
    REG * reg = parse_expr();

    //Adding token name and its reg to a list
    NameAndREG nameAndReg;
    nameAndReg.token_name = t.lexeme;
    nameAndReg.reg = reg;
    nameAndREGList.push_back(nameAndReg);

}

//Method called from the parse_token_section() and calls methods according to grammar
void Parser::parse_token_list() {
    Token t;
    parse_token();
    t = lexer.peek(1);

    if (t.token_type == COMMA) {
        expectSnytax(COMMA);
        parse_token_list();
    }

}

//Method called from parse_input() and calls methods according to the grammar
void Parser::parse_tokens_section() {
    parse_token_list();
    expectSnytax(HASH);
}

//Method taking in the input and calling methods according to the grammar
void Parser::parse_input() {
    parse_tokens_section();
    expectSnytax(INPUT_TEXT);
    expectSnytax(END_OF_FILE);
}

//Takes in the list of all tokens and returns a new list that consists only of the duplicates
void Parser::findDuplicateTokens() {

    if (list.size() > 0) {

        Token first = list[0];
        Token compared;

        for (int i = 0; i < list.size(); i++) {
            first = list[i];
            for (int j = 0; j < i; j++) {

                Token t2;
                t2 = list[j];

                if (first.lexeme == t2.lexeme) {

                    cout << "Line " << first.line_no << ": " << first.lexeme << " already declared on line " << t2.line_no << endl;
                    break;
                }

            }
        }

    }

    //print duplicate lexemes

}

//Method taking in the list of duplicates and printing them out
void Parser::print_duplicate_message(vector<Token> dupList) {

    cout << "DUP SIZE" << dupList.size() << endl;
    for (int j = 0; j < dupList.size(); j++) {
        cout << dupList[j].lexeme << endl;
    }

    if (returnList.size() == 2) {

        cout << "Line " << returnList[1].line_no << ": " << returnList[0].lexeme << " already declared on line "
             << returnList[0].line_no << endl;
    } else if (returnList.size() > 2) {
            for (int i = 1; i < returnList.size(); i++) {
                cout << "Line " << returnList[i].line_no << ": " << returnList[0].lexeme << " already declared on line "
                     << returnList[0].line_no << endl;
            }
        }

}

//Method taking in a set of *REG_nodes, and a character and returning a new set of *REG_nodes
set<REG_node*> Parser::match_one_char(set<REG_node*> S, char c) {

    //Find nodes that can be reached from S by consuming c
    set<REG_node*> sNot;

    for (auto elem : S) {
        if (elem->first_neighbor != NULL && elem->first_label == c) {
            sNot.insert(elem->first_neighbor);
        }
        if (elem->second_neighbor != NULL && elem->second_label == c) {
            sNot.insert(elem->second_neighbor);
        }
    }

    if (sNot.empty()) {
        return sNot;
    }

    bool changed = true;
    set<REG_node*> sNotNot;

    while(changed) {
        changed = false;
        for (auto elem : sNot) {
            sNotNot.insert(elem);
            //For every neighbour of n
            if (elem->first_neighbor != NULL) {
                if (elem->first_label == '_') {
                    //add first neighbor
                    sNotNot.insert(elem->first_neighbor);
                }
            }
            if (elem->second_neighbor != NULL) {
                if (elem->second_label == '_') {
                    //add neighbor
                    sNotNot.insert(elem->second_neighbor);
                }
            }
        }

        //equal
        if(sNot != sNotNot) {
            changed = true;
            sNot = sNotNot;
            sNotNot.clear();
        }

    }

    return sNot;

}

//Method calling the match_one_char function while looping thru all tokens to see if any of them return epsilon
void Parser::findEpsilonErrors() {

    int count = 0;
    //Loop thru vector struct
    for (int i = 0; i < nameAndREGList.size(); i++) {

        REG_node *regNode;
        regNode = nameAndREGList[i].reg->start;
        set<REG_node*> epsilons = match_one_char({regNode}, '_');


        if (find(epsilons.begin(), epsilons.end(), nameAndREGList[i].reg->accept) != epsilons.end()) {
            if (count == 0) {
                cout << "EPSILON IS NOOOOOOOT A TOKEN !!! " << nameAndREGList[i].token_name;
             } else {
                cout << " " << nameAndREGList[i].token_name;
            }
                count++;
        } else {
         //   cout << "NO EPSILON";
        }
    }

}

//Main function driver code
int main() {

    Parser parser;
    parser.parse_input();

    //Semantic analysis
    parser.findDuplicateTokens();

    //Epsilon errors
    parser.findEpsilonErrors();

    return 0;
}



