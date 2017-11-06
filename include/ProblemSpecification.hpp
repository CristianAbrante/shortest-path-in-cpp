
#ifndef IA_P1_PROBLEMSPECIFICATION_H
#define IA_P1_PROBLEMSPECIFICATION_H

#include <algorithm>
#include <chrono>
#include <fstream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

// struct that defines a position.
typedef struct {
    unsigned int x;
    unsigned int y;
} position;

// ENumeration of heuristics
enum heuristicsName {NOT_HEUSRISTIC, HEURISTIC_1, HEURISTIC_2, HEURISTIC_3};

// Number of heuristics to solve the problem.
const int NUMBER_OF_HEURISTICS = 4;

// Max size of the grid.
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
  int car_position_;
  int final_position_;
  std::vector<int> obstacle_positions_;

  position matrixPos(const int vector_position) const;
  int vectorPos(const position matrix_position) const;

  bool variablesAreConfigured(int number_of_obstacles) const;
  void generateRandomObstacles(int obstacles_to_generate);

  bool positionIsIntroduced(int to_check_position) const;
  void eraseIntroducedPositions(std::vector<int> &posible_obstacles) const;
};

#endif //IA_P1_PROBLEMSPECIFICATION_H
