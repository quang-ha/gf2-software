#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;

int main(int argc, char **argv)
// If this program is put in a file called small.cc, it can
// be compiled with the command  g++ -g -o small small.cc
{
  ifstream inf; // input file
  char ch;      // current character
  bool eofile;  // true when end of file is reached

  if (argc != 2) {
    cout << "Usage:         " << argv[0] << " [filename]" << endl;
    exit(1);
  }

  inf.open(argv[1]);
  if (!inf) {
    cout << "Error: cannot open file " << argv[1] << " for reading" << endl;
    exit(1);
  }

  eofile = (inf.get(ch) == 0); // get first character

  while (!eofile) {
    if (isspace(ch)) cout << '.';
    else cout << ch;
    eofile = (inf.get(ch) == 0); // get next character
  }

  inf.close();
}
