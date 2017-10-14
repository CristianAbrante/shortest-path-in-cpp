
#ifndef IA_P1_PROBLEMSPECIFICATION_H
#define IA_P1_PROBLEMSPECIFICATION_H

#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>

typedef struct {
    int x;
    int y;
} position;

enum heuristicsName {HEURISTIC_1, HEURISTIC_2, HEURISTIC_3};

class problemSpecification {
    private:
        heuristicsName heuristic_;
        int M_;
        int N_;
        position car_position_;
        position final_position_;
        std::vector<position> obstacle_positions_;
    public:
        problemSpecification(std::ifstream &textFile);
        ~problemSpecification();
        int heuristic(void)const;
        int row(void)const;
        int column(void)const;
        position car_position(void)const;
        position final_position(void)const;
        int getNumberOfObstaces(void) const;
        position getObstacle(int i) const;
};

#endif //IA_P1_PROBLEMSPECIFICATION_H
