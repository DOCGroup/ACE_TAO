#include <algorithm>

#include "ArgPair.h"

ArgPair::ArgPair (int c, char **v)
  : argc (c),
    argv (new char *[c])
{
  std::copy (v, v + c, this->argv);
}

ArgPair::ArgPair (const ArgPair &ap)
  : argc (ap.argc),
    argv (new char *[ap.argc])
{
  std::copy (ap.argv, ap.argv + ap.argc, this->argv);
}
  
ArgPair & ArgPair::operator = (const ArgPair &ap)
{
  if (this != &ap)
  {
    ArgPair temp (ap);
    this->swap (temp);
  }
  return *this;
}

void ArgPair::swap (ArgPair &ap)
{
  std::swap (this->argc, ap.argc);
  std::swap (this->argv, ap.argv);
}

ArgPair::~ArgPair()
{
  delete [] argv;
}

