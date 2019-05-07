/* Justin Gnatiuk | Algorithms | Project 1 Part 1
 * > Generates two large prime numbers p and q, multiplies them to produce n,
 * > generates phi which is equal to (p-1)(q-1) as well as e and d which is the
 * > mod inverse of phi.
 * > All these values are used for the RSA encryption algorithm
 */

#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <ctime>


// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

int main(){
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
	try {

      // Random number seed
      srand(time(NULL));
      // Declaration of e
      BigUnsigned e = 14504;

      // Generates a large prime number per the bigint library, continues to generate until number given is prime
      std::cout << "Generating P" << std::endl;
      BigUnsigned big1 = BigUnsigned(4);
      while(modexp(2, big1-1, big1) != 1)
      {
         big1 = BigUnsigned(4);
         for (int i=0;i<100;i++)
         {
            big1 = big1*10 +rand();
         }
      }
      if(big1 % 2 == 0) big1++;

      std::cout <<"Prime P generated" << std::endl;

      // Generates a large prime number per the bigint library, continues to generate until number given is prime
      std::cout << "Generating Q" << std::endl;
      BigUnsigned big2 = BigUnsigned(4);
      while(modexp(2, big2-1, big2) != 1)
      {
         big2 = BigUnsigned(4);
         for (int i=0;i<100;i++)
         {
            big2 = big2*10 +rand();
         }
      }
      if(big2 % 2 == 0) big2++;
      std::cout << "Prime Q generated" << std::endl;

      // Multiply the two prime numbers to get N
      BigUnsigned n = big1*big2;
      std::cout << "N generated ( N = PQ )" << std::endl;

      // Output p and q to .txt file
      std::ofstream p_q("p_q.txt");
      p_q << big1 << "\n";
      p_q << big2 << "\n";
      p_q.close();
      std::cout << "FILE p_q.txt generated" << std::endl;

      // Generate phi
      BigUnsigned phi = (big1-1) * (big2-1);
      std::cout << "PHI generated ( PHI = (P-1)(Q-1) )" << std::endl;

      // If e is not relatively prime to phi, increment e
      while(gcd(e, phi) != 1)
      {
          e += 1;
      }
      if(e%2 == 0)
      {
          e++;
      }

      std::cout << "e is relatively prime to phi" << std::endl;

      // Generate d
      BigUnsigned d = modinv(e, phi);

      std::cout << "d generated as multiplicative inverse of e mod phi" << std::endl;

      // output d and n to .txt file
      std::ofstream d_n("d_n.txt");
      d_n << d << "\n";
      d_n << n << "\n";
      d_n.close();

      std::cout << "FILE d_n.txt generated" << std::endl;

      // output e and n to .txt file
      std::ofstream e_n("e_n.txt");
      e_n << e << "\n";
      e_n << n << "\n";
      e_n.close();

      std::cout << "FILE e_n.txt generated" << std::endl;

      std::cout << "RSA Complete" << std::endl;


	} catch(char const* err) {
		std::cout << "The library threw an exception:\n"
			<< err << std::endl;
	}

	return 0;
}
