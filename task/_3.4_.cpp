#include "main.h"
#include "hcmcampaign.h"



class ARVN;

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

class LiberationArmy : public Army{
    public:
        LiberationArmy ( const Unit ** unitArray , int size , string name , BattleField * battleField ) : 
        Army(unitArray, size, name, battleField){}

        void fight ( Army * enemy , bool defense = false ){
            double newLF = LF;
            double newEXP = EXP;
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
};

