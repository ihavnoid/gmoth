#ifndef __GMOTH_SIMPLE_MIN_MAX_ENGINE_HH__
#define __GMOTH_SIMPLE_MIN_MAX_ENGINE_HH__

#include "Engine.hh"

class SimpleMinMaxEngine : public Engine {
private:
    int depth_ = 8;
    int find_max_score(Board & b, int polarity, int depth) {
        if(depth == 0) {
            if(b.winner() == 0) {
                return b.score();
            } else {
                return b.winner() * 100;
            }
        }
    
        const int bias_table[8][8] = {
            {30, -5, 0, 0, 0, 0, -5, 30},
            {-5, -10, 0, 0, 0, 0, -10, -5},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {-5, -10, 0, 0, 0, 0, -10, -5},
            {30, -5, 0, 0, 0, 0, -5, 30},
        };
        int max_score = -100;
        int num_legal_moves = 0;
        for(int y=0; y<8; y++) {
            for(int x=0; x<8; x++) {
                Board bb = b;
                if(bb.move(x, y)) {
                    int score = polarity * find_max_score(bb, -polarity, depth-1);
                    if(b.movenum() + depth < 64) {
                        score += bias_table[x][y];
                    }
                    max_score = std::max(score, max_score);
                    num_legal_moves++;
    //                for(int i=0; i<4-depth; i++) {
    //                    std::cerr << "  ";
    //                }
    //                std::cerr << "#MOVE " << x << " " << y << " " << score << std::endl;
                }
            }
        }
        if(num_legal_moves == 0) {
            b.pass();
            return find_max_score(b, -polarity, depth-1);
        } else {
            return polarity * (max_score + num_legal_moves);
        }
    }
    
public:
    SimpleMinMaxEngine(int depth) : depth_(depth) {}
    std::pair<int,int> think(const Board & b) {
        int max_score = -100;
        int best_x = -1;
        int best_y = -1;
    
        const int bias_table[8][8] = {
            {20, -5, 0, 0, 0, 0, -5, 20},
            {-5, -10, 0, 0, 0, 0, -10, -5},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
            {-5, -10, 0, 0, 0, 0, -10, -5},
            {20, -5, 0, 0, 0, 0, -5, 20},
        };
        for(int y=0; y<8; y++) {
            for(int x=0; x<8; x++) {
                Board bb = b;
                if(bb.move(x, y)) {
                    int score = b.side() * find_max_score(bb, -b.side(), depth_-1);
                    if(b.movenum() + depth_ < 64) {
                        score += bias_table[x][y];
                    }
                    if(score > max_score) {
                        best_x = x; best_y = y; max_score = score;
                    }
                }
            }
        }
        std::cout << best_x << " " << best_y << std::endl;
        if(best_x >= 0) {
            return std::make_pair(best_x, best_y);
        } else {
            return std::make_pair(-1, -1);
        }
    }
};

#endif
