#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////
// Unit class
double distance(const Position &pos1, const Position &pos2) {
    int dx = pos1.getRow() - pos2.getRow();
    int dy = pos1.getCol() - pos2.getCol();
    return sqrt(double(dx * dx + dy * dy));
}
int safeCeil(double value) {
    double d = abs(round(value) - value);
    return (d < 1e-7) ? round(value) : ceil(value);
}
bool isBase(int num, int prime) {
    if (num == 0) return true;
    while (num > 0) {
        int remainder = num % prime;
        if (remainder != 0 && remainder != 1) {
            return false;
        }
        num /= prime;
    }
    return true;
}
bool isSpecial(int num) {
    if (num < 0) return false;
    if (num == 0 || num == 1) return true;
    int primes[] = {3, 5, 7};
    for (int prime : primes) {
    if (isBase(num, prime)) {
        return true;
        }
    }
    return false;
}
vector<Unit*> knapsack(UnitList::Node* head, int minScore) {
    vector<Unit*> units;
    UnitList::Node* current = head;
    while (current && current->unit) {
        units.push_back(current->unit);
        current = current->next;
    }

    int n = units.size();
    vector<Unit*> bestCombination;
    int bestScore = INT_MAX;

    for (int mask = 0; mask < (1 << n); mask++) {
        vector<Unit*> currentCombination;
        int currentScore = 0;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                currentCombination.push_back(units[i]);
                currentScore += units[i]->getAttackScore();
            }
        }

        if (currentScore > minScore && currentScore < bestScore) {
            bestScore = currentScore;
            bestCombination = currentCombination;
        }
    }

    return bestCombination;
}

// Task 3.1
Position Unit::getCurrentPosition() const {
    return pos;
}
int Unit::getQuantity() const {
    return quantity;
}
int Unit::getWeight() const {
    return weight;
}
void Unit::setQuantity(int quantity) {
    this->quantity = quantity;
}
void Unit::setWeight(int weight) {
    this->weight = weight;
}
void Unit::setMod(double mod) {
    this->mod = mod;
}
Unit::Unit(int quantity, int weight, Position pos) {
    this->quantity = quantity;
    this->weight = weight;
    this->pos = pos;
}
Unit::~Unit() {}
// Task 3.2
int Vehicle::getAttackScore(){
    return safeCeil(((static_cast<int>(vehicleType) * 304 + quantity * weight) / 30.0) * mod);
}
string Vehicle::str() const {
    string typeStr;
    switch(vehicleType) {
        case TRUCK: typeStr = "TRUCK"; break;
        case MORTAR: typeStr = "MORTAR"; break;
        case ANTIAIRCRAFT: typeStr = "ANTIAIRCRAFT"; break;
        case ARMOREDCAR: typeStr = "ARMOREDCAR"; break;
        case APC: typeStr = "APC"; break;
        case ARTILLERY: typeStr = "ARTILLERY"; break;
        case TANK: typeStr = "TANK"; break;
        default: typeStr = "UNKNOWN"; break;
    }
    
    return "Vehicle[vehicleType=" + typeStr + 
           ",quantity=" + to_string(quantity) + 
           ",weight=" + to_string(weight) + 
           ",position=" + pos.str() + "]";
}
VehicleType Vehicle::getVehicleType() const {
    return vehicleType;
}
Vehicle::Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType) :
    Unit(quantity, weight, pos) {
    this->vehicleType = vehicleType;
}
Vehicle::~Vehicle() {}
int Vehicle::getQuantity() const {
    return quantity;
}
int Vehicle::getWeight() const {
    return weight;
}
void Vehicle::setVehicleType(VehicleType vehicleType) {
    this->vehicleType = vehicleType;
}
void Vehicle::setQuantity(int quantity) {
    this->quantity = quantity;
}
void Vehicle::setWeight(int weight) {
    this->weight = weight;
}

// Task 3.3
Infantry::Infantry(int quantity, int weight, Position pos, InfantryType infantryType) :
    Unit(quantity, weight, pos) {
    this->infantryType = infantryType;
}
Infantry::~Infantry() {}
bool Infantry::Isperfect(int n)
{
    if (n<0) return false;
    if (n == 0 || n == 1) return true;
    int k = sqrt(n);
    return (k * k == n);
}
int Infantry::personalNumber(int n) {
    while (n >= 10) {
        int t = 0;
        while (n > 0) {
            t += n%10;
            n /= 10;
        }
        n = t;
    }
    return n;
}
int Infantry::getAttackScore() {
    int score = static_cast<int>(infantryType) * 56 + quantity * weight;

    if (infantryType == SPECIALFORCES && Isperfect(weight)) {
        score += 75;
    }

    int personal_number = personalNumber(score + 4);
    if (personal_number > 7) {
        quantity = quantity + safeCeil(quantity * 0.2);
    } else if (personal_number < 3) {
        quantity = quantity - safeCeil(quantity * 0.1);
    }

    score = static_cast<int>(infantryType) * 56 + quantity * weight;
    return safeCeil(score * mod);
}
string Infantry::str() const {
    string typeStr;
    switch(infantryType) {
        case SNIPER: typeStr = "SNIPER"; break;
        case ANTIAIRCRAFTSQUAD: typeStr = "ANTIAIRCRAFTSQUAD"; break;
        case MORTARSQUAD: typeStr = "MORTARSQUAD"; break;
        case ENGINEER: typeStr = "ENGINEER"; break;
        case SPECIALFORCES: typeStr = "SPECIALFORCES"; break;
        case REGULARINFANTRY: typeStr = "REGULARINFANTRY"; break;
        default: typeStr = "UNKNOWN"; break;
    }
    
    return "Infantry[infantryType=" + typeStr + 
           ",quantity=" + to_string(quantity) + 
           ",weight=" + to_string(weight) + 
           ",position=" + pos.str() + "]";
}
int Infantry::getQuantity() const {
    return quantity;
}
int Infantry::getWeight() const {
    return weight;
}
InfantryType Infantry::getInfantryType() const {
    return infantryType;
}
void Infantry::setInfantryType(InfantryType infantryType) {
    this->infantryType = infantryType;
}
void Infantry::setQuantity(int quantity) {
    this->quantity = quantity;
}
void Infantry::setWeight(int weight) {
    this->weight = weight;
}

// Task 3.4
Army::Army(Unit **unitArray, int size, string name, BattleField *battleField) {
    this->LF = 0;
    this->EXP = 0;
    this->name = name;
    this->battleField = battleField;
    
    for (int i = 0; i < size; i++) {
        Unit* unit = unitArray[i];
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(unit)) {
            LF += vehicle->getAttackScore();
        }
        else if (Infantry* infantry = dynamic_cast<Infantry*>(unit)) {
            EXP += infantry->getAttackScore();
        }
    }
    LF = max(0, min(1000, LF));
    EXP = max(0, min(500, EXP));

    this->unitList = new UnitList(LF + EXP);
    for (int i = 0; i < size; i++) {
        Unit* unit = unitArray[i];
        unitList->insert(unit);
    }
}
Army::~Army() {
    delete unitList;
    unitList = nullptr;
}
void Army::updateScore() {
    if (!unitList) return;
    
    LF = 0;
    EXP = 0;
    UnitList::Node* current = unitList->getHead();
    
    while (current && current->unit) {
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(current->unit)) {
            LF += vehicle->getAttackScore();
        } else if (Infantry* infantry = dynamic_cast<Infantry*>(current->unit)) {
            EXP += infantry->getAttackScore();
        }
        current = current->next;
    }
    
    LF = max(0, min(1000, LF));
    EXP = max(0, min(500, EXP));
}
int Army::getLF() const {
    return LF;
}
int Army::getEXP() const {
    return EXP;
}
void Army::setLF(int LF) {
    this->LF = LF;
}
void Army::setEXP(int EXP) {
    this->EXP = EXP;
}

// 3.4.1 | LiberationArmy
LiberationArmy::LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField) :
Army(unitArray, size, name, battleField) {}

Unit* LiberationArmy::cloneUnit(Unit* unit) {
    if (!unit) return nullptr;
    if (Infantry* infantry = dynamic_cast<Infantry*>(unit)) {
        Infantry* clone = new Infantry(
                            infantry->getQuantity(),
                            infantry->getWeight(),
                            infantry->getCurrentPosition(),
                            infantry->getInfantryType());
        CloneUnitAddress.push_back(clone);
        return clone;
    } else if (Vehicle* vehicle = dynamic_cast<Vehicle*>(unit)) {
        Vehicle* clone = new Vehicle(
                            vehicle->getQuantity(),
                            vehicle->getWeight(),
                            vehicle->getCurrentPosition(),
                            vehicle->getVehicleType());
        CloneUnitAddress.push_back(clone);
        return clone;
    }

    return nullptr;
}

int LiberationArmy::nearestFibonacci(int value) {
    if (value <= 1) return 1;

    int fib1 = 1, fib2 = 1;
    while (fib2 <= value) {
        int tmp = fib1 + fib2;
        fib1 = fib2;
        fib2 = tmp;
    }
    return fib2;
}
void LiberationArmy::fight(Army* enemy, bool defense) {
    if (!enemy) return; 

    if (!defense){
        int newLF = min(1000, static_cast<int>(LF * 1.5));
        int newEXP = min(500, static_cast<int>(EXP * 1.5));

        UnitList::Node* cur = unitList->getHead();
        UnitList::Node* infantryHead = nullptr;
        UnitList::Node* vehicleHead = nullptr;
        while(cur && cur->unit) {
            UnitList::Node* next = cur->next;
            if (Infantry* infantry = dynamic_cast<Infantry*>(cur->unit)) {
                UnitList::Node* newNode = new UnitList::Node(infantry);
                newNode->next = infantryHead;
                infantryHead = newNode;
            } else if (Vehicle* vehicle = dynamic_cast<Vehicle*>(cur->unit)) {
                UnitList::Node* newNode = new UnitList::Node(vehicle);
                newNode->next = vehicleHead;
                vehicleHead = newNode;
            }
            cur = next;
        }

        vector<Unit*> comboA = knapsack(infantryHead, enemy->getEXP());
        vector<Unit*> comboB = knapsack(vehicleHead, enemy->getLF());

        while (infantryHead) {
            UnitList::Node* temp = infantryHead;
            infantryHead = infantryHead->next;
            delete temp;
        }
        while (vehicleHead) {
            UnitList::Node* temp = vehicleHead;
            vehicleHead = vehicleHead->next;
            delete temp;
        }

        bool win = false;

        if (!comboA.empty() && !comboB.empty()){
            win = true;
            for (Unit* unit : comboA) {
                unitList->remove(unit);
            }
            for (Unit* unit : comboB) {
                unitList->remove(unit);
            }
        }
        else if (!comboA.empty() && newLF > enemy->getLF()){
            win = true;
            cur = unitList->getHead();
            while (cur && cur->unit) {
                UnitList::Node* next = cur->next;
                if (Vehicle* vehicle = dynamic_cast<Vehicle*>(cur->unit)) {
                    unitList->remove(vehicle);
                }
                cur = next;
            }

            for (Unit* unit : comboA) {
                unitList->remove(unit);
            }
        }
        else if (!comboB.empty() && newEXP > enemy->getEXP()){
            win = true;
            cur = unitList->getHead();
            while (cur) {
                UnitList::Node* next = cur->next;
                if (Infantry* infantry = dynamic_cast<Infantry*>(cur->unit)) {
                    unitList->remove(infantry);
                }
                cur = next;
            }

            for (Unit* unit : comboB) {
                unitList->remove(unit);
            }
        }

        if (win) {
            UnitList::Node* enemyCur = enemy->getUnitList()->getHead();
            while (enemyCur && enemyCur->unit) {
                UnitList::Node* next = enemyCur->next;
                Unit* clone = cloneUnit(enemyCur->unit);
                if (clone && unitList->insert(clone)) {
                    enemy->getUnitList()->remove(enemyCur->unit);
                }
                enemyCur = next;
            }
            enemy->updateScore();
        }
        else {
            UnitList::Node* current = unitList->getHead();
            while (current) {
                current->unit->setWeight(safeCeil(current->unit->getWeight() * 0.9));
                current = current->next;
            }
        }
        updateScore();
    }
    else {
        int newLF = min(1000, static_cast<int>(LF * 1.3));
        int newEXP = min(500, static_cast<int>(EXP * 1.3));

        if (newLF >= enemy->getLF() && newEXP >= enemy->getEXP()) {
            LF = newLF;
            EXP = newEXP;
        } 
        else if (newLF >= enemy->getLF() || newEXP >= enemy->getEXP()) {
            UnitList::Node* cur = unitList->getHead();
            while (cur) {
                cur->unit->setWeight(safeCeil(cur->unit->getWeight() * 0.9));
                cur = cur->next;
            }
            updateScore();
        }
        else {
            bool Backup = true;
            int maxIterations = 3;
            while (Backup && maxIterations > 0) {
                UnitList::Node* cur = unitList->getHead();
                while (cur) {
                    cur->unit->setQuantity(nearestFibonacci(cur->unit->getQuantity()));
                    cur = cur->next;
                }
                updateScore();

                newLF = min(1000, static_cast<int>(LF * 1.3));
                newEXP = min(500, static_cast<int>(EXP * 1.3));

                if (newLF >= enemy->getLF() && newEXP >= enemy->getEXP()) {
                    LF = newLF;
                    EXP = newEXP;
                    Backup = false;
                } 
                else {
                    UnitList::Node* cur = unitList->getHead();
                    while (cur) {
                        cur->unit->setQuantity(safeCeil(cur->unit->getQuantity() * 0.9));
                        cur = cur->next;
                    }
                    updateScore();
                    maxIterations--;
                }
            }
        }
    }
}
LiberationArmy::~LiberationArmy() {
    for (auto x : CloneUnitAddress) {
        delete x;  
    }
    CloneUnitAddress.clear();  
}
string LiberationArmy::str() const {
    string result = "LiberationArmy[";
    result += "LF=" + to_string(LF) + ",";
    result += "EXP=" + to_string(EXP) + ",";
    result += "unitList=" + unitList->str() + ",";
    result += "battleField=]";  // Add battleField= even if empty
    return result;
}
// Task 3.4.2 | ARVN
ARVN::~ARVN() {
    for (auto x : CloneUnitAddress) {
        delete x;  
    }
    CloneUnitAddress.clear();
}
ARVN::ARVN(Unit **unitArray, int size, string name, BattleField *battleField) :
Army(unitArray, size, name, battleField) {}
Unit* ARVN::cloneUnit(Unit* unit) {
    if (!unit) return nullptr; 
    if (Infantry* infantry = dynamic_cast<Infantry*>(unit)) {
        Infantry* clone = new Infantry(
                            infantry->getQuantity(),
                            infantry->getWeight(),
                            infantry->getCurrentPosition(),
                            infantry->getInfantryType());
        CloneUnitAddress.push_back(clone);
        return clone;
    }
    else if (Vehicle* vehicle = dynamic_cast<Vehicle*>(unit)) {
        Vehicle* clone = new Vehicle(
                            vehicle->getQuantity(),
                            vehicle->getWeight(),
                            vehicle->getCurrentPosition(),
                            vehicle->getVehicleType());
        CloneUnitAddress.push_back(clone);
        return clone;
    }
    return nullptr;
}
string ARVN::str() const {
    string result = "ARVN[";
    result += "LF=" + to_string(LF) + ",";
    result += "EXP=" + to_string(EXP) + ",";
    result += "unitList=" + unitList->str() + ",";
    result += "battleField=]";  
    return result;
}
void ARVN::updateScores() {
    LF = 0;
    EXP = 0;
    UnitList::Node* cur = unitList->getHead();
    vector<Unit*> Units = unitList->toVector();
    vector<Unit*> remaining;
    
    while(cur) {

        Unit* unit = cur->unit;
        UnitList::Node* next = cur->next;
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(unit)) {
            if (vehicle->getQuantity() == 1) {
                continue;
            }
            LF += vehicle->getAttackScore();
            remaining.push_back(unit);
        } else if (Infantry* infantry = dynamic_cast<Infantry*>(unit)) {
            if (infantry->getQuantity() == 1) {
                continue;
            }
            int score = infantry->getAttackScore();
            if (score >= 0) {
                EXP += score;
            }
            remaining.push_back(unit);
        }
        cur = next;
    }

    for (Unit* unit : Units) {
        if (unit->getQuantity() == 1) {
            unitList->remove(unit);
        }
    }

    unitList->setUnitList(remaining);
    LF = max(0, min(1000, LF));
    EXP = max(0, min(500, EXP));
}
void ARVN::fight(Army* enemy, bool defense){
    if (!enemy) return; 
    
    if (!defense) {  
        vector<Unit*> remaining;

        UnitList::Node* cur = unitList->getHead();
        while (cur) {
            UnitList::Node* next = cur->next;
            int newQuantity = max(1, static_cast<int>(safeCeil(cur->unit->getQuantity() * 0.8)));
            cur->unit->setQuantity(newQuantity);
            if (newQuantity > 1) {
                remaining.push_back(cur->unit);
            }
            cur = next;
        }
        cur = unitList->getHead();
        while (cur) {
            UnitList::Node* next = cur->next;
            if (cur->unit->getQuantity() == 1) {
                unitList->remove(cur->unit);
            }
            cur = next;
        }
        
        updateScores(); 
    }
    else {
        UnitList::Node* EnemyCur = enemy->getUnitList()->getHead();
        int newLF = min(1000, static_cast<int>(LF * 1.5));
        int newEXP = min(500, static_cast<int>(EXP * 1.5));
        UnitList::Node* infantryHead = nullptr;
        UnitList::Node* vehicleHead = nullptr;
        while(EnemyCur) {
            UnitList::Node* next = EnemyCur->next;
            if (Infantry* infantry = dynamic_cast<Infantry*>(EnemyCur->unit)) {
                UnitList::Node* newNode = new UnitList::Node(infantry);
                newNode->next = infantryHead;
                infantryHead = newNode;
            } else if (Vehicle* vehicle = dynamic_cast<Vehicle*>(EnemyCur->unit)) {
                UnitList::Node* newNode = new UnitList::Node(vehicle);
                newNode->next = vehicleHead;
                vehicleHead = newNode;
            }
            EnemyCur = next;
        }
        vector<Unit*> comboA = knapsack(infantryHead, enemy->getEXP());
        vector<Unit*> comboB = knapsack(vehicleHead, enemy->getLF());

        while (infantryHead) {
            UnitList::Node* temp = infantryHead;
            infantryHead = infantryHead->next;
            delete temp;
        }
        while (vehicleHead) {
            UnitList::Node* temp = vehicleHead;
            vehicleHead = vehicleHead->next;
            delete temp;
        }

        if (!comboA.empty() && !comboB.empty()) {
            UnitList::Node* cur = unitList->getHead();
            while (cur) {
                cur->unit->setWeight(safeCeil(cur->unit->getWeight() * 0.8));
                cur = cur->next;
            }
            updateScore();
        } 
        else if (!comboA.empty() || !comboB.empty()) {  
            UnitList::Node* cur = unitList->getHead();
            while (cur) {
                cur->unit->setWeight(safeCeil(cur->unit->getWeight() * 0.8));
                cur = cur->next;
            }
            updateScore();
        } 
    }
}

// Task 3.5
UnitList::UnitList(int S) {
    if (isSpecial(S))    
        S = 12;
    else 
        S = 8;
    this->capacity = S;
    head = nullptr;
    countInfantry = 0;
    countVehicle = 0;
}
UnitList::~UnitList() {
}
bool UnitList::insert(Unit *unit){
    if (!unit) return false;
    if (countInfantry + countVehicle >= capacity) return false; 

    Node* current = head;
    Node* prev = nullptr;

    while (current) {
        if (Vehicle* existingVehicle = dynamic_cast<Vehicle*>(current->unit)) {
            if (Vehicle* newVehicle = dynamic_cast<Vehicle*>(unit)) {
                if (existingVehicle->getVehicleType() == newVehicle->getVehicleType()) {
                    if (existingVehicle->getWeight() >= newVehicle->getWeight()) {
                        existingVehicle->setQuantity(existingVehicle->getQuantity() + newVehicle->getQuantity());
                    } else {
                        newVehicle->setQuantity(existingVehicle->getQuantity() + newVehicle->getQuantity());
                        current->unit = newVehicle;
                    }
                    return false;
                }
            }
        }
        else if (Infantry* existingInfantry = dynamic_cast<Infantry*>(current->unit)) {
            if (Infantry* newInfantry = dynamic_cast<Infantry*>(unit)) {
                if (existingInfantry->getInfantryType() == newInfantry->getInfantryType()) {
                    if (existingInfantry->getWeight() >= newInfantry->getWeight()) {
                        existingInfantry->setQuantity(existingInfantry->getQuantity() + newInfantry->getQuantity());
                        existingInfantry->getAttackScore();
                    } else {
                        newInfantry->setQuantity(existingInfantry->getQuantity() + newInfantry->getQuantity());
                        current->unit = newInfantry;
                    }
                    return false;
                }
            }
        }
        prev = current;
        current = current->next;
    }
    Node* newNode = new Node(unit);
    if (dynamic_cast<Infantry*>(unit)) {
        newNode->next = head;
        head = newNode;
        countInfantry++;
    } else if (dynamic_cast<Vehicle*>(unit)) {
        if (!head) {
            head = newNode;
        } else {
            prev->next = newNode;
        }
        countVehicle++;
    }

    return true;
}
bool UnitList::isContain(VehicleType vehicleType){
    Node* current = head;
    while (current) {
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(current->unit)) {
            if (vehicle->getVehicleType() == vehicleType) {
                return true;
            }
        }
        current = current->next;
    }
    return false;
}
bool UnitList::isContain(InfantryType infantryType){
    Node* current = head;
    while (current) {
        if (Infantry* infantry = dynamic_cast<Infantry*>(current->unit)) {
            if (infantry->getInfantryType() == infantryType) {
                return true;
            }
        }
        current = current->next;
    }
    return false;
}
void UnitList::remove(Unit* unit) {
    if (!unit || !head) return;

    Node* current = head;
    Node* prev = nullptr;

    while (current) {
        if (current->unit == unit) {
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            if (dynamic_cast<Infantry*>(unit)) {
                countInfantry--;
            } else if (dynamic_cast<Vehicle*>(unit)) {
                countVehicle--;
            }
            delete current;
            return;
        }
        prev = current;
        current = current->next;
    }
}
UnitList::Node* UnitList::getHead() const {
    return head;
}
string UnitList::str() const {
    string result = "UnitList[count_vehicle=" + to_string(countVehicle) + 
                   ";count_infantry=" + to_string(countInfantry);
    if (!head) {
        result += "]";
        return result;
    }
    result += ";";
    Node* current = head;
    result += current->unit->str();
    current = current->next;
    while (current) {
        result += "," + current->unit->str();
        current = current->next;
    }
    result += "]";
    return result;
}
int UnitList::getCapacity() const {
    return capacity;
}
int UnitList::getSize() const {
    return countVehicle + countInfantry;
}
vector<Unit*> UnitList::toVector() const {
    vector<Unit*> units;
    Node* current = head;
    while (current) {
        units.push_back(current->unit);
        current = current->next;
    }
    return units;
}
UnitList* Army::getUnitList() const {
    return unitList;
}
void UnitList::setUnitList(vector<Unit*>& units) {
    Node* current = head;
    while (current) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    countInfantry = 0;
    countVehicle = 0;

    for (Unit* unit : units) {
        if (countInfantry + countVehicle < capacity)
            insert(unit);
    }
}
// Task 3.6
Position::Position(const string& str_pos) {
    r = 0;
    c = 0;
    
    try {
        string cleaned = "";
        for (char c : str_pos) {
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                cleaned += c;
            }
        }
        
        if (cleaned[0] != '(' || cleaned[cleaned.length()-1] != ')') {
            return;
        }
        
        string content = cleaned.substr(1, cleaned.length() - 2);
        size_t comma = content.find(',');
        if (comma == string::npos) {
            return;
        }
        
        string row_str = content.substr(0, comma);
        string col_str = content.substr(comma + 1);
        
        r = stoi(row_str);
        c = stoi(col_str);
    } catch (const exception& e) {
        r = 0;
        c = 0;
    }
}
Position::Position(int r, int c) {
    this->r = r;
    this->c = c;
}
int Position::getRow() const {
    return r;
}
int Position::getCol() const {
    return c;
}
void Position::setRow(int r) {
    this->r = r;
}
void Position::setCol(int c) {
    this->c = c;
}
string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
}
// Task 3.7
/// Road
///---------------------------------------------------------------------------------
Road::Road(const Position &p) : TerrainElement(p) {}
Road::~Road() {}
void Road::getEffect(Army* army){
    if (!army) return;
}

/// Mountain
///---------------------------------------------------------------------------------
Mountain::Mountain(const Position &p) : TerrainElement(p) {}
Mountain::~Mountain() {}
void Mountain::getEffect(Army* army) {
    int newLF = army->getLF();
    int newEXP = army->getEXP();

    double InfantryIncreasePercent;
    double VehicleDecreasePercent; 
    bool isLA = dynamic_cast<LiberationArmy*>(army) != nullptr;
    int radius;
    if (isLA)
    {
        radius = 2;
        InfantryIncreasePercent = 0.3;
        VehicleDecreasePercent = 0.1;
    }
    else
    {
        radius = 4;
        InfantryIncreasePercent = 0.2;
        VehicleDecreasePercent = 0.05;
    }
    UnitList::Node* cur = army->getUnitList()->getHead();
    while (cur) {
        Unit* unit = cur->unit;
        double d = distance(this->pos, unit->getCurrentPosition());
        if (d <= radius)
        {
            if (Vehicle* ve = dynamic_cast<Vehicle*>(unit))
            {
                newLF -= safeCeil(VehicleDecreasePercent * ve->getAttackScore());
            }
            else if (Infantry* in = dynamic_cast<Infantry*>(unit))
            {
                newEXP += safeCeil(InfantryIncreasePercent * in->getAttackScore());
            }
        }
        cur = cur->next;
    }
    newLF = max(0,min(1000,newLF));
    newEXP = max(0,min(500,newEXP));
    army->setLF(newLF);
    army->setEXP(newEXP);
}

/// River
///---------------------------------------------------------------------------------
River::River(const Position &p) : TerrainElement(p) {}
River::~River() {}
void River::getEffect(Army* army) {
    if (!army) return;
    UnitList::Node* cur = army->getUnitList()->getHead();
    while (cur) {
        Unit* unit = cur->unit;
        double d = distance(this->pos, unit->getCurrentPosition());
        if (d <= 2) {
            if (Infantry* in = dynamic_cast<Infantry*>(unit)) {
                in->setMod(0.9);
            }
        }
        cur = cur->next;
    }

}
///Urban
///---------------------------------------------------------------------------------
Urban::Urban(const Position &p) : TerrainElement(p) {}
Urban::~Urban() {}
void Urban::getEffect(Army* army) {
    if (!army) return;
    UnitList::Node* cur = army->getUnitList()->getHead();
    bool isLA = dynamic_cast<LiberationArmy*>(army) != nullptr;
    while (cur) {
        Unit* unit = cur->unit;
        unit->setMod(1.0);
        double d = distance(this->pos, unit->getCurrentPosition());
        if (isLA)
        {
            if (Infantry* in = dynamic_cast<Infantry*>(unit))
            {
                if (in->getInfantryType() == SPECIALFORCES ||
                    in->getInfantryType() == REGULARINFANTRY &&
                    d<=5)
                {
                    in->setMod(1.0 + 2 / d);                
                }
            }
            else if (Vehicle* ve = dynamic_cast<Vehicle*>(unit))
            {
                if (ve->getVehicleType() == ARTILLERY && d<=2)
                {
                    ve->setMod(0.5);               
                }
            }
        }
        else
        {
            if (Infantry* in = dynamic_cast<Infantry*>(unit))
            {
                if (in->getInfantryType() == REGULARINFANTRY && d<=3)
                {
                    in->setMod(1.0 + 3 / (2 * d));                
                }
            }
        }
        cur = cur->next;
    }
}

/// Fortification
///---------------------------------------------------------------------------------
Fortification::Fortification(const Position &p) : TerrainElement(p) {}
Fortification::~Fortification() {}
void Fortification::getEffect(Army* army) {
    if (!army) return;
    UnitList::Node* cur = army->getUnitList()->getHead();
    bool isLA = dynamic_cast<LiberationArmy*>(army) != nullptr;
    while (cur) {
        Unit* unit = cur->unit;
        unit->setMod(1.0);
        double d = distance(this->pos, unit->getCurrentPosition());
        if (d<=2)
        {
            if (isLA)
            {
                unit->setMod(0.8);
            }
            else
            {
                unit->setMod(1.2);
            }
        }
        cur = cur->next;
    }
}
/// Speacial Zone
///---------------------------------------------------------------------------------
SpecialZone::SpecialZone(const Position &p) : TerrainElement(p) {}
SpecialZone::~SpecialZone() {}
void SpecialZone::getEffect(Army* army) {
    if (!army) return;

    UnitList::Node* cur = army->getUnitList()->getHead();
    while (cur) {
        Unit* unit = cur->unit;
        unit->setMod(1.0);
        double d = distance(this->pos, unit->getCurrentPosition());
        if (d<=1)
        {
            unit->setMod(0.0);
        }
        cur = cur->next;
    }
}

// Task 3.8
BattleField::BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                        vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                        vector<Position *> arrayUrban, vector<Position *> arraySpecialZone){
    this->n_rows = n_rows;
    this->n_cols = n_cols;

    // Initialize 2D array
    terrain = new TerrainElement**[n_rows];
    for (int i = 0; i < n_rows; i++) {
        terrain[i] = new TerrainElement*[n_cols];
        // Initialize all positions as ROAD
        for (int j = 0; j < n_cols; j++) {
            terrain[i][j] = new Road(Position(i, j));
        }
    }

    // Place Forest elements (Mountain)
    for (Position* pos : arrayForest) {
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new Mountain(*pos);
        }
    }

    // Place River elements
    for (Position* pos : arrayRiver) {
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new River(*pos);
        }
    }

    // Place Fortification elements
    for (Position* pos : arrayFortification) {
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new Fortification(*pos);
        }
    }

    // Place Urban elements
    for (Position* pos : arrayUrban) {
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new Urban(*pos);
        }
    }

    // Place SpecialZone elements
    for (Position* pos : arraySpecialZone) {
        int r = pos->getRow();
        int c = pos->getCol();
        if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
            delete terrain[r][c];
            terrain[r][c] = new SpecialZone(*pos);
        }
    }
}
BattleField::~BattleField() {
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            delete terrain[i][j];  // Delete each terrain element
        }
        delete[] terrain[i];  // Delete each row
    }
    delete[] terrain;  // Delete the terrain array
}
int BattleField::getRow() const {
    return n_rows;
}
int BattleField::getCol() const {
    return n_cols;
}
TerrainElement* BattleField::getTerrain(const Position& pos) const {
    int row = pos.getRow();
    int col = pos.getCol();
    
    // Check if position is within valid bounds
    if (row >= 0 && row < n_rows && col >= 0 && col < n_cols) {
        return terrain[row][col];
    }
    return nullptr;
}
// Task 3.9
vector<Position*> Configuration::parsePositions(const string& input) {
    vector<Position*> positions;
    size_t start = input.find('[');
    size_t end = input.find(']');
    if (start == string::npos || end == string::npos) return positions;
    
    string content = input.substr(start + 1, end - start - 1);
    stringstream ss(content);
    string pos;
    bool insideParentheses = false;
    string currentPosition = "";
    
    // Read character by character
    for (char c : content) {
        if (c == '(') {
            insideParentheses = true;
            currentPosition = "(";
        }
        else if (c == ')') {
            insideParentheses = false;
            currentPosition += ")";
            // Only add position if it's a complete (x,y) format
            if (currentPosition.find(',') != string::npos) {
                positions.push_back(new Position(currentPosition));
            }
            currentPosition = "";
        }
        else if (insideParentheses) {
            currentPosition += c;
        }
    }
    return positions;
}

void Configuration::parseUnits(const string& input) {
    vector<Unit*> libUnits, arvnUnits;
    size_t start = input.find('[');
    size_t end = input.find_last_of(']');
    if (start == string::npos || end == string::npos) return;

    string content = input.substr(start + 1, end - start - 1);
    
    // Split into individual unit strings
    vector<string> unitStrings;
    string currentUnit;
    int parenthesesCount = 0;
    
    for (char c : content) {
        if (c == '(' ) {
            parenthesesCount++;
            currentUnit += c;
        }
        else if (c == ')') {
            parenthesesCount--;
            currentUnit += c;
            if (parenthesesCount == 0) {
                unitStrings.push_back(currentUnit);
                currentUnit = "";
            }
        }
        else if (c == ',' && parenthesesCount == 0) {
            // Skip commas between units
            continue;
        }
        else {
            currentUnit += c;
        }
    }

    // Process each unit string
    for (const string& unitStr : unitStrings) {
        // Extract type and parameters
        size_t typeEnd = unitStr.find('(');
        if (typeEnd == string::npos) continue;

        string type = unitStr.substr(0, typeEnd);
        string params = unitStr.substr(typeEnd + 1);
        params = params.substr(0, params.length() - 1); // Remove last ')'

        // Split parameters
        vector<string> paramList;
        string param;
        parenthesesCount = 0;

        for (char c : params) {
            if (c == '(') {
                parenthesesCount++;
                param += c;
            }
            else if (c == ')') {
                parenthesesCount--;
                param += c;
                if (parenthesesCount == 0) {
                    paramList.push_back(param);
                    param = "";
                }
            }
            else if (c == ',' && parenthesesCount == 0) {
                if (!param.empty()) {
                    paramList.push_back(param);
                    param = "";
                }
            }
            else {
                param += c;
            }
        }
        if (!param.empty()) {
            paramList.push_back(param);
        }

        // Create unit if we have all parameters
        if (paramList.size() >= 4) {
            try {
                int quantity = stoi(paramList[0]);
                int weight = stoi(paramList[1]);
                Position pos(paramList[2]);
                int armyBelong = stoi(paramList[3]);

                Unit* newUnit = nullptr;
                // Vehicle types
                if (type == "TRUCK") {
                    newUnit = new Vehicle(quantity, weight, pos, TRUCK);
                }
                else if (type == "MORTAR") {
                    newUnit = new Vehicle(quantity, weight, pos, MORTAR);
                }
                else if (type == "ANTIAIRCRAFT") {
                    newUnit = new Vehicle(quantity, weight, pos, ANTIAIRCRAFT);
                }
                else if (type == "ARMOREDCAR") {
                    newUnit = new Vehicle(quantity, weight, pos, ARMOREDCAR);
                }
                else if (type == "APC") {
                    newUnit = new Vehicle(quantity, weight, pos, APC);
                }
                else if (type == "ARTILLERY") {
                    newUnit = new Vehicle(quantity, weight, pos, ARTILLERY);
                }
                else if (type == "TANK") {
                    newUnit = new Vehicle(quantity, weight, pos, TANK);
                }
                // Infantry types
                else if (type == "SNIPER") {
                    newUnit = new Infantry(quantity, weight, pos, SNIPER);
                }
                else if (type == "ANTIAIRCRAFTSQUAD") {
                    newUnit = new Infantry(quantity, weight, pos, ANTIAIRCRAFTSQUAD);
                }
                else if (type == "MORTARSQUAD") {
                    newUnit = new Infantry(quantity, weight, pos, MORTARSQUAD);
                }
                else if (type == "ENGINEER") {
                    newUnit = new Infantry(quantity, weight, pos, ENGINEER);
                }
                else if (type == "SPECIALFORCES") {
                    newUnit = new Infantry(quantity, weight, pos, SPECIALFORCES);
                }
                else if (type == "REGULARINFANTRY") {
                    newUnit = new Infantry(quantity, weight, pos, REGULARINFANTRY);
                }

                if (newUnit) {
                    if (armyBelong == 0) {
                        libUnits.push_back(newUnit);
                    } else {
                        arvnUnits.push_back(newUnit);
                    }
                }
            }
            catch (...) {
                continue;
            }
        }
    }

    // Store units in arrays
    liberationSize = libUnits.size();
    ARVNSize = arvnUnits.size();

    liberationUnits = new Unit*[liberationSize];
    ARVNUnits = new Unit*[ARVNSize];

    for (int i = 0; i < liberationSize; i++) {
        liberationUnits[i] = libUnits[i];
    }
    for (int i = 0; i < ARVNSize; i++) {
        ARVNUnits[i] = arvnUnits[i];
    }
}

 Configuration::Configuration(const string& filepath) {
    // Initialize members
    liberationUnits = nullptr;
    ARVNUnits = nullptr;
    liberationSize = 0;
    ARVNSize = 0;
    eventCode = 0;
    num_rows = 0;
    num_cols = 0;
    
    // Read configuration file
    ifstream file(filepath);
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        size_t pos = line.find('=');
        if (pos == string::npos) continue;
        
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        
        if (key == "NUM_ROWS") num_rows = stoi(value);
        else if (key == "NUM_COLS") num_cols = stoi(value);
        else if (key == "ARRAY_FOREST") arrayForest = parsePositions(value);
        else if (key == "ARRAY_RIVER") arrayRiver = parsePositions(value);
        else if (key == "ARRAY_FORTIFICATION") arrayFortification = parsePositions(value);
        else if (key == "ARRAY_URBAN") arrayUrban = parsePositions(value);
        else if (key == "ARRAY_SPECIAL_ZONE") arraySpecialZone = parsePositions(value);
        else if (key == "UNIT_LIST") parseUnits(value);
        else if (key == "EVENT_CODE") {
            int code = stoi(value);
            if (code > 99) code %= 100;
            if (code < 0) code = 0;
            eventCode = code;
        }
    }
    file.close();
}

Configuration::~Configuration() {
    for (Position* p : arrayForest) delete p;
    for (Position* p : arrayRiver) delete p;
    for (Position* p : arrayFortification) delete p;
    for (Position* p : arrayUrban) delete p;
    for (Position* p : arraySpecialZone) delete p;

    for (int i = 0; i < liberationSize; i++) {
        delete liberationUnits[i];
    }
    delete[] liberationUnits;

    for (int i = 0; i < ARVNSize; i++) {
        delete ARVNUnits[i];
    }
    delete[] ARVNUnits;
}
// Getters
int Configuration::getNumRows() const { return num_rows; }
int Configuration::getNumCols() const { return num_cols; }
vector<Position*> Configuration::getArrayForest() const { return arrayForest; }
vector<Position*> Configuration::getArrayRiver() const { return arrayRiver; }
vector<Position*> Configuration::getArrayFortification() const { return arrayFortification; }
vector<Position*> Configuration::getArrayUrban() const { return arrayUrban; }
vector<Position*> Configuration::getArraySpecialZone() const { return arraySpecialZone; }
Unit** Configuration::getLiberationUnits() const { return liberationUnits; }
Unit** Configuration::getARVNUnits() const { return ARVNUnits; }
int Configuration::getLiberationSize() const { return liberationSize; }
int Configuration::getARVNSize() const { return ARVNSize; }
int Configuration::getEventCode() const { return eventCode; }

string Configuration::str() const {
    string result = "[";
    result += "num_rows=" + to_string(num_rows) + ",";
    result += "num_cols=" + to_string(num_cols) + ",";
    
    result += "arrayForest=[";
    for (size_t i = 0; i < arrayForest.size(); i++) {
        result += arrayForest[i]->str();
        if (i < arrayForest.size() - 1) result += ",";
    }
    result += "],";
    
    result += "arrayRiver=[";
    for (size_t i = 0; i < arrayRiver.size(); i++) {
        result += arrayRiver[i]->str();
        if (i < arrayRiver.size() - 1) result += ",";
    }
    result += "],";
    
    result += "arrayFortification=[";
    for (size_t i = 0; i < arrayFortification.size(); i++) {
        result += arrayFortification[i]->str();
        if (i < arrayFortification.size() - 1) result += ",";
    }
    result += "],";
    
    result += "arrayUrban=[";
    for (size_t i = 0; i < arrayUrban.size(); i++) {
        result += arrayUrban[i]->str();
        if (i < arrayUrban.size() - 1) result += ",";
    }
    result += "],";
    
    result += "arraySpecialZone=[";
    for (size_t i = 0; i < arraySpecialZone.size(); i++) {
        result += arraySpecialZone[i]->str();
        if (i < arraySpecialZone.size() - 1) result += ",";
    }
    result += "],";
    
    result += "liberationUnits=[";
    for (int i = 0; i < liberationSize; i++) {
        result += liberationUnits[i]->str();
        if (i < liberationSize - 1) result += ",";
    }
    result += "],";
    
    result += "ARVNUnits=[";
    for (int i = 0; i < ARVNSize; i++) {
        result += ARVNUnits[i]->str();
        if (i < ARVNSize - 1) result += ",";
    }
    result += "],";
    
    result += "eventCode=" + to_string(eventCode);
    result += "]";
    return result;
}

/// Task 3.10
HCMCampaign::HCMCampaign(const string& config_file_path) {
    // Initialize configuration
    config = new Configuration(config_file_path);
    
    // Create battlefield with proper getter methods
    battleField = new BattleField(
        config->getNumRows(),      // Use getter instead of direct access
        config->getNumCols(),      // Use getter instead of direct access
        config->getArrayForest(),  // Use getter instead of direct access
        config->getArrayRiver(),   // Use getter instead of direct access
        config->getArrayFortification(), // Use getter instead of direct access
        config->getArrayUrban(),   // Use getter instead of direct access
        config->getArraySpecialZone()   // Use getter instead of direct access
    );

    // Create armies
    liberationArmy = new LiberationArmy(
        config->getLiberationUnits(),
        config->getLiberationSize(),
        "Liberation Army",
        battleField
    );
    
    arvn = new ARVN(
        config->getARVNUnits(),
        config->getARVNSize(),
        "ARVN",
        battleField
    );
}

void HCMCampaign::applyTerrainEffects() {
    // Apply terrain effects for each position
    for (int i = 0; i < battleField->getRow(); i++) {
        for (int j = 0; j < battleField->getCol(); j++) {
            Position pos(i, j);
            TerrainElement* terrain = battleField->getTerrain(pos);
            if (terrain) {
                terrain->getEffect(liberationArmy);
                terrain->getEffect(arvn);
            }
        }
    }
}

void HCMCampaign::run() {
    // Apply terrain effects before battle
    applyTerrainEffects();
    
    // Determine attack sequence based on event code
    if (config->getEventCode() < 75) {
        // Liberation Army attacks, ARVN defends
        liberationArmy->fight(arvn, false);
        arvn->fight(liberationArmy, true);
    } else {
        // ARVN attacks first
        arvn->fight(liberationArmy, false);
        liberationArmy->fight(arvn, false);  // Liberation Army counterattack
    }
    
    // Remove units with attackScore <= 5
    UnitList::Node* current;
    
    // Check Liberation Army units
    current = liberationArmy->getUnitList()->getHead();
    while (current) {
        int attackScore = 0;
        Unit* unitToRemove = nullptr;
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(current->unit)) {
            attackScore = vehicle->getAttackScore();
        } else if (Infantry* infantry = dynamic_cast<Infantry*>(current->unit)) {
            attackScore = infantry->getAttackScore();
        }
        
        if (attackScore <= 5) {
            unitToRemove = current->unit;
            liberationArmy->getUnitList()->remove(unitToRemove);
        }
        current = current->next;
    }
    // Check ARVN units
    current = arvn->getUnitList()->getHead();
    while (current) {
        Unit* unitToRemove = nullptr;
        int attackScore = 0;
        if (Vehicle* vehicle = dynamic_cast<Vehicle*>(current->unit)) {
            attackScore = vehicle->getAttackScore();
        } else if (Infantry* infantry = dynamic_cast<Infantry*>(current->unit)) {
            attackScore = infantry->getAttackScore();
        }
        
        if (attackScore <= 5) {
            unitToRemove = current->unit;
            arvn->getUnitList()->remove(unitToRemove);
        }
        current = current->next;
    }
    
    // Update final scores
    liberationArmy->updateScore();
    arvn->updateScore();
}

string HCMCampaign::printResult(){
    return "LIBERATIONARMY[LF=" + to_string(liberationArmy->getLF()) + 
           ",EXP=" + to_string(liberationArmy->getEXP()) + "]-" +
           "ARVN[LF=" + to_string(arvn->getLF()) + 
           ",EXP=" + to_string(arvn->getEXP()) + "]";
}
HCMCampaign::~HCMCampaign() {
    delete config;
    delete battleField;
    delete liberationArmy;
    delete arvn;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
