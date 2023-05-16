/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "Wordle.h"
#include "Tree.h"
#include <iostream>   //Input/Output Library
#include <string>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <stack>
#include <array>
#include <algorithm>
#include <fstream>
#include <ctime>

using namespace std; 

Wordle::Wordle(){
    maxTurn = 6;
    win = false;
    //difficulty set to normal by default
    mode = 1;
    
    //Maps letters to numbers for ease when displaying
    letters = {{1,'q'}, {2,'w'}, {3,'e'}, {4,'r'}, {5,'t'}, {6,'y'},{7,'u'},
               {8,'i'}, {9,'o'}, {10,'p'}, {11,'a'}, {12,'s'}, {13,'d'}, {14,'f'},
               {15,'g'}, {16,'h'}, {17,'j'}, {18,'k'}, {19,'l'}, {20,'z'}, {21,'x'},
               {22,'c'}, {23,'v'}, {24,'b'}, {25,'n'}, {26,'m'}};
    
    //fills inverse map with inverse of letters map
    for(int n=1; n<=26; n++){
        inverse[letters[n]] = n;
    }
    
    //fills result data structure with spaces so they don't show up until data is modified
    for(int n=0; n<6; n++){
        for(int i=0; i<5; i++){
            result[n][i] = ' ';
        }
    }
    
    //sets target word
    word = setWord();
    
    //if difficulty was changed to easy in start function then initialize stack, queue, and tree
    for(int n=0; n<5; n++){
        backHint.push(word[n]);
        frntHint.push(word[n]);
        alphabetic.insert(word[n]);
    } 
    turn = 0;
}

string Wordle::setWord(){
    string temp;
    unordered_set<string>::iterator itr;
    
    //accesses the text file that contains list of words
    read.open("words.txt");
    
    //checking for errors opening the text document
    if(!read){
        cout<<"Error reading from file\n";
        exit (0);
    }
    
    //sends every word from the text file into a set 
    while(read >> temp){
        hold.insert(temp); 
    }
    
    //closes text file
    read.close();
    
    //array gets data transferred to it by the set
    array<string, 36> tempAry;
    
    //uses iterator to transfer data from set to array
    int n = 0;
    for (itr = hold.begin(); itr != hold.end(); itr++) {
        tempAry[n] = *itr;
        n++;
    }
    
    //shuffles the array that holds all possible words
    random_shuffle(tempAry.begin(), tempAry.end());
    
    //sets temp equivalent to the random value of shuffled array then returns it
    temp = tempAry[rand()%36]; 
    
    return temp;
}

void Wordle::help(){
    cout<<"\nWelcome to Wordle !\n";
    cout<<"You will have 6 attempts to guess a five letter word.\n";
    cout<<"If your guess has a letter in the same position as the target word you will \n";
    cout<<"be notified by a 1 next to your guess\n";
    cout<<"If your guess contains a word that is also found in the target, but in a different position\n";
    cout<<"you will be notified by a 0 next to your guess\n";
    cout<<"For example:\n\n";
    cout<<setw(20)<<"Target = olive\n\n"<<setw(26)<<"guess  = scone //0/1\n\n";
    cout<<"The '/' marks that the letter in that position of the guess shares nothing with the target\n";
    cout<<"The '0' marks that the o in that position in the guess is found in the target but not in that spot\n";
    cout<<"The '1' marks that the e in that position of the guess is found in the target in the same spot \n\n";
    cout<<"A copy of your keyboard will be displayed underneath your game and will update after every round\n";
    cout<<"If one of the letters you chose does not have any correlation to the target word it will be removed from the keyboard\n";
    cout<<"If one of the letters you chose does have correlation to the target word it will be displayed in caps\n";
    cout<<"This is done to help remind you of what letters are still in play\n\n";
    cout<<"The difficulty is defaulted to 'normal' which is standard game play\n";
    cout<<"This can be changed to 'easy' in the main menu if you are looking for less of a challenge\n";
    cout<<"Changing the difficulty to easy allows for 1 hint per turn\n";
    cout<<"The hints consist of either a letter revealed from the front or the back of the target\n\n";
    cout<<"(You may have to scroll up to view full instructions)\n";
}

void Wordle::start(){
    int choice = 0;
    
    cout<<"Wordle\n";
    cout<<"by Christopher Delgado\n";
    cout<<"Press 1 to play, press 2 for instructions, press 3 to change difficulty, press 4 to exit\n";
    cout<<"Difficulty : ";
    mode == 1? cout<<"normal\n" : cout<<"easy\n";
    
    cin>>choice;
    while(choice > 4 || choice < 1){
        cout<<"Please enter a number that corresponds to one of the options\n";
        cin>>choice;
    }
    
    if(choice == 2){
        help();
    }
    if(choice == 3){
        mode = 0;
        start();
    }
    if(choice == 4){
        exit(0);
    }
}

void Wordle::board(){
    //displays a record of your guesses on the board
    cout<<"\n";
    auto iter = guesses.begin();
    for(int n=0; n<6; n++){
        if(iter == guesses.end()){
            cout<<" _ _ _ _ _\n";
        }else{
            string temp = *iter++;
            for(int i=0; i<5; i++){
                cout<<setw(2)<<temp[i];

                if(i == 4){
                    cout<<"  ";
                    for(int k=0; k<5; k++){
                        cout<<result[n][k];
                    }
                    cout<<"\n";
                }
            }
        }
    }
    
    //displays the keyboard under the guesses
    cout<<"\n";
    for(int n=1; n<=26; n++){
        cout<<letters[n]<<" ";
        if(n == 10 || n == 19 || n == 26){
            cout<<"\n";
        }
    }
}

void Wordle::usrGuess(){
    cout<<"\nInput your guess\n";
    cout<<"If difficulty was set to easy enter '?' for a hint\n";
    cin>>guess;
    
    //hint function guides users to choosing their hint
    if(guess == "?" && mode == 0){
        hint();
    }
    
    //input validation checks for a 5 letter word
    while(guess.size() > 5 || guess.size() < 5){
        cout<<"Please input a 5 letter word\n";
        cin>>guess;
    }
    
    //checking if inputted word has duplicate letters
    //if duplicates found asks user to input again
    bool repeat;
    string sorted;
 
    do{
        //sort the "guess" string in order to compare ascii values for duplicates
        repeat = false;
        sorted = guess;
        sort(sorted.begin(), sorted.end());
        
        for (int n=0; n<sorted.length()-1; n++){
            //if string contains repeats it will prompt user to reenter a guess
            if(sorted[n] == sorted[n + 1]){
                cout<<"Please input a word with no repeating letters\n";
                cin>>guess;
                
                //input validation checks for a 5 letter word
                while(guess.size() > 5 || guess.size() < 5){
                    cout<<"Please input a 5 letter word\n";
                    cin>>guess;
                }
                
                repeat = true;
            }
        }
    }while(repeat);
    
    //makes sure every char is lowercase for uniformity
    for(int n=0; n<5; n++){
        guess[n] = tolower(guess[n]);
    }
    
    //updates guesses data structure that will display on the board
    auto iter = guesses.begin();
    for(int n=0; n<turn; n++){
        iter;
    } 
    if(turn == 0){
        guesses.push_back(guess);
    }else{
        string temp = *iter; 
        temp = guess;
        guesses.push_back(temp);
    }
    
}

void Wordle::hint(){
    int choice;
    
    cout<<"Do you want to reveal a letter from the front, from the back or \n";
    cout<<"from alphabetical order of the target\n";
    cout<<"Press 1 for a letter from the back, Press 2 for a letter from the front\n";
    cout<<"Press 3 for a letter from alphabetical order\n";
    cin>>choice;
    
    while(choice < 1 || choice > 3){
        cout<<"Please enter either 1, 2, or 3\n";
        cin>>choice;
    }
    
    //uses stack to display a letter from the back
    //when a letter is revealed it gets popped off so that the next hint wont be a repeat
    if(choice == 1){
        cout<<"\nA letter from the back is "<<backHint.top()<<"\n\n";
        backHint.pop();
    }
    //uses queue to display a letter from the front
    //when a letter is revealed it gets popped off so that the next hint wont be a repeat
    if(choice == 2){
        cout<<"\nA letter from the front is "<<frntHint.front()<<"\n\n";
        frntHint.pop();
    }
    //uses tree to display letter in alphabetical order
    //when letter is revealed it gets removed from the trr
    if(choice == 3){
        char temp = alphabetic.smallest();
        cout<<"\nA letter in alphabetical order is "<<temp<<"\n\n";
        alphabetic.remove(temp);
    }
}

bool Wordle::compare(){
    bool same; 
    
    //takes the last user guess and uses iterator to store into a string
    auto iter = guesses.begin();
    for(int n=0; n<turn; n++){
        iter++;
    }
    string guess = *iter;
    
    //sets bool to true if both guess and target are the same
    //ends the game with a win if set to true
    if(guess == word){
        same = true;
    }
    
    //converts the target word to a char array in order to use find()
    char temp[5];
    for(int n=0; n<5; n++){
        temp[n] = word[n];
    }
    
    //iterates through every letter from guess and searches char array for it
    char *ptr;
    for(int n=0; n<5; n++){
        ptr = find(temp, temp + 5, guess[n]);
        
        //sets values after search
        if(ptr == temp + 5){
            result[turn][n] = '/';
        }else if(ptr - temp == n){
            result[turn][n] = '1';
        }else{
            result[turn][n] = '0';
        }
    }
    
    return same;
}

void Wordle::end(){ 
    //message shown when user wins
    if(win){
        cout<<"\nCongradualtions You've Won!\n";
        cout<<"You guessed correctly in "<<turn<<" turn(s)\n";
    }
    
    //message shown when user loses
    if(turn == maxTurn){
        cout<<"\nYou ran out of turns :(\n";
        cout<<"Turns : "<<turn<<" out of "<<maxTurn<<"\n";
        cout<<"The correct answer was '"<<word<<"'\n";
    }
}

void Wordle::setKeys(){
    //use inverse map to find key used in letters map and adjust values
    int key = 0;
    
    for(int n=0; n<5; n++){
        //when there is correlation between a letter in guess and a letter in answer
        if(result[turn][n] == '1' || result[turn][n] == '0'){
            key = inverse[guess[n]];                    
            letters[key] = toupper(letters[key]);
        }
        //when there is no correlation between a letter in guess and a letter in answer
        if(result[turn][n] == '/'){
            key = inverse[guess[n]];                    
            letters[key] = ' ';
        }
    }
    
}

bool Wordle::getWin(){
    return win;
}

int Wordle::getTurn(){
    return turn;
}

int Wordle::getMaxT(){
    return maxTurn;
}

void Wordle::setTurn(){
    if(turn == 5){
        turn = 6;
    }else{
        turn++;
    }
}

void Wordle::setWin(){
    win = compare();
}

void Wordle::clear(){
    win = false;
    //difficulty set to normal by default
    mode = 1;    
    
    //Maps letters to numbers for ease when displaying
    letters = {{1,'q'}, {2,'w'}, {3,'e'}, {4,'r'}, {5,'t'}, {6,'y'},{7,'u'},
               {8,'i'}, {9,'o'}, {10,'p'}, {11,'a'}, {12,'s'}, {13,'d'}, {14,'f'},
               {15,'g'}, {16,'h'}, {17,'j'}, {18,'k'}, {19,'l'}, {20,'z'}, {21,'x'},
               {22,'c'}, {23,'v'}, {24,'b'}, {25,'n'}, {26,'m'}};
    
    //fills result data structure with spaces so they don't show up until data is modified
    for(int n=0; n<6; n++){
        for(int i=0; i<5; i++){
            result[n][i] = ' ';
        }
    }
    
    //sets target word
    word = setWord();
    
    //if difficulty was changed to easy in start function then initialize stack and queue
    //but first clear them
    while(backHint.empty() != true){
        backHint.pop();
    }
    while(frntHint.empty() != true){
        frntHint.pop();
    }
    alphabetic.clear();
    
    for(int n=0; n<5; n++){
        backHint.push(word[n]);
        frntHint.push(word[n]);
        alphabetic.insert(word[n]);
    }
    
    //clear data structures
    hold.clear();
    guesses.clear();
    
    turn = 0;
}

//used for testing purposes only
const string Wordle::getWord(){
    return word;
}