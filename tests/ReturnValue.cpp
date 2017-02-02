// Description: Test class to check functions for return values
// Created by Jonas Knarbakk (09:42:32-02/02/2017)

#include "ReturnValue.hpp"

ReturnValue::ReturnValue(){
}

ReturnValue::~ReturnValue(){
}

bool ReturnValue::notNull(void * ptr){
    return ptr != nullptr;
}

bool ReturnValue::expectedFloat(float &value, float &expected){
    return value == expected;
}

bool ReturnValue::expectedInteger(int &value, int &expected){
    return value == expected;
}

bool ReturnValue::expectedDouble(double &value, double &expected){
    return value == expected;
}

bool ReturnValue::expectedBool(bool &value, bool &expected){
    return value == expected;
}

bool ReturnValue::expectedString(std::string &value, std::string &expected){
    return value.compare(expected) == 0;
}
