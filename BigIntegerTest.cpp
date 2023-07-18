/********************************************************************************* 
 * Keaton Shawhan, kshawhan
 * 2023 Spring CSE101 PA6
 * BigIntegerTest.cpp
 * Main test file for BigInteger ADT
*********************************************************************************/
#include<iostream>
#include<string>
#include<stdexcept>
#include"BigInteger.h"

using namespace std;

int main(){

   string s1 = "123123123213123";
   string s3 = "-1231231231212213123123123454645645664565465463123";
   string s5 = "0";

   //BigInteger N;
   BigInteger A = BigInteger(s1);
   BigInteger B = BigInteger(s3);
   BigInteger C = BigInteger(s5);

   cout << endl;

   cout << "A = " << A << endl;
   cout << "B = " << B << endl;
   cout << "C = " << C << endl;
   cout << endl;

   BigInteger D = A+B;
   BigInteger E = B+A;
   BigInteger F = D-E;
   BigInteger G = 53234*A + 29384747*B + 928374*C;
   BigInteger H = A*B;
   BigInteger I = B*A;
   BigInteger J = G*H*I;


   cout << endl;

   A += B;
   B -= C;
   C *= J;
   cout << "A = " << A << endl;
   cout << "B = " << B << endl;
   cout << "C = " << C << endl;
   cout << endl;

   cout << A*B*C*D*E*G*H*I*J << endl << endl;

   cout << endl;

   string s2 = "-789706477239920309";
   string s4 = "+12398127389123798123781237918237129837219832137129832198379";


   BigInteger test1 = BigInteger(s2);
   BigInteger test2 = BigInteger(s4);

   test1 += test2;
   cout << "test1 = " << test1 << endl;
   test1 -= test1;
   cout << "test1 = " << test1 << endl;

   BigInteger zero = BigInteger();
   if (test1 == zero){
      cout << "Makezero() works" << endl;
   }

   test2 *= test2;
   cout << "test2^2 = " << test2 << endl;

   cout << "finished functional tests" << endl;



   // test exceptions
   try{
      BigInteger J = BigInteger("");
   }catch( std::invalid_argument& e ){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      BigInteger J = BigInteger("+");
   }catch( std::invalid_argument& e ){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      BigInteger J = BigInteger("12329837492387492837492$4982379487293847");
   }catch( std::invalid_argument& e ){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }
   try{
      BigInteger J = BigInteger("5298374902837409+82734098729287349827398");
   }catch( std::invalid_argument& e ){
      cout << e.what() << endl;
      cout << "   continuing without interruption" << endl;
   }

   cout << endl;

   return EXIT_SUCCESS;
}
