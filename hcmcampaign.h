/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

public:
    Army(Unit **unitArray, int size, string name, BattleField *battleField);
    ~Army();
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;
    void updateScore();
    int getLF() const;
    int getEXP() const;
    void setLF(int LF);
    void setEXP(int EXP);
    UnitList *getUnitList() const;
};

class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos); // Example: str_pos = "(1,15)"
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const; // Example: returns "(1,15)"
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;
    double mod = 1.0;

public:
    Unit(int quantity, int weight, Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;
    int getQuantity() const;
    int getWeight() const;
    void setQuantity(int quantity);
    void setWeight(int weight);
    void setMod(double mod);

};

class UnitList
{
public:
    struct Node{
        Unit *unit;
        Node *next;
        Node(Unit *unit){
            this->unit = unit;
            this->next = nullptr;
        }
    };
private:
    Node *head;
    int capacity;
    int size;
    int countInfantry;
    int countVehicle;

    // TODO
public:
    UnitList(int capacity);
    ~UnitList();
    bool insert(Unit *unit);                   // return true if insert successfully
    bool isContain(VehicleType vehicleType);   // return true if it exists
    bool isContain(InfantryType infantryType); // return true if it exists
    string str() const;
    Node* getHead() const;
    void remove(Unit *unit); // remove unit from list
    int getCapacity() const;
    int getSize() const;
    vector<Unit*> toVector() const; // convert to vector
    void setUnitList(vector<Unit*>& units);
    // TODO
};
class Vehicle : public Unit{
private:
    VehicleType vehicleType;
public:
    Vehicle(int quantity, int weight, Position pos, VehicleType vehicleType);
    ~Vehicle();
    int getAttackScore();
    string str() const;
    VehicleType getVehicleType() const;
    int getQuantity() const;
    int getWeight() const;
    void setVehicleType(VehicleType vehicleType);
    void setQuantity(int quantity);
    void setWeight(int weight);
};
class Infantry : public Unit{
private:
    InfantryType infantryType;
public:
    Infantry(int quantity, int weight, Position pos, InfantryType infantryType);
    ~Infantry();
    bool Isperfect(int n);
    int personalNumber(int n);
    int getAttackScore();
    string str() const;
    InfantryType getInfantryType() const;
    int getQuantity() const;
    int getWeight() const;
    void setInfantryType(InfantryType infantryType);
    void setQuantity(int quantity);
    void setWeight(int weight);
};
class LiberationArmy : public Army
{
private:
    vector<Unit*> CloneUnitAddress;
public:
    LiberationArmy(Unit **unitArray, int size, string name, BattleField *battleField);
    ~LiberationArmy();
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
    Unit* cloneUnit(Unit* unit);
    int nearestFibonacci(int value);    
};
class ARVN : public Army{
private:
    vector<Unit*> CloneUnitAddress;
public:
    ARVN(Unit **unitArray, int size, string name, BattleField *battleField);
    ~ARVN();
    void fight(Army *enemy, bool defense = false) override;
    string str() const override;
    Unit* cloneUnit(Unit* unit);
    void updateScores();
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
class Road : public TerrainElement
{
public:
    Road(const Position &p);
    ~Road();
    void getEffect(Army *army) override;
};
class Mountain : public TerrainElement
{
public:
    Mountain(const Position &p);
    ~Mountain();
    void getEffect(Army *army) override;
};
class River : public TerrainElement
{
public:
    River(const Position &p);
    ~River();
    void getEffect(Army *army) override;
};
class Urban : public TerrainElement
{
public:
    Urban(const Position &p);
    ~Urban();
    void getEffect(Army *army) override;
};
class Fortification : public TerrainElement
{
public:
    Fortification(const Position &p);
    ~Fortification();
    void getEffect(Army *army) override;
};
class SpecialZone : public TerrainElement
{
public:
    SpecialZone(const Position &p);
    ~SpecialZone();
    void getEffect(Army *army) override;
};

class BattleField
{
private:
    int n_rows, n_cols;
    TerrainElement*** terrain;
    // TODO
public:
    BattleField(int n_rows, int n_cols, vector<Position *> arrayForest,
                vector<Position *> arrayRiver, vector<Position *> arrayFortification,
                vector<Position *> arrayUrban, vector<Position *> arraySpecialZone);
    ~BattleField();
    int getRow() const;
    int getCol() const;
    TerrainElement* getTerrain(const Position& pos) const;
};

class Configuration {
private:
    int num_rows;
    int num_cols;
    vector<Position*> arrayForest;
    vector<Position*> arrayRiver;
    vector<Position*> arrayFortification;
    vector<Position*> arrayUrban;
    vector<Position*> arraySpecialZone;
    Unit** liberationUnits;
    Unit** ARVNUnits;
    int liberationSize;
    int ARVNSize;
    int eventCode;

    vector<Position*> parsePositions(const string& input);
    void parseUnits(const string& input);

public:
    Configuration(const string& filepath);
    ~Configuration();
    string str() const;
    
    // Getters
    int getEventCode() const;
    int getNumRows() const;
    int getNumCols() const;
    Unit** getLiberationUnits() const;
    int getLiberationSize() const;
    Unit** getARVNUnits() const;
    int getARVNSize() const;
    vector<Position*> getArrayForest() const;
    vector<Position*> getArrayRiver() const;
    vector<Position*> getArrayFortification() const;
    vector<Position*> getArrayUrban() const;
    vector<Position*> getArraySpecialZone() const;
};
class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *arvn;
    void applyTerrainEffects();
public:
    HCMCampaign(const string &config_file_path);
    ~HCMCampaign();
    void run();
    string printResult();
};

#endif
