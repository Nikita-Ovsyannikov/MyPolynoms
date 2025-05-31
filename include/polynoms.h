#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <stdexcept>

struct MonomialDegrees {
    int dx, dy, dz;

    MonomialDegrees(int x_deg = 0, int y_deg = 0, int z_deg = 0)
        : dx(x_deg), dy(y_deg), dz(z_deg) {
    }

    bool operator<(const MonomialDegrees& other) const {
        if (dx != other.dx) return dx < other.dx;
        if (dy != other.dy) return dy < other.dy;
        return dz < other.dz;
    }

    bool operator==(const MonomialDegrees& other) const {
        return dx == other.dx && dy == other.dy && dz == other.dz;
    }
};

class Monomial {
public:
    int coefficient;
    MonomialDegrees degrees;

    Monomial(int coeff = 0, int dx = 0, int dy = 0, int dz = 0)
        : coefficient(coeff) {
        if (dx < 0 || dx > 9 || dy < 0 || dy > 9 || dz < 0 || dz > 9) {
            throw std::out_of_range("Degree out of range (0-9)."); 
        }
        degrees = MonomialDegrees(dx, dy, dz);
        if (coeff == 0) {
            this->degrees = MonomialDegrees(0, 0, 0);
        }
    }

    bool isZero() const {
        return coefficient == 0;
    }

    bool hasSameDegrees(const Monomial& other) const {
        return degrees == other.degrees;
    }

    Monomial operator*(const Monomial& other) const {
        if (this->isZero() || other.isZero()) {
            return Monomial(0, 0, 0, 0);
        }

        int new_dx = degrees.dx + other.degrees.dx;
        int new_dy = degrees.dy + other.degrees.dy;
        int new_dz = degrees.dz + other.degrees.dz;

        if (new_dx > 9 || new_dy > 9 || new_dz > 9) {
            return Monomial(0, 0, 0, 0);
        }

        return Monomial(coefficient * other.coefficient, new_dx, new_dy, new_dz);
    }

    Monomial operator-() const {
        return Monomial(-coefficient, degrees.dx, degrees.dy, degrees.dz);
    }

    std::string toString() const {
        if (isZero()) return "0";

        std::stringstream ss;
        bool term_printed = false;

        if (coefficient == -1) {
            if (degrees.dx == 0 && degrees.dy == 0 && degrees.dz == 0) {
                ss << "-1";
                term_printed = true;
            }
            else {
                ss << "-";
            }
        }
        else if (coefficient == 1) {
            if (degrees.dx == 0 && degrees.dy == 0 && degrees.dz == 0) {
                ss << "1";
                term_printed = true;
            }
        }
        else {
            ss << coefficient;
            term_printed = true;
        }

        if (degrees.dx > 0) {
            ss << "x";
            if (degrees.dx > 1) ss << "^" << degrees.dx;
            term_printed = true;
        }
        if (degrees.dy > 0) {
            ss << "y";
            if (degrees.dy > 1) ss << "^" << degrees.dy;
            term_printed = true;
        }
        if (degrees.dz > 0) {
            ss << "z";
            if (degrees.dz > 1) ss << "^" << degrees.dz;
            term_printed = true;
        }

        if (!term_printed && coefficient == 1) {
            ss << "1";
        }

        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Monomial& m) {
        os << m.toString();
        return os;
    }

    bool operator==(const Monomial& other) const {
        if (coefficient == 0 && other.coefficient == 0) return true;
        return coefficient == other.coefficient && degrees == other.degrees;
    }

    bool operator!=(const Monomial& other) const {
        return !(*this == other);
    }
};

class Polynomial {
private:
    std::map<MonomialDegrees, int> terms;

    void addOrUpdateTerm(const Monomial& m) {
        if (m.isZero()) {
            return;
        }
        terms[m.degrees] += m.coefficient;
        if (terms[m.degrees] == 0) {
            terms.erase(m.degrees);
        }
    }

public:
    Polynomial() = default;

    Polynomial(const Monomial& m) {
        addOrUpdateTerm(m);
    }

    Polynomial(std::initializer_list<Monomial> m_list) {
        for (const auto& m : m_list) {
            addOrUpdateTerm(m);
        }
    }

    bool isZero() const {
        return terms.empty();
    }

    Polynomial& operator+=(const Polynomial& other) {
        for (const auto& pair : other.terms) {
            Monomial term_to_add(pair.second, pair.first.dx, pair.first.dy, pair.first.dz);
            addOrUpdateTerm(term_to_add);
        }
        return *this;
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result = *this;
        result += other;
        return result;
    }

    Polynomial operator-() const {
        Polynomial result;
        for (const auto& pair : terms) {
            Monomial negated_term(-pair.second, pair.first.dx, pair.first.dy, pair.first.dz);
            if (negated_term.coefficient != 0) {
                result.terms[negated_term.degrees] = negated_term.coefficient;
            }
        }
        return result;
    }

    Polynomial& operator-=(const Polynomial& other) {
        *this += (-other);
        return *this;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial result = *this;
        result -= other;
        return result;
    }

    Polynomial& operator*=(const Polynomial& other) {
        if (this->isZero() || other.isZero()) {
            terms.clear();
            return *this;
        }

        Polynomial result_poly;
        for (const auto& this_pair : this->terms) {
            Monomial m1(this_pair.second, this_pair.first.dx, this_pair.first.dy, this_pair.first.dz);
            for (const auto& other_pair : other.terms) {
                Monomial m2(other_pair.second, other_pair.first.dx, other_pair.first.dy, other_pair.first.dz);
                Monomial product_m = m1 * m2;
                if (!product_m.isZero()) {
                    result_poly.addOrUpdateTerm(product_m);
                }
            }
        }
        *this = result_poly;
        return *this;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial result = *this;
        result *= other;
        return result;
    }

    bool operator==(const Polynomial& other) const {
        return terms == other.terms;
    }

    bool operator!=(const Polynomial& other) const {
        return !(*this == other);
    }

    std::string toString() const {
        if (isZero()) {
            return "0";
        }
        std::stringstream ss;
        bool first_term = true;

        for (auto it = terms.crbegin(); it != terms.crend(); ++it) {
            const MonomialDegrees& deg = it->first;
            int coeff = it->second;
            Monomial m(coeff, deg.dx, deg.dy, deg.dz);

            if (!first_term) {
                if (m.coefficient > 0) {
                    ss << " + ";
                }
                else {
                    ss << " - ";
                    Monomial temp_m_for_print(-m.coefficient, m.degrees.dx, m.degrees.dy, m.degrees.dz);
                    ss << temp_m_for_print.toString();
                }
            }

            if (first_term || m.coefficient > 0) {
                ss << m.toString();
            }
            first_term = false;
        }
        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        os << p.toString();
        return os;
    }
};