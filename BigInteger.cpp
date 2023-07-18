/********************************************************************************* 
 * Keaton Shawhan, kshawhan
 * 2023 Spring CSE101 PA6
 * BigInteger.cpp
 * Main file for BigInteger ADT
*********************************************************************************/

#include "BigInteger.h"
#include <cmath>
#include <cstdlib>
#include <string>

// initialize static variables
const int power = 9;
const long base = pow(10, power);

// Constructor that creates a new BigInteger in the zero state: 
// signum=0, digits=().
BigInteger::BigInteger(){
    signum = 0;
    digits = List();
}

// Constructor that creates a new BigInteger from the long value x.
BigInteger::BigInteger(long x){
    if (x == 0){
        signum = 0;
        digits = List();
    }
    else if (x > 0){
        signum = 1;
        digits = List();
        while (x > 0){
            digits.insertAfter(x % base);
            x = floor(x/base);
        }
    }
    else{
        signum = -1;
        x *= -1;
        digits = List();
        while (x > 0){
            digits.insertAfter(x % base);
            x = floor(x/base);
        }
    }
}

// helper function to remove the elements at the front of the list with a value of 0
void strip_zero(List& L){
    L.moveFront();
    while ((L.length() > 0) && (L.peekNext() == 0)){
        L.eraseAfter();
    }
}

// Constructor that creates a new BigInteger from the string s.
BigInteger::BigInteger(std::string s){
    if (s.length() == 0){
        throw std::invalid_argument("BigInteger: Constructor: empty string");
        exit(EXIT_FAILURE);
    }
    if (s.find_first_not_of("+-0123456789") != std::string::npos){
        throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        exit(EXIT_FAILURE);
    }
    if ((s.length() == 1) && (s[0] == '-' || s[0] == '+')){
        throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        exit(EXIT_FAILURE);
    }
    std::size_t found = s.find_first_of("-+");
    if ((found > 0) && (found < s.length())){
        throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        exit(EXIT_FAILURE);
    }
    int sign_offset = 0;
    if (s[0] == '-'){
        signum = -1;
        sign_offset = 1;
    }else{
        signum = 1;
    }
    if (s[0] == '+'){
        sign_offset = 1;
    }
    digits = List();
    std::string temp = "";
    for (int i = s.length()-1; i >= sign_offset; i--){
        temp.insert(0, &s[i], 1);
        if (temp.length() == power){
            digits.insertAfter(stol(temp));
            temp = "";
        }
    }
    if ((temp.length() > 0)){
        digits.insertAfter(stol(temp));
    }
    strip_zero(digits);
    if (digits.length() == 0){
        signum = 0;
    }
}

// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger& N){
    signum = N.signum;
    digits = List(N.digits);
}

// Returns -1, 1 or 0 according to whether this BigInteger is positive, 
// negative or 0, respectively.
int BigInteger::sign() const{
    return signum;
}

// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger& N) const{
    BigInteger passed = BigInteger(N);
    BigInteger original = BigInteger(*this);
    if (original.signum < passed.signum){
        return -1;
    }
    if (original.signum > passed.signum){
        return 1;
    }
    if (passed.digits.length() != original.digits.length()){
        if (original.signum == -1 && passed.signum == -1){
            if (original.digits.length() > N.digits.length()){
                return -1;
            }else if (original.digits.length() < N.digits.length()){
                return 1;
            }
        }else{
            if (original.digits.length() < N.digits.length()){
                return -1;
            }else if (original.digits.length() > N.digits.length()){
                return 1;
        }
        }
    }
    original.digits.moveFront();
    passed.digits.moveFront();
    while (original.digits.position() < original.digits.length()){
        if ((original.digits.peekNext()*original.signum) > (passed.digits.peekNext()*passed.signum)){
            return 1;
        }else if ((original.digits.peekNext()*original.signum) < (passed.digits.peekNext()*passed.signum)){
            return -1;
        }else{
            original.digits.moveNext();
            passed.digits.moveNext();
        }
    }
    return 0;
}

// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero(){
    digits.clear();
    signum = 0;
}

// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate(){
    signum *= -1;
}

// Changes the sign of each integer in List L. Used by sub().
void negateList(List& L){
    L.moveFront();
    while (L.position() < L.length()){
        L.setAfter((L.peekNext()*-1));
        L.moveNext();
    }
}

// Overwrites the state of S with A + sgn*B (considered as vectors).
// Used by both sum() and sub().
void sumList(List& S, List A, List B, int sgn){
    S.clear();
    List B_copy = List(B);
    List A_copy = List(A);
    A_copy.moveBack();
    B_copy.moveBack();
    while ((A_copy.position() > 0) && (B_copy.position() > 0)){
        S.insertAfter(A_copy.peekPrev() + (B_copy.peekPrev()*sgn));
        A_copy.movePrev();
        B_copy.movePrev();
    }
    while (A_copy.position() > 0){
        S.insertAfter(A_copy.peekPrev());
        A_copy.movePrev();
    }
    while (B_copy.position() > 0){
        S.insertAfter(B_copy.peekPrev()*sgn);
        B_copy.movePrev();
    }
}

// Performs carries from right to left (least to most significant
// digits), then returns the sign of the resulting integer. Used  
// by add(), sub() and mult().
int normalizeList(List& L){
    long carry = 0;
    strip_zero(L);
    if (L.length() == 0){
        return 0;
    }
    int sign;
    if (L.front() < 0){
        negateList(L);
        sign = -1;
    }else{
        sign = 1;
    }
    L.moveBack();
    while (L.position() > 0){
        L.setBefore(L.peekPrev()+carry);
        L.movePrev();
        long remainder = 0;
        if (L.peekNext() < 0){
            carry = ((L.peekNext()/base)-1);
            remainder = (L.peekNext()-((L.peekNext()/base)-1)*base);
        }else{
            carry = (L.peekNext()/base);
            remainder = (L.peekNext()-((L.peekNext()/base))*base);
        }
        L.setAfter(remainder);
    }
    if (carry != 0){
        L.insertBefore(carry);
    }
    strip_zero(L);
    return sign;
}

// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) const{
    BigInteger passed = BigInteger(N);
    BigInteger original = BigInteger(*this);
    BigInteger out = BigInteger();
    if (original.sign() == -1){
        negateList(original.digits);
    }
    if (passed.sign() == -1){
        sumList(out.digits, original.digits, passed.digits, -1);
    }else{
        sumList(out.digits, original.digits, passed.digits, 1);
    }
    int sign = normalizeList(out.digits);
    out.signum = sign;
    return out;
}

// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) const{
    BigInteger passed = BigInteger(N);
    BigInteger original = BigInteger(*this);
    BigInteger out = BigInteger();
    if (original.sign() == -1){
        negateList(original.digits);
    }
    if (passed.sign() == -1){
        sumList(out.digits, original.digits, passed.digits, 1);
    }else{
        sumList(out.digits, original.digits, passed.digits, -1);
    }
    int sign = normalizeList(out.digits);
    if (out.digits.length() > 0){
        normalizeList(out.digits);
    }
    out.signum = sign;
    //std::cout << out << std::endl;
    return out;
}

// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, ListElement m){
    L.moveBack();
    while (L.position() > 0){
        L.setBefore(L.peekPrev()*m);
        L.movePrev();
    }
}

// Returns a BigInteger representing the product of this and N.
BigInteger BigInteger::mult(const BigInteger& N) const{
    BigInteger out = BigInteger();
    if (sign() == 0 || N.sign() == 0){
        out.signum = 0;
        return out;
    }
    BigInteger original = BigInteger(*this);
    BigInteger passed = BigInteger(N);
    List original_L = original.digits;
    List passed_L = passed.digits;
    original_L.moveBack();
    passed_L.moveBack();
    long digits = 0;
    while (original_L.position() > 0){
        long current = original_L.peekPrev();
        original_L.movePrev();
        List hold = List(passed_L);
        if (current == 0){
            digits += 1;
            continue;
        }
        scalarMultList(hold, (current));
        hold.moveBack();
        for (int i = 0; i < digits; i++){
            hold.insertAfter(0);
        }
        normalizeList(hold);
        sumList(out.digits, out.digits, hold, 1);
        normalizeList(out.digits);
        digits += 1;
    }
    if ((passed.sign() == -1 && original.sign() == -1) || (original.sign() == 1 && passed.sign() == 1)){
        out.signum = 1;
    }
    else if ((passed.sign() == -1 && original.sign() == 1) || (original.sign() == -1 && passed.sign() == 1)){
        out.signum = -1;
    }else{
        out.signum = 0;
    }
    return out;
}

// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string() {
    BigInteger copy = BigInteger(*this);
    std::string total = "";
    copy.digits.moveBack();
    if (copy.digits.position() == 0){
        total += "0";
        return total;
    }
    while (copy.digits.position() > 1){
        long current = copy.digits.movePrev();
        std::string temp = std::to_string(current);
        while ((temp.length() != power)){
            temp = "0" + temp;
        }
        total = temp + total;
    }
    long current = copy.digits.movePrev();
    std::string temp = std::to_string(current);
    total = temp + total;
    if (copy.signum == -1){
        total = "-" + total;
    }
    return total;
}

// Inserts string representation of N into stream.
std::ostream& operator<<(std::ostream& stream, BigInteger N){
    return stream << N.to_string();
}

// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 0){
        return true;
    }else{
        return false;
    }
}

// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == -1){
        return true;
    }else{
        return false;
    }
}

// Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ){
    if ((A.compare(B) == -1) || (A.compare(B) == 0)){
        return true;
    }else{
        return false;
    }
}

// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ){
    if (A.compare(B) == 1){
        return true;
    }else{
        return false;
    }
}

// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ){
    if ((A.compare(B) == 1) || (A.compare(B) == 0)){
        return true;
    }else{
        return false;
    }
}

// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    BigInteger out = A.add(B);
    return out;
}

// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    A = A.add(B);
    return A;
}

// Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    BigInteger out = A.sub(B);
    return out;
}

// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    A = A.sub(B);
    return A;
}

// Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    BigInteger out = A.mult(B);
    return out;
}

// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    A = A.mult(B);
    return A;
}
