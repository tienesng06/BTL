#include "main.h"
#include "hcmcampaign.h"



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

class Vehicle : public Unit{
    private:
        VehicleType vehicleType;
    public:
        Vehicle ( int quantity , int weight , const Position pos , VehicleType
        vehicleType ) : Unit(quantity , weight , pos){
            this->vehicleType = vehicleType;
        }

        int getAttackScore(){
            int score = (static_cast<int>(vehicleType) * 304 + quantity * weight) / 30;
            return score;
        }

        string str(){
            return "Vehicle[Vehicle Type = " + to_string(vehicleType) + ", "
                    + "quantity = "+ to_string(quantity) + ", "
                    + "weight = " + to_string(weight) + ", "
                    + "pos = " + pos.str() + "]";  
        }
        
                                                                                                            
};