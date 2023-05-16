/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Wordle.h
 * Author: Christopher Delgado
 *
 * Created on May 7, 2022, 8:08 PM
 */

#ifndef WORDLE_H
#define WORDLE_H

#include "Tree.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <stack>
#include <array>
#include <fstream>

using namespace std;

class Wordle{
private:
    //used to open text file containing words
    ifstream read;
    
    //holds the words that are transferred from text file
    unordered_set<string> hold;
    
    //target word that users are trying to guess
    string word;
    
    //holds user input of their guess
    string guess;
    
    //map containing the letters of keyboard that are displayed below guesses
    unordered_map<int, char> letters;
    
    //holds an inverse version of letter map to quickly find keys
    unordered_map<char, int> inverse;  
    
    //holds a record of all user guesses
    list<string> guesses;
    
    //compares guess and target and displays accuracy of guess
    char result[6][5];                
    
    //hold current turn and the max turns allowed
    int turn;
    int maxTurn;
    
    //game ends with a win if set to true/ set to true when guess and target are equal
    bool win;
    
    //holds answer in stack and queue and allows for users to view letters from front and back
    stack<char> backHint;                                
    queue<char> frntHint;
    
    //holds answer in tree allowing for quick alphabetical lookup for hint
    Tree alphabetic;
    
    //used to set difficulty, 1 = normal, 0 = easy
    int mode;         
    
public:
    //constructor
    Wordle();
    
    //gives users instructions on how to play game and features offered
    void help();
    
    //start screen where users choose to play, learn more, or exit
    void start();
    
    //displays board
    void board();
    
    //grabs user guess and does all necessary checks to it
    void usrGuess();
    
    //compares user guess and target word
    bool compare();
    
    //displays win screen if word is guessed, and fail screen if turns run out
    void end();
    
    //adjusts keyboard displayed depending on guess accuracy
    void setKeys();
    
    //if difficulty set to easy then allows users to access hints from front or back
    void hint();
    
    //randomly picks out a word from text file and uses it for game
    string setWord();
    
    //getter functions
    bool getWin();
    int getTurn();
    int getMaxT();
    
    //setter functions
    void setTurn();
    void setWin();
    
    //used to reset all values back to default
    void clear();
    
    //special getter function used for testing purposes
    //reveals answer
    const string getWord();
    
};

#endif /* WORDLE_H */