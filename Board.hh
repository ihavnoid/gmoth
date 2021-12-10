#ifndef __GMOTH_BOARD_HH__
#define __GMOTH_BOARD_HH__
class Board {
private:
    std::int8_t _board[8][8];
    int movenum_;
    int score_;
    int pass_count_;
    int side_;
private:
    bool check_flip(int x, int y, int xo, int yo) const {
        x += xo;
        y += yo;
        if(x >= 8 || x < 0 || y >= 8 || y < 0) {
            return false;
        }
        if(_board[y][x] != -side_) {
            return false;
        }
        while(true) {
            x += xo;
            y += yo;
            if(x >= 8 || x < 0 || y >= 8 || y < 0) {
                return false;
            }
            if(_board[y][x] == side_) {
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
            else if(_board[y][x] == side_) {
                return;
            }
            else {
                _board[y][x] = side_;
                score_ += 2 * side_;
            }
        }
    }
public:
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
        score_ = 0;
        pass_count_ = 0;
        side_ = 1;
        movenum_ = 0;
    }
    Board(const Board & b) {
        for(int y=0; y<8; y++) {
            for(int x=0; x<8; x++) {
                _board[y][x] = b._board[y][x];
            }
        }
        score_ = b.score_;
        pass_count_ = b.pass_count_;
        side_ = b.side_;
        movenum_ = b.movenum_;
    }

    int score() {
        return score_;
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
            _board[y][x] = side_;
            score_ += side_;
            pass_count_ = 0;
            side_ = -side_;
            movenum_++;
        }
        return legal;
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
        std::cerr << score_ << " " << pass_count_ << " " << side_ << std::endl;;
    }
    void pass() {
        pass_count_++;
        side_ = -side_;
        movenum_++;
    }
    bool done() const {
        return pass_count_ >= 2;
    }
    int movenum() const {
        return movenum_;
    }
    int side() const {
        return side_;
    }
    int winner() const {
        if(done()) {
            return score_ > 0 ? 1 : score_ < 0 ? -1 : 0;
        } else {
            return 0;
        }
    }
};


#endif
