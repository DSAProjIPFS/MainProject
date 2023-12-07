#include <iostream>
#include <string>

// Function to perform bitwise left rotation
unsigned int leftRotate(unsigned int value, unsigned int count) {
    return (value << count) | (value >> (32 - count));
}

// SHA-1 algorithm implementation
long long int getHash(std::string& input) {
    // Initialize variables
    unsigned int h0 = 0x67452301;
    unsigned int h1 = 0xEFCDAB89;
    unsigned int h2 = 0x98BADCFE;
    unsigned int h3 = 0x10325476;
    unsigned int h4 = 0xC3D2E1F0;

    // Pre-processing
    unsigned long long totalBits = input.length() * 8;
    input += 0x80; // Append '1' bit to the message
    while ((input.length() + 8) % 64 != 0) {
        input += static_cast<char>(0x00); // Pad with '0' bits until message length is 64 bits fewer than a multiple of 512
    }
    input += (totalBits >> 56) & 0xFF;
    input += (totalBits >> 48) & 0xFF;
    input += (totalBits >> 40) & 0xFF;
    input += (totalBits >> 32) & 0xFF;
    input += (totalBits >> 24) & 0xFF;
    input += (totalBits >> 16) & 0xFF;
    input += (totalBits >> 8) & 0xFF;
    input += totalBits & 0xFF;

    // Process the message in 512-bit blocks
    for (size_t i = 0; i < input.length(); i += 64) {
        unsigned int w[80];

        // Break block into sixteen 32-bit big-endian words
        for (size_t j = 0; j < 16; ++j) {
            w[j] = (input[i + j * 4] << 24) |
                (input[i + j * 4 + 1] << 16) |
                (input[i + j * 4 + 2] << 8) |
                (input[i + j * 4 + 3]);
        }

        // Extend the sixteen 32-bit words into eighty 32-bit words
        for (size_t j = 16; j < 80; ++j) {
            w[j] = leftRotate(w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16], 1);
        }

        // Initialize hash values for this chunk
        unsigned int a = h0;
        unsigned int b = h1;
        unsigned int c = h2;
        unsigned int d = h3;
        unsigned int e = h4;

        // Main loop
        for (size_t j = 0; j < 80; ++j) {
            unsigned int f, k;

            if (j < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            }
            else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            unsigned int temp = leftRotate(a, 5) + f + e + k + w[j];
            e = d;
            d = c;
            c = leftRotate(b, 30);
            b = a;
            a = temp;
        }

        // Add this chunk's hash to the result so far
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // Combine the hash values into a single long long int
    long long int hash =
        (static_cast<long long int>(h0) << 32 * 4) |
        (static_cast<long long int>(h1) << 32 * 3) |
        (static_cast<long long int>(h2) << 32 * 2) |
        (static_cast<long long int>(h3) << 32) |
        static_cast<long long int>(h4);

    return hash;
}
