/********************************************************************************* 
 * Keaton Shawhan, kshawhan
 * 2023 Spring CSE101 PA6
 * Arithmetic.cpp
 * Main executable file for BigInteger ADT
*********************************************************************************/

#include "BigInteger.h"
#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
    ifstream in;
    ofstream out;
    string line;

    // check command line for correct number of arguments
    if( argc != 3 ){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    // open files for reading and writing 
    in.open(argv[1]);
    if( !in.is_open() ){
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if( !out.is_open() ){
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }

    // initialize strings to read each line from the input file
    std::string first = "";
    std::string empty = "";
    std::string second = "";
    getline(in, first);
    getline(in, empty);
    getline(in, second);
    BigInteger A = BigInteger(first);
    BigInteger B = BigInteger(second);

    // prints for 𝐴, 𝐵, 𝐴+𝐵, 𝐴−𝐵, 𝐴−𝐴, 3𝐴−2𝐵, 𝐴𝐵, 𝐴^2, 𝐵^2, 9𝐴^4+16𝐵^5
    out << A << "\n" << endl;
    out << B << "\n" << endl;

    BigInteger sum;
    sum = A.add(B);
    out << sum << "\n" << endl;

    BigInteger diff;
    diff = A.sub(B);
    out << diff << "\n" << endl;

    BigInteger diff2;
    diff2 = A.sub(A);
    out << diff2 << "\n" << endl;

    BigInteger test1;
    test1 = A.mult(3);
    BigInteger test2;
    test2 = B.mult(2);
    BigInteger test3 = test1.sub(test2);
    out << test3 << "\n" << endl;

    BigInteger test4;
    test4 = A.mult(B);
    out << test4 << "\n" << endl;

    BigInteger test5;
    test5 = A.mult(A);
    out << test5 << "\n" << endl;

    BigInteger test6;
    test6 = B.mult(B);
    out << test6 << "\n" << endl;

    BigInteger test7;
    BigInteger test8 = A.mult(A);
    test8 = test8.mult(test8);
    BigInteger test9 = test8.mult(9);

    BigInteger test10;
    test10 = B.mult(16);
    BigInteger test11 = B.mult(B);
    test11 = test11.mult(test11);
    BigInteger test12 = test10.mult(test11);

    BigInteger test13 = test9.add(test12);

    out << test13 << "\n" << endl;

    // close files 
    in.close();
    out.close();

    return(EXIT_SUCCESS);
}
