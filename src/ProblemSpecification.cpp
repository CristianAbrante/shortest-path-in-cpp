
#include "ProblemSpecification.hpp"

problemSpecification::problemSpecification(std::string &file_name){

  if (file_name == "") {
    file_name = DEFAULT_FILE_PATH;
  }

  // We open the file in an input stream.
  std::ifstream input_text_file(file_name.c_str());

  if(input_text_file.is_open()) {

    // We read the heusristic number from file and then we cast it
    // to our enum configuration.
    int heuristic_number;
    input_text_file >> heuristic_number;
    heuristic_ = static_cast<heuristicsName>(heuristic_number);

    // We read number of columns and rows.
    input_text_file >> number_of_colums_;
    input_text_file >> number_of_rows_ ;

    position car_position;
    // We read the intial car position.
    input_text_file >> car_position.x;
    input_text_file >> car_position.y;

    // We store the car position in vector.
    car_position_ = vectorPos(car_position);

    position final_position;
    // We read the final position.
    input_text_file >> final_position.x;
    input_text_file >> final_position.y;

    // We store the final position in vector.
    final_position_ = vectorPos(final_position);

    // We read the number of obstacles that user want.
    int number_of_obstacles;
    input_text_file >>  number_of_obstacles;


    if (variablesAreConfigured(number_of_obstacles)) {

      int obstacles_entered = 0;

      while (!input_text_file.eof() && obstacles_entered < number_of_obstacles) {

        position obstacle_pos;
        input_text_file >> obstacle_pos.x;
        input_text_file >> obstacle_pos.y;

        if (!input_text_file.eof() && !positionIsIntroduced(vectorPos(obstacle_pos)) ){
          obstacle_positions_.push_back(vectorPos(obstacle_pos));
          obstacles_entered++;
        }

      }

      if (obstacles_entered < number_of_obstacles) {
        generateRandomObstacles(number_of_obstacles - obstacles_entered);
      }


    } else {

      input_text_file.close();
      throw std::out_of_range("One of the arguments is out of range.");
    }

  } else {
    throw std::invalid_argument("Error loading the configuration file of the project.");
  }
}

problemSpecification::~problemSpecification(){}

heuristicsName problemSpecification::heuristic(void) const {
    return heuristic_;
}

int problemSpecification::rows(void) const {
    return number_of_rows_;
}

int problemSpecification::columns(void) const {
    return number_of_colums_;
}

position problemSpecification::car_position(void) const {
    return matrixPos(car_position_);
}

position problemSpecification::final_position(void) const {
    return matrixPos(final_position_);
}

int problemSpecification::getNumberOfObstaces(void) const {
    return obstacle_positions_.size();
}

position problemSpecification::getObstacle(int i) const {
    return matrixPos(obstacle_positions_[i]);
}

// PRIVATE METHODS.

int problemSpecification::vectorPos(const position matrix_position) const {
  return matrix_position.y + number_of_colums_ * matrix_position.x;
}

position problemSpecification::matrixPos(const int vector_position) const {

  position matrix_pos;

  matrix_pos.x = static_cast<int>(vector_position / columns());
  matrix_pos.y = vector_position - (matrix_pos.x  * columns());

  return matrix_pos;
}


bool problemSpecification::variablesAreConfigured(int number_of_obstacles) const {

  // Check if the problem settings are in the range.

  if (heuristic_ < 0 || heuristic_ >= NUMBER_OF_HEURISTICS)
    return false;

  if (number_of_rows_ < 0 || number_of_rows_ > MAX_ROW)
    return false;

  if (number_of_colums_ < 0 || number_of_colums_ > MAX_COLUMN)
    return false;

  if (car_position_ < 0 || car_position_ >= number_of_colums_ * number_of_rows_)
    return false;

  if (final_position_ < 0 || final_position_ >= number_of_colums_ * number_of_rows_)
    return false;

  if (number_of_obstacles < 0 || number_of_obstacles > (number_of_rows_ * number_of_colums_ - 2))
    return false;

  return true;
}

bool problemSpecification::positionIsIntroduced(int to_check_position) const {

  // Check if position is on the vector range.
  if (to_check_position < 0 || to_check_position >= number_of_colums_ * number_of_rows_)
      return false;

  // Check if position is alredy in the vector
  for (int i = 0; i < obstacle_positions_.size(); ++i) {
    if (obstacle_positions_[i] == to_check_position)
        return true;
  }

  // Check if position is the car position or the final position.
  if (to_check_position == car_position_ || to_check_position == final_position_)
      return true;

  return false;

}


void problemSpecification::generateRandomObstacles(int obstacles_to_generate) {

  // Vector that store all the posible obstacle positions.
  std::vector<int> posible_obstacles;

  // filling the vector with all the positions.
  for (int i = 0; i < number_of_rows_ * number_of_colums_; ++i)
    posible_obstacles.push_back(i);

  // erasing objets position previously introduced.
  // Also we erase car and final position.
  eraseIntroducedPositions(posible_obstacles);

  // We shuffle the vector.

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(posible_obstacles.begin(), posible_obstacles.end(), std::default_random_engine(seed));

  // We introudce in the vector the random obstacles.
  for (int i = 0; i < obstacles_to_generate; ++i)
    obstacle_positions_.push_back(posible_obstacles[i]);

}

void problemSpecification::eraseIntroducedPositions(std::vector<int> &posible_obstacles) const {

  // We erase all the posible obstacles from the vector.
  for (auto obstacle : obstacle_positions_) {

    if (*posible_obstacles.end() == obstacle) {
        posible_obstacles.pop_back();
    } else {
      posible_obstacles.erase(posible_obstacles.begin() + obstacle);
    }

  }

  if (*posible_obstacles.end() == car_position_) {
    posible_obstacles.pop_back();
  } else {
    posible_obstacles.erase(posible_obstacles.begin() + car_position_);
  }

  if (*posible_obstacles.end() == final_position_) {
    posible_obstacles.pop_back();
  } else {
    posible_obstacles.erase(posible_obstacles.begin() + final_position_);
  }

}
