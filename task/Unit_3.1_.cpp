#include "main.h"

#include "hcmcampaign.h"


class Unit{
    protected:
        int quantity, weight;
        Position pos;
    public:
        Unit(int quantity, int weight, const Position pos){
            this->quantity = quantity;
            this->weight = weight;
            this->pos = pos;
        }
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition () const ;
    virtual string str () const = 0;
    
};

