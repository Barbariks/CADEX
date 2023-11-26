//
//  main.cpp
//  Test
//
//  Created by Andrew on 23.11.2023.
//

#include "Header.h"

vector<double> randomaizer(int size){
    vector<double>mass_chisl;
    double value;
    srand(time(0));
    for(int i = 0; i < size; i++){
        value = rand() % (int)pow(10, 5);
        value = -10 + (value / pow(10, 5)) * (10 - -10);
        mass_chisl.push_back(value);
    }
    return mass_chisl;
}


float Curcle(float a, float b, float x, float y, float r){
    float cons;
    cons = pow(x-a, 2)+pow(y-b, 2);
    if(cons == pow(r,2)){
        return 1;
    }
    else{return 0;}
}

int main(int argc, const char * argv[]) {
    constexpr size_t sz = 10;
    const double pi = std::acos(-1);
    vector<Geometry> arrayGeom;
    vector<double> arrayDot, ArrayCircle;
    arrayDot = randomaizer(5);
    Circle circle1(arrayDot[0], arrayDot[1], abs(arrayDot[2]));
    sleep(1);
    arrayDot = randomaizer(5);
    ArrayCircle = circle1.curve_vector(3);
    for (int i = 0; i < ArrayCircle.size(); i++) {
        cout << ArrayCircle[i] << endl;
    }
    arrayGeom.emplace_back(circle1);
    return 0;
}
