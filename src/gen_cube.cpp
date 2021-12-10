#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>

void gen_cube(unsigned side, std::ostream &out) {
  out << side * side * side << std::endl;
  auto num = [&side] (unsigned i, unsigned j, unsigned k) {return i * side * side + j * side + k; };
  for(int i=0; i < side; ++i) {
    for(int j=0; j < side; ++j) {
      for(int k=0; k < side; ++k) {
        unsigned degree = (i+1 < side) + (j+1 < side) + (k+1 < side) + (i-1 >= 0) + (j-1 >= 0) + (k-1 >= 0);
        out << degree;
        if(i+1 < side) {
          out << " " << num(i+1, j, k);
        }
        if(j+1 < side) {
          out  << " " << num(i, j+1, k);
        }
        if(k+1 < side) {
          out << " " << num(i, j, k+1);
        }
        if(i-1 >= 0) {
          out << " " << num(i-1, j, k);
        }
        if(j-1 >= 0) {
          out  << " " << num(i, j-1, k);
        }
        if(k-1 >= 0) {
          out << " " << num(i, j, k-1);
        }
        out << std::endl;
      }
    }
  }
}

int main(int argc, const char **argv) {
  unsigned side = atoi(argv[1]);
  std::string filename(argv[2]);
  std::ofstream out(filename);
  gen_cube(side, out);
}
