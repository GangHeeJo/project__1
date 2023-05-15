#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "page.h"
#include <algorithm>

using std::endl;
using std::ofstream;
using std::map;
using namespace std;
vector<Page> all_of_page; // page storage
vector<int> storage; // id storage


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
    int delete_pages(int idx);
    void above_page(int x, int y, int page_width, int page_height, int id, char content);
    void below_page(int x, int y, int page_width, int page_height, int id, char content);
    int find_id(int idx);

private:
    int num_jobs, width, height;
    ofstream& output;
    char* board;
    char* id_board;
    map <int, Page> page_map;
    
};



Board::Board(int num_jobs, int width, int height, ofstream& output_stream) : output(output_stream) {
    this->width = width;
    this->height = height;
    this->num_jobs = num_jobs;

    board = new char[width * height];
    id_board = new char[width * height];

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

    Page new_page = Page(x, y, page_width, page_height, id, content); // ���� insert�� page ����

    
    all_of_page.push_back(new_page);
    page_map.insert({ id, new_page });

    for (int i = y; i < (y + page_height); i++) {
        for (int j = x; j < (x + page_width); j++) {
            id_board[i * width + j] = content;
        }
    }

    for (int i = y; i < (y + page_height); i++) {
        for (int j = x; j < (x + page_width); j++) {
            board[i * width + j] = content;
            below_page(x, y, page_width, page_height, id, content);
        }
    }
    print_board();
    v.push_back(new_page.get_id());


    /*for (int i = 0; i < page_map[id].below_id.size(); i++) {
        output << page_map[id].below_id[i];
    } // below_id�� ����� �߰��Ǿ����� Ȯ���ϴ� �ڵ� */
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


/*void Board::delete_above(int idx) {

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
}*/


void Board::delete_page(int id) {
    
    int idx = find_index(id);

    delete_pages(idx);
    storage.erase(storage.begin() + storage.size()-1);
    
    std::reverse(storage.begin(), storage.end());

    
    for (int i = 0; i < storage.size(); i++) {
        int id = storage[i];

        int a_x = page_map[id].get_x();
        int a_y = page_map[id].get_y();
        int a_id = page_map[id].get_id();
        int a_width = page_map[id].get_width();
        int a_height = page_map[id].get_height();
        char a_content = page_map[id].get_content();
        /*for (int k = a_y; k < (a_y + a_height); k++) {
            for (int j = a_x; j < (a_x + a_width); j++) {
                board[k * width + j] = a_content;
                below_page(a_x, a_y, a_width, a_height, a_id, a_content);
            }
        }
        print_board();*/
        insert_page(a_x, a_y, a_width, a_height, a_id, a_content);
    }
    storage.erase(storage.begin());
}

void Board::modify_content(int id, char content) {
    
    int idx = find_index(id);
    delete_pages(idx);

    page_map[id].set_content(content);
    all_of_page.insert(all_of_page.begin()+idx, page_map[id]);
    
    std::reverse(storage.begin(), storage.end());
    for (int i = 0; i < storage.size(); i++) {
        int id = storage[i];

        int a_x = page_map[id].get_x();
        int a_y = page_map[id].get_y();
        int a_id = page_map[id].get_id();
        int a_width = page_map[id].get_width();
        int a_height = page_map[id].get_height();
        char a_content = page_map[id].get_content();
        /*for (int k = a_y; k < (a_y + a_height); k++) {
            for (int j = a_x; j < (a_x + a_width); j++) {
                board[k * width + j] = a_content;
                below_page(a_x, a_y, a_width, a_height, a_id, a_content);
            }
        }
        print_board();*/
        insert_page(a_x, a_y, a_width, a_height, a_id, a_content);
    }
    storage.erase(storage.begin());
}

void Board::modify_position(int id, int x, int y) {
    int idx = find_index(id);

    delete_pages(idx);

    page_map[id].set_x(x);
    page_map[id].set_y(y);

    all_of_page.insert(all_of_page.begin()+idx, page_map[id]);

    std::reverse(storage.begin(), storage.end());
    
    for (int i = 0; i < storage.size(); i++) {
        int id = storage[i];
        int a_x = page_map[id].get_x();
        int a_y = page_map[id].get_y();
        int a_id = page_map[id].get_id();
        int a_width = page_map[id].get_width();
        int a_height = page_map[id].get_height();
        char a_content = page_map[id].get_content();
        /*for (int k = a_y; k < (a_y + a_height); k++) {
            for (int j = a_x; j < (a_x + a_width); j++) {
                board[k * width + j] = a_content;
                below_page(a_x, a_y, a_width, a_height, a_id, a_content);
            }
        }
        print_board();*/
        insert_page(a_x, a_y, a_width, a_height, a_id, a_content);
    }
    storage.erase(storage.begin());
}


void Board::below_page(int x, int y, int page_width, int page_height, int id, char content) { //�̰� ����Ǹ� id�� page�� ��� ������ below_id�� ���Ϳ� �������� ����
    int id_of_above;
    
    for (int i = y; i < (y + page_height); i++) {
        for (int j = x; j < (x + page_width); j++) {
            if (board[i * width + j] != ' ') {
                for (int k = 0; k < v.size(); k++) {
                    int index = find_index(v[k]); // v[k]�� ���� k��° id��.
                    if (board[i * width + j] == all_of_page[index].get_content()) { 
                        id_of_above = all_of_page[index].get_id();
                        page_map[id].below_id.push_back(id_of_above);
                        
                    }
                }
                
            }
        }
    } 
    if (page_map[id].below_id.size() > 0) {
        for (int i = 0; i < page_map[id].below_id.size() - 1; i++) {
            if (page_map[id].below_id[i] == page_map[id].below_id[i + 1]) {
                page_map[id].below_id.erase(page_map[id].below_id.begin() + i + 1); // ���� �ڵ忡�� ���ο� page�� ���̸�ŭ id�� �߰��ǹǷ� �ߺ��Ǵ� �� �� �����ϴ� �ڵ�.
            }
        }
   }
}

void Board::above_page(int x, int y, int page_width, int page_height, int id, char content) {

}



int Board::delete_pages(int idx) {


    int min_page_id = 3276800;
    int delete_idx = 0;
    bool delete_exist = false;

    do {
        delete_exist = false;
        min_page_id = 3276800;
        for (int i = all_of_page.size() - 1; i > idx; i--)
        {

            int temp = 2;
            for (int j = 0; j < page_map[all_of_page[i].get_id()].below_id.size(); j++) {
                if (all_of_page[idx].get_id() == page_map[all_of_page[i].get_id()].below_id[j]) {
                    temp = 1; 
                    break;
                }
                else {
                    temp = 2; 
                }
            }

            if (temp == 1) // (if i page is in below_page vector of idx page)
            {
                if (min_page_id > all_of_page[i].get_id()) {
                    min_page_id = all_of_page[i].get_id();
                    delete_idx = i;
                    delete_exist = true;

                }
            }
        }
        if (delete_exist) {
            delete_pages(delete_idx);
        }


    } while (delete_exist);

    int id = all_of_page[idx].get_id();
    storage.push_back(id);

    all_of_page.erase(all_of_page.begin() + idx);

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            board[h * width + w] = ' ';
        }
    } 
    for (int i = 0; i < all_of_page.size(); i++) {
        int a_x = all_of_page[i].get_x();
        int a_y = all_of_page[i].get_y();
        int a_id = all_of_page[i].get_id();
        int a_width = all_of_page[i].get_width();
        int a_height = all_of_page[i].get_height();
        char content = all_of_page[i].get_content();
        for (int h = a_y; h < (a_y + a_height); h++) {
            for (int w = a_x; w < (a_x + a_width); w++) {
                board[h * width + w] = content;
            }
        }

    }

    print_board();

    return all_of_page.size();
}


/*for (int f = 0; f < all_of_page.size(); f++) {
    output << all_of_page[f].get_id();
}*/

//std::find(all_of_page[i].below_id.begin(), all_of_page[i].below_id.end(), all_of_page[idx].get_id()) != all_of_page[i].below_id.end()
//all_of_page.insert(all_of_page.begin()+idx, page_map[id]);
//all_of_page.insert(all_of_page.begin()+idx, page_map[id]);