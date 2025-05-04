#include "main.h"
#include "hcmcampaign.h"
#include "Unit_3.1_.cpp"
class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;
class LiberationArmy{};
class UnitList{
    private:
        struct Node{
            Unit* unit;
            Node* next;
            Node(Unit* u){
                this->unit = unit;
                this->next = nullptr;
            }
        };
        Node* head;
        int capacity;
        int countVehicle;
        int countInfantry;
    public:
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

        bool insert(Unit* unit){
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
        bool isContain(VehicleType vehicleType){
            for (Node* cur = head; cur; cur = cur->next)
            {
                Vehicle* v = dynamic_cast<Vehicle*>(cur->unit);
                if (v && v->getVehicleType() == vehicleType)
                    return true;
            }
            return false;
        }
        bool isContain(InfantryType infantryType){
            for (Node* cur = head; cur; cur = cur->next)
            {
                Infantry* v = dynamic_cast<Infantry*>(cur->unit);
                if (v && v->getInfantryType() == infantryType)
                    return true;
            }
            return false;
        }
        string str() const{
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
        }
        vector<Unit*> toVector() const {
            vector<Unit*> v;
            Node* cur = head;
            while (cur) {
                v.push_back(cur->unit);
                cur = cur->next;
            }
            return v;
        }
};
static int distance(const Position &a, const Position &b) {
    int dr = a.getRow() - b.getRow();
    int dc = a.getCol() - b.getCol();
    return static_cast<int>(sqrt(dr*dr + dc*dc));
}
class Army
{
    protected:
        int LF, EXP;
        string name;
        UnitList *unitList;
        BattleField *battleField;

    private:
        bool isSpecialNumber(int S) const {
            int bases[] = {3, 5, 7}; // Odd prime bases less than 10
            for (int base : bases) {
                int sum = 0;
                int power = 1;
                while (sum < S) {
                    sum += power;
                    if (sum == S) return true;
                    power *= base;
                }
            }
            return false;
        }

    public:
        Army( const Unit **unitArray, int size, string name, BattleField *battleField)
        {
            this->LF = 0;
            this->EXP = 0;
            this-> name = name;
            this->battleField = battleField;

            int tempLF = 0, tempEXP = 0;
            for (int i = 0; i < size; ++i) {
                const Unit* unit = unitArray[i];
                if (const Vehicle* v = dynamic_cast<const Vehicle*>(unit)) {
                    tempLF += const_cast<Vehicle*>(v)->getAttackScore();
                } else if (const Infantry* in = dynamic_cast<const Infantry*>(unit)) {
                    tempEXP += const_cast<Infantry*>(in)->getAttackScore();
                }
            }

            LF = max(0,min(tempLF,1000));
            EXP = max(0,min(tempEXP,500));

            int S = LF + EXP;
            int capacity;
            if (isSpecialNumber(S))
                capacity = 12;
            else
                capacity = 8;
            unitList = new UnitList(capacity);

            for (int i = 0; i < size; ++i) {
                unitList->insert(const_cast<Unit*>(unitArray[i]));
            }
        }
        UnitList* getUnitList(){
            return unitList;}
        virtual ~Army() {
            delete unitList;
        }
        virtual void fight(Army *enemy, bool defense = false) = 0;
        virtual string str() const = 0;
        
        int getLF(){
            return this->LF;
        }
        void setLF(int LF)
        {
            this->LF = LF;
        }
        int getEXP(){
            return this->EXP;
        }
        void setEXP(int EXP)
        {
            this->EXP = EXP;
        }
};

class TerrainElement
{
    protected:
        Position pos;
    public:
        TerrainElement(const Position &p) : pos(p) {}
        virtual ~TerrainElement() {}
        virtual void getEffect(Army *army) = 0;
};

class Road : public TerrainElement{
    public:
        Road(const Position &p) : TerrainElement(p) {}
        void getEffect(Army *army){}
};


class Mountain : public TerrainElement{
    public:
        Mountain(const Position &p) : TerrainElement(p) {}
        void getEffect(Army *army){
            bool isLA = dynamic_cast<LiberationArmy*>(army) != nullptr;
            int radius;
            if (isLA)
                radius = 2;
            else
                radius = 4;
            float InfantryBoost, VehicleBoost;
            if (isLA)
            {
                InfantryBoost = 0.3f;
                VehicleBoost = 0.1f;
            }
            else 
            {
                InfantryBoost = 0.2f;
                VehicleBoost = 0.05f;
            }
            for (Unit *u : army->getUnitList()->toVector()) 
            {
                int d = distance(pos, u->getCurrentPosition());
                if (d <= radius) 
                {
                    if (Infantry *inf = dynamic_cast<Infantry*>(u)) 
                    {
                        int delta = static_cast<int>(inf->getAttackScore() * InfantryBoost);
                        army->setEXP(max(0, min(500, army->getEXP() + delta)));
                    } else if (Vehicle *veh = dynamic_cast<Vehicle*>(u)) 
                    {
                        int delta = static_cast<int>(veh->getAttackScore() * VehicleBoost);
                        army->setLF(max(0, army->getLF() - delta));
                    }
                }
            }
        }
};

class River :public TerrainElement{
    River(const Position &p) : TerrainElement(p) {}
    void getEffect(Army *army){
        if (dynamic_cast<LiberationArmy*>(army)) 
            return;
        for (Unit *u : army->getUnitList()->toVector())
        {
            if (Infantry* inf = dynamic_cast<Infantry*>(u))
            {
                int d = distance(pos, inf->getCurrentPosition());
                if (d <= 2)
                {
                    int delta = static_cast<int>(inf->getAttackScore() * 0.1f);
                    army->setEXP(max(0,army->getEXP() - delta));
                }
            }
        }
    }
};

class Urban :public TerrainElement{
    Urban(const Position &p) : TerrainElement(p) {}
    void getEffect(Army* army){
        bool isLA = dynamic_cast<LiberationArmy*>(army) != nullptr;
        for (Unit *u : army->getUnitList()->toVector())
        {
            int d = distance(pos, u->getCurrentPosition());
            if (Infantry* in = dynamic_cast<Infantry*>(u))
            {
                if (isLA)
                {
                    if (in->getInfantryType() == SPECIALFORCES || 
                    in->getInfantryType() == REGULARINFANTRY && d <= 5)
                    {
                        int delta = static_cast<int>((0.3 * in->getAttackScore()) / d);
                        army->setEXP(min(500,army->getEXP() + delta));
                    }
                }
                else if (in->getInfantryType() == REGULARINFANTRY && d <= 3)
                {
                    int delta = static_cast<int>((0.3 * in->getAttackScore()) / (2 * d));
                    army->setEXP(min(500,army->getEXP() + delta));
                }
            }
            else if (Vehicle *ve = dynamic_cast<Vehicle*>(u))
            {
                if (isLA && ve->getVehicleType() == ARTILLERY && d <=2)
                {
                    int delta = static_cast<int>(ve->getAttackScore() * 0.5f);
                    army->setLF(max(0, army->getLF() - delta));
                }
            }
        }
    }
};

class Fortification :public TerrainElement{
    Fortification(const Position &p) : TerrainElement(p) {}
    void getEffect(Army* army){
        bool isLA = dynamic_cast<LiberationArmy*>(army) != nullptr;
        for (Unit *u : army->getUnitList()->toVector())
        {
            int d = distance(pos, u->getCurrentPosition());
            if (d <= 2)
            {
                if (Infantry* in = dynamic_cast<Infantry*>(u))
                {
                    int delta;
                    if (isLA)  delta = static_cast<int>(in->getAttackScore() * 0.2f);
                    else delta = static_cast<int>(in->getAttackScore() * -0.2f);
                    army->setEXP(max(0,min(500,army->getEXP() + delta)));
                }
                else if (Vehicle *ve = dynamic_cast<Vehicle*>(u))
                {
                    int delta;
                    if (isLA)  delta = static_cast<int>(in->getAttackScore() * 0.2f);
                    else delta = static_cast<int>(in->getAttackScore() * -0.2f);
                    army->setLF(max(0,min(500,army->getLF() + delta)));
                }
            }
        }
    }
};

class SpeacialZone: public TerrainElement{
    SpeacialZone(const Position &p) : TerrainElement(p) {}
    void getEffect(Army* army){
        for (Unit *u : army->getUnitList()->toVector())
        {
            int d = distance(pos, u->getCurrentPosition());
            if (d <= 1)
            {
                
            }
        }
    }
};


