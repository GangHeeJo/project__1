#include <vector>
#include <iostream>

using std::vector;

class Page {
    public:
        Page() {}
        Page(int x, int y, int width, int height, int id, int content);
        int get_x();
        int get_y(); 
        int get_width();
        int get_height();
        int get_id();
        char get_content();

        void set_x(int x) { this->x = x; }
        void set_y(int y) { this->y = y; }
        void set_width(int width) { this->width = width; }
        void set_height(int height) { this->height = height; }
        void set_id(int id) { this->id = id; }
        void set_content(int content) { this->content = content; }
        void add_vec_top(Page add);
        vector<Page> get_above_pages();
        void delete_vec_top(int idx);
        //void delete_above()

        vector<int> above_id;
        vector<int> below_id;


        
    private:
        int x, y; // position of the page on the board
        int width, height; // width and height of the page 
        int id; // unique id for each page
        char content;
        vector<Page> vec_top = {};
};

Page::Page(int x, int y, int width, int height, int id, int content) :x(x), y(y), width(width), height(height), id(id), content((char)content)
{ above_id.clear(); below_id.clear(); }

int Page::get_x() {
    return x;
}

int Page::get_y() {
    return y;
}

int Page::get_width() {
    return width;
}

int Page::get_height() {
    return height;
}

int Page::get_id() {
    return id;
}

char Page::get_content() {
    return content;
}

vector<Page> Page::get_above_pages() {
    return vec_top;
}

void Page::add_vec_top(Page add) {
    vec_top.push_back(add);
}

void Page::delete_vec_top(int idx) {
    vec_top.erase(vec_top.begin()+idx);
}