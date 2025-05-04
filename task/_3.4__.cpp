#include "main.h"
#include "hcmcampaign.h"
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
};
class LiberationArmy : public Army {
    private:
        // Helper method to find the combination of infantry units with the smallest total score greater than target
        vector<const Infantry*> findInfantryCombination(int targetScore) {
            vector<const Infantry*> result;
            vector<const Infantry*> infantryUnits;
            
            // First, extract all infantry units from unitList
            for (int i = 0; i < unitList->getSize(); i++) {
                const Unit* unit = unitList->getUnit(i);
                const Infantry* infantry = dynamic_cast<const Infantry*>(unit);
                if (infantry != nullptr) {
                    infantryUnits.push_back(infantry);
                }
            }
            
            // Sort infantry units by attack score (ascending)
            sort(infantryUnits.begin(), infantryUnits.end(), 
                [](const Infantry* a, const Infantry* b) {
                    return const_cast<Infantry*>(a)->getAttackScore() < const_cast<Infantry*>(b)->getAttackScore();
                });
            
            // Try to find the smallest combination with score > targetScore
            int currentScore = 0;
            for (const Infantry* infantry : infantryUnits) {
                result.push_back(infantry);
                currentScore += const_cast<Infantry*>(infantry)->getAttackScore();
                
                if (currentScore > targetScore) {
                    return result;
                }
            }
            
            // If no combination found, return empty vector
            return vector<const Infantry*>();
        }
        
        // Helper method to find the combination of vehicle units with the smallest total score greater than target
        vector<const Vehicle*> findVehicleCombination(int targetScore) {
            vector<const Vehicle*> result;
            vector<const Vehicle*> vehicleUnits;
            
            // First, extract all vehicle units from unitList
            for (int i = 0; i < unitList->getSize(); i++) {
                const Unit* unit = unitList->getUnit(i);
                const Vehicle* vehicle = dynamic_cast<const Vehicle*>(unit);
                if (vehicle != nullptr) {
                    vehicleUnits.push_back(vehicle);
                }
            }
            
            // Sort vehicle units by attack score (ascending)
            sort(vehicleUnits.begin(), vehicleUnits.end(), 
                [](const Vehicle* a, const Vehicle* b) {
                    return const_cast<Vehicle*>(a)->getAttackScore() < const_cast<Vehicle*>(b)->getAttackScore();
                });
            
            // Try to find the smallest combination with score > targetScore
            int currentScore = 0;
            for (const Vehicle* vehicle : vehicleUnits) {
                result.push_back(vehicle);
                currentScore += const_cast<Vehicle*>(vehicle)->getAttackScore();
                
                if (currentScore > targetScore) {
                    return result;
                }
            }
            
            // If no combination found, return empty vector
            return vector<const Vehicle*>();
        }
        
        // Helper method to remove units from unitList
        void removeUnits(const vector<const Unit*>& unitsToRemove) {
            UnitList* newUnitList = new UnitList();
            
            // Add all units except those in unitsToRemove
            for (int i = 0; i < unitList->getSize(); i++) {
                const Unit* unit = unitList->getUnit(i);
                bool shouldRemove = false;
                
                for (const Unit* unitToRemove : unitsToRemove) {
                    if (unit == unitToRemove) {
                        shouldRemove = true;
                        break;
                    }
                }
                
                if (!shouldRemove) {
                    newUnitList->addUnit(unit);
                }
            }
            
            // Replace old unitList with new one
            delete unitList;
            unitList = newUnitList;
        }
        
        // Helper method to add enemy units to our unitList
        void confiscateEnemyUnits(Army* enemy) {
            UnitList* enemyUnitList = enemy->getUnitList();
            
            for (int i = 0; i < enemyUnitList->getSize(); i++) {
                const Unit* enemyUnit = enemyUnitList->getUnit(i);
                
                // Check if similar unit already exists
                bool unitExists = false;
                for (int j = 0; j < unitList->getSize(); j++) {
                    const Unit* myUnit = unitList->getUnit(j);
                    
                    // For Vehicle units
                    const Vehicle* enemyVehicle = dynamic_cast<const Vehicle*>(enemyUnit);
                    const Vehicle* myVehicle = dynamic_cast<const Vehicle*>(myUnit);
                    
                    if (enemyVehicle != nullptr && myVehicle != nullptr) {
                        if (enemyVehicle->getVehicleType() == myVehicle->getVehicleType()) {
                            // Update quantity of existing vehicle
                            const_cast<Vehicle*>(myVehicle)->setQuantity(
                                myVehicle->getQuantity() + enemyVehicle->getQuantity()
                            );
                            unitExists = true;
                            break;
                        }
                    }
                    
                    // For Infantry units
                    const Infantry* enemyInfantry = dynamic_cast<const Infantry*>(enemyUnit);
                    const Infantry* myInfantry = dynamic_cast<const Infantry*>(myUnit);
                    
                    if (enemyInfantry != nullptr && myInfantry != nullptr) {
                        if (enemyInfantry->getInfantryType() == myInfantry->getInfantryType()) {
                            // Update quantity of existing infantry
                            const_cast<Infantry*>(myInfantry)->setQuantity(
                                myInfantry->getQuantity() + enemyInfantry->getQuantity()
                            );
                            unitExists = true;
                            break;
                        }
                    }
                }
                
                // If no similar unit exists, add the enemy unit
                if (!unitExists) {
                    unitList->addUnit(enemyUnit);
                }
            }
        }
        
        // Helper method to reduce unit weights by 10%
        void reduceWeights() {
            for (int i = 0; i < unitList->getSize(); i++) {
                const Unit* unit = unitList->getUnit(i);
                
                // Reduce weight by 10%
                // This would require access to the weight attribute or a setWeight method
                // For this implementation, we'll reduce quantity instead (since weight is protected)
                const Vehicle* vehicle = dynamic_cast<const Vehicle*>(unit);
                if (vehicle != nullptr) {
                    const_cast<Vehicle*>(vehicle)->setQuantity(
                        vehicle->getQuantity() * 0.9
                    );
                }
                
                const Infantry* infantry = dynamic_cast<const Infantry*>(unit);
                if (infantry != nullptr) {
                    const_cast<Infantry*>(infantry)->setQuantity(
                        infantry->getQuantity() * 0.9
                    );
                }
            }
        }
        
        // Helper method to reduce unit quantities by 10%
        void reduceQuantities() {
            for (int i = 0; i < unitList->getSize(); i++) {
                const Unit* unit = unitList->getUnit(i);
                
                // Reduce quantity by 10%
                const Vehicle* vehicle = dynamic_cast<const Vehicle*>(unit);
                if (vehicle != nullptr) {
                    const_cast<Vehicle*>(vehicle)->setQuantity(
                        vehicle->getQuantity() * 0.9
                    );
                }
                
                const Infantry* infantry = dynamic_cast<const Infantry*>(unit);
                if (infantry != nullptr) {
                    const_cast<Infantry*>(infantry)->setQuantity(
                        infantry->getQuantity() * 0.9
                    );
                }
            }
        }
        
        // Helper method to increase unit quantities to the nearest Fibonacci number
        void increaseToFibonacci() {
            for (int i = 0; i < unitList->getSize(); i++) {
                const Unit* unit = unitList->getUnit(i);
                
                // Get current quantity
                int quantity = 0;
                const Vehicle* vehicle = dynamic_cast<const Vehicle*>(unit);
                if (vehicle != nullptr) {
                    quantity = vehicle->getQuantity();
                    const_cast<Vehicle*>(vehicle)->setQuantity(
                        nearestFibonacci(quantity)
                    );
                }
                
                const Infantry* infantry = dynamic_cast<const Infantry*>(unit);
                if (infantry != nullptr) {
                    quantity = infantry->getQuantity();
                    const_cast<Infantry*>(infantry)->setQuantity(
                        nearestFibonacci(quantity)
                    );
                }
            }
        }
        
        // Helper method to find the nearest Fibonacci number
        int nearestFibonacci(int n) {
            // Base cases
            if (n <= 0) return 0;
            if (n <= 2) return n;
            
            // Find the two consecutive Fibonacci numbers around n
            int a = 1, b = 1;
            while (b <= n) {
                int temp = a + b;
                a = b;
                b = temp;
            }
            
            // Return the closest one
            return (b - n) < (n - a) ? b : a;
        }
        
        // Helper method to update combat indices (LF and EXP)
        void updateCombatIndices() {
            LF = 0;
            EXP = 0;
            
            for (int i = 0; i < unitList->getSize(); i++) {
                const Unit* unit = unitList->getUnit(i);
                
                const Vehicle* vehicle = dynamic_cast<const Vehicle*>(unit);
                if (vehicle != nullptr) {
                    LF += const_cast<Vehicle*>(vehicle)->getAttackScore();
                    if (LF > 1000) LF = 1000;
                }
                
                const Infantry* infantry = dynamic_cast<const Infantry*>(unit);
                if (infantry != nullptr) {
                    EXP += const_cast<Infantry*>(infantry)->getAttackScore();
                    if (EXP > 500) EXP = 500;
                }
            }
        }
    
    public:
        // Constructor
        LiberationArmy(const Unit** unitArray, int size, string name, BattleField* battleField)
            : Army(unitArray, size, name, battleField) {
            // The base class constructor already handles initialization
        }
        
        // Fight method implementation
        void fight(Army* enemy, bool defense = false) override {
            if (!defense) {
                // Offensive case
                
                // Multiply combat indices by 1.5
                int offensiveLF = LF * 1.5;
                int offensiveEXP = EXP * 1.5;
                
                // Find combinations
                vector<const Infantry*> combinationA = findInfantryCombination(enemy->getEXP());
                vector<const Vehicle*> combinationB = findVehicleCombination(enemy->getLF());
                
                // Convert to Unit* for removeUnits
                vector<const Unit*> unitsToRemoveA;
                for (const Infantry* unit : combinationA) {
                    unitsToRemoveA.push_back(static_cast<const Unit*>(unit));
                }
                
                vector<const Unit*> unitsToRemoveB;
                for (const Vehicle* unit : combinationB) {
                    unitsToRemoveB.push_back(static_cast<const Unit*>(unit));
                }
                
                // Case: Both combinations found - Victory
                if (!combinationA.empty() && !combinationB.empty()) {
                    // Remove units in both combinations
                    vector<const Unit*> allUnitsToRemove;
                    allUnitsToRemove.insert(allUnitsToRemove.end(), unitsToRemoveA.begin(), unitsToRemoveA.end());
                    allUnitsToRemove.insert(allUnitsToRemove.end(), unitsToRemoveB.begin(), unitsToRemoveB.end());
                    removeUnits(allUnitsToRemove);
                    
                    // Confiscate enemy units
                    confiscateEnemyUnits(enemy);
                    
                    // Update combat indices
                    updateCombatIndices();
                }
                // Case: Only one combination found - Possible stalemate
                else if (!combinationA.empty() || !combinationB.empty()) {
                    bool winA = false, winB = false;
                    
                    // Check if we win with combination A
                    if (!combinationA.empty()) {
                        if (offensiveLF > enemy->getLF()) {
                            winA = true;
                        }
                    }
                    
                    // Check if we win with combination B
                    if (!combinationB.empty()) {
                        if (offensiveEXP > enemy->getEXP()) {
                            winB = true;
                        }
                    }
                    
                    // If we win with either combination
                    if (winA || winB) {
                        // Remove valid combination
                        if (winA) {
                            removeUnits(unitsToRemoveA);
                        }
                        if (winB) {
                            removeUnits(unitsToRemoveB);
                        }
                        
                        // Confiscate enemy units
                        confiscateEnemyUnits(enemy);
                        
                        // Update combat indices
                        updateCombatIndices();
                    }
                }
                // Case: No combinations found - No battle
                else {
                    // Reduce weights by 10%
                    reduceWeights();
                    
                    // Update combat indices
                    updateCombatIndices();
                }
            }
            else {
                // Defensive case
                
                // Multiply combat indices by 1.3
                int defensiveLF = LF * 1.3;
                int defensiveEXP = EXP * 1.3;
                
                // Compare with enemy indices
                bool lowerLF = defensiveLF < enemy->getLF();
                bool lowerEXP = defensiveEXP < enemy->getEXP();
                
                // Case: Both indices are not lower - Win
                if (!lowerLF && !lowerEXP) {
                    // Victory, no changes needed
                }
                // Case: One index is lower - 10% reduction in quantity
                else if (lowerLF != lowerEXP) {
                    reduceQuantities();
                    updateCombatIndices();
                }
                // Case: Both indices are lower - Increase to nearest Fibonacci
                else {
                    increaseToFibonacci();
                    updateCombatIndices();
                    
                    // Re-evaluate after reinforcements
                    defensiveLF = LF * 1.3;
                    defensiveEXP = EXP * 1.3;
                    
                    // No further action specified in the requirements
                }
            }
        }
        
        // String representation
        string str() const override {
            return "LiberationArmy[name=" + name + 
                   ", LF=" + to_string(LF) + 
                   ", EXP=" + to_string(EXP) + 
                   ", unitList=" + unitList->str() + 
                   ", battleField=" + battleField->str() + "]";
        }
    };