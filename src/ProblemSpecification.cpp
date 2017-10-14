
#include "ProblemSpecification.hpp"

problemSpecification::problemSpecification(std::ifstream &textFile){

    int obstacle = 0;

    if(textFile.is_open()){

        int heuristic_number;

        textFile >> heuristic_number;
        heuristic_ = (heuristicsName)heuristic_number;

        textFile >> M_ >> N_ >> car_position_.x >> car_position_.y >> final_position_.x >> final_position_.y >> obstacle;

        if((heuristic_ < 0) || (M_ < 1) || (N_ < 1) ||
            (car_position_.x < 0) || (car_position_.x > M_) ||
            (car_position_.y < 0) || (car_position_.y > N_) || (final_position_.x < 0) ||
            (final_position_.x > M_) || (final_position_.y < 0) || (final_position_.y > N_) ||
            (obstacle < -1) || (obstacle > (N_ * M_ - 2)))
            throw "TODO";

    } else {
        throw "TODO";
    }

    if (obstacle != -1) {

        obstacle_positions_.resize(obstacle);

        for (int i = 0; i < obstacle; i++) {
            textFile >> obstacle_positions_[i].x;
            textFile >> obstacle_positions_[i].y;
        }

    }

}

problemSpecification::~problemSpecification(){}

int problemSpecification::heuristic(void) const {
    return heuristic_;
}

int problemSpecification::row(void) const {
    return N_;
}

int problemSpecification::column(void) const {
    return M_;
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
