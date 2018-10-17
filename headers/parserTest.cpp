/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <gtest/gtest.h>
#include "comParser.h"

TEST(ParserTest, Checkers){
    comParser* comp = new comParser
            ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    
    
    ASSERT_EQ(-1, comp->checkStart(0));
    ASSERT_EQ(false, comp->checkTail(0));
}