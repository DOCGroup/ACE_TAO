/**
 * @file compare_histo.cpp
 *
 * $Id$
 *
 */

#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <map>

typedef std::map<long,double> Vector;
typedef std::pair<long,double> Element;

std::ostream &
operator<< (std::ostream &os,
            const Vector::value_type &p)
{
  return os << p.first << " " << p.second;
}


void
load_file (Vector &vector,
           const char *filename)
{
  std::ifstream is (filename);
  if (!is)
    throw "Cannot open file";

  while (is && !is.eof ())
    {
      long i; double v;
      is >> i >> v;
      Vector::value_type e (i, v);
      vector.insert (e);
    }
}

int
main (int argc, char *argv[])
{
  Vector X;
  load_file (X, argv[1]);
  Vector Y;
  load_file (Y, argv[2]);

  Vector Z;
  for (Vector::iterator i = X.begin (); i != X.end (); ++i) {
    for (Vector::iterator j = Y.begin (); j != Y.end (); ++j) {
      long ki = (*i).first - (*j).first;
      double p = (*i).second * (*j).second;

      Vector::iterator k = Z.find (ki);
      if (k == Z.end ())
        Z.insert (Element (ki, p));
      else
        Z[ki] += p;
    }
  }

  for (Vector::iterator i = Z.begin (); i != Z.end (); ++i)
    {
      std::cout << (*i) << "\n";
    }

  return 0;
}
