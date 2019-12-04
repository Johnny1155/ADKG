#ifndef ORCUNTOURS_H
#define ORCUNTOURS_H

#include "qpoint3d.h"
#include "edge.h"

#include <vector>

class OrCuntours
{

private:
    std::vector<Edge> orient_cont;

public:
    OrCuntours();

    std::vector<Edge> getOrientCont(){return orient_cont;}

    void setOrientCont(std::vector<Edge> orient_cont_){orient_cont=orient_cont_;}
};

#endif // ORCUNTOURS_H
