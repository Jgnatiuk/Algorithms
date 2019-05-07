/* Justin Gnatiuk | Algorithms | Project 1 Part 2
 * > Generates an encrypted hash value for a file and verifies the signature to make sure
 * > that the file was not modified in any way
 * > Takes encryption values from p_q.txt , e_n.txt, & d_n.txt
 */


#include "BigIntegerLibrary.hh"
#include "sha256.h"
#include <fstream>
#include <iostream>
#include <string.h>


// Function to hash the char array and return the SHA256 hash
// in a BigUnsigned data type
BigUnsigned hashMemblock(char memblock[]) {
  // convert to string and hash using sha256
  std::string memblockString(memblock);
  std::string hashedFileContent = sha256(memblockString);

  // turn hashed string into BigUnsigned data type
  BigUnsignedInABase hashedFileContentNum =
      BigUnsignedInABase(hashedFileContent, 16);
  return BigUnsigned(hashedFileContentNum);
}

int main(int argc, char *argv[]) {
  if ((argv[1][0] != 's' && argv[1][0] != 'v'))
  {
    std::cout << "wrong format!" << std::endl;
    std::cout << "Format should be ./sign [v or s] [filename]" << std::endl;
  }
    else {

    // Read in file name to be signed or verified
    std::string filename = argv[1];

    // Read the file
    std::streampos begin, end;
    std::ifstream infile(filename.c_str(), std::ios::binary);
    begin = infile.tellg();
    infile.seekg(0, std::ios::end);
    end = infile.tellg();
    std::streampos size = end - begin;
    std::cout << "size of the file: " << size << " bytes.\n";

    infile.seekg(0, std::ios::beg);
    char *memblock = new char[size];
    infile.read(memblock,
                size); // read file; it's saved in the char array memblock
    infile.close();

    // hash file.txt contents
    BigUnsigned hash = hashMemblock(memblock);
    std::cout << "Hashed contents from file.txt: " << hash << std::endl;

    if (argv[1][0] == 's') {
      std::cout << "Signing " << filename << "..." << std::endl;

      // load from d_n.txt
      std::ifstream d_nfile("d_n.txt");
      std::string dString, nString;
      if(d_nfile.good()) {

          d_nfile >> dString;
          d_nfile >> nString;
      }
      d_nfile.close();

      BigUnsigned d = stringToBigUnsigned(dString);
      BigUnsigned n = stringToBigUnsigned(nString);

      // sign the hashed contents
      BigUnsigned signedContents = modexp(hash, d, n);
      std::cout << "Signed contents: " << signedContents << std::endl;

      // save contents to file.txt.signature
      std::ofstream outfile;
      outfile.open("file.txt.signature",
                   std::ofstream::out | std::ofstream::trunc);
      outfile << signedContents;
      outfile.close();

      std::cout << filename << " signed and saved to \"file.txt.signature\"!"
                << std::endl;

    } else {
      std::cout << "Verifying the signed file..." << std::endl;

      // read contents of signature file
      std::string signaturefilename = argv[2];
      std::ifstream sigfile(signaturefilename.c_str());
      std::string signatureString;
      if(sigfile.good())
      {
          sigfile >> signatureString;
      }
      sigfile.close();

      BigUnsigned signature = stringToBigUnsigned(signatureString);
      std::cout << "Signature from file.txt.signature: " << signature << std::endl;


      // read contents from e_n.txt
      std::ifstream e_nfile("e_n.txt");
      std::string eString, nString;
      if(e_nfile.good()) {

          e_nfile >> eString;
          e_nfile >> nString;
      }
      e_nfile.close();

      BigUnsigned e = stringToBigUnsigned(eString);
      BigUnsigned n = stringToBigUnsigned(nString);

      // "encrypt" signature gotten from file with e and n
      BigUnsigned encryptedSignature = modexp(signature, e, n);
      std::cout << "Encrypted signature: " << encryptedSignature << std::endl;

      // compare hash with encrypted signatureString
      if (encryptedSignature == hash)
        std::cout << "Verification passed!"
                  << std::endl;
      else
        std::cout << "Verification failed!"
                  << std::endl;

    }
    delete[] memblock;
  }
  return 0;
}
