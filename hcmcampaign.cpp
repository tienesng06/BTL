#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////
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
        virtual ~Unit(){};
        virtual int getAttackScore() = 0;
        Position getCurrentPosition() const;
        virtual string str() const = 0;
};

//Task 3.2
class Vehicle : public Unit{
    private:
        VehicleType vehicleType;
    public:
        Vehicle ( int quantity , int weight , const Position pos , VehicleType
        vehicleType ) : Unit(quantity , weight , pos){
            this->vehicleType = vehicleType;
        }

        int getAttackScore();
        string str();
        VehicleType getVehicleType() const;
        int getQuantity() const;
        void setQuantity(int quantity);
};

int Vehicle::getAttackScore(){
    int score = (static_cast<int>(vehicleType) * 304 + quantity * weight) / 30;
    return score;
}

string Vehicle::str(){
    return "Vehicle[Vehicle Type = " + to_string(vehicleType) + ", "
            + "quantity = "+ to_string(quantity) + ", "
            + "weight = " + to_string(weight) + ", "
            + "pos = " + pos.str() + "]";  
}

VehicleType Vehicle::getVehicleType() const{
    return vehicleType;
}

int Vehicle::getQuantity() const{
    return quantity;
}

void Vehicle::setQuantity(int quantity){
    this->quantity = quantity;
}

//Task 3.3
class Infantry : public Unit{
    private:
        InfantryType infantryType;
    public:
        bool perfectNumber(int n);
        int personalNumber(int n);
        int getAttackScore();
        string str();
        InfantryType getInfantryType() const;
        int getQuantity() const;
        void setQuantity(int quantity);


        Infantry ( int quantity , int weight , const Position pos , InfantryType
            infantryType ) : Unit(quantity, weight, pos){
                this->infantryType = infantryType;
        }
};

bool Infantry::perfectNumber(int n){
    int k = sqrt(n);
    if (k * k == n)
        return true;
    else 
        return false;
}

int Infantry::personalNumber(int n)
{
    while (n >= 10)
    {
        int t = 0;
        while (n > 0)
        {
            t+=n%10;
            n/=10;
        }
        n = t;
    }
    return n;
}

int Infantry::getAttackScore(){
    int type = static_cast<int>(infantryType);
    int score = type*56 + quantity * weight;
    if (infantryType == InfantryType::SPECIALFORCES && perfectNumber(weight))
        score += 75;
    int personal_number = personalNumber(score + 4);
    if (personal_number > 7) 
        quantity += quantity*0.2;
    else if (personal_number < 3)
        quantity -= quantity*0.1;
    else quantity = quantity;
    score = type*56 + quantity * weight;
    return score;
}

string Infantry::str(){
    return "Infantry[Infantry Type = " + to_string(infantryType) + ", "
            + "quantity = "+ to_string(quantity) + ", "
            + "weight = " + to_string(weight) + ", "
            + "pos = " + pos.str() + "]";  
}

InfantryType Infantry::getInfantryType() const{
    return infantryType;
}

int Infantry::getQuantity() const{
    return quantity;
}

void Infantry::setQuantity(int quantity){
    this->quantity = quantity;
}

//Task 3.4

//Task 3.5
class UnitList{
    private:
        struct Node{
            Unit* unit;
            Node* next;
            Node(Unit* u){
                this->unit = u;
                this->next = nullptr;
            }
        };
        Node* head;
        int capacity;
        int countVehicle;
        int countInfantry;
        
    public:
        bool insert(Unit *unit);                   // return true if insert successfully
        bool isContain(VehicleType vehicleType);   // return true if it exists
        bool isContain(InfantryType infantryType); // return true if it exists
        string str() const;

        UnitList(int capacity){
            this-> head = nullptr;
            this->countInfantry = 0;
            this->countVehicle = 0;
            this->capacity = capacity;
        }

        ~UnitList(){
            Node* cur = head;
            while (cur)
            {
                Node* tmp = cur->next;
                delete cur;
                cur = tmp;
            }
        }
};
bool UnitList::insert(Unit* unit){
    if (!unit) return false;
    Vehicle* v = dynamic_cast<Vehicle*>(unit);
    Infantry* i = dynamic_cast<Infantry*>(unit);

    //Update quantity
    for (Node* cur = head; cur; cur = cur->next)
    {
        if (v)
        {
            Vehicle* v1 = dynamic_cast<Vehicle*>(cur->unit);
            if (v1 && v1->getVehicleType() == v->getVehicleType())
            {
                v1->setQuantity(v1->getQuantity() + v->getQuantity());
                return true;
            }
        }
        else if (i)
        {
            Infantry* i1 = dynamic_cast<Infantry*>(cur->unit);
            if (i1 && i1->getInfantryType() == i->getInfantryType())
            {
                i1->setQuantity(i1->getQuantity() + i->getQuantity());
                return true;
            }
        }
    }

    //Check if Maximum quantity
    if (countInfantry + countVehicle >= capacity) return false;

    //Insert new unit
    Node* newNode = new Node(unit);
    if (i)
    {
        newNode->next = head;
        head = newNode;
        ++countInfantry;
    }
    else if (v)
    {
        Node* k = head;
        while (k->next)
            k = k->next;
        k->next = newNode;
        ++countVehicle;
    }
    return true;
}

bool UnitList::isContain(VehicleType vehicleType){
    for (Node* cur = head; cur; cur = cur->next)
    {
        Vehicle* v = dynamic_cast<Vehicle*>(cur->unit);
        if (v && v->getVehicleType() == vehicleType)
            return true;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType){
    for (Node* cur = head; cur; cur = cur->next)
    {
        Infantry* v = dynamic_cast<Infantry*>(cur->unit);
        if (v && v->getInfantryType() == infantryType)
            return true;
    }
    return false;
}

string UnitList::str() const{
    ostringstream oss;
    oss << "UnitList[count_vehicle=" << countVehicle
        << ";count_infantry=" << countInfantry
        << ";";
    
    bool first = true;
    for (Node* cur = head; cur; cur = cur->next)
    {
        if (!first) oss << ";";
        oss << cur->unit->str();
        first = false;
    }
    return oss.str();
}

//Task 3.6

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

//Task 3.7

//Task 3.8

//Task 3.9

//Task 3.10
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////