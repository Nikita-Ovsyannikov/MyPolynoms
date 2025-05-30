#include "polynoms.h"
#include <gtest.h>

TEST(MonomialTest, ConstructorAndProperties) {
    Monomial m1(3, 2, 1, 0);
    EXPECT_EQ(m1.coefficient, 3);
    EXPECT_EQ(m1.degrees.dx, 2);
    EXPECT_EQ(m1.degrees.dy, 1);
    EXPECT_EQ(m1.degrees.dz, 0);
    EXPECT_FALSE(m1.isZero());

    Monomial m_zero(0, 5, 5, 5);
    EXPECT_TRUE(m_zero.isZero());
    EXPECT_EQ(m_zero.coefficient, 0);
    EXPECT_EQ(m_zero.degrees.dx, 0);
    EXPECT_EQ(m_zero.degrees.dy, 0);
    EXPECT_EQ(m_zero.degrees.dz, 0);

    Monomial m_default;
    EXPECT_TRUE(m_default.isZero());
    EXPECT_EQ(m_default.coefficient, 0);
    EXPECT_EQ(m_default.degrees.dx, 0);

    EXPECT_THROW(Monomial(1, 10, 0, 0), std::out_of_range);
    EXPECT_THROW(Monomial(1, 0, -1, 0), std::out_of_range);
}

TEST(MonomialTest, HasSameDegrees) {
    Monomial m1(3, 2, 1, 0);
    Monomial m2(5, 2, 1, 0);
    Monomial m3(-1, 2, 0, 0);
    EXPECT_TRUE(m1.hasSameDegrees(m2));
    EXPECT_FALSE(m1.hasSameDegrees(m3));
}

TEST(MonomialTest, UnaryMinus) {
    Monomial m1(3, 2, 1, 0);
    Monomial m1_neg = -m1;
    EXPECT_EQ(m1_neg.coefficient, -3);
    EXPECT_EQ(m1_neg.degrees.dx, 2);
    EXPECT_EQ(m1_neg.degrees.dy, 1);
    EXPECT_EQ(m1_neg.degrees.dz, 0);

    Monomial m_zero(0, 1, 1, 1);
    EXPECT_EQ((-m_zero).coefficient, 0);
}

TEST(MonomialTest, Multiplication) {
    Monomial m1(3, 2, 1, 0);
    Monomial m2(2, 1, 1, 2);
    Monomial m_prod = m1 * m2;
    EXPECT_EQ(m_prod.coefficient, 6);
    EXPECT_EQ(m_prod.degrees.dx, 3);
    EXPECT_EQ(m_prod.degrees.dy, 2);
    EXPECT_EQ(m_prod.degrees.dz, 2);

    Monomial m_zero(0, 1, 1, 1);
    Monomial m_prod_with_zero = m1 * m_zero;
    EXPECT_TRUE(m_prod_with_zero.isZero());

    Monomial m_high_deg1(2, 8, 0, 0);
    Monomial m_high_deg2(3, 3, 0, 0);
    Monomial m_prod_overflow = m_high_deg1 * m_high_deg2;
    EXPECT_TRUE(m_prod_overflow.isZero());
    EXPECT_EQ(m_prod_overflow.coefficient, 0);

    Monomial m_deg_limit_y(1, 0, 9, 0);
    Monomial m_deg_1_y(1, 0, 1, 0);
    Monomial m_prod_overflow_y = m_deg_limit_y * m_deg_1_y;
    EXPECT_TRUE(m_prod_overflow_y.isZero());
}

TEST(MonomialTest, ToString) {
    EXPECT_EQ(Monomial(0, 0, 0, 0).toString(), "0");
    EXPECT_EQ(Monomial(5, 0, 0, 0).toString(), "5");
    EXPECT_EQ(Monomial(-3, 0, 0, 0).toString(), "-3");
    EXPECT_EQ(Monomial(1, 1, 0, 0).toString(), "x");
    EXPECT_EQ(Monomial(-1, 0, 1, 0).toString(), "-y");
    EXPECT_EQ(Monomial(2, 1, 0, 0).toString(), "2x");
    EXPECT_EQ(Monomial(1, 2, 0, 0).toString(), "x^2");
    EXPECT_EQ(Monomial(-4, 3, 2, 1).toString(), "-4x^3y^2z");
    EXPECT_EQ(Monomial(1, 0, 0, 0).toString(), "1");
    EXPECT_EQ(Monomial(-1, 0, 0, 0).toString(), "-1");
    EXPECT_EQ(Monomial(1, 0, 0, 5).toString(), "z^5");
}

TEST(MonomialTest, EqualityOperator) {
    Monomial m1(3, 1, 2, 3);
    Monomial m2(3, 1, 2, 3);
    Monomial m3(2, 1, 2, 3);
    Monomial m4(3, 0, 2, 3);
    Monomial m_zero1(0, 1, 1, 1);
    Monomial m_zero2(0, 5, 5, 5);

    EXPECT_EQ(m1, m2);
    EXPECT_NE(m1, m3);
    EXPECT_NE(m1, m4);
    EXPECT_EQ(m_zero1, m_zero2);
    EXPECT_NE(m1, m_zero1);
}

TEST(PolynomialTest, DefaultConstructor) {
    Polynomial p;
    EXPECT_TRUE(p.isZero());
    EXPECT_EQ(p.toString(), "0");
}

TEST(PolynomialTest, ConstructorFromMonomial) {
    Polynomial p1(Monomial(3, 1, 0, 0));
    EXPECT_FALSE(p1.isZero());
    EXPECT_EQ(p1.toString(), "3x");

    Polynomial p2(Monomial(0, 2, 2, 2));
    EXPECT_TRUE(p2.isZero());
    EXPECT_EQ(p2.toString(), "0");
}

TEST(PolynomialTest, ConstructorFromInitializerList) {
    Polynomial p({ Monomial(2, 1, 0, 0), Monomial(3, 0, 1, 0), Monomial(-2, 1, 0, 0) });
    EXPECT_EQ(p.toString(), "3y");

    Polynomial p_complex({ Monomial(5,2,1,0), Monomial(-3,0,0,1), Monomial(1,0,0,0) });
    EXPECT_EQ(p_complex.toString(), "5x^2y - 3z + 1");

    Polynomial p_empty({});
    EXPECT_TRUE(p_empty.isZero());
}

TEST(PolynomialTest, Addition) {
    Polynomial p1({ Monomial(3, 2, 0, 0), Monomial(2, 0, 1, 0) });
    Polynomial p2({ Monomial(-1, 2, 0, 0), Monomial(5, 0, 0, 1) });
    Polynomial p_sum = p1 + p2;
    EXPECT_EQ(p_sum.toString(), "2x^2 + 2y + 5z");

    Polynomial p3({ Monomial(1,1,0,0) });
    Polynomial p4({ Monomial(-1,1,0,0) });
    Polynomial p_sum_zero = p3 + p4;
    EXPECT_TRUE(p_sum_zero.isZero());
    EXPECT_EQ(p_sum_zero.toString(), "0");

    Polynomial p_zero;
    Polynomial p_sum_with_zero = p1 + p_zero;
    EXPECT_EQ(p_sum_with_zero.toString(), "3x^2 + 2y");
}

TEST(PolynomialTest, Subtraction) {
    Polynomial p1({ Monomial(3, 2, 0, 0), Monomial(2, 0, 1, 0) });
    Polynomial p2({ Monomial(1, 2, 0, 0), Monomial(-5, 0, 0, 1) });
    Polynomial p_diff = p1 - p2;
    EXPECT_EQ(p_diff.toString(), "2x^2 + 2y + 5z");

    Polynomial p3({ Monomial(1,1,0,0) });
    Polynomial p_diff_self = p3 - p3;
    EXPECT_TRUE(p_diff_self.isZero());

    Polynomial p_zero;
    Polynomial p_diff_from_zero = p_zero - p1;
    EXPECT_EQ(p_diff_from_zero.toString(), "-3x^2 - 2y");
}

TEST(PolynomialTest, UnaryMinusPolynomial) {
    Polynomial p1({ Monomial(3,2,0,0), Monomial(-2,0,1,0), Monomial(1,0,0,0) });
    Polynomial p_neg = -p1;
    EXPECT_EQ(p_neg.toString(), "-3x^2 + 2y - 1");

    Polynomial p_zero;
    EXPECT_EQ(-p_zero, p_zero);
}

TEST(PolynomialTest, Multiplication) {
    Polynomial p1({ Monomial(2, 1, 0, 0), Monomial(3, 0, 0, 0) });
    Polynomial p2({ Monomial(1, 1, 0, 0), Monomial(1, 0, 0, 0) });
    Polynomial p_prod = p1 * p2;
    EXPECT_EQ(p_prod.toString(), "2x^2 + 5x + 3");

    Polynomial p_const(Monomial(5, 0, 0, 0));
    Polynomial p_prod_by_const = p1 * p_const;
    EXPECT_EQ(p_prod_by_const.toString(), "10x + 15");

    Polynomial p_zero;
    Polynomial p_prod_with_zero = p1 * p_zero;
    EXPECT_TRUE(p_prod_with_zero.isZero());

    Polynomial px5({ Monomial(1,5,0,0) });
    Polynomial px6({ Monomial(1,6,0,0) });
    Polynomial p_prod_overflow = px5 * px6;
    EXPECT_TRUE(p_prod_overflow.isZero());
    EXPECT_EQ(p_prod_overflow.toString(), "0");

    Polynomial p_complex1({ Monomial(1,1,0,0), Monomial(1,0,0,0) });
    Polynomial p_complex2({ Monomial(1,1,0,0), Monomial(-1,0,0,0) });
    Polynomial p_prod_complex = p_complex1 * p_complex2;
    EXPECT_EQ(p_prod_complex.toString(), "x^2 - 1");
}

TEST(PolynomialTest, CombinedOperationsAndCanonicalForm) {
    Polynomial p1({ Monomial(1,1,0,0) });
    Polynomial p2({ Monomial(2,0,1,0) });
    Polynomial p3({ Monomial(3,0,0,1) });

    Polynomial res1 = p1 + p2;
    Polynomial res2 = p2 + p1;
    EXPECT_EQ(res1, res2);
    EXPECT_EQ(res1.toString(), "x + 2y");
    EXPECT_EQ(res2.toString(), "x + 2y");

    Polynomial term_a = (p1 + p2) * p3;
    Polynomial term_b = (p1 * p3) + (p2 * p3);
    EXPECT_EQ(term_a, term_b);
    EXPECT_EQ(term_a.toString(), "3xz + 6yz");
}

TEST(PolynomialTest, EqualityOperator) {
    Polynomial p1({ Monomial(3,1,0,0), Monomial(2,0,1,0) });
    Polynomial p2({ Monomial(2,0,1,0), Monomial(3,1,0,0) });
    Polynomial p3({ Monomial(3,1,0,0), Monomial(1,0,1,0) });
    Polynomial p_zero1;
    Polynomial p_zero2({});

    EXPECT_EQ(p1, p2);
    EXPECT_NE(p1, p3);
    EXPECT_EQ(p_zero1, p_zero2);
    EXPECT_NE(p1, p_zero1);
}

TEST(PolynomialTest, ComplexToString) {
    Polynomial p({ Monomial(1,2,0,0), Monomial(-3,1,1,0), Monomial(5,0,0,1), Monomial(-7,0,0,0) });
    EXPECT_EQ(p.toString(), "x^2 - 3xy + 5z - 7");

    Polynomial p_single_neg(Monomial(-5, 1, 0, 0));
    EXPECT_EQ(p_single_neg.toString(), "-5x");

    Polynomial p_leading_plus({ Monomial(1,0,0,0), Monomial(1,1,0,0) });
    EXPECT_EQ(p_leading_plus.toString(), "x + 1");
}

TEST(MonomialTest, MultiplicationWithConstants) {
    Monomial m1(3, 2, 1, 0);
    Monomial constant(5, 0, 0, 0);
    Monomial result = m1 * constant;
    EXPECT_EQ(result.coefficient, 15);
    EXPECT_EQ(result.degrees.dx, 2);
    EXPECT_EQ(result.degrees.dy, 1);
    EXPECT_EQ(result.degrees.dz, 0);

    Monomial m2(-2, 0, 0, 1);
    Monomial constant2(-3, 0, 0, 0);
    Monomial result2 = m2 * constant2;
    EXPECT_EQ(result2.coefficient, 6);
    EXPECT_EQ(result2.degrees.dx, 0);
    EXPECT_EQ(result2.degrees.dy, 0);
    EXPECT_EQ(result2.degrees.dz, 1);
}

TEST(MonomialTest, MaxDegreeMultiplication) {
    Monomial m1(2, 5, 4, 3);
    Monomial m2(3, 4, 5, 6);
    Monomial result = m1 * m2;
    EXPECT_EQ(result.coefficient, 6);
    EXPECT_EQ(result.degrees.dx, 9);
    EXPECT_EQ(result.degrees.dy, 9);
    EXPECT_EQ(result.degrees.dz, 9);

    Monomial m3(1, 9, 0, 0);
    Monomial m4(1, 1, 1, 0);
    Monomial result_overflow = m3 * m4;
    EXPECT_TRUE(result_overflow.isZero());

    Monomial m5(1, 8, 0, 0);
    Monomial m6(1, 1, 9, 0);
    Monomial result_ok = m5 * m6;
    EXPECT_EQ(result_ok.coefficient, 1);
    EXPECT_EQ(result_ok.degrees.dx, 9);
    EXPECT_EQ(result_ok.degrees.dy, 9);
    EXPECT_EQ(result_ok.degrees.dz, 0);
}

TEST(PolynomialTest, AddSubtractZeroPolynomial) {
    Polynomial p1({ Monomial(3,1,0,0), Monomial(-2,0,1,0) });
    Polynomial p_zero;

    Polynomial sum_with_zero = p1 + p_zero;
    EXPECT_EQ(sum_with_zero, p1);
    EXPECT_EQ(sum_with_zero.toString(), "3x - 2y");

    Polynomial sum_zero_with_p1 = p_zero + p1;
    EXPECT_EQ(sum_zero_with_p1, p1);
    EXPECT_EQ(sum_zero_with_p1.toString(), "3x - 2y");

    Polynomial diff_with_zero = p1 - p_zero;
    EXPECT_EQ(diff_with_zero, p1);
    EXPECT_EQ(diff_with_zero.toString(), "3x - 2y");

    Polynomial diff_zero_with_p1 = p_zero - p1;
    Polynomial expected_neg_p1 = -p1;
    EXPECT_EQ(diff_zero_with_p1, expected_neg_p1);
    EXPECT_EQ(diff_zero_with_p1.toString(), "-3x + 2y");
}

TEST(PolynomialTest, MultiplyByZeroPolynomial) {
    Polynomial p1({ Monomial(3,1,0,0), Monomial(-2,0,1,0) });
    Polynomial p_zero;

    Polynomial prod1 = p1 * p_zero;
    EXPECT_TRUE(prod1.isZero());
    EXPECT_EQ(prod1.toString(), "0");

    Polynomial prod2 = p_zero * p1;
    EXPECT_TRUE(prod2.isZero());
    EXPECT_EQ(prod2.toString(), "0");
}

TEST(PolynomialTest, MultiplyByConstantPolynomial) {
    Polynomial p1({ Monomial(2,2,0,0), Monomial(-3,1,0,0), Monomial(1,0,0,0) });
    Polynomial p_const(Monomial(5, 0, 0, 0));
    Polynomial expected({ Monomial(10,2,0,0), Monomial(-15,1,0,0), Monomial(5,0,0,0) });

    Polynomial result = p1 * p_const;
    EXPECT_EQ(result, expected);
    EXPECT_EQ(result.toString(), "10x^2 - 15x + 5");

    Polynomial p_const_neg(Monomial(-1, 0, 0, 0));
    Polynomial expected_neg({ Monomial(-2,2,0,0), Monomial(3,1,0,0), Monomial(-1,0,0,0) });
    Polynomial result_neg = p1 * p_const_neg;
    EXPECT_EQ(result_neg, expected_neg);
    EXPECT_EQ(result_neg.toString(), "-2x^2 + 3x - 1");

    Polynomial p_const_zero_monomial(Monomial(0, 5, 5, 5));
    Polynomial result_zero_const = p1 * p_const_zero_monomial;
    EXPECT_TRUE(result_zero_const.isZero());
}

TEST(PolynomialTest, SelfAssignmentOperators) {
    Polynomial p1({ Monomial(2,1,0,0), Monomial(3,0,0,0) });
    Polynomial p2({ Monomial(1,1,0,0), Monomial(1,0,0,0) });
    Polynomial p1_copy = p1;

    p1 += p2;
    Polynomial expected_sum({ Monomial(3,1,0,0), Monomial(4,0,0,0) });
    EXPECT_EQ(p1, expected_sum);

    p1 = p1_copy;
    p1 -= p2;
    Polynomial expected_diff({ Monomial(1,1,0,0), Monomial(2,0,0,0) });
    EXPECT_EQ(p1, expected_diff);

    p1 = p1_copy;
    p1 *= p2;
    Polynomial expected_prod({ Monomial(2,2,0,0), Monomial(5,1,0,0), Monomial(3,0,0,0) });
    EXPECT_EQ(p1, expected_prod);
}

TEST(PolynomialTest, ChainedOperations) {
    Polynomial p_x(Monomial(1, 1, 0, 0));
    Polynomial p_y(Monomial(1, 0, 1, 0));
    Polynomial p_z(Monomial(1, 0, 0, 1));
    Polynomial p_one(Monomial(1, 0, 0, 0));
    Polynomial p_two(Monomial(2, 0, 0, 0));

    Polynomial res = (p_x + p_y) * (p_x - p_y) + p_z * p_two - (p_one + p_one);
    Polynomial expected({
        Monomial(1,2,0,0),
        Monomial(-1,0,2,0),
        Monomial(2,0,0,1),
        Monomial(-2,0,0,0)
        });
    EXPECT_EQ(res, expected);
    EXPECT_EQ(res.toString(), "x^2 - y^2 + 2z - 2");
}

TEST(PolynomialTest, OperationsResultingInZeroPolynomial) {
    Polynomial p1({ Monomial(1,1,0,0), Monomial(1,0,0,0) });
    Polynomial p_neg_p1 = -p1;

    Polynomial sum_to_zero = p1 + p_neg_p1;
    EXPECT_TRUE(sum_to_zero.isZero());
    EXPECT_EQ(sum_to_zero.toString(), "0");

    Polynomial diff_to_zero = p1 - p1;
    EXPECT_TRUE(diff_to_zero.isZero());
    EXPECT_EQ(diff_to_zero.toString(), "0");

    Polynomial p_x8(Monomial(1, 8, 0, 0));
    Polynomial p_x2(Monomial(1, 2, 0, 0));
    Polynomial prod_overflow = p_x8 * p_x2;
    EXPECT_TRUE(prod_overflow.isZero());
    EXPECT_EQ(prod_overflow.toString(), "0");
}

TEST(PolynomialTest, ToStringEdgeCases) {
    Polynomial p_single_neg_term(Monomial(-5, 1, 2, 3));
    EXPECT_EQ(p_single_neg_term.toString(), "-5xy^2z^3");

    Polynomial p_single_x(Monomial(1, 1, 0, 0));
    EXPECT_EQ(p_single_x.toString(), "x");

    Polynomial p_single_one(Monomial(1, 0, 0, 0));
    EXPECT_EQ(p_single_one.toString(), "1");

    Polynomial p_single_neg_one(Monomial(-1, 0, 0, 0));
    EXPECT_EQ(p_single_neg_one.toString(), "-1");

    Polynomial p_neg_x_plus_one({ Monomial(-1,1,0,0), Monomial(1,0,0,0) });
    EXPECT_EQ(p_neg_x_plus_one.toString(), "-x + 1");

    Polynomial p_x_minus_one({ Monomial(1,1,0,0), Monomial(-1,0,0,0) });
    EXPECT_EQ(p_x_minus_one.toString(), "x - 1");

    Polynomial p_neg_x_minus_one({ Monomial(-1,1,0,0), Monomial(-1,0,0,0) });
    EXPECT_EQ(p_neg_x_minus_one.toString(), "-x - 1");

    Polynomial p_xyz_rev({ Monomial(1,0,0,2), Monomial(1,0,1,0), Monomial(1,1,0,0) });
    EXPECT_EQ(p_xyz_rev.toString(), "x + y + z^2");
}

TEST(PolynomialTest, VeryLargeDegreeMultiplicationResultingInZero) {
    Polynomial p_x9(Monomial(1, 9, 0, 0));
    Polynomial p_y9(Monomial(1, 0, 9, 0));
    Polynomial p_z9(Monomial(1, 0, 0, 9));
    Polynomial p_one(Monomial(1, 0, 0, 0));
    Polynomial p_x(Monomial(1, 1, 0, 0));

    Polynomial res1 = (p_x9 + p_one) * p_x;
    Polynomial expected1(Monomial(1, 1, 0, 0));
    EXPECT_EQ(res1, expected1);
    EXPECT_EQ(res1.toString(), "x");

    Polynomial p_max_deg_xyz({ Monomial(1,9,9,9) });
    Polynomial res2 = (p_max_deg_xyz + p_x) * p_x;
    Polynomial expected2(Monomial(1, 2, 0, 0));
    EXPECT_EQ(res2, expected2);
    EXPECT_EQ(res2.toString(), "x^2");

    Polynomial prod_x9_x9 = p_x9 * p_x9;
    EXPECT_TRUE(prod_x9_x9.isZero());
}

TEST(PolynomialTest, InitializerListWithDuplicatesAndZeros) {
    Polynomial p({
        Monomial(3, 1, 1, 0),
        Monomial(0, 5, 5, 5),
        Monomial(-2, 1, 1, 0),
        Monomial(5, 0, 0, 1)
        });
    Polynomial expected({ Monomial(1,1,1,0), Monomial(5,0,0,1) });
    EXPECT_EQ(p, expected);
    EXPECT_EQ(p.toString(), "xy + 5z");
}