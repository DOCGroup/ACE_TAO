
/**
 *  @file     vb.cpp
 *
 *  The Vb class is an encapsulation of the snmp variable binding.
 * This module contains the class definition for the variable binding (VB)
 * class. The VB class is an encapsulation of a SNMP VB. A VB object is
 * composed of one SNMP++ Oid and one SMI value. The Vb class utilizes Oid
 * objects and thus requires the Oid class. To use this class,
 * set oid, value then call valid() to be sure object was constructed correctly.
 *
 *  @author Peter E MellquistMichael R MacFaden  mrm@cisco.com - rework & ACE port
 */

/*===================================================================
  Copyright (c) 1996
  Hewlett-Packard Company

  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  Permission to use, copy, modify, distribute and/or sell this software
  and/or its documentation is hereby granted without fee. User agrees
  to display the above copyright notice and this license notice in all
  copies of the software and any documentation of the software. User
  agrees to assume all liability for the use of the software; Hewlett-Packard
  makes no representations about the suitability of this software for any
  purpose. It is provided "AS-IS without warranty of any kind,either express
  or implied. User hereby grants a royalty-free license to any and all
  derivatives based upon this software code base.
=====================================================================*/

#include "asnmp/oid.h"         // include oid class defs
#include "asnmp/vb.h"                  // include vb class defs
#include "asnmp/snmperrs.h"      // error codes
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <memory>
#include <new>
#include <utility>

/// Construct an uninitialized variable binding.
Vb::Vb ()
{
}

/// Construct a variable binding with its OID initialized.
Vb::Vb (Oid const &oid)
  : iv_vb_oid_ (oid)
{
}

Vb::Vb (Oid const &oid, SnmpSyntax const &val, SmiUINT32 status)
  : iv_vb_oid_ (oid), iv_vb_value_ (val.clone ()), exception_status_ (status)
{
}

Vb::Vb (Vb const &vb)
{
   *this = vb;
}

/// Release the OID and value storage owned by this variable binding.
Vb::~Vb() = default;

/// Return whether this variable binding has a valid OID and value.
int Vb::valid () const
{
   return iv_vb_oid_.valid () && iv_vb_value_ != nullptr && iv_vb_value_->valid ();
}

/// Assign another variable binding, cloning its value.
Vb &Vb::operator= (Vb const &vb)
{
   if (this == std::addressof (vb))
   {
     return *this;
   }

   std::unique_ptr<SnmpSyntax> value_copy {
     vb.iv_vb_value_ != nullptr ? vb.iv_vb_value_->clone () : nullptr
   };
   iv_vb_oid_ = vb.iv_vb_oid_;
   iv_vb_value_ = std::move (value_copy);
   exception_status_ = vb.exception_status_;

   return *this; // return self reference
}

/// Set this variable binding to null if it is not already.
void Vb::set_null ()
{
   free_vb();
}

/// Set the OID value.
void Vb::set_oid (Oid const &oid)
{
  iv_vb_oid_ = oid;
}

/// Get the OID portion.
void Vb::get_oid (Oid &oid) const
{
  oid = iv_vb_oid_;
}

/// Free the value storage and reset the exception status.
void Vb::free_vb()
{
  iv_vb_value_.reset ();
  exception_status_ = SNMP_CLASS_SUCCESS;
}

void Vb::set_value (SnmpInt32 const &i)
{
  free_vb();
  iv_vb_value_ = std::make_unique<SnmpInt32> (i);
}

void Vb::set_value (SnmpUInt32 const &u)
{
  free_vb();
  iv_vb_value_ = std::make_unique<SnmpUInt32> (u);
}

void Vb::set_value (Gauge32 const &g)
{
  free_vb();
  iv_vb_value_ = std::make_unique<Gauge32> (g);
}

void Vb::set_value (Counter32 const &c)
{
  free_vb();
  iv_vb_value_ = std::make_unique<Counter32> (c);
}

void Vb::set_value (Counter64 const &c)
{
  free_vb();
  iv_vb_value_ = std::make_unique<Counter64> (c);
}

void Vb::set_value (TimeTicks const &t)
{
  free_vb();
  iv_vb_value_ = std::make_unique<TimeTicks> (t);
}

void Vb::set_value (OctetStr const &s)
{
  free_vb();
  iv_vb_value_ = std::make_unique<OctetStr> (s);
}

void Vb::set_value (Oid const &o)
{
  free_vb();
  iv_vb_value_ = std::make_unique<Oid> (o);
}

void Vb::set_value (SnmpSyntax const &val)
{
  free_vb();
  iv_vb_value_.reset (val.clone ());
}

int Vb::get_value (SnmpInt32 &i)
{
   if (iv_vb_value_ != nullptr &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_INT32 )) {
     i = *static_cast<SnmpInt32 *> (iv_vb_value_.get ());
     return SNMP_CLASS_SUCCESS;
   }
   else
   {
     return SNMP_CLASS_INVALID;
   }
}

int Vb::get_value (SnmpUInt32 &u)
{
   if (iv_vb_value_ != nullptr && iv_vb_value_->valid())
   {
       SmiUINT32 const syntax = iv_vb_value_->get_syntax();
       if (syntax == sNMP_SYNTAX_GAUGE32 ||
           syntax == sNMP_SYNTAX_CNTR32 ||
           syntax == sNMP_SYNTAX_TIMETICKS ||
           syntax == sNMP_SYNTAX_UINT32)
       {
           u = *static_cast<SnmpUInt32 *> (iv_vb_value_.get ());
           return SNMP_CLASS_SUCCESS;
       }
   }
   return SNMP_CLASS_INVALID;
}

/* return a uint or a gauge. this is casting, but no semantic difference
 * at this level
 */
int Vb::get_value (Gauge32 &g)
{
   if (iv_vb_value_ != nullptr &&
       iv_vb_value_->valid() &&
       ((iv_vb_value_->get_syntax() == sNMP_SYNTAX_GAUGE32) ||
            iv_vb_value_->get_syntax() == sNMP_SYNTAX_UINT32) ) {
     g = *static_cast<Gauge32 *> (iv_vb_value_.get ());
     return SNMP_CLASS_SUCCESS;
   }
   else
   {
     return SNMP_CLASS_INVALID;
   }
}

int Vb::get_value (Counter32 &c)
{
   if (iv_vb_value_ != nullptr &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_CNTR32 )) {
     c = *static_cast<Counter32 *> (iv_vb_value_.get ());
     return SNMP_CLASS_SUCCESS;
   }
   else
   {
     return SNMP_CLASS_INVALID;
   }
}

int Vb::get_value (Counter64 &c)
{
   if (iv_vb_value_ != nullptr &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_CNTR64 )) {
     c = *static_cast<Counter64 *> (iv_vb_value_.get ());
     return SNMP_CLASS_SUCCESS;
   }
   else
   {
     return SNMP_CLASS_INVALID;
   }
}

int Vb::get_value (TimeTicks &t)
{
   if (iv_vb_value_ != nullptr &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_TIMETICKS )) {
     t = *static_cast<TimeTicks *> (iv_vb_value_.get ());
     return SNMP_CLASS_SUCCESS;
   }
   else
   {
     return SNMP_CLASS_INVALID;
   }
}

int Vb::get_value (OctetStr &s)
{
   if (iv_vb_value_ != nullptr &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_OCTETS )) {
     s = *static_cast<OctetStr *> (iv_vb_value_.get ());
     return SNMP_CLASS_SUCCESS;
   }
   else
   {
     return SNMP_CLASS_INVALID;
   }
}

int Vb::get_value (Oid &s)
{
   if (iv_vb_value_ != nullptr &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_OID )) {
     s = *static_cast<Oid *> (iv_vb_value_.get ());
     return SNMP_CLASS_SUCCESS;
   }
   else
   {
     return SNMP_CLASS_INVALID;
   }
}

int Vb::get_value (SnmpSyntax &val)
{
  if (iv_vb_value_ != nullptr) {
    val = *iv_vb_value_;
    if (val.valid())
      {
        return SNMP_CLASS_SUCCESS;
      }
    else
    {
      return SNMP_CLASS_INVALID;
    }
  }
  else
  {
//TM: should set val to be invalid
    return SNMP_CLASS_INVALID;
  }
}

/// Return the syntax, or the exception status if an exception is present.
/// This can be useful when a caller does not know the value type.
SmiUINT32 Vb::get_syntax ()
{
  if ( exception_status_ != SNMP_CLASS_SUCCESS)
    {
      return exception_status_;
    }
  else
  {
    return iv_vb_value_ != nullptr ? iv_vb_value_->get_syntax() : sNMP_SYNTAX_NULL;
  }
}

/// Return the printable value.
char const *Vb::to_string_value ()
{
  if (iv_vb_value_ != nullptr)
    {
      return iv_vb_value_->to_string();
    }
  else
  {
    return "";
  }
}

/// Return the printable OID.
char const *Vb::to_string_oid ()
{
  return iv_vb_oid_.to_string();
}

/// Generate a string in name/value format.
char const *Vb::to_string ()
{
  size_t len = std::strlen (iv_vb_oid_.to_string ());
  char const * const ptr = iv_vb_value_ != nullptr ? iv_vb_value_->to_string () : "";
  len += std::strlen (ptr) + 3 + 1; // " / " + null
  std::unique_ptr<char[]> buffer {new (std::nothrow) char[len]};
  if (buffer == nullptr)
    {
      errno = ENOMEM;
      return "";
    }
  output_ = std::move (buffer);
  std::sprintf (output_.get (), "%s / %s", iv_vb_oid_.to_string (), ptr);
  return output_.get ();
}

/// Set the exception status.
void set_exception_status (Vb *vb, SmiUINT32 status)
{
   vb->exception_status_ = status;
}

/// Compare variable bindings for equivalence using their formatted output.
bool operator== (Vb const &lhs, Vb const &rhs)
{
  if ( lhs.iv_vb_oid_ != rhs.iv_vb_oid_)
  {
    return false;
  }

  if (lhs.iv_vb_value_ != nullptr && rhs.iv_vb_value_ != nullptr)
    {
      int const val =
        std::strcmp (lhs.iv_vb_value_->to_string(),
                     rhs.iv_vb_value_->to_string());
      return !val;
    }
  else
  {
    return false;
  }
}
