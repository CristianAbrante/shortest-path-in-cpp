#include "file.hpp"

File::File(char name[]){

    ifstream textfile;
    textfile.open(name);

    int obstacle = 0;

    if(textfile.is_open()){

        textfile >> heuristic_;
        textfile >> M_ >> N_ >> cache_.x >> cache_.y >> final_.x >> final.y >> obstacle;

        if((heuristic_ < 0) || (M_ < 1) || (N_ < 1) ||
            (cache_.x < 0) || (cache_.x > M_) ||
            (cache_.y < 0) || (cache_.y > N_) || (final_.x < 0) ||
            (final_.x > M_) || (final.y < 0) || (final.y > N_) ||
            (obstacle < -1) || (obstacle > (N_*M_-2)))
            throw "TODO";

    } else {
        throw "TODO";
    }

    if (obstacle != -1) {

        obstacle_positions_.resize(obstacle);
        int index = 0;

        while (index < obstacle) {

            textfile >> obstacle_positions_[index].x;
            textfile >> obstacle_positions_[index].y;

            index++;
        }

    }

    textfile.close();
}

File::~File(){}

int File::heuristic(void) const {
    return heuristic_;
}

int File::row(void) const {
    return M_;
}

int File::column(void) const {
    return N_;
}

position File::pos_cache(void) const {
    return cache_;
}

position File::pos_final(void) const {
    return final_;
}

int File::getNumberOfObstaces(void) const {
    return obstacle_positions_.size();
}

position File::getObstacle(int i) const {
    return obstacle_positions_[i];
}
