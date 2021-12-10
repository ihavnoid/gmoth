#include <tuple>
#include <cassert>
#include <iostream>

#include "SimpleMinMaxEngine.hh"

int main() {
    Board b;
    int x, y;
    while(!b.done()) {
        b.print();
        bool had_legal_move = false;
        {
            for(y=0; y<8; y++) {
                for(x=0; x<8; x++) {
                    if(b.is_legal_move(x, y)) {
                        std::cerr << "#LEGAL_MOVE " << x << " " << y << std::endl;
                        had_legal_move = true;
                    }
                }
            }
            if(!had_legal_move) {
                std::cerr << "#LEGAL_MOVE " << -1 << " " << -1 << std::endl;
            }
        }
        std::cin >> x >> y;
    
        if(std::cin.eof()) {
            return 0;
        }

        SimpleMinMaxEngine e(10);
        if(x == -2 && y == -2) {
            std::tie(x, y) = e.think(b);
            auto mv = b.move(x, y);
            assert(mv);
        } else if(x == -1 && y == -1) {
            b.pass();
        } else {
            auto mv = b.move(x, y);
            assert(mv);
        }
    }

    return 0;
}

