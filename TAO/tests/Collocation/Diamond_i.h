// $Id$

#if !defined (TAO_DIAMOND_I_H)
#define TAO_DIAMOND_I_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DiamondS.h"

class Top_i : public POA_Diamond::Top
{
public:
  Top_i ();
  ~Top_i ();
  // Ctor and dtor.

  virtual CORBA::String shape (CORBA::Environment &);
  // Return the shape of this object (interface.)
};

class Left_i : public POA_Diamond::Left
{
public:
  Left_i ();
  ~Left_i ();
  // Ctor, dtor.

  virtual CORBA::String shape (CORBA::Environment &);
  // Return the shape of this object (interface.)

  virtual CORBA::String color (CORBA::Environment &);
  // Return the color of this object (interface.)
};

class Right_i : public POA_Diamond::Right
{
public:
  Right_i ();
  ~Right_i ();
  // Ctor, dtor.

  virtual CORBA::String shape (CORBA::Environment &);
  // Return the shape of this object (interface.)

  virtual CORBA::String color (CORBA::Environment &);
  // Return the color of this object (interface.)

  virtual CORBA::Long width (CORBA::Environment &);
  // Return the width of the stuff.
};

class Buttom_i : public POA_Diamond::Buttom
{
public:
  Buttom_i ();
  ~Buttom_i ();
  // Ctor, dtor.

  virtual CORBA::String shape (CORBA::Environment &);
  // Return the shape of this object (interface.)

  virtual CORBA::String color (CORBA::Environment &);
  // Return the color of this object (interface.)

  virtual CORBA::Long width (CORBA::Environment &);
  // Return the width of the stuff.

  virtual CORBA::String name (CORBA::Environment &);
  // Return the name of the object.
};
#endif /* TAO_DIAMOND_I_H */
