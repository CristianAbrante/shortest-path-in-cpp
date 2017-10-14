
#include "ProblemSpecification.hpp"

problemSpecification::problemSpecification(std::string &file_name){

  if (file_name == "") {
    file_name = DEFAULT_FILE_PATH;
  }

  // We open the file in an input stream.
  std::ifstream input_text_file(file_name.c_str());

  if(input_text_file.is_open()) {

    // We read the heusristic number from file and then we cast it
    // to our unum configuration.
    int heuristic_number;
    input_text_file >> heuristic_number;
    heuristic_ = static_cast<heuristicsName>(heuristic_number);

    // We read number of columns and rows.
    input_text_file >> number_of_colums_;
    input_text_file >> number_of_rows_ ;

    // We read the intial car position.
    input_text_file >> car_position_.x;
    input_text_file >> car_position_.y;

    // We read the final position.
    input_text_file >> final_position_.x;
    input_text_file >> final_position_.y;

    // We read the number of obstacles that user want.
    int number_of_obstacles;
    input_text_file >>  number_of_obstacles;


    if (variablesAreConfigured(number_of_obstacles)) {

      obstacle_positions_.resize(number_of_obstacles);

      int obstacles_entered = 0;
      while (!input_text_file.eof() && obstacles_entered < number_of_obstacles) {

        int x_pos, y_pos;
        input_text_file >> x_pos >> y_pos;

        if (!input_text_file.eof() && !positionIsIntroduced({x_pos, y_pos}, obstacles_entered) ){
          obstacle_positions_[obstacles_entered].x = x_pos;
          obstacle_positions_[obstacles_entered].y = y_pos;

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
    return car_position_;
}

position problemSpecification::final_position(void) const {
    return final_position_;
}

int problemSpecification::getNumberOfObstaces(void) const {
    return obstacle_positions_.size();
}

position problemSpecification::getObstacle(int i) const {
    return obstacle_positions_[i];
}

// PRIVATE METHODS.

bool problemSpecification::variablesAreConfigured(int number_of_obstacles) const {

  bool variables_are_configured = true;

  if (heuristic_ < 0 || heuristic_ >= NUMBER_OF_HEURISTICS)
    variables_are_configured = false;

  if (number_of_rows_ < 0 || number_of_rows_ > MAX_ROW)
    variables_are_configured = false;

  if (number_of_colums_ < 0 || number_of_colums_ > MAX_COLUMN)
    variables_are_configured = false;

  if (car_position_.x < 0 || car_position_.x >= number_of_colums_ ||
      car_position_.y < 0 || car_position_.y >= number_of_colums_)
    variables_are_configured = false;

  if (final_position_.x < 0 || final_position_.x >= number_of_colums_ ||
      final_position_.y < 0 || final_position_.y >= number_of_colums_)
    variables_are_configured = false;

  if (number_of_obstacles < 0 || number_of_obstacles > (number_of_rows_ * number_of_colums_ - 2))
    variables_are_configured = false;

}

void problemSpecification::generateRandomObstacles(int obstacles_to_generate) {

  std::random_device rd;     // only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
  std::uniform_int_distribution<int> uni(0, rows() * columns() - 1); // guaranteed unbiased

  int current_position = 0;

  int vector_index = obstacle_positions_.size() - obstacles_to_generate;

  int i = 0;
  while (i < obstacles_to_generate) {

    int random_vector_position = uni(rng);

    current_position = (current_position + random_vector_position) % (rows() * columns());

    position matrix_position = matrixPos(current_position);

    if (!positionIsIntroduced(matrix_position, vector_index)) {
      obstacle_positions_[vector_index].x = matrix_position.x;
      obstacle_positions_[vector_index].y = matrix_position.y;
      vector_index++;
      i++;
    }

  }

}

position problemSpecification::matrixPos(int vector_position) const {

  position matrix_pos;

  matrix_pos.x = static_cast<int>(vector_position / columns());
  matrix_pos.y = vector_position - (matrix_pos.x  * columns());

  return matrix_pos;
}

bool problemSpecification::positionIsIntroduced(position to_check_position, int obstacle_entered) const {

  if (to_check_position.x < 0 || to_check_position.x >= number_of_colums_ ||
      to_check_position.y < 0 || to_check_position.y >= number_of_rows_)
      return false;

  for (int i = 0; i < obstacle_entered; ++i) {
    if (to_check_position.x == obstacle_positions_[i].x &&
        to_check_position.y == obstacle_positions_[i].y )
          return true;
  }

  if (to_check_position.x == car_position_.x &&
      to_check_position.y == car_position_.y )
      return true;

  if (to_check_position.x == final_position_.x &&
      to_check_position.y == final_position_.y )
      return true;

  return false;

}
