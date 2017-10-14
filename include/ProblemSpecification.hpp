
#ifndef IA_P1_PROBLEMSPECIFICATION_H
#define IA_P1_PROBLEMSPECIFICATION_H

#include <fstream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

// struct that defines the position.

typedef struct {
    int x;
    int y;
} position;

enum heuristicsName {HEURISTIC_1, HEURISTIC_2, HEURISTIC_3};

// Number of heuristics to solve the problem.
const int NUMBER_OF_HEURISTICS = 3;

const int MAX_COLUMN = 1000;
const int MAX_ROW = 1000;

// The default path of configuration file.
const std::string DEFAULT_FILE_PATH = "test/default.txt";

class problemSpecification {

 public:

  // Constructor of the class: argument is the path of the
  // problem file.
  problemSpecification(std::string &file_name);

  // Destructor of the class.
  ~problemSpecification();

  // Returns the heuristic that the user want for the
  // problem.
  heuristicsName heuristic(void)const;

  // Returns the number of rows that the user want for the grid.
  int rows(void)const;

  // Returns the number of columns that the user want for the grid.
  int columns(void)const;

  // Return the initial car position that the user want.
  position car_position(void)const;

  // Return the final position that the user want.
  position final_position(void)const;

  // Return the number of obstacles that the usar want to add to
  // the problem.
  int getNumberOfObstaces(void) const;

  // Return the specified i-object that the user want to get.
  position getObstacle(int i) const;

 private:

  heuristicsName heuristic_;
  int number_of_colums_;
  int number_of_rows_;
  position car_position_;
  position final_position_;
  std::vector<position> obstacle_positions_;

  bool variablesAreConfigured(int number_of_obstacles) const;
  void generateRandomObstacles(int obstacles_to_generate);
  position matrixPos(int vector_position) const;
  bool positionIsIntroduced(position to_check_position, int obstacle_entered) const;

};

#endif //IA_P1_PROBLEMSPECIFICATION_H
