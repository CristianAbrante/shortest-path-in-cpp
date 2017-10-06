
#ifndef IA_P1_FILE_H
#define IA_P1_FILE_H

#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>

typedef struct{
    int x;
    int y;
} position;

enum heuristicsName {HEURISTIC_1, HEURISTIC_2, HEURISTIC_3}

class File {
    private:
        heuristicsName heuristic_;
        int M_;
        int N_;
        position cache_;
        position final_;
        std::vector<position> obstacle_positions_;
    public:
        File(char name[]);
        ~File();
        int heuristic(void)const;
        int row(void)const;
        int column(void)const;
        position pos_cache(void)const;
        position pos_final(void)const;
        int getNumberOfObstaces(void) const;
        position getObstacle(int i) const;
};

#endif //IA_P1_FILE_H