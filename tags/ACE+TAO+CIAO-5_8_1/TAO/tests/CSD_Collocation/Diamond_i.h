// $Id$

#if !defined (TAO_DIAMOND_I_H)
#define TAO_DIAMOND_I_H

#include "DiamondS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class Top_i : public POA_Diamond::Top
{
public:
  Top_i (void);
  ~Top_i (void);
  // Ctor and dtor.

  virtual char * shape (void);
  // Return the shape of this object (interface.)
};

class Left_i : public POA_Diamond::Left
{
public:
  Left_i (void);
  ~Left_i (void);
  // Ctor, dtor.

  virtual char * shape (void);
  // Return the shape of this object (interface.)

  virtual char * color (void);
  // Return the color of this object (interface.)
};

class Right_i : public POA_Diamond::Right
{
public:
  Right_i (void);
  ~Right_i (void);
  // Ctor, dtor.

  virtual char * shape (void);
  // Return the shape of this object (interface.)

  virtual char * color (void);
  // Return the color of this object (interface.)

  virtual CORBA::Long width (void);
  // Return the width of the stuff.
};

class Buttom_i : public POA_Diamond::Buttom
{
public:
  Buttom_i (void);
  ~Buttom_i (void);
  // Ctor, dtor.

  virtual char * shape (void);
  // Return the shape of this object (interface.)

  virtual char * color (void);
  // Return the color of this object (interface.)

  virtual CORBA::Long width (void);
  // Return the width of the stuff.

  virtual char * name (void);
  // Return the name of the object.
};

#endif /* TAO_DIAMOND_I_H */
