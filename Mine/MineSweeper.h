#ifndef MINESWEEPER_MINESWEEPTER_H
#define MINESWEEPER_MINESWEEPTER_H
#include <vector>
#include <iostream>
#include <random>
#include <map>

const unsigned int default_n = 8;
const unsigned int min_n = 3;
const double default_prop = 0.1;
const unsigned int max_rebuild = 10;

enum macro_state {BLANK, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, CLOSE, FLAG, MINE, BOOM};
enum game_state {PLAYING, WIN, LOSE};

class Mine{
public:
    explicit Mine(unsigned int rows = default_n, unsigned int cols = default_n, double prop = default_prop)
        : n_rows(rows), n_cols(cols), proportion(prop), n_mines(0), first_click_flag(true),
          n_visited(0), n_flags(0), board(0), state(nullptr), state_map()
    {}
    Mine(const Mine &) = delete;
    Mine& operator=(const Mine*) = delete;
    ~Mine() {
        delete state;
    }

    // 初始化
    void init();
    // 重建棋盘
    void rebuild(unsigned int rows, unsigned int cols, double prop);

    // 点击操作
    void left_click(unsigned int row, unsigned col);
    void right_click(unsigned int row, unsigned col);

    // 棋盘表示
    std::vector<std::vector<macro_state>>* get_state() { return state; };

    // 返回行、列、雷比例、剩余雷数
    unsigned int get_rows() { return n_rows; };
    unsigned int get_cols() { return n_cols; };
    double get_prop() { return proportion; };
    int get_mines() { return static_cast<int>(n_mines) - static_cast<int>(n_flags); };

    // 状态
    game_state get_game_state() { return state_flag; };

    // 测试
    void print_board();

private:
    unsigned int n_rows;
    unsigned int n_cols;
    double proportion;              // 雷的比例
    unsigned int n_mines;           // 雷数
    bool first_click_flag;          // 是否为第一次点击
    unsigned int n_visited;         // 翻开的格子数量
    unsigned int n_flags;           // 旗子的数量
    game_state state_flag;
    std::vector<std::vector<int>> board;                // 棋盘表示：雷(-1)，或周围雷的数量
    std::vector<std::vector<macro_state>> *state;        // 外部表示：未翻开、已翻开、插旗
    std::map<int, macro_state> state_map;               // map，数字->棋盘状态

    // 打乱雷的分布
    void shuffle();
    // 扩散空白区域
    void spread(unsigned int row, unsigned col);
    // 计算雷数
    void count_mines(unsigned int row, unsigned col);

};

#endif //MINESWEEPER_MINESWEEPTER_H
