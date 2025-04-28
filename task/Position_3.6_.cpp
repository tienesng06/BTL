#include "main.h"

#include "hcmcampaign.h"

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0){
        this->r = r;
        this->c = c;
    };
    Position(const string &str_pos){
        char ch;
        stringstream ss(str_pos);
        ss >> ch >> r >> ch >> c >> ch;
    }; // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
};

int Position::getRow() const{
    return r;
}

int Position::getCol() const{
    return c;
}

void Position::setRow(int r){
    this->r = r;
}

void Position::setCol(int c){
    this->c = c;
}

string Position::str() const{
    return "(" + to_string(r) + "," + to_string(c) + ")";
}
