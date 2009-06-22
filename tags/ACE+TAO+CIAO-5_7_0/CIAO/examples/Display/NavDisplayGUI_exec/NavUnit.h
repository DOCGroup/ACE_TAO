// $Id$

#ifndef __NAVUNIT_H
#define __NAVUNIT_H

#include "ace/SString.h"


class UnitLocation
{
public:
  UnitLocation() {x_ = y_ = z_ = 0.0;}
  UnitLocation(
    double x,
    double y,
    double z) :
      x_(x), y_(y), z_(z) {}

  UnitLocation(const UnitLocation &ul) { operator = (ul); }

public:
  UnitLocation & operator = (const UnitLocation &ul)
  {
    x_ = ul.x_;
    y_ = ul.y_;
    z_ = ul.z_;
    return *this;
  }

public:
  double x_, y_, z_;
};

typedef unsigned long UnitID;

class NavUnit
{
public:
  NavUnit(const UnitID &id, const char *short_descr);
  virtual ~NavUnit() {}

public:
  UnitID getID() const {return id_;}

  const char *getShortDescr() const {return short_descr_.c_str();}
  void setShortDescr(const char *short_descr) {short_descr_ = short_descr;}

  UnitLocation getLocation() const {return loc_;}
  void setLocation(const UnitLocation &loc) {loc_ = loc;}

protected:
  UnitID id_;
  ACE_CString short_descr_;
  UnitLocation loc_;
};

#endif // __NAVUNIT_H
