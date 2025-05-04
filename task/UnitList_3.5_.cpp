#include "main.h"
#include "hcmcampaign.h"
#include "InfantryType_3.3_.cpp"
#include "VehicleType_3.2_.cpp"
class UnitList{
    public:
        struct Node{
            Unit* unit;
            Node* next;
            Node(Unit* unit){
                this->unit = unit;
                this->next = nullptr;
            }
        };
    private:
        Node* head;
        Node* tail;
        int capacity;
        int countVehicle;
        int countInfantry;
        static int size;
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
                Infantry* i = dynamic_cast<Infantry*>(cur->unit);
                if (i && i->getInfantryType() == infantryType)
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
        Node* getHead() const { 
            return head; 
        }

        bool remove(Unit* unit) {
            Node* prev = nullptr;
            Node* cur = head;
            size = countInfantry + countVehicle;
            while (cur) {
                if (cur->unit == unit) {
                    if (prev)
                        prev->next = cur->next;
                    else
                        head = cur->next;
        
                    if (dynamic_cast<Infantry*>(unit))
                        --countInfantry;
                    else if (dynamic_cast<Vehicle*>(unit))
                        --countVehicle;
        
                    --size;
                    delete cur;
                    return true;
                }
                prev = cur;
                cur = cur->next;
            }
        
            return false;
        }
};
