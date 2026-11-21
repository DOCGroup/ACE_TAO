#if !defined (TAO_DIAMOND_I_H)
#define TAO_DIAMOND_I_H

#include "DiamondS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Top_i : public POA_Diamond::Top
{
public:
  /// Constructor and destructor.
  Top_i ();
  ~Top_i () override;

  /// Returns the shape of this object.
  char * shape () override;
};

class Left_i : public POA_Diamond::Left
{
public:
  /// Constructor and destructor.
  Left_i ();
  ~Left_i () override;

  /// Returns the shape of this object.
  char * shape () override;

  /// Returns the color of this object.
  char * color () override;
};

class Right_i : public POA_Diamond::Right
{
public:
  /// Constructor and destructor.
  Right_i ();
  ~Right_i () override;

  /// Returns the shape of this object.
  char * shape () override;

  /// Returns the color of this object.
  virtual char * color ();

  /// Returns the width of the object.
  CORBA::Long width () override;
};

class Buttom_i : public POA_Diamond::Buttom
{
public:
  /// Constructor and destructor.
  Buttom_i ();
  ~Buttom_i () override;

  /// Returns the shape of this object.
  char * shape () override;

  /// Returns the color of this object.
  char * color () override;

  /// Returns the width of the object.
  CORBA::Long width () override;

  /// Returns the name of the object.
  char * name () override;
};

#endif /* TAO_DIAMOND_I_H */
