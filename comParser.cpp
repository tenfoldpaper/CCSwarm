/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "./headers/comParser.h"
#include <iostream>

using namespace std;
comParser::comParser(){
    length = 0;
    headCount = 0;
    tailCount = 0;
}

comParser::comParser(string input){
    ogInput = input;
}

comParser::~comParser(){};

//private methods
int comParser::checkStart(int start){
    int res = -1;
    int i = start;
    if(ogInput[i] == (char) 0xeb && ogInput[i+1] == (char) 0x90){
        res = i;
        cout << "Head found at " << res << endl;
        return res;
    }
    return res;
}

bool comParser::checkTail(int start){
    int i = start;
    //ending style 1
    if(ogInput[i] == (char) 0x55 && ogInput[i+1] == (char) 0x55 && 
            ogInput[i+2] == (char) 0x55 && ogInput[i+3] == (char) 0x55 &&
            ogInput[i+4] == (char) 0x55 && ogInput[i+5] == (char) 0x55 &&
            ogInput[i+6] == (char) 0x55 && ogInput[i+7] == (char) 0x55 ){
        return true;
    }
    //ending style 2
    if(ogInput[i] == (char) 0xc5 && ogInput[i+1] == (char) 0xc5 && 
            ogInput[i+2] == (char) 0xc5 && ogInput[i+3] == (char) 0xc5 &&
            ogInput[i+4] == (char) 0xc5 && ogInput[i+5] == (char) 0xc5 &&
            ogInput[i+6] == (char) 0xc5 && ogInput[i+7] == (char) 0x79 ){
        return true;
    }
    return false;
}

void comParser::getHeadPositions(){
    headCount = 0;
    int currentPosition = 0;
    for(int i = 0; i < ogInput.length() - 1; i++){
        currentPosition = checkStart(i);
        if(currentPosition != -1){
            i = currentPosition+2; 
            headPositions.push_back(currentPosition);
            headCount++;
        }
    }
    cout << "Head count: " << headCount << endl;
    
}

void comParser::getTailPositions(){
    tailCount = 0;
    for(int i = 0; i < ogInput.length() - 7; i++){
        if(checkTail(i)){
            tailPositions.push_back(i);
            tailCount++;
        }            
    }
    cout << "Tail count: " << tailCount << endl;
}

void comParser::parse(){
    if(ogInput.length() < 20){
        return;
    }
    getHeadPositions();
    getTailPositions();
    if(headCount != tailCount){
        cout << "Head Count != Tail Count. Broken message? " << endl;
        return; 
    }
    for(int i = 0; i < headCount; i++){
        int strLen = tailPositions[i] - headPositions[i] + 8;
        parserList.push_back(ogInput.substr(headPositions[i], strLen));
        
        
    }
      
}

void comParser::printParserList(){
    if(headCount != tailCount || parserList.size() == 0 || headCount == 0 ){
        cout << "Parser List Empty!" << endl;
        return;
    }
    else{
        for(int i = 0; i < headCount; i++){
            cout << "Index " << i << ": " << parserList[i] << 
                endl << "   size: " << parserList[i].size() << endl;
        }
    }
    
}