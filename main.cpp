#include <array>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "Sweeper.h"

const std::string CONFIG_FILE = "config.cfg";

void Main_game(DifficultyRules rules) {
  
  std::vector<Record> records;
  read_recordsFile(records,rules);
  
  unsigned long seed = 0;
  std::srand(seed);
  
  std::chrono::time_point<std::chrono::system_clock> start, end;
  
  Map map;
  create_map(map, rules);
  show_map(map, rules);
  
  bool first_move = true;
  int this_action = 0;
  int SweeperX = -10;
  int SweeperY = 0;
  int total_time;
  int game_status = 1;
  std::string win_player; 

  while (game_status == 1) {
    std::cout << "\033[0;92m";
    std::cout << std::endl
              << "Actions:" << std::endl
              << "1-Reveal position" << std::endl
              << "2-Put/Remove Red flag position" << std::endl
              << "3-See time counter";
    std::cout << std::endl << "Action:";
    std::cin >> this_action;
    //valid input
    valid_input_for_a_int(this_action,this_action);
    //valid input
    if (this_action == 1) {
      std::cout << std::endl << "Position X:";
      std::cin >> SweeperX;
      //valid input
      valid_input_for_a_int(SweeperX,SweeperY);
      //valid input
      
      //debug cheat code
      if (SweeperX == 42 && first_move == false) {
        game_status = 2;
        break;
      }
      //debug cheat code
      std::cout << std::endl << "Position Y:";
      std::cin >> SweeperY;
      //valid input
      valid_input_for_a_int(SweeperX,SweeperY);
      //valid input
      
      game_status = act1_change_map(map, rules, SweeperX, SweeperY, first_move);
      
      if (first_move == true) {
        start = std::chrono::system_clock::now();
      }
      
      first_move = false;
    } else if (this_action == 2) {
      std::cout << std::endl << "Position of RedFlag X:";
      std::cin >> SweeperX;
      //valid input
      valid_input_for_a_int(SweeperX,SweeperY);
      //valid input
      std::cout << std::endl << "Position of RedFlag Y:";
      std::cin >> SweeperY;
      //valid input
      valid_input_for_a_int(SweeperX,SweeperY);
      //valid input
      act2_red_flag(map, rules, SweeperX, SweeperY);
    } else if (this_action == 3) {
      if (first_move == true) {
        std::cout << "Timer only start at first reveal";

      } else {
        end = std::chrono::system_clock::now();
        total_time = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        std::cout << "Elapsed time in seconds: " << total_time << " sec";
      }

    } else {
      std::cout << std::endl << "Invalid Action, only 1 - 3";
    }
    std::cout << "\033[0;37m";
    show_map(map, rules);
  }

  //
  if (game_status == 2) {
    std::cout << std::endl;
    end = std::chrono::system_clock::now();
    total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\033[0;32m";
    std::cout << "You win, your time is: " <<  std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s, ";
    std::cout <<  total_time % 1000 << "ms";
    save_record(total_time,records);
    sort_records(records);
    write_recordsFile(records,rules);
    print_records(records);
    
    }
  
}

void show_usage(void) {
  std::cout << "Usage: game [option]" << std::endl;
  std::cout << "Option:" << std::endl;
  std::cout << " -h or --help                  Display this information."
            << std::endl;
  std::cout << " -r or --records               Display the records."
            << std::endl;
  std::cout << " -d or --difficulty <option>   Change the game difficulty for "
               "<option>"
            << std::endl;
  std::cout << "                               -b or --beginner" << std::endl;
  std::cout << "                               -i or --intermediary"
            << std::endl;
  std::cout << "                               -a or --advanced" << std::endl;
}

void store_difficulty(const std::string config_file, Difficulty level) {
  std::ofstream file;
  file.open(config_file.c_str(), std::ifstream::out);
  if (file.is_open()) {
    switch (level) {
    case Difficulty::beginner:
      file << 'b';
      break;
    case Difficulty::intermediary:
      file << 'i';
      break;
    case Difficulty::advanced:
      file << 'a';
      break;
    }
    file.close();
  }
}

Difficulty load_difficulty(const std::string config_file) {
  Difficulty level;
  std::ifstream file;
  file.open(config_file.c_str(), std::ifstream::in);
  if (file.is_open()) {
    char c;
    file >> c;
    switch (c) {
    case 'b':
      level = Difficulty::beginner;
      break;
    case 'i':
      level = Difficulty::intermediary;
      break;
    case 'a':
      level = Difficulty::advanced;
      break;
    }
    file.close();
  } else {
    store_difficulty(config_file, Difficulty::beginner);
    level = Difficulty::beginner;
  }
  return level;
}

int main(int argc, char **argv) {
  Difficulty level = load_difficulty(CONFIG_FILE);
  DifficultyRules rules;
  
  if (Difficulty::beginner == level) {
    rules.Bombs_N = rules.MapSize_X = rules.MapSize_Y = 10;
    rules.Rule = 0;
  } else if (Difficulty::intermediary == level) {
    rules.MapSize_X = rules.MapSize_Y = 15;
    rules.Bombs_N = 40;
    rules.Rule = 1;
  } else if (Difficulty::advanced == level) {
    rules.MapSize_X = 30;
    rules.MapSize_Y = 15;
    rules.Bombs_N = 100;
    rules.Rule = 2;
  }
  
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "-r" || arg == "--records") {
      std::vector<Record> records;
      read_recordsFile(records,rules);
      print_records(records);
    }
    else if (arg == "-h" || arg == "--help") {
      show_usage();
    } else if (arg == "-d" || arg == "--difficulty") {
      if (argc > 2) {
        std::string newlevel = argv[2];
        if (newlevel == "-b" || newlevel == "--beginner") {
          store_difficulty(CONFIG_FILE, Difficulty::beginner);
        } else if (newlevel == "-i" || newlevel == "-intermediary") {
          store_difficulty(CONFIG_FILE, Difficulty::intermediary);
        } else if (newlevel == "-a" || newlevel == "--advanced") {
          store_difficulty(CONFIG_FILE, Difficulty::advanced);
        } else {
          std::cout << "Unknown difficulty argument: " << newlevel << std::endl;
          show_usage();
        }
      } else {
        std::cout << "It was expected a difficulty for: " << argv[1]
                  << std::endl;
        show_usage();
      }
    }
    else {
      std::cout << "Unknown argument: " << argv[1] << std::endl;
      show_usage();
    }
  } else {
    Main_game(rules);
  }
  return 0;
}