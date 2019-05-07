Justin gnatiuk | Algorithms | Project 1

> For this project, we were to utilize the BigInt and SHA256 Libraries to implement the RSA encryption algorithm

[Part 1]
rsa435.cc : contains the code to implement the generation of RSA variables P, Q, e, d, phi, n that are used for the
            algorithm.
            P and Q are two very large prime numbers, generated using fermats theorem to test for primes
            n is the product of P and Q
            phi is the product of (p-1)*(q-1)
            e is a number that is relatively prime to phi
            d is the multiplicative inverse of e mod phi
[Part 2]
sign.cpp : contains the driver code that utilizes the variables from part 1 to generate a hash signature to sign a file with
           as well as verify that a given file has not been modified in any way by decrypting the hash signature of a file.