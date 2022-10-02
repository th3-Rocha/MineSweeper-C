#ifndef SWEEPER_H
#define SWEEPER_H

#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string R_B_TXT = "RecordsDiffs/beginner_r.txt";
const std::string R_I_TXT = "RecordsDiffs/intermediate_r.txt";
const std::string R_A_TXT = "RecordsDiffs/advanced_r.txt";

enum class Difficulty { beginner, intermediary, advanced };



struct Cell {
  bool is_hidden {true};

  bool has_bomb {false};
  int near_bombs;

  bool has_redFlag {false};
  //pos flag;
  
};

struct DifficultyRules {
  int MapSize_X ;
  int MapSize_Y ;
  int Bombs_N;
  int Rule = 0;
};

struct Record {
  std::string name;
  int milliseconds;
};


using Map = std::array<std::array<Cell,16>,31>;

using ListRecords = std::vector<Record>;

//map
void create_map(Map &map, DifficultyRules &rules);

void show_map(Map &map, DifficultyRules &rules);

void near_bombs_map(Map &map, DifficultyRules &rules);

//map

//rules


int is_winner(Map &map, DifficultyRules &rules);

void check_rules(Map &map, DifficultyRules &rules, int posX,int posY);

void valid_input_for_a_int( int posX,int posY);
//rules

//actions
int act1_change_map(Map &map, DifficultyRules &rules, int posX,int posY,bool IsFirstAction);

void act2_red_flag(Map &map, DifficultyRules &rules, int posX,int posY);

void adjacent_reveal(Map &map, DifficultyRules &rules, int posX,int posY);

int adjacent_reveal_flag(Map &map, DifficultyRules &rules, int posX,int posY);
//actions

//record

void print_records(std::vector<Record> &ListRecords);

void sort_records(std::vector<Record> &ListRecords);

void save_record(double recordTime, std::vector<Record> &ListRecords);

void read_recordsFile(std::vector<Record> &ListRecords, DifficultyRules &rules);

void write_recordsFile(std::vector<Record> &ListRecords, DifficultyRules &rules);

//record


#endif