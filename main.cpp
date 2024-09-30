#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <string>
//FCM Project 1
//Jonathan Engle
// Due Oct 4th, 2023


//initializing our data as given
int beta=10;
int t = 4;
int emin=-8;
int emax=0;
//Generating our Floating point array, this allows us to to frame our floating point number into mantissa,
//exponent (base 10), the remainder and sign

struct Flarray {
    int m, e, remainder, s;
};


//Algorithm 1
//Round to nearest
//
Flarray roundToNearest(int& remainder, int& m, int& e) {
    if (remainder >= 5) {
        m += 1;
        if (m == pow(10, t)) {
            m = m / 10;
            e += 1;
        }
    }
    Flarray result;
    result.m = m;
    result.e = e;
    result.remainder = remainder;
    
    return result;
}

// Algorithm 2
//Translate
//
Flarray translate(float Xvec, int t) {
    Flarray result;
    int s = (Xvec >= 0) ? 1 : -1;
    float a = abs(Xvec);

    int m, e, remainder;
    if (a >= pow(10, t)) {
        m = static_cast<int>(floor(a));
        e = 0;
        while (m >= pow(10, t)) {
            remainder = m % 10;
            m = (m /10);
            e += 1;
        }
    }
    else {
        e = 0;
        while (a < pow(10, (t - 1))) {
            a = 10 * a;
            e -= 1;
        }
        remainder = static_cast<int>(floor(10 * (a - floor(a))));
        m = static_cast<int>(floor(a));
    }

    m = s * m;

    result.m = m;
    result.e = e;
    result.remainder = remainder;
    roundToNearest(remainder, m, e);
    result.s = s;
    return result;
}
//Algorithm 3
//Addition
Flarray Addition(int mX, int eX, int mY, int eY) {
    int a, b, c, m, e, s, remainder;
    int shift = eX - eY;
    if (shift > 0) {
        a = mX * pow(10, shift);
        b = mY;
        e = eY;
    }
    else {
        a = mY * pow(10, -shift);
        b = mX;
        e = eX;
    }
    c = a + b;
    s = (c >= 0) ? 1 : -1;
    c = abs(c);
    if (c >= pow(10, t)) {
        while (c >= pow(10, t)) {
            remainder = c % 10;
            c = c / 10;
            e += 1;
        }
    }
    else {
        while (c < pow(10, (t - 1))) {
            c *= 10;
            e -= 1;
        }
    }
    
    long long c_long = static_cast<long long>(c);
    m = static_cast<int>(c_long);
    roundToNearest(remainder, m, e);
    m = s * m;

    Flarray result;
    result.m = m;
    result.e = e;
    result.remainder = remainder;
    result.s = s;

    return result;
}

//Algorithm 4
//Accumulation

Flarray Accumulation(float x) {
    float s = x;
    int n=100;
    int k = 1;
    while (k < n) {
        s = s + x;
        k += 1;
        std::cout << "s:" << s<< std::endl;
    }
    Flarray result;
    result.s = s;
  
    return result;
}

//Putting it together with our driver/main
int main() {
    std::random_device random;
    std::mt19937 rng(random());
    
    //Uncomment to change for A vec,
    //This is to ensure that our sum will be within our Fl point range
    //int flmin=0.00001
    // int flmax=1
    
    //Uncomment for addition
    //  //int flmin=-4000
    // int flmax=4000
    
    //Uncomment for sum
    // int flmin=-150
    // int flmax=150
    
    
    //Note that this is where change occurs for each test
    std::uniform_real_distribution<float> distribution(-150, 150);
    int flmax=pow(beta,emax-t-1);
    int flmin=pow(beta,emax-t-1);
    
    //If running you may change data5 to represent your corresponding file
    const std::string data5 = "data5.csv";
    std::ofstream outputFile(data5);
    
    if(!outputFile) {
        std::cerr << "Error opening file: " << data5 << std::endl;
        return 1;
    }
    
    outputFile << "Xvec,Yvec,Mantisa_X,Mantisa_Y,Exponent_X,Exponent_Y,AdditionM,AdditionExp,AdittionRem,AdditionSgn,s" << std::endl;
    
    
    //Stays constant throughout
    int n=100;
    for (int i = 0; i < n; i++) {
        float Xvec = distribution(rng);
        float Yvec = distribution(rng);
        float Avec =distribution(rng);
        Flarray resultX = translate(Xvec, t);
        Flarray resultY = translate(Yvec, t);
        
        Flarray sumFl = Addition(resultX.m, resultX.e, resultY.m, resultY.e);
        
        Flarray acc = Accumulation(Xvec);
        
        std::cout << "Accumulation Result " << acc.m << std::endl;
        //std::cout << "s:" << result.s<< std::endl;
        
        //Computations about X
        std::cout << "Comp point " << i + 1 << ":" << std::endl;
        std::cout << "Random Number X: " << Xvec
        << " Mantissa_X: " << resultX.m
        << " Exponent_X: " << resultX.e
        << " Remainder_X: " << resultX.remainder
        << " Sign X: " << resultX.s << std::endl;
        //Computations about Y
        std::cout << "Random Number Y: " << Yvec
        << " Mantissa_Y: " << resultY.m
        << " Exponent_Y: " << resultY.e
        << " Remainder_Y: " << resultY.remainder
        << " Sign Y: " << resultY.s << std::endl;
        //Addition outputs
        std::cout << "Addition Result: Mantissa: " << sumFl.m
        << " Exponent: " << sumFl.e
        << " Remainder: " << sumFl.remainder
        << " Sign: " << sumFl.s << std::endl;
        
        std::cout << "---------------------------------------------" << std::endl;
        
        outputFile <<Xvec<< ","<< Yvec<<","<< resultX.m<<","<<resultY.m<<","<<resultX.e<<","<<resultY.e<<","<<sumFl.m<<","<<sumFl.e<<","<<sumFl.remainder<<","<<sumFl.s <<","<<acc.s <<std::endl;
        
    }
    outputFile.close();
    return 0;
}
