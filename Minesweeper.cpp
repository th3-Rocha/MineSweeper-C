#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

#include "Sweeper.h"

void fill_bombs(Map &map,DifficultyRules &rules ) {
  // Seed do mapa
  srand(time(0));
  // Remove todas as bombas do mapa
  for (int y = rules.MapSize_Y; y > 0 ; y--) {
    for (int x = 1; x <= rules.MapSize_X ; x++) {
      map[x][y].has_bomb = false;
    }
  }

  
  for (int i = 0; i < rules.Bombs_N; i++) {
    for (int b = 0; b < 1; b++) {
      int randX = (std::rand() % rules.MapSize_X) + 1;
      int randY = (std::rand() % rules.MapSize_Y) + 1;
      if(map[randX][randY].has_bomb == false){
        map[randX][randY].has_bomb = true;
        
        //std::cout << "Bomb at: " << randX << " " << randY << std::endl;
      }
      else{
        b--;
      }
    }
  }
}

void near_bombs_map(Map &map, DifficultyRules &rules){
  // Reseta as bombas proximas de todas as celulas
  for (int y = rules.MapSize_Y; y > 0 ; y--) {
    for (int x = 1; x <= rules.MapSize_X ; x++) {
      map[x][y].near_bombs = 0;
    }
  }

  //Adiciona o numero de bombas proximas a cada célula
  for (int y = rules.MapSize_Y; y > 0 ; y--) {
    for (int x = 1; x <= rules.MapSize_X ; x++) {
      if(y + 1 <=rules.MapSize_Y){
        if(x + 1 <=rules.MapSize_X && map[x+1][y+1].has_bomb){
          map[x][y].near_bombs++;
        }
        if(map[x][y+1].has_bomb){
          map[x][y].near_bombs++;
        }
      }
      if(x + 1 <=rules.MapSize_X){
        if(y - 1 >= 1 && map[x+1][y-1].has_bomb){
          map[x][y].near_bombs++;
        }
        if(map[x+1][y].has_bomb){
          map[x][y].near_bombs++;
        }
      }
      if(y - 1 >= 1){
        if(x - 1 >= 1 && map[x-1][y-1].has_bomb){
          map[x][y].near_bombs++;
        }
        if(map[x][y-1].has_bomb){
          map[x][y].near_bombs++;
        }
      }
      if(x - 1 >= 1){
        if(y + 1 <=rules.MapSize_Y && map[x-1][y+1].has_bomb){
          map[x][y].near_bombs++;
        }
        if(map[x-1][y].has_bomb){
          map[x][y].near_bombs++;
        }
      }
      
      // debug para revelar o mapa
      //map[x][y].is_hidden = false; 
    }
  }
}


// Função que cria o mapa inicial do jogo
void create_map(Map &map, DifficultyRules &rules) {
  fill_bombs(map, rules); 
  near_bombs_map(map, rules);
}

// Função que revela os campos ao redor
void adjacent_reveal(Map &map, DifficultyRules &rules, int posX,int posY){
  int x = posX, y = posY;
      
  if(y + 1 <=rules.MapSize_Y){
    // Verifica posição superior
    if(map[x][y+1].is_hidden == true && map[x][y+1].has_bomb == false){
      map[x][y+1].is_hidden = false;
      if(map[x][y+1].near_bombs == 0) adjacent_reveal(map,rules,x,y+1);
    }
    // Verifica posição superior direita
    if(x + 1 <=rules.MapSize_X  && map[x+1][y+1].is_hidden == true && map[x+1][y+1].has_bomb == false){
      map[x+1][y+1].is_hidden = false;
      if(map[x+1][y+1].near_bombs == 0) adjacent_reveal(map,rules,x+1,y+1);
    }
  }
  
  if(x + 1 <=rules.MapSize_X){
    // Verifica posição direita
    if(map[x+1][y].is_hidden == true && map[x+1][y].has_bomb == false){
      map[x+1][y].is_hidden = false;
      if(map[x+1][y].near_bombs == 0) adjacent_reveal(map,rules,x+1,y);
    }
    // Verifica posição direta inferior
    if(y - 1 >= 1 && map[x+1][y-1].is_hidden == true && map[x+1][y-1].has_bomb == false){
      map[x+1][y-1].is_hidden = false;
      if(map[x+1][y-1].near_bombs == 0) adjacent_reveal(map,rules,x+1,y-1);
    }
  }
  
  if(y - 1 >= 1){
    // Verifica posição inferior
    if(map[x][y-1].is_hidden == true && map[x][y-1].has_bomb == false){
      map[x][y-1].is_hidden = false;
      if(map[x][y-1].near_bombs == 0) adjacent_reveal(map,rules,x,y-1);
    }
    // Verifica posição inferior esquerda
    if(x - 1 >= 1 && map[x-1][y-1].is_hidden == true  && map[x-1][y-1].has_bomb == false){
      map[x-1][y-1].is_hidden = false;
      if(map[x-1][y-1].near_bombs == 0) adjacent_reveal(map,rules,x-1,y-1);
    }
  }
  
  if(x - 1 >= 1){
    // Verifica posição esquerda
    if(map[x-1][y].is_hidden == true && map[x-1][y].has_bomb == false){
      map[x-1][y].is_hidden = false;
      if(map[x-1][y].near_bombs == 0) adjacent_reveal(map,rules,x-1,y);
    }
    // Verifica posiçao esquerda superior
    if(y + 1 <=rules.MapSize_Y && map[x-1][y+1].is_hidden == true && map[x-1][y+1].has_bomb == false){
      map[x-1][y+1].is_hidden = false;
      if(map[x-1][y+1].near_bombs == 0) adjacent_reveal(map,rules,x-1,y+1);
    }
  }
}

int adjacent_reveal_flag(Map &map, DifficultyRules &rules, int x,int y){
  bool has_flag = false;
  if(y + 1 <=rules.MapSize_Y){
    if(x + 1 <=rules.MapSize_X && map[x+1][y+1].has_redFlag){
      has_flag = true;
    }
    if(map[x][y+1].has_redFlag){
      has_flag = true;
    }
  }
  if(x + 1 <=rules.MapSize_X){
    if(y - 1 >= 1 && map[x+1][y-1].has_redFlag){
      has_flag = true;
    }
    if(map[x+1][y].has_redFlag){
      has_flag = true;
    }
  }
  if(y - 1 >= 1){
    if(x - 1 >= 1 && map[x-1][y-1].has_redFlag){
      has_flag = true;
    }
    if(map[x][y-1].has_redFlag){
      has_flag = true;
    }
  }
  if(x - 1 >= 1){
    if(y + 1 <=rules.MapSize_Y && map[x-1][y+1].has_redFlag){
      has_flag = true;
    }
    if(map[x-1][y].has_redFlag){
      has_flag = true;
    }
  }  

  //caso tenha uma flag perto 
  if(has_flag == true){
    bool has_bomb = false;
    if(y + 1 <=rules.MapSize_Y){
      if(x + 1 <=rules.MapSize_X && map[x+1][y+1].has_redFlag == false){
        map[x+1][y+1].is_hidden = false;
        adjacent_reveal(map,rules,x+1,y+1);
        if(map[x+1][y+1].has_bomb){
          has_bomb = true;
        }
      }
      if(map[x][y+1].has_redFlag == false){
        map[x][y+1].is_hidden = false;
        adjacent_reveal(map,rules,x,y+1);
        if(map[x][y+1].has_bomb){
          has_bomb = true;
        }
      }
    }
    if(x + 1 <=rules.MapSize_X){
      if(y - 1 >= 1 && map[x+1][y-1].has_redFlag == false){
        map[x+1][y-1].is_hidden = false;
        adjacent_reveal(map,rules,x+1,y-1);
        if(map[x+1][y-1].has_bomb){
          has_bomb = true;
        }
      }
      if(map[x+1][y].has_redFlag == false ){
        map[x+1][y].is_hidden = false;
        adjacent_reveal(map,rules,x+1,y);
        if(map[x+1][y].has_bomb){
          has_bomb = true;
        }
      }
    }
    if(y - 1 >= 1){
      if(x - 1 >= 1 && map[x-1][y-1].has_redFlag == false){
        map[x-1][y-1].is_hidden = false;
        adjacent_reveal(map,rules,x-1,y-1);
        if(map[x-1][y-1].has_bomb){
          has_bomb = true;
        }
      }
      if(map[x][y-1].has_redFlag == false ){
        map[x][y-1].is_hidden = false;
        adjacent_reveal(map,rules,x,y-1);
        if(map[x][y-1].has_bomb){
          has_bomb = true;
        }
      }
    }
    if(x - 1 >= 1){
      if(y + 1 <=rules.MapSize_Y && map[x-1][y+1].has_redFlag == false){
        map[x-1][y+1].is_hidden = false;
        adjacent_reveal(map,rules,x-1,y+1);
        if(map[x-1][y+1].has_bomb){
          has_bomb = true;
        }
      }
      if(map[x-1][y].has_redFlag == false){
        map[x-1][y].is_hidden = false;
        adjacent_reveal(map,rules,x-1,y);
        if(map[x-1][y].has_bomb){
          has_bomb = true;
        }
      }
    }
    if(has_bomb == true){
    
    /////////////////////////////////////
    return 0;
    }
    
  }
  else{
    std::cout  << std::endl << "There is no flag near ";
  }
  return 1;
}

void show_map(Map &map, DifficultyRules &rules) {
  system("clear");
  if(rules.Rule == 0){
    std::cout << std::endl << "---------- MineSweeper ---------" << std::endl ; 
  }
  else if(rules.Rule == 1){
    std::cout << std::endl << "----------------- MineSweeper -----------------" << std::endl ; 
  }
  else if(rules.Rule == 2){
    std::cout << std::endl << "--------------------------------------- MineSweeper ----------------------------------------" << std::endl ; 
  }
  
  char c = ' ';
  std::cout << std::endl;
  for (int y = rules.MapSize_Y; y > 0 ; y--) {
    if(y < 10){
      std::cout << "\033[0;93m";
      std::cout << "0" << y << " ";
    }
    else{
      std::cout << "\033[0;93m";
      std::cout << y << " ";
    }
    
    for (int x = 1; x <= rules.MapSize_X ; x++) {
      if (map[x][y].has_redFlag == true && map[x][y].is_hidden == false && map[x][y].has_bomb == true) {
        std::cout << "\033[0;31m";
        c = 'B';
      }else if (map[x][y].has_redFlag) {
        std::cout << "\033[0;31m";
        c = 'R';
      }else {
        if (map[x][y].is_hidden) {
          std::cout << "\033[0;37m";
          c = '#';
        }
        else{
          if (map[x][y].has_bomb) {
            //system("Color 05");
            std::cout << "\033[0;35m";
            c = 'B';
          }
          else{
            if(map[x][y].near_bombs == 0){
              c = ' ';
            }
            if(map[x][y].near_bombs == 1){
              std::cout << "\033[0;96m";
              c = '1';
            }
            if(map[x][y].near_bombs == 2){
              std::cout << "\033[0;34m";
              c = '2';
            }
            if(map[x][y].near_bombs == 3){
              std::cout << "\033[0;92m";
              c = '3';
            }
            if(map[x][y].near_bombs == 4){
              std::cout << "\033[0;32m";
              c = '4';
            }
            if(map[x][y].near_bombs == 5){
              std::cout << "\033[0;91m";
              c = '5';
            }
            if(map[x][y].near_bombs == 6){
              std::cout << "\033[0;31m";
              c = '6';
            }
            if(map[x][y].near_bombs == 7){
              std::cout << "\033[0;95m";
              c = '7';
            }
            if(map[x][y].near_bombs == 8){
              std::cout << "\033[0;35m";
              c = '8';
            }
            
              //função para calcular quantas bombas estão perto
          }
        }
      }
      std::cout << ' ' << c << ' '; 
      }
    std::cout << std::endl;
  }
  
  for(int c = 0; c <=rules.MapSize_X;c++ ){
    if(c < 10){
      std::cout << "\033[0;93m";
      std::cout << "0" << c << " ";
    }
    else{
      std::cout << "\033[0;93m";
      std::cout << c << " ";
    }
  }
  std::cout << "\033[0;37m";
}

int is_winner(Map &map, DifficultyRules &rules){
  int hidden_n = 0;

  for(int l = 0; l < rules.MapSize_X; l++){
    for(int c = 0; c < rules.MapSize_Y; c++){
      if(map[l][c].is_hidden == true && map[l][c].has_bomb == false)
        hidden_n++;
    }
  }
  if(hidden_n == 0 ){
    return 2;
  }
  else {
    return 1;
  }
}

void check_rules(Map &map, DifficultyRules &rules, int posX,int posY){
  if(rules.Rule == 1){
    if(map[posX][posY].near_bombs != 0 || map[posX][posY].has_bomb == true){
      create_map(map, rules);
      check_rules(map, rules, posX, posY);
    }
    else{
      return;
    }
  }
  
  else if(rules.Rule == 2){
    if(map[posX][posY].has_bomb == true){
      create_map(map, rules);
      check_rules(map, rules, posX, posY);
    }
    else{
      return;
    }
  }

  
}

int act1_change_map(Map &map, DifficultyRules &rules, int posX,int posY, bool IsFirstAction){
  if((posX > rules.MapSize_X || posY > rules.MapSize_Y) || (posX < 1 || posY <  1)){
    std::cout  << std::endl << "This position is invalid";
    return 1;
  }
  else if(map[posX][posY].is_hidden == false && map[posX][posY].near_bombs == 0){
    std::cout  << std::endl << "This position is already visible";
    return 1;
  }
  else if(map[posX][posY].is_hidden == false && map[posX][posY].near_bombs != 0){
    if(adjacent_reveal_flag(map,rules,posX,posY) == 0){
      std::cout << "\033[0;31m";
      std::cout  << std::endl << "Game Over, A bomb was revealed";
      return 0;
    }
  }
  if (IsFirstAction == true){
    check_rules(map, rules, posX, posY);
  }
  map[posX][posY].is_hidden = false;
  
  if(map[posX][posY].has_bomb == true){
    std::cout << "\033[0;31m";
    std::cout  << std::endl << "Game Over, A bomb was revealed";
    return 0;
  }
  else{
    if(map[posX][posY].near_bombs == 0){
      //adjacentes
      adjacent_reveal(map,rules,posX,posY);
    }
    else{

      //do nothing because cliked
    }
    return is_winner(map,rules);
  }

  
}
void act2_red_flag(Map &map, DifficultyRules &rules, int posX,int posY){
  if((posX > rules.MapSize_X || posY > rules.MapSize_Y) || (posX < 1 || posY < 1)){
    std::cout  << std::endl << "This position is invalid";
    return void();
  }
  if(map[posX][posY].has_redFlag == true){
    map[posX][posY].has_redFlag = false;
  }
  else{
    map[posX][posY].has_redFlag = true;
  }
  
}

void valid_input_for_a_int(int posX,int posY){
  if(std::cin.fail() == 1 ){
    posX = 0;
    posY = 0;
    std::cin.clear();
    std::cin.ignore();
  }
}

//record Functions
bool comparasion(Record &a, Record &b) {
  return a.milliseconds < b.milliseconds;
}

void sort_records(std::vector<Record> &recordsVec){
    std::sort(recordsVec.begin(), recordsVec.end(), comparasion);
}

void save_record(double time, std::vector<Record> &recordsVec){
  Record current;
  current.milliseconds = time;
  std::cout << "\033[0;31m";
  std::cout << std::endl << "Enter your name: " << std::endl;
  std::cin.ignore();
  std::getline(std::cin,current.name);
  recordsVec.push_back(current);
}

void read_recordsFile(std::vector<Record> &recordsVec, DifficultyRules &rules) {
  int size;
  std::fstream recordsFile;
  if(rules.Rule == 0)
    recordsFile.open(R_B_TXT);
  else if(rules.Rule == 1)
    recordsFile.open(R_I_TXT);
  else if(rules.Rule == 2)
    recordsFile.open(R_A_TXT);
  if(recordsFile.is_open()){
    recordsVec.clear();
    recordsFile >> size;
    recordsFile.ignore();
        
    for(int i = 0; i < size; i++){
      Record temp;
      std::getline(recordsFile, temp.name, '>');
      recordsFile >> temp.milliseconds;
      recordsFile.ignore();
      recordsVec.push_back(temp);
    }
  }
}

void print_records(std::vector<Record> &recordsVec){
  std::cout << "\033[0;35m";
  for (int i = 0; i < recordsVec.size(); i++){
    std::cout << std::left;
    std::setprecision(3);
    std::cout << recordsVec[i].name << " -> ";
    std::cout << recordsVec[i].milliseconds/1000 << "s, ";
    std::cout <<  recordsVec[i].milliseconds % 1000 << "ms"<< std::endl;
  }
}

void write_recordsFile(std::vector<Record> &recordsVec, DifficultyRules &rules){
    std::ofstream recordsFile;

    if(rules.Rule == 0)
        recordsFile.open(R_B_TXT);
    else if(rules.Rule == 1)
        recordsFile.open(R_I_TXT);
    else if(rules.Rule == 2)
        recordsFile.open(R_A_TXT);

    if(recordsFile.is_open()){
        recordsFile << recordsVec.size() << std::endl;
        for(int i = 0; i < recordsVec.size(); i++){
            recordsFile << recordsVec[i].name << ">" << recordsVec[i].milliseconds << std::endl;
        }
        recordsFile.close();
    }
}






//