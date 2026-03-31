#include <iostream>
#include <stdexcept>

// Exception class
struct divided_by_zero final : std::exception {
public:
    divided_by_zero()   = default;
    ~divided_by_zero()  = default;
    const char *what() const noexcept override { return "Divided by zero!"; };
};

// GCD helper function
template <typename _Tp>
inline _Tp gcd(_Tp x, _Tp y) {
    while (y) {
        x %= y;
        std::swap(x, y);
    }
    return x;
}

// Absolute value helper
template <typename _Tp>
inline _Tp abs(_Tp x) {
    return x < _Tp(0) ? -x : x;
}

// Fraction template class
template <typename _Tp>
struct fraction {
private:
    _Tp numerator;
    _Tp denominator;

    // Reduce fraction to simplest form
    void reduce() {
        if (denominator == _Tp(0)) {
            throw divided_by_zero();
        }

        // Move negative sign to numerator
        if (denominator < _Tp(0)) {
            numerator = -numerator;
            denominator = -denominator;
        }

        // Handle zero case
        if (numerator == _Tp(0)) {
            denominator = _Tp(1);
            return;
        }

        // Calculate GCD and reduce
        _Tp g = gcd(abs(numerator), abs(denominator));
        numerator /= g;
        denominator /= g;
    }

public:
    // Default constructor: 0/1
    fraction() : numerator(_Tp(0)), denominator(_Tp(1)) {}

    // Constructor from single value: x/1
    fraction(_Tp x) : numerator(x), denominator(_Tp(1)) {}

    // Constructor from two values: x/y
    fraction(_Tp x, _Tp y) : numerator(x), denominator(y) {
        reduce();
    }

    // Addition
    fraction operator + (const fraction &other) const {
        _Tp new_num = numerator * other.denominator + other.numerator * denominator;
        _Tp new_den = denominator * other.denominator;
        return fraction(new_num, new_den);
    }

    // Subtraction
    fraction operator - (const fraction &other) const {
        _Tp new_num = numerator * other.denominator - other.numerator * denominator;
        _Tp new_den = denominator * other.denominator;
        return fraction(new_num, new_den);
    }

    // Multiplication
    fraction operator * (const fraction &other) const {
        _Tp new_num = numerator * other.numerator;
        _Tp new_den = denominator * other.denominator;
        return fraction(new_num, new_den);
    }

    // Division
    fraction operator / (const fraction &other) const {
        if (other.numerator == _Tp(0)) {
            throw divided_by_zero();
        }
        _Tp new_num = numerator * other.denominator;
        _Tp new_den = denominator * other.numerator;
        return fraction(new_num, new_den);
    }

    // Power (exponentiation)
    fraction operator ^ (long long exp) const {
        if (exp == 0) {
            return fraction(_Tp(1), _Tp(1));
        }

        if (exp < 0) {
            if (numerator == _Tp(0)) {
                throw divided_by_zero();
            }
            // Negative exponent: invert and use positive exponent
            fraction inverted(denominator, numerator);
            return inverted ^ (-exp);
        }

        // Fast exponentiation for positive exponents
        fraction result(_Tp(1), _Tp(1));
        fraction base = *this;

        while (exp > 0) {
            if (exp & 1) {
                result *= base;
            }
            base *= base;
            exp >>= 1;
        }

        return result;
    }

    // Compound assignment operators
    fraction &operator += (const fraction &other) {
        *this = *this + other;
        return *this;
    }

    fraction &operator -= (const fraction &other) {
        *this = *this - other;
        return *this;
    }

    fraction &operator *= (const fraction &other) {
        *this = *this * other;
        return *this;
    }

    fraction &operator /= (const fraction &other) {
        *this = *this / other;
        return *this;
    }

    fraction &operator ^= (long long exp) {
        *this = *this ^ exp;
        return *this;
    }

    // Conversion to double
    explicit operator double() const {
        return static_cast<double>(numerator) / static_cast<double>(denominator);
    }

    // Conversion to bool (false if zero, true otherwise)
    explicit operator bool() const {
        return numerator != _Tp(0);
    }

    // Equality comparison
    friend bool operator == (const fraction &a, const fraction &b) {
        return a.numerator == b.numerator && a.denominator == b.denominator;
    }

    // Less than comparison
    friend bool operator < (const fraction &a, const fraction &b) {
        // Compare a/b < c/d by checking a*d < c*b
        // Since denominators are always positive after reduce(), no sign adjustment needed
        return a.numerator * b.denominator < b.numerator * a.denominator;
    }

    // Output operator
    friend std::ostream & operator << (std::ostream &os, const fraction &f) {
        os << f.numerator << "/" << f.denominator;
        return os;
    }
};
