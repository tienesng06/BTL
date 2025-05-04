#include "main.h"
#include "hcmcampaign.h"

enum InfantryType { 
    SNIPER , ANTIAIRCRAFTSQUAD , MORTARSQUAD , ENGINEER ,
    SPECIALFORCES , REGULARINFANTRY 
};

class Infantry : public Unit{
    private:
        InfantryType infantryType;
    public:

        Infantry ( int quantity , int weight , const Position pos , InfantryType
            infantryType ) : Unit(quantity, weight, pos){
                this->infantryType = infantryType;
        }
        
        
        bool perfectNumber(int n)
        {
            int k = sqrt(n);
            if (k * k == n)
                return true;
            else 
                return false;
        }

        int personalNumber(int n)
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

        
    
        int getAttackScore(){
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

        string str(){
            return "Infantry[Infantry Type = " + to_string(infantryType) + ", "
                    + "quantity = "+ to_string(quantity) + ", "
                    + "weight = " + to_string(weight) + ", "
                    + "pos = " + pos.str() + "]";  
        }

        InfantryType getInfantryType() const{
            return infantryType;
        }

        int getQuantity() const{
            return quantity;
        }

        void setQuantity(int quantity){
            this->quantity = quantity;
        }


};
