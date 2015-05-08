#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>
using namespace std;

// Define namestring
const int maxlength = 8;
typedef string namestring; 

// Skipspaces functions
void skipspaces(ifstream *infp, char &curch, bool &eofile)
{
  while (!eofile)
  {
    if (!isspace(curch)) break;
    eofile = (infp->get(curch) == 0); // get next character
  }
}

// Check if char is digit
bool isDigit(char chr)
{
  if ((chr >= '0') && (chr <= '9'))
    return true;
  else
    return false;
}

// Getnumber function
void getnumber(ifstream *infp, char &curch, bool &eofile, int &number)
{
  number = 0; 
  while (!eofile)
  {
    if (isDigit(curch))
    {
      number = number*10 + (int)curch - 48;  
    }
    else break;
    eofile = (infp->get(curch) == 0);
  }
}

// Check if char is letter
bool isLetter(char chr)
{
  if (((chr >= 'a') && (chr <= 'z')) or ((chr >='A') && (chr <= 'Z')))
  {
    return true;
  }
  else return false;
}

// Getname function
void getname(ifstream *infp, char &curch, bool &eofile, namestring &str)
{
  str = "";
  bool start = false;
  while (!eofile)
  {
    if ((!start) && isLetter(curch))
    {
      start = true;
      str+=curch; 
    }
    else
    {
      if ((isLetter(curch)) or (isDigit(curch))) str+=curch;
      else break;
    }
    eofile = (infp->get(curch) == 0);
  }
}

int main(int argc, char **argv)
// If this program is put in a file called small.cc, it cannot
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

  eofile = (inf.get(ch) == 0);
  namestring name; 
  

  int number;
  while (!eofile)
  {
    if (isspace(ch))
    {
      skipspaces(&inf, ch, eofile);
    }
    else
    {
      if (isDigit(ch))
      {
        getnumber(&inf, ch, eofile, number);
        cout << number << endl;
      }
      if (isLetter(ch))
      {
        getname(&inf, ch, eofile, name);
        if (name.length() > maxlength)
        {
          cout << "Warning: name '" << name << "' was truncated!" << endl;
          cout << string(name.begin(), name.begin() + 8) << endl;
        }
        else cout << name << endl;
      }
    }
  }
  
  inf.close();
}


