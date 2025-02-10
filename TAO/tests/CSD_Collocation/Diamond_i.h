#if !defined (TAO_DIAMOND_I_H)
#define TAO_DIAMOND_I_H

#include "DiamondS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class Top_i : public POA_Diamond::Top
{
public:
  Top_i ();
  ~Top_i ();
  // Ctor and dtor.

  virtual char * shape ();
  // Return the shape of this object (interface.)
};

class Left_i : public POA_Diamond::Left
{
public:
  Left_i ();
  ~Left_i ();
  // Ctor, dtor.

  virtual char * shape ();
  // Return the shape of this object (interface.)

  virtual char * color ();
  // Return the color of this object (interface.)
};

class Right_i : public POA_Diamond::Right
{
public:
  Right_i ();
  ~Right_i ();
  // Ctor, dtor.

  virtual char * shape ();
  // Return the shape of this object (interface.)

  virtual char * color ();
  // Return the color of this object (interface.)

  virtual CORBA::Long width ();
  // Return the width of the stuff.
};

class Buttom_i : public POA_Diamond::Buttom
{
public:
  Buttom_i ();
  ~Buttom_i ();
  // Ctor, dtor.

  virtual char * shape ();
  // Return the shape of this object (interface.)

  virtual char * color ();
  // Return the color of this object (interface.)

  virtual CORBA::Long width ();
  // Return the width of the stuff.

  virtual char * name ();
  // Return the name of the object.
};

#endif /* TAO_DIAMOND_I_H */
