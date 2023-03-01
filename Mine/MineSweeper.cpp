#include "MineSweeper.h"

void Mine::shuffle() {
    // 毫秒级的种子
    struct timeb timeSeed{};
    ftime(&timeSeed);
    std::default_random_engine e(timeSeed.time * 1000 + timeSeed.millitm);
    std::uniform_int_distribution<unsigned int> uid_row(0, this->n_rows - 1);
    std::uniform_int_distribution<unsigned int> uid_col(0, this->n_cols - 1);
    unsigned int i_row = 0, i_col = 0;
    for (int i = 0; i < this->n_rows; ++i) {
        for (int j = 0; j < this->n_cols; ++j) {
            i_row = uid_row(e);
            i_col = uid_col(e);
            std::swap(this->board[i][j], this->board[i_row][i_col]);
        }
    }
}

void Mine::init() {
    // 检查长宽是否合理
    if (n_rows < min_n || n_cols < min_n) {
        // 长宽太小，初始化失败
        std::cout << "[error] 长宽格子数量太少，初始化失败" << std::endl;
        return;
    }
    // 不修改棋盘的长宽与雷的比例
    const unsigned int rows = n_rows;
    const unsigned int cols = n_cols;
    const double prop = proportion;

    // 雷数
    auto mines = static_cast<unsigned int>(1. * rows * cols * prop);
    mines = mines == 0 ? 1 : mines;
    this->n_mines = mines;

    // 生成状态矩阵
    if (state == nullptr) {
        state = new std::vector<std::vector<macro_state>>(0);
    }
    state->resize(rows);
    for (auto & e : *state) {
        e.resize(cols, CLOSE);
    }

    // 生成map，建立数字到状态的映射
    if (state_map.empty()) {
        for (int i = 0; i <= 8; ++i) {
            state_map[i] = static_cast<macro_state>(i);
        }
        state_map[-1] = MINE;
    }

    // 按雷的比例生成棋盘，前n*prop个为雷
    board.resize(rows);
    for (auto & e : board) {
        if (mines == 0) {               // 无雷
            e.resize(cols, 0);
        } else if (mines >= cols) {     // 雷数多于当前列数，整行均放雷
            e.resize(cols, -1);
            mines -= cols;
        } else {                         // 雷数不足一行
            e.resize(cols, 0);
            while (mines > 0) {
                e[mines - 1] = -1;
                --mines;
            }
        }
    }

    // 打乱棋盘
    shuffle();

    // 生成数字
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] == -1) {
                count_mines(i, j);
            }
        }
    }

    // 更新棋盘状态
    state_flag = PLAYING;
}

void Mine::print_board() {

    // 打印棋盘
    std::cout << "棋盘参数：" << n_rows << " x " << n_cols << ", 雷数：" << n_mines << std::endl;

    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // 打印棋盘状态
    std::cout << "棋盘状态：" << std::endl;
    std::map<macro_state, std::string> mp = {
            {BLANK, " 0 "}, {ONE, " 1 "}, {TWO, " 2 "}, {THREE, " 3 "}, {FOUR, " 4 "}, {FIVE, " 5 "},
            {SIX, " 6 "}, {SEVEN, " 7 "}, {EIGHT, " 8 "}, {CLOSE, "[ ]"}, {FLAG, " |>"}, {MINE, " * "}
    };
    for (unsigned int i = 0; i < n_rows; ++i) {
        std::cout << "|";
        for (unsigned int j = 0; j < n_cols; ++j) {
            std::cout << mp[(*state)[i][j]] << "|";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Mine::rebuild(unsigned int rows, unsigned int cols, double prop) {
    if (rows != this->n_rows && cols != this->n_cols && prop != this->proportion) {
        this->n_rows = rows;
        this->n_cols = cols;
        this->proportion = prop;
    }
    board.clear();
    (*state).clear();
    first_click_flag = true;
    n_visited = 0;
    n_flags = 0;
    return init();
}

void Mine::left_click(unsigned int row, unsigned int col) {
    // 检查，如果游戏结束，什么也不做
    if (state_flag != PLAYING)
        return;
    // 检查，如果已翻开或插旗，什么也不做
    if ((*state)[row][col] == FLAG || int((*state)[row][col]) <= 8)
        return;
    // 第一次如果不是空白，重新生成棋盘
    if (first_click_flag) {
        while (board[row][col] != 0) {
            print_board();
            rebuild(n_rows, n_cols, proportion);
        }
        first_click_flag = false;
    }
    // 点击该格子
    const int value = board[row][col];
    if (value == -1) {          // 雷，爆炸
        (*state)[row][col] = BOOM;
        state_flag = LOSE;
    } else if (value == 0) {    // 空白，执行区域扩散
        spread(row, col);
    } else {                    // 正常翻开该格子
        (*state)[row][col] = state_map[board[row][col]];
        ++n_visited;
        // 检查是否扫雷成功
        if (n_visited + n_mines == n_cols * n_rows)
            state_flag = WIN;
    }
}

void Mine::spread(unsigned int row, unsigned int col) {
    (*state)[row][col] = state_map[board[row][col]];
    ++n_visited;
    if (board[row][col] > 0)    return; // 非空白，扩散结束
    int rows = static_cast<int>(row);
    int cols = static_cast<int>(col);
    for (int i_row = rows - 1; i_row <= rows + 1; ++i_row) {
        for (int i_col = cols - 1; i_col <= cols + 1; ++i_col) {
            if (i_row >= 0 && i_row < n_rows && i_col >= 0 && i_col < n_cols && (*state)[i_row][i_col] == CLOSE) {
                spread(i_row, i_col);
            }
        }
    }
}

void Mine::right_click(unsigned int row, unsigned int col) {
    // 检查，如果游戏结束，什么也不做
    if (state_flag != PLAYING)
        return;

    const macro_state value = (*state)[row][col];

    if (int(value) <= 8)  // 如果已翻开，就什么也不做
        return;
    if (value == FLAG){    // 如果已插旗，将其取消
        (*state)[row][col] = CLOSE;
        --n_flags;
    }
    else if (value == CLOSE) {  // 如果没插旗，插旗
        (*state)[row][col] = FLAG;
        ++n_flags;
    }
}

void Mine::count_mines(unsigned int row, unsigned int col) {
    int rows = static_cast<int>(row);
    int cols = static_cast<int>(col);
    for (int i_row = rows - 1; i_row <= rows + 1; ++i_row) {
        for (int i_col = cols - 1; i_col <= cols + 1; ++i_col) {
            if (i_row >= 0 && i_row < n_rows && i_col >= 0 && i_col < n_cols && board[i_row][i_col] != -1) {
                ++board[i_row][i_col];
            }
        }
    }
}
