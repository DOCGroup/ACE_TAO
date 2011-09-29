// $Id$

#ifndef __NAVUNIT_H
#define __NAVUNIT_H

#include "ace/SString.h"


class UnitLocation
{
public:
  UnitLocation() {x_ = y_ = 0.0;}
  UnitLocation(
    double x,
    double y) :
      x_(x), y_(y) {}

  UnitLocation(const UnitLocation &ul) { operator = (ul); }

public:
  UnitLocation & operator = (const UnitLocation &ul)
  {
    x_ = ul.x_;
    y_ = ul.y_;
    return *this;
  }

public:
  double x_, y_;
};

typedef unsigned long UnitID;

class NavUnit
{
public:
  NavUnit(const UnitID &id);
  NavUnit(const NavUnit&);
  virtual ~NavUnit() {}

public:
  UnitID getID() const {return id_;}

  UnitLocation getLocation() const {return loc_;}
  void setLocation(const UnitLocation &loc) {loc_ = loc;}

protected:
  UnitID id_;
  UnitLocation loc_;
};

#endif // __NAVUNIT_H
