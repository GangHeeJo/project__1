#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "page.h"

using std::endl;
using std::ofstream;
using std::map;
using namespace std;
vector<Page> all_of_page;


class Board {
public:
    Board(int num_jobs, int width, int height, ofstream& output_stream);
    ~Board();

    void print_board();
    void print_job(int job_idx, char job_type, int id);

    //job functions
    void insert_page(int x, int y, int width, int height, int id, char content);
    void delete_page(int id);
    void modify_content(int id, char content);
    void modify_position(int id, int x, int y);
    int find_index(int id);
    int check_above(int a, int b);
    void delete_above(int idx);

private:
    int num_jobs, width, height;
    ofstream& output;
    char* board;
    map <int, Page> page_map;
    
};



Board::Board(int num_jobs, int width, int height, ofstream& output_stream) : output(output_stream) {
    this->width = width;
    this->height = height;
    this->num_jobs = num_jobs;

    board = new char[width * height];

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h * width + w] = ' ';
        }
    }
    
}

Board::~Board() {
    delete board;

}


void Board::print_board() {
    int h, w;
    for (w = 0; w < width + 2; w++) output << "- ";
    output << endl;

    for (h = 0; h < height; h++) {
        output << "| ";
        for (w = 0; w < width; w++) {
            output << board[h * width + w] << " ";
        }
        output << "| " << endl;
    }

    for (w = 0; w < width + 2; w++) output << "- ";
    output << endl;
}

void Board::print_job(int job_idx, char job_type, int id) {
    output << ">> (" << job_idx << ") ";
    switch (job_type) {

    case 'i':
        output << "Insert ";
        break;
    case 'd':
        output << "Delete ";
        break;
    case 'm':
        output << "Modify ";
        break;
    }

    output << id << endl;
}


std::vector<int> v;

void Board::insert_page(int x, int y, int page_width, int page_height, int id, char content) {

    Page new_page = Page(x, y, page_width, page_height, id, content); // 새로 insert한 page 저장
    //page_map.insert({ id, new_page });
    all_of_page.push_back(new_page);

    for (int i = y; i < (y + page_height); i++) {
        for (int j = x; j < (x + page_width); j++) {
            board[i * width + j] = content;
        }
    }
    print_board();
    v.push_back(new_page.get_id());
    
    
}

int Board::find_index(int id) {
    int index = 0;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == id) {
            index = i;
            return index;
        }
    }
}

void Board::delete_above(int idx) {

    if (all_of_page.size() == 0) {
        return;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i * width + j] = ' ';
        }
    }
    int index = idx;

    int delete_x = all_of_page[index].get_x();
    int delete_y = all_of_page[index].get_y();
    int delete_width = all_of_page[index].get_width();
    int delete_height = all_of_page[index].get_height();
    char delete_content = all_of_page[index].get_content();

    for (int i = delete_y; i < (delete_y + delete_height); i++) {
        for (int j = delete_x; j < (delete_x + delete_width); j++) {
            board[i * width + j] = delete_content;
        }
    }
    print_board();
}


void Board::delete_page(int id) {

    if (all_of_page.size() == 0) {
        return;
    }

    int index = find_index(id);
    int delete_x = all_of_page[index].get_x();
    int delete_y = all_of_page[index].get_y();
    int delete_width = all_of_page[index].get_width();
    int delete_height = all_of_page[index].get_height();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i * width + j] = ' ';
        }
    }

    delete_above(index);

}

void Board::modify_content(int id, char content) {


}
void Board::modify_position(int id, int x, int y) {


}

int Board::check_above(int a, int b) {
    int a_x = all_of_page[a].get_x();
    int b_x = all_of_page[b].get_x();
    int a_y = all_of_page[a].get_y();
    int b_y = all_of_page[b].get_y();
    int a_id = all_of_page[a].get_id();
    int b_id = all_of_page[b].get_id();
    int a_width = all_of_page[a].get_width();
    int b_width = all_of_page[b].get_width();
    int a_height = all_of_page[a].get_height();
    int b_height = all_of_page[b].get_height();

    for (int i = a_x; i < (a_x + a_width); i++) {
        for (int j = a_y; j < (a_y + a_height); j++) {
            for (int k = b_x; k < (b_x + b_width); k++) {
                for (int s = b_y; s < (b_y + b_height); s++) {
                    if (i == k && j == s)
                        return 1; // 1은 겹친다는 뜻
                    else
                        return 2; // 2는 안 겹친다는 뜻
                }
            }
        }
    }
}