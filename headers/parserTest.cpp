/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <gtest/gtest.h>
#include "comParser.h"
#include <iostream>
#include <string>
TEST(ParserTest, Checkers){
    
    //Ending initialization
    
    //Somehow this is automatically appending the characters at the end? 
    char end1ca[] = {(char)0xeb, (char)0x90, 'a','a','a','a',
            (char)0x55, (char)0x55, (char)0x55, (char)0x55, 
            (char)0x55,(char)0x55,(char)0x55,(char)0x55, 
            (char)0xeb, (char)0x90, 'a','a','a','a',
            (char)0x55, (char)0x55, (char)0x55, (char)0x55, 
            (char)0x55,(char)0x55,(char)0x55,(char)0x55, 
            (char)0xeb, (char)0x90, 'a','a','a','a',
            (char)0x55, (char)0x55, (char)0x55, (char)0x55, 
            (char)0x55,(char)0x55,(char)0x55,(char)0x55, 
            (char)0xeb, (char)0x90, 'a','a','a','a',
            (char)0x55, (char)0x55, (char)0x55, (char)0x55, 
            (char)0x55,(char)0x55,(char)0x55,(char)0x55};
    
    std::string test(end1ca);
    std::cout << test << std::endl;
    comParser *testCom = new comParser(test);
    testCom->parse();
    testCom->printParserList();
    //----------------------//

}