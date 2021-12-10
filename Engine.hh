#ifndef __GMOTH_ENGINE_HH__
#define __GMOTH_ENGINE_HH__

#include "Board.hh"

class Engine {
public:
    virtual std::pair<int,int> think(const Board & b) = 0;
};
#endif

