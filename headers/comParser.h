/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   com_parser.h
 * Author: whitebox
 *
 * Created on October 15, 2018, 4:00 PM
 */

/*
 * Description:
 * Parses a message sent by COM that has equal number of headers and tails,
 * and stores the data into parser_list which should then be assigned to
 * msgHandler. 
 */
#ifndef COM_PARSER_H
#define COM_PARSER_H
#include <vector>
#include <string>

class comParser{
private:
    int headCount;
    int tailCount;
    std::vector<int> headPositions;
    std::vector<int> tailPositions;
    int length;
    std::vector<std::string> parserList; 
    std::string ogInput;
    void getHeadPositions();
    void getTailPositions();
    void parse();
public:
    comParser();
    comParser(std::string);
    ~comParser();
    int checkStart(int);
    bool checkTail(int);
    void setParserList(std::vector<std::string>);
    std::string toHex(std::string);
    void printRawBytes(std::string);
    std::vector<std::string> getParserList();
};


#endif /* COM_PARSER_H */

