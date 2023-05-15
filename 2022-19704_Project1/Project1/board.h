#include <fstream>
#include <vector>

#include "page.h"

using std::vector;
using std::endl;
using std::ofstream;


class Board {
    public:
        Board(int num_jobs, int width, int height, ofstream& output_stream);
        ~Board();

        void print_board();
        void print_job(int job_idx, char job_type, int id);

        //job functions
        void insert_page(int x, int y, int width, int height, int id, int content);
        void delete_page(int id);
        void modify_content(int id, char content);
        void modify_position(int id, int x, int y);
        
        void update(vector<Page> all_of_pages);
        void recursion(int id);
        int find_index(int id); //id로부터 index 검색

        int check (vector<Page> vec_top, int id); // 있는지 확인하는 코드

    private:
        int num_jobs, width, height; 
        ofstream& output; 
        char* board; 
        int* id_board;
        vector<Page> all_of_pages;
        vector<Page> delete_above_pages;
};

Board::Board(int num_jobs, int width, int height, ofstream& output_stream): output(output_stream) {
    this->width = width;
    this->height = height;
    this->num_jobs = num_jobs;

    board = new char[width*height];
    id_board = new int[width*height];

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
            id_board[h*width + w] = 0;
        }
    }

}

Board::~Board() {
    delete board;
    
}


void Board::print_board() {
    int h, w;
    for (w = 0; w < width+2; w++) output << "- ";
    output << endl;
    
    for (h = 0; h < height; h++) {
        output << "| ";
        for (w = 0; w < width; w++) {
            output << board[h*width + w] << " ";
        }
        output << "| " << endl;
    }

    for (w = 0; w < width+2; w++) output << "- ";
    output << endl;
}

void Board::print_job(int job_idx, char job_type, int id) {
    output << ">> (" << job_idx <<") ";
    switch(job_type) {
        
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

int Board::find_index(int id) {
    int index = 0;
    for (int i = 0; i < all_of_pages.size(); i++) {
        if (all_of_pages[i].get_id() == id) {
            index = i;
            return index;
        }
    }
}

// recursion 파트를 하나의 함수로 만들어서 delete, modify 등에 써먹을 수 있도록 한다.

void Board::insert_page(int x, int y, int width, int height, int id, int content) {
    Page new_page = Page(x, y, width, height, id, content);
    all_of_pages.push_back(new_page);
    bool on[all_of_pages.size()];
    for (int i=0; i<all_of_pages.size(); i++) {
        on[i] = false;
    }

    for (int h = y; h < y + height; h++) {
        for (int w = x; w < x + width; w++) {
            for (int i=0; i<all_of_pages.size(); i++) {
                if (id_board[h * this->width + w] == all_of_pages[i].get_id()) {
                    on[i] = true;
                }
            }
            board[h * this->width + w] = content;
            id_board[h * this->width + w] = id;
        }
    }
    for (int i=0; i<all_of_pages.size(); i++) {
        if (on[i] == true) {
            all_of_pages[i].add_vec_top(new_page);
        }
    }

    print_board();
}

void Board::delete_page(int id) {
    recursion(id);
    delete_above_pages.erase(delete_above_pages.begin() + delete_above_pages.size() -1);
    for (int i=0; i <delete_above_pages.size(); i++) {
        int idx = delete_above_pages.size() -i -1;

        int a_x = delete_above_pages[idx].get_x();
        int a_y = delete_above_pages[idx].get_y();
        int a_id = delete_above_pages[idx].get_id();
        int a_width = delete_above_pages[idx].get_width();
        int a_height = delete_above_pages[idx].get_height();
        char a_content = delete_above_pages[idx].get_content();

        insert_page(a_x, a_y, a_width, a_height, a_id, a_content); //insert new pages

    }
    delete_above_pages = {};
}

void Board::modify_content(int id, char content) { // delete와 동일하다. 하지만 recursion 이후 새로운 page를 생성하여 insert하는 과정이 추가된다.

}


void Board::modify_position(int id, int x, int y) {
    
}

void Board::recursion(int id) { // Here is very important part(recursion). This is used at delete, modify .etc
    /*int tidx;
    for (int i=0; i<vec.size(); i++) {
        if (vec[i].get_id() == id) {
            tidx = i;
        }
    }*/

    int idx = find_index(id);
    
    int min_id;
    int min_idx;
    while (all_of_pages[idx].get_above_pages().size() > 0) {

        min_id = all_of_pages[idx].get_above_pages()[0].get_id();
        min_idx = 0;

        for (int i=0; i<all_of_pages[idx].get_above_pages().size(); i++) {
            if (all_of_pages[idx].get_above_pages()[i].get_id() < min_id) {
                min_id = all_of_pages[idx].get_above_pages()[i].get_id();
                min_idx = i; // recursion을 돌리는 와중에도 작은 id가 나타나면 업데이트 하도록 함.
            }
        }
        recursion(min_id);
    }

    delete_above_pages.push_back(all_of_pages[idx]);
    all_of_pages.erase(all_of_pages.begin() + idx);
    for (int i=0; i<all_of_pages.size(); i++){
        if (check(all_of_pages[i].get_above_pages(), id) == 1) {
            for (int j=0; j<all_of_pages[i].get_above_pages().size(); j++) {
                if (all_of_pages[i].get_above_pages()[j].get_id() == id) {
                    all_of_pages[i].delete_vec_top(j);
                }
            }
        }
    }
    update(all_of_pages);
    print_board();
}

int Board::check (vector<Page> vec_top, int id) { // 있는지 확인하는 코드
    for (int i=0; i<vec_top.size(); i++) {
        if (vec_top[i].get_id() == id) {
            return 1;
        }
        else {
            continue;
        }
    }
    return 0;
}

void Board::update(vector<Page> all_of_pages) { // recursion 이후 all_of_pages를 업데이트한다.
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h*width + w] = ' ';
            id_board[h*width + w] = 0;
        }
    }
    for (int i=0; i<all_of_pages.size(); i++) {
        for (int h = all_of_pages[i].get_y(); h < all_of_pages[i].get_y() + all_of_pages[i].get_height(); h++) {
            for (int w = all_of_pages[i].get_x(); w < all_of_pages[i].get_x() + all_of_pages[i].get_width(); w++) {
                board[h * this->width + w] = all_of_pages[i].get_content();
                id_board[h * this->width + w] = all_of_pages[i].get_id();
            }
        }
    }
}
