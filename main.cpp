/* 
 * File:   main.cpp
 * Author: Christopher Delgado
 * Created on 6/10/22
 * Purpose:  Wordle Clone 
 */

//Libraries
#include <iostream>   //Input/Output Library
#include <ctime>
#include "Wordle.h"

using namespace std;  //STD Name-space where Library is compiled

//Global Constants not Variables
//Math/Physics/Science/Conversions/Dimensions

//Function Prototypes
int again();

//Code Begins Execution Here with function main
int main(int argc, char** argv) {
    //Set random number seed once here
    srand(static_cast<unsigned int>(time(0)));
    
    //**//Declare variables here//**//
    Wordle game;
    
    //**//Initialize variables here//**//
    //**//Map inputs to outputs here, i.e. the process//**// 
    game.start();
    
    //display empty board
    game.board();
    
    //displays answer
    cout<<game.getWord()<<"\n";
    
    //**//Display the results//**// 
    do{
        //gets user guess and updates data structures
        game.usrGuess(); 
        
        //compares user guess to the target word
        game.setWin();
        
        //adjusts keyboard based on results on guess
        game.setKeys();
        
        //displays the updated board
        game.board();
        
        //increments the turn      
        game.setTurn();
        
        //if correct word is guessed it will send players a congrats message
        game.end();
        
        //asks user if they want to play again if won or lost
        if(game.getTurn() == game.getMaxT() || game.getWin()){
            if(again() == 2){
                cout<<"Thank you for playing\n";
                continue;
            }
            //sets all values in object back to default to start game over
            game.clear();
            
            //display the start screen again
            game.start();
            
            //displays board
            game.board();
            
            //displays answer
            cout<<game.getWord()<<"\n";
        }
    }while(game.getTurn() != game.getMaxT() && !game.getWin());

    return 0;
}

int again(){
    int choice;
    
    cout<<"\nWould you like to play again ?\n";
    cout<<"Press 1 for yes and 2 for no\n";
    cin>>choice;
    
    while(choice > 2 || choice < 1){
        cout<<"Please enter a valid value, either 1 or 2\n";
        cin>>choice;
    }
    
    return choice;
}