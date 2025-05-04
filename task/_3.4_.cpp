#include "main.h"
#include "hcmcampaign.h"
#include "UnitList_3.5_.cpp"




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
        virtual void setAttackScore();
        Position getCurrentPosition() const;
        virtual string str() const = 0;
        int getQuantity();
        int getWeight();
        void setQuantity(int n);
        void setWeight(int n);
};

int Unit::getQuantity(){
    return quantity;
}

void Unit::setQuantity(int quantity){
    this->quantity = quantity;
}

int Unit::getWeight(){
    return weight;
}

void Unit::setWeight(int weight){
    this->quantity = weight;
}
class Army
{
    protected:
        int LF, EXP;
        string name;
        UnitList *unitList;
        BattleField *battleField;
        bool lastSuccess;

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
        UnitList* getUnitList() const {
            return unitList;
        }
        public:
            bool lastAttackSuccessful() const {
                return lastSuccess;
            }

            void setLastAttackSuccessful(bool success) {
                lastSuccess = success;
            }
};

class LiberationArmy : public Army {
    public:
        LiberationArmy(const Unit** unitArray, int size, const std::string& name, BattleField* battleField)
            : Army(unitArray, size, name, battleField) {}
    
        void fight(Army* enemy, bool defense = false){
            if (!defense) offensiveBattle(enemy);
            else defensiveBattle(enemy);
        }
    
        std::string str() const override {
            std::ostringstream oss;
            oss << "LiberationArmy[name=" << name
                << ",LF=" << LF
                << ",EXP=" << EXP
                << ",unitList=" << unitList->str()
                << ",battleField=" << battleField
                << "]";
            return oss.str();
        }
    
    private:
        // Offensive engagement
        void offensiveBattle(Army* enemy) {
            int origLF = LF, origEXP = EXP;
            LF = static_cast<int>(LF * 1.5);
            EXP = static_cast<int>(EXP * 1.5);
    
            auto combA = findMinimalCombination<Infantry>(enemy->getEXP());
            auto combB = findMinimalCombination<Vehicle>(enemy->getLF());
    
            bool haveA = !combA.empty();
            bool haveB = !combB.empty();
    
            if (haveA && haveB) {
                removeCombination(combA);
                removeCombination(combB);
                confiscateEnemy(enemy);
                updateIndices();
            } else if (haveA || haveB) {
                if (compareUnmatched(enemy, haveA)) {
                    auto& valid = haveA ? combA : combB;
                    auto& invalid = haveA ? combB : combA;
                    removeCombination(valid);
                    removeCombination(invalid);
                    confiscateEnemy(enemy);
                    updateIndices();
                } else {
                    rollbackIndices(origLF, origEXP);
                    degradeUnits();
                }
            } else {
                rollbackIndices(origLF, origEXP);
                degradeUnits();
            }
        }
    
        // Defensive engagement
        void defensiveBattle(Army* enemy) {
            LF = static_cast<int>(LF * 1.3);
            EXP = static_cast<int>(EXP * 1.3);
    
            bool lfOK = LF >= enemy->getLF();
            bool expOK = EXP >= enemy->getEXP();
    
            if (lfOK && expOK) {
                // Victory: no change
            } else if (!lfOK && !expOK) {
                reinforceUnits();
                updateIndices();
                defensiveBattle(enemy);
            } else {
                degradeUnits();
                updateIndices();
            }
        }
    
        // Find smallest subset of units of type T whose summed attack > target
        template<typename T>
        std::vector<Unit*> findMinimalCombination(int target) {
            // Collect units of type T
            std::vector<Unit*> units;
            for (auto node = unitList->getHead(); node; node = node->next) {
                if (T* t = dynamic_cast<T*>(node->unit)) {
                    units.push_back(node->unit);
                }
            }
            int n = units.size();
            std::vector<Unit*> best;
            int bestSum = INT_MAX;
            // Try all subsets via bitmask (n small)
            for (int mask = 1; mask < (1<<n); ++mask) {
                int sum = 0;
                std::vector<Unit*> subset;
                for (int i = 0; i < n; ++i) if (mask & (1<<i)) {
                    sum += units[i]->getAttackScore();
                    subset.push_back(units[i]);
                }
                if (sum > target && sum < bestSum) {
                    bestSum = sum;
                    best = subset;
                }
            }
            return best;
        }
    
        void removeCombination(const std::vector<Unit*>& combo) {
            for (Unit* u : combo) {
                unitList->remove(u);
            }
        }
    
        void confiscateEnemy(Army* enemy) {
            // Transfer enemy units
            for (auto node = enemy->getUnitList()->getHead(); node; node = node->next) {
                unitList->insert(node->unit);
            }
        }
    
        void degradeUnits() {
            for (auto node = unitList->getHead(); node; node = node->next) {
                int w = node->unit->getWeight();
                node->unit->setWeight(static_cast<int>(w * 0.9));
            }
        }
    
        void reinforceUnits() {
            auto fibNext = [](int x) {
                int a = 1, b = 1;
                while (b < x) { int t = a + b; a = b; b = t; }
                return b;
            };
            for (auto node = unitList->getHead(); node; node = node->next) {
                int q = node->unit->getQuantity();
                node->unit->setQuantity(fibNext(q));
            }
        }
    
        void updateIndices() {
            LF = EXP = 0;
            for (auto node = unitList->getHead(); node; node = node->next) {
                if (Vehicle* v = dynamic_cast<Vehicle*>(node->unit)) LF += v->getAttackScore();
                else if (Infantry* inf = dynamic_cast<Infantry*>(node->unit)) EXP += inf->getAttackScore();
            }
            LF = std::min(LF, 1000);
            EXP = std::min(EXP, 500);
        }
    
        void rollbackIndices(int oldLF, int oldEXP) {
            LF = oldLF;
            EXP = oldEXP;
        }

        bool compareUnmatched(Army* enemy, bool infantryMatched) {
            if (infantryMatched) {
                // compare vehicle index
                return LF >= enemy->getLF();
            } else {
                // compare infantry index
                return EXP >= enemy->getEXP();
            }
        }
    };

    class ARVN : public Army {
        public:
            ARVN(const Unit** unitArray, int size, std::string name, BattleField* battleField)
                : Army(unitArray, size, name, battleField) {}
        
            void fight(Army* enemy, bool defense = false) override {
                if (!enemy) return;
        
                if (!defense) {
                    // Attack case: ARVN always loses
                    UnitList::Node* cur = unitList->getHead();
                    while (cur) {
                        Unit* unit = cur->unit;
                        int qty = unit->getQuantity();
                        if (qty == 1) {
                            unitList->remove(unit);
                        } else {
                            unit->setQuantity(std::ceil(qty * 0.8));
                        }
                        cur = cur->next;
                    }
                    updateCombatIndices();
                } else {
                    // Defense case: LiberationArmy attacks
                    LiberationArmy* lib = dynamic_cast<LiberationArmy*>(enemy);
                    if (!lib) return;
                    // If LiberationArmy wins, remove ARVN units
                    if (lib->lastAttackSuccessful()) {
                        UnitList::Node* cur = unitList->getHead();
                        while (cur) {
                            Unit* unit = cur->unit;
                            unit->setWeight(unit->getWeight() * 0.8);
                            cur = cur->next;
                        }
                        updateCombatIndices();
                    }
                }
            }
        
            std::string str() const override {
                std::ostringstream oss;
                oss << "ARVN[name=" << name
                    << ",LF=" << LF
                    << ",EXP=" << EXP
                    << "," << unitList->str()
                    << ",battleField=" << battleField->getName()
                    << "]";
                return oss.str();
            }
        
        private:
            void updateCombatIndices() {
                LF = 0;
                EXP = 0;
                UnitList::Node* cur = unitList->getHead();
                while (cur) {
                    if (Vehicle* v = dynamic_cast<Vehicle*>(cur->unit)) {
                        LF += v->getAttackScore();
                    } else if (Infantry* i = dynamic_cast<Infantry*>(cur->unit)) {
                        EXP += i->getAttackScore();
                    }
                    cur = cur->next;
                }
                if (LF > 1000) LF = 1000;
                if (EXP > 500) EXP = 500;
            }
        };

    class BattleField
    {
    private:
        int n_rows, n_cols;
        // TODO
    public:
        BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                    vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                    vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
        ~BattleField();
        string getName(){
            return 0;
        }
    };

