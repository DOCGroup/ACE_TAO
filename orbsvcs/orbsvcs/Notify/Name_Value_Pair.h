// -*- C++ -*-

//=============================================================================
/**
*  @file    Name_Value_Pair.h
*
*  $Id$
*
*  @author Jonathan Pollack <pollack_j@ociweb.com>
*/
//=============================================================================

#ifndef NAME_VALUE_PAIR_H
#define NAME_VALUE_PAIR_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/Property.h"
#include "orbsvcs/Notify/Property_Boolean.h"
#include "orbsvcs/Notify/notify_serv_export.h"

#include "ace/SString.h"
#include "ace/Vector_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
  /**
   * \brief Name/Value Pair
   *
   * Associates a name (character string) with a value (encoded into a character string)
   * For use in managing properties.
   */
  class TAO_Notify_Serv_Export NVP {
  public:
    /// Construct an undefined name/value (to allow use in collections)
    NVP();
    /// Construct from a property containing a short value.
    explicit NVP(const TAO_Notify_Property_Short& p);
    /// Construct from a property containing a long value.
    explicit NVP(const TAO_Notify_Property_Long& p);
    /// Construct from a property containing a time value
    explicit NVP(const TAO_Notify_Property_Time& p);
    /// Construct from a property containing a boolean value
    explicit NVP(const TAO_Notify_Property_Boolean& p);

    /// Construct from name and long value.
    NVP(const char * n, CORBA::Long v);
    /// Construct from name and string value.
    NVP(const char * n, const char * v);
    /// Construct from name and string value.
    NVP(const char * n, const ACE_CString & v);

    /// Assignment operator
    bool operator==(const NVP& rhs) const;
    /// Comparison operator compares only name (not value)
    bool operator!=(const NVP& rhs) const;
    ACE_CString name;
    ACE_CString value;
  };

  /**
   * \brief Collection of Name/Value Pairs
   */
  class TAO_Notify_Serv_Export NVPList {
  public:
    /// Find the NVP with the same name as the property, copy the value.
    void load(TAO_Notify_Property_Short& p) const;
    /// Find the NVP with the same name as the property, copy the value.
    void load(TAO_Notify_Property_Long& p) const;
    /// Find the NVP with the same name as the property, copy the value.
    void load(TAO_Notify_Property_Time& p) const;
    /// Find the NVP with the same name as the property, copy the value.
    void load(TAO_Notify_Property_Boolean& p) const;
    /// Find the NVP with specified name, copy the value to a string.
    bool load(const char * n, CORBA::Long& v) const;
    /// Find the NVP with specified name, copy the value to a long.
    bool load(const char * n, ACE_CString& v) const;
    /// add a new name/value pair.
    void push_back(const NVP& v);
    /// size of the collection.
    size_t size() const;
    /// access NVP via numeric index.
    const NVP& operator[](size_t ndx) const;
    /// low level access: find
    bool find (const char * name, ACE_CString& val) const;
    /// low level access:  use this when you don't need a CString
    bool find (const char * name, const char *& val) const;

  private:
    ACE_Vector<NVP> list_;
  };
} // namespace TAO_Notify

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* NAME_VALUE_PAIR_H */
