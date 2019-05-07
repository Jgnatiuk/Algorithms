// You need to complete this program for a part of your first project.

// Standard libraries
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

      srand(time(NULL));
      BigUnsigned e = 14504;

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
      std::cout <<"Prime P generated" << std::endl;

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
      std::cout << "Prime Q generated" << std::endl;

      BigUnsigned n = big1*big2;
      std::cout << "N generated ( N = PQ )" << std::endl;

      std::ofstream p_q("p_q.txt");
      p_q << big1 << "\n";
      p_q << big2 << "\n";
      p_q.close();
      std::cout << "FILE p_q.txt generated" << std::endl;

      BigUnsigned phi = (big1-1) * (big2-1);
      std::cout << "PHI generated ( PHI = (P-1)(Q-1) )" << std::endl;

      while(gcd(e, phi) != 1)
      {
          e += 1;
      }

      std::cout << "e is relatively prime to phi" << std::endl;

      BigUnsigned d = modinv(e, phi);

      std::cout << "d generated as multiplicative inverse of e mod phi" << std::endl;

      std::ofstream d_n("d_n.txt");
      d_n << d << "\n";
      d_n << n << "\n";
      d_n.close();

      std::cout << "FILE d_n.txt generated" << std::endl;

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
