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

    // ��ʼ��
    void init();
    // �ؽ�����
    void rebuild(unsigned int rows, unsigned int cols, double prop);

    // �������
    void left_click(unsigned int row, unsigned col);
    void right_click(unsigned int row, unsigned col);

    // ���̱�ʾ
    std::vector<std::vector<macro_state>>* get_state() { return state; };

    // �����С��С��ױ�����ʣ������
    unsigned int get_rows() { return n_rows; };
    unsigned int get_cols() { return n_cols; };
    double get_prop() { return proportion; };
    int get_mines() { return static_cast<int>(n_mines) - static_cast<int>(n_flags); };

    // ״̬
    game_state get_game_state() { return state_flag; };

    // ����
    void print_board();

private:
    unsigned int n_rows;
    unsigned int n_cols;
    double proportion;              // �׵ı���
    unsigned int n_mines;           // ����
    bool first_click_flag;          // �Ƿ�Ϊ��һ�ε��
    unsigned int n_visited;         // �����ĸ�������
    unsigned int n_flags;           // ���ӵ�����
    game_state state_flag;
    std::vector<std::vector<int>> board;                // ���̱�ʾ����(-1)������Χ�׵�����
    std::vector<std::vector<macro_state>> *state;        // �ⲿ��ʾ��δ�������ѷ���������
    std::map<int, macro_state> state_map;               // map������->����״̬

    // �����׵ķֲ�
    void shuffle();
    // ��ɢ�հ�����
    void spread(unsigned int row, unsigned col);
    // ��������
    void count_mines(unsigned int row, unsigned col);

};

#endif //MINESWEEPER_MINESWEEPTER_H
