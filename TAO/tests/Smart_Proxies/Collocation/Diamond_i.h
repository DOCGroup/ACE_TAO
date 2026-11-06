#if !defined (TAO_DIAMOND_I_H)
#define TAO_DIAMOND_I_H

#include "DiamondS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class Diamond_Export Top_i : public POA_Diamond::Top
{
public:
  Top_i ();
  ~Top_i ();
  // Ctor and dtor.

  char* shape () override;
  // Return the shape of this object (interface.)
};

class Diamond_Export Left_i : public POA_Diamond::Left
{
public:
  Left_i ();
  ~Left_i ();
  // Ctor, dtor.

  char * shape () override;
  // Return the shape of this object (interface.)

  char * color () override;
  // Return the color of this object (interface.)

  CORBA::Long combine (CORBA::Long first,
                       CORBA::Long &second,
                       CORBA::Long_out third) override;
};

class Diamond_Export Right_i : public POA_Diamond::Right
{
public:
  Right_i ();
  ~Right_i ();
  // Ctor, dtor.

  char * shape () override;
  // Return the shape of this object (interface.)

  virtual char * color ();
  // Return the color of this object (interface.)

  CORBA::Long width () override;
  // Return the width of the stuff.
};

class Diamond_Export Buttom_i : public POA_Diamond::Buttom
{
public:
  Buttom_i ();
  ~Buttom_i ();
  // Ctor, dtor.

  char * shape () override;
  // Return the shape of this object (interface.)

  char * color () override;
  // Return the color of this object (interface.)

  CORBA::Long combine (CORBA::Long first,
                       CORBA::Long &second,
                       CORBA::Long_out third) override;

  CORBA::Long width () override;
  // Return the width of the stuff.

  char * name () override;
  // Return the name of the object.
};

#endif /* TAO_DIAMOND_I_H */

