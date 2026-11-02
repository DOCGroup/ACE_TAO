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
  ~Top_i () override;

  Top_i (Top_i const &) = delete;
  Top_i (Top_i &&) = delete;
  Top_i &operator= (Top_i const &) = delete;
  Top_i &operator= (Top_i &&) = delete;

  char * shape () override;
  // Return the shape of this object (interface.)
};

class Diamond_Export Left_i : public POA_Diamond::Left
{
public:
  Left_i ();
  ~Left_i () override;

  Left_i (Left_i const &) = delete;
  Left_i (Left_i &&) = delete;
  Left_i &operator= (Left_i const &) = delete;
  Left_i &operator= (Left_i &&) = delete;

  char * shape () override;
  // Return the shape of this object (interface.)

  char * color () override;
  // Return the color of this object (interface.)
};

class Diamond_Export Right_i : public POA_Diamond::Right
{
public:
  Right_i ();
  ~Right_i () override;

  Right_i (Right_i const &) = delete;
  Right_i (Right_i &&) = delete;
  Right_i &operator= (Right_i const &) = delete;
  Right_i &operator= (Right_i &&) = delete;

  char * shape () override;
  // Return the shape of this object (interface.)

  CORBA::Long width () override;
  // Return the width of the stuff.
};

class Diamond_Export Buttom_i : public POA_Diamond::Buttom
{
public:
  Buttom_i ();
  ~Buttom_i () override;

  Buttom_i (Buttom_i const &) = delete;
  Buttom_i (Buttom_i &&) = delete;
  Buttom_i &operator= (Buttom_i const &) = delete;
  Buttom_i &operator= (Buttom_i &&) = delete;

  char * shape () override;
  // Return the shape of this object (interface.)

  char * color () override;
  // Return the color of this object (interface.)

  CORBA::Long width () override;
  // Return the width of the stuff.

  char * name () override;
  // Return the name of the object.
};

#endif /* TAO_DIAMOND_I_H */
