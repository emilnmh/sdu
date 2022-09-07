#include <iostream>
#include <cmath>

class Fraction {
    int numerator = 0;
    int denominator = 0;

    int gcd(int x, int y) {
        if (x == 0) return y;
        return gcd(y % x, x);
    }

    int lcm(int x, int y) {
        return std::abs(x * y) / gcd(x, y);
    }

    void simplify() {
        int greatestCommonDivisor = gcd(numerator, denominator);
        numerator = numerator / greatestCommonDivisor;
        denominator = denominator / greatestCommonDivisor;
        correctSign();
    }

    void correctSign() {
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

public:
    Fraction(int numerator, int denominator) : numerator(numerator), denominator(denominator) {
        simplify();
    }
    Fraction() { }



    /**
     * Adds fraction and assigns new value.
     */
    Fraction &operator+=(const Fraction &other) {
        int newDenominator = gcd(denominator, other.denominator);
        newDenominator = (denominator * other.denominator) / newDenominator;
        numerator = numerator * (newDenominator / denominator) + other.numerator * (newDenominator / other.denominator);
        denominator = newDenominator;
        simplify();
        return *this;
    }


    /**
     * Adds integer and assigns new value.
     */
    Fraction &operator+=(const int &i) {
        numerator += i * denominator;
        return *this;
    }


    /**
     * Subtracts fraction and assigns new value.
     */
    Fraction &operator-=(const Fraction &other) {
        int newDenominator = gcd(denominator, other.denominator);
        newDenominator = (denominator * other.denominator) / newDenominator;
        numerator = numerator * (newDenominator / denominator) - other.numerator * (newDenominator / other.denominator);
        denominator = newDenominator;
        simplify();
        return *this;
    }


    /**
     * Subtracts integer and assigns new value.
     */
    Fraction &operator-=(const int &i) {
        return *this += -i;
    }


    /**
     * Multiplies fraction and assigns new value.
     */
    Fraction &operator*=(const Fraction &other) {
        numerator *= other.numerator;
        denominator *= other.denominator;
        simplify();
        return *this;
    }


    /**
     * Multiplies integer and assigns new value.
     */
    Fraction &operator*=(const int &i) {
        numerator *= i;
        simplify();
        return *this;
    }


    /**
     * Divides fraction and assigns new value.
     */
    Fraction &operator/=(const Fraction &other) {
        numerator *= other.denominator;
        denominator *= other.numerator;
        simplify();
        return *this;
    }


    /**
     * Divides integer and assigns new value.
     */
    Fraction &operator/=(const int &i) {
        denominator *= i;
        simplify();
        return *this;
    } 


    /**
     * Compare equal with other fraction.
     */ 
    bool operator==(const Fraction &other) const {
        return numerator == other.numerator && denominator == other.numerator;
    }   


    /**
     * Compare not equal with other.
     */ 
    bool operator!=(const Fraction &other) const {
        return !(*this == other);
    }


    /**
     * Compare less than with other.
     */ 
    bool operator<(const Fraction &other) const {
        return (numerator * other.denominator) < (other.numerator * denominator);
    }


    /**
     * Compare less than or equal with other.
     */
    bool operator<=(const Fraction &other) const {
        return *this == other || *this < other;
    }


    /**
     * Compare greater than with other.
     */ 
    bool operator>(const Fraction &other) const {
        return (numerator * other.denominator) > (other.numerator * denominator); 
    }


    /**
     * Compare greater than or equal with other.
     */ 
    bool operator>=(const Fraction &other) const {
        return *this == other || *this > other;
    }


    /**
     * Increment by 1 and return.
     */ 
    Fraction &operator++() {
        numerator += denominator;
        simplify();
        return *this;
    }


    /**
     * Return and then increment by 1.
     */ 
    Fraction operator++(int) {
        Fraction f = *this;
        numerator += denominator;
        simplify();
        return f;
    }


    /**
     * Decrement by 1 and return.
     */
    Fraction &operator--() {
        numerator -= denominator;
        simplify();
        return *this;
    }


    /**
     * Return and then decrement by 1.
     */
    Fraction operator--(int) {
        Fraction f = *this;
        numerator += denominator;
        simplify();
        return f;
    }


    /**
     * Returns the numerator of the fraction.
     */
    int getNumerator() const {
        return numerator;
    }


    /**
     * Returns the denominator of the fraction.
     */
    int getDenominator() const {
        return denominator;
    }

};


/**
 * Adds fraction to another fraction.
 */
Fraction operator+(const Fraction &f1, const Fraction &f2) {
    Fraction result = f1;
    return result += f2;
}


/**
 * Adding fraction and integer.
 */
Fraction operator+(const Fraction &f, const int &i) {
    Fraction result = f;
    return result += i;
}


/**
 * Adding integer and fraction.
 */
Fraction operator+(const int &i, const Fraction &f) {
    Fraction result = f;
    return result += i;
}


/**
 * Unary subtraction.
 */
Fraction operator-(const Fraction &f) {
    return Fraction(-f.getNumerator(), f.getDenominator());
}


/**
 * Subtracts integer from fraction.
 */
Fraction operator-(const Fraction &f, const int &i) {
    Fraction result = f;
    return result -= i;
}


/**
 * Subtracts fraction from integer.
 */
Fraction operator-(const int &i, const Fraction &f) {
    Fraction result = f;
    return -(result -= i);
}


/**
 * Multiplies fraction and fraction.
 */
Fraction operator*(const Fraction &f1, const Fraction &f2) {
    Fraction result = f1;
    return result *= f2;
}


/**
 * Multiplies fraction and integer.
 */
Fraction operator*(const Fraction &f, const int &i) {
    Fraction result = f;
    return result *= i;
}


/**
 * Multiplies integer and fraction.
 */
Fraction operator*(const int &i, const Fraction &f) {
    Fraction result = f;
    return result *= i;
}


/**
 * Divides fraction and fraction.
 */
Fraction operator/(const Fraction &f1, const Fraction &f2) {
    Fraction result = f1;
    return result /= f2;
}


/**
 * Divides fraction and integer.
 */
Fraction operator/(Fraction f, int i) {
    return f /= i;
}


/**
 * Divides integer and fraction.
 */
Fraction operator/(const int &i, const Fraction &f) {
    Fraction result = f;
    return result *= i;
}


/**
 * Compare equal between fraction and integer.
 */ 
bool operator==(const Fraction &f, const int &i) {
    return f.getNumerator() / f.getDenominator() == i && f.getNumerator() % f.getDenominator() == 0;
}


/**
 * Compare equal between integer and fraction.
 */ 
bool operator==(const int &i, const Fraction &f) {
    return f == i;
}


/**
 * Compare not equal between fraction and integer.
 */
bool operator!=(const Fraction &f, const int &i) {
    return !(f == i);
}


/**
 * Compare not equal between integer and fraction.
 */ 
bool operator!=(const int &i, const Fraction &f) {
    return !(f == i);
}


/**
 * Compare less than between fraction and integer.
 */
bool operator<(const Fraction &f, const int &i) {
    const int result = f.getNumerator() / f.getDenominator();
    const int remainder = f.getNumerator() % f.getDenominator();
    return result < i || (result == i && remainder != 0);
}


/**
 * Compare less than between integer and fraction.
 */ 
bool operator<(const int &i, const Fraction &f) {
    const int result = f.getNumerator() / f.getDenominator();
    const int remainder = f.getNumerator() % f.getDenominator();
    return i < result || (result == i && remainder != 0);
}


/**
 * Compare less than or equal between fraction and integer.
 */ 
bool operator<=(const Fraction &f, const int &i) {
    return f == i || f < i;
}


/**
 * Compare less than or equal between integer and fraction.
 */ 
bool operator<=(const int &i, const Fraction &f) {
    return f == i || i < f;
}


/**
 * Compare greater than between fraction and integer.
 */
bool operator>(const Fraction &f, const int &i) {
    return i < f;
}


/**
 * Compare greater than between integer and fraction.
 */ 
bool operator>(const int &i, const Fraction &f) {
    return f < i;
}


/**
 * Compare greater than or equal between fraction and integer.
 */ 
bool operator>=(const Fraction &f, const int &i) {
    return i <= f;
}


/**
 * Compare greater than or equal between integer and fraction.
 */ 
bool operator>=(const int &i, const Fraction &f) {
    return f <= i;
}


/**
 * Returns ostream of the textural representation of the fraction.
 */
std::ostream &operator<<(std::ostream &s, const Fraction &f) {
    return s << f.getNumerator() << "/" << f.getDenominator();
}

int main() {
    Fraction f1(7, 5);
    Fraction f2(1, 4);
    Fraction f3(1, 3);
    Fraction f4(1, 2);

    std::cout << f1++ << ", " << f1 << std::endl;
}