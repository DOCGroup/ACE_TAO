/**
 * @file compare_histo.cpp
 */

#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <map>

#include "ace/OS_main.h"

using Vector = std::map<long, double>;
using Element = std::pair<long, double>;

std::ostream &
operator<< (std::ostream &os,
            Vector::value_type const &p)
{
  return os << p.first << " " << p.second;
}


void
load_file (Vector &vector,
           char const *filename)
{
  std::ifstream is (filename);
  if (!is)
    throw "Cannot open file";

  long i {};
  double v {};

  while (is >> i >> v)
    {
      Vector::value_type e (i, v);
      vector.insert (e);
    }
}

int
ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  Vector X;
  load_file (X, argv[1]);
  Vector Y;
  load_file (Y, argv[2]);

  Vector Z;
  for (Vector::value_type const &x : X)
    {
      for (Vector::value_type const &y : Y)
        {
          long const ki = x.first - y.first;
          double const p = x.second * y.second;

          Vector::iterator k = Z.find (ki);
          if (k == Z.end ())
            Z.insert (Element (ki, p));
          else
            Z[ki] += p;
        }
    }

  for (Vector::value_type const &entry : Z)
    {
      std::cout << entry << "\n";
    }

  return 0;
}
