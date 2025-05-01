#include "main.h"
#include "hcmcampaign.h"

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class Army
{
    protected:
        int LF, EXP;
        string name;
        UnitList *unitList;
        BattleField *battleField;

    public:
        Army(Unit **unitArray, int size, string name, BattleField *battleField);
        virtual void fight(Army *enemy, bool defense = false) = 0;
        virtual string str() const = 0;
};

class TerrainElement
{
    public:
        TerrainElement();
        ~TerrainElement();
        virtual void getEffect(Army *army) = 0;
};


class Road : public TerrainElement{
    public:
        void getEffect(){}
        
};