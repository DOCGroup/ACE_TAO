#if !defined (TAO_DIAMOND_I_H)
#define TAO_DIAMOND_I_H

#include "DiamondS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Diamond_Export Top_i : public POA_Diamond::Top
{
public:
  Top_i () = default;
  ~Top_i () override = default;

  Top_i (Top_i const &) = delete;
  Top_i (Top_i &&) = delete;
  Top_i &operator= (Top_i const &) = delete;
  Top_i &operator= (Top_i &&) = delete;

  /// Returns the shape of this object.
  char * shape () override;
};

class Diamond_Export Left_i : public POA_Diamond::Left
{
public:
  Left_i () = default;
  ~Left_i () override = default;

  Left_i (Left_i const &) = delete;
  Left_i (Left_i &&) = delete;
  Left_i &operator= (Left_i const &) = delete;
  Left_i &operator= (Left_i &&) = delete;

  /// Returns the shape of this object.
  char * shape () override;

  /// Returns the color of this object.
  char * color () override;
};

class Diamond_Export Right_i : public POA_Diamond::Right
{
public:
  Right_i () = default;
  ~Right_i () override = default;

  Right_i (Right_i const &) = delete;
  Right_i (Right_i &&) = delete;
  Right_i &operator= (Right_i const &) = delete;
  Right_i &operator= (Right_i &&) = delete;

  /// Returns the shape of this object.
  char * shape () override;

  /// Returns the width of the object.
  CORBA::Long width () override;
};

class Diamond_Export Buttom_i : public POA_Diamond::Buttom
{
public:
  Buttom_i () = default;
  ~Buttom_i () override = default;

  Buttom_i (Buttom_i const &) = delete;
  Buttom_i (Buttom_i &&) = delete;
  Buttom_i &operator= (Buttom_i const &) = delete;
  Buttom_i &operator= (Buttom_i &&) = delete;

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
