#include <cassert>
#include <iostream>

class Board {
public:
    std::int8_t _board[8][8];
    int movenum;
    int score;
    int pass_count;
    int side;
    
    Board() {
        for(int y=0; y<8; y++) {
            for(int x=0; x<8; x++) {
                _board[y][x] = 0;
            }
        }
        _board[3][3] = 1;
        _board[4][4] = 1;
        _board[3][4] = -1;
        _board[4][3] = -1;
        score = 0;
        pass_count = 0;
        side = 1;
        movenum = 0;
    }
    Board(const Board & b) {
        for(int y=0; y<8; y++) {
            for(int x=0; x<8; x++) {
                _board[y][x] = b._board[y][x];
            }
        }
        score = b.score;
        pass_count = b.pass_count;
        side = b.side;
        movenum = b.movenum;
    }
    
    void print() {
        for(int y=0; y<8; y++) {
            for(int x=0; x<8; x++) {
                if(_board[y][x] == 0) {
                    std::cerr << " .";
                } else if(_board[y][x] == 1) {
                    std::cerr << " O";
                } else if(_board[y][x] == -1) {
                    std::cerr << " X";
                }
            }
            std::cerr << std::endl;
        }
        std::cerr << score << " " << pass_count << " " << side << std::endl;;
    }
    bool check_flip(int x, int y, int xo, int yo) const {
        x += xo;
        y += yo;
        if(x >= 8 || x < 0 || y >= 8 || y < 0) {
            return false;
        }
        if(_board[y][x] != -side) {
            return false;
        }
        while(true) {
            x += xo;
            y += yo;
            if(x >= 8 || x < 0 || y >= 8 || y < 0) {
                return false;
            }
            if(_board[y][x] == side) {
                return true;
            }
            else if(_board[y][x] == 0) {
                return false;
            }
        }
    }
    
    void do_flip(int x, int y, int xo, int yo) {
        if(!check_flip(x, y, xo, yo)) {
            return;
        }
        while(true) {
            x += xo;
            y += yo;
            if(x >= 8 || x < 0 || y >= 8 || y < 0) {
                assert(false);
            }
            if(_board[y][x] == 0) {
                assert(false);
            }
            else if(_board[y][x] == side) {
                return;
            }
            else {
                _board[y][x] = side;
                score += 2 * side;
            }
        }
    }
    
    bool move(int x, int y) {
        if(_board[y][x] != 0) return false;
        bool legal = false;
        if(check_flip(x, y,  1,  0)) { do_flip(x, y,  1,  0); legal = true; }
        if(check_flip(x, y, -1,  0)) { do_flip(x, y, -1,  0); legal = true; }
        if(check_flip(x, y,  0,  1)) { do_flip(x, y,  0,  1); legal = true; }
        if(check_flip(x, y,  0, -1)) { do_flip(x, y,  0, -1); legal = true; }
        if(check_flip(x, y,  1,  1)) { do_flip(x, y,  1,  1); legal = true; }
        if(check_flip(x, y, -1,  1)) { do_flip(x, y, -1,  1); legal = true; }
        if(check_flip(x, y,  1, -1)) { do_flip(x, y,  1, -1); legal = true; }
        if(check_flip(x, y, -1, -1)) { do_flip(x, y, -1, -1); legal = true; }

        if(legal) {
            _board[y][x] = side;
            score += side;
            pass_count = 0;
            side = -side;
            movenum++;
        }
        return legal;
    }

    bool is_legal_move(int x, int y) const {
        if(_board[y][x] != 0) return false;
        if(check_flip(x, y,  1,  0)) return true;
        if(check_flip(x, y, -1,  0)) return true;
        if(check_flip(x, y,  0, -1)) return true;
        if(check_flip(x, y,  0,  1)) return true;
        if(check_flip(x, y,  1,  1)) return true;
        if(check_flip(x, y, -1,  1)) return true;
        if(check_flip(x, y,  1, -1)) return true;
        if(check_flip(x, y, -1, -1)) return true;
        return false;
    }
    
    void pass() {
        pass_count++;
        side = -side;
        movenum++;
    }
    bool winner() {
        if(pass_count == 2) {
            return score;
        } else {
            return 0;
        }
    }
};

int find_max_score(Board & b, int xp, int yp, int polarity, int depth) {
    if(depth == 0) {
        if(b.winner() == 0) {
            return b.score;
        } else if(b.winner() > 0) {
            return 100;
        } else {
            return -100;
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
                int score = polarity * find_max_score(bb, x, y, -polarity, depth-1);
                if(b.movenum + depth < 64) {
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
        return find_max_score(b, -1, -1, -polarity, depth-1);
    } else {
        return polarity * (max_score + num_legal_moves);
    }
}

void think(Board & b, int depth=8) {
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
                int score = b.side * find_max_score(bb, x, y, -b.side, depth-1);
                if(b.movenum + depth < 64) {
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
        auto mv = b.move(best_x, best_y);
        assert(mv);
    } else {
        b.pass();
    }
}

int main() {
    Board b;
    int x, y;
    while(b.winner() == 0 && b.pass_count < 2) {
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

        if(x == -2 && y == -2) {
            think(b);
        } else if(x == -1 && y == -1) {
            b.pass();
        } else {
            auto mv = b.move(x, y);
            assert(mv);
        }
    }

    return 0;
}

