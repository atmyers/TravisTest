/**
   @file
   @brief This defines some helper functions used to put Nyx's binary
   particle initial condition files into morton order. 
*/

#include <fstream>
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <sstream>

#include <stdint.h>

/**
   This function separates the bits in the input integer by 3 positions
 */
inline uint64_t split(unsigned int a) {
    uint64_t x = a & 0x1fffff;
    x = (x | x << 32) & 0x1f00000000ffff;
    x = (x | x << 16) & 0x1f0000ff0000ff;
    x = (x | x << 8)  & 0x100f00f00f00f00f;
    x = (x | x << 4)  & 0x10c30c30c30c30c3;
    x = (x | x << 2)  & 0x1249249249249249;
    return x;
}

/**
   Given 3 integers, this computes the morton index by interleaving
   the bits of x, y, and z. 
 */ 
inline uint64_t get_morton_index(unsigned int x,
                                 unsigned int y,
                                 unsigned int z) {
    uint64_t morton_index = 0;
    morton_index |= split(x) | ( split(y) << 1) | (split(z) << 2);
    return morton_index;
}

/**
   This defines the particle data type. Note that each particle has been
   assigned a unique identifier based on its morton index.
 */
struct Particle {
  float x;
  float y;
  float z;
  float m;
  float ux;
  float uy;
  float uz;
  uint64_t morton_id;
};

/**
   This struct is used to sort the particles by their morton id
 */
struct by_morton_id {
  bool operator()(const Particle& p1, const Particle& p2) {
    return p1.morton_id < p2.morton_id;
  }  
};

/**
   This is the header for one of Nyx's binary particle files.
 */
struct FileHeader {
  long NP;
  int  DM;
  int  NX;
};

/**
   A utility function for appending a number to a base string.
 */
std::string get_file_name(const std::string& base, int file_num) {
  std::stringstream ss;
  ss << base << file_num;
  return ss.str();
}
