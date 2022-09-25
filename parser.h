//
// Created by Jannis Grimm on 9/17/22.
//
#pragma once
#ifndef PROJECT1_PARSER_H
#define PROJECT1_PARSER_H

/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */

#include <string>
#include "lexer.h"
#include <vector>
#include <set>
 using namespace std;

struct REG {
    struct REG_node * start;
    struct REG_node * accept;
};

struct REG_node {
    struct REG_node * first_neighbor;
    char first_label;
    struct REG_node * second_neighbor;
    char second_label;
};

struct NameAndREG {
    string token_name;
    REG *reg;
};


class Parser {

public:
    void parse_input();
    REG * parse_expr();
    void findDuplicateTokens();
    void print_duplicate_message(std::vector<Token> dupList);
    bool listContainsToken(Token token);
    void findEpsilonErrors();
    std::set<REG_node*> match_one_char(std::set<REG_node*> S, char c);
private:
    LexicalAnalyzer lexer;
    InputBuffer inputbuffer;
    void syntax_error();
    void snytax_error();
    Token expectSyntax(TokenType expected_type);
    Token expectSnytax(TokenType expected_type);
    void parse_tokens_section();
    void parse_token_list();
    void parse_token();

};



#endif //PROJECT1_PARSER_H
