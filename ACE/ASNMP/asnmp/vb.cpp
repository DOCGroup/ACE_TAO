
//=============================================================================
/**
 *  @file     vb.cpp
 *
 *  $Id$
 *
 *  The Vb class is an encapsulation of the snmp variable binding.
 * This module contains the class definition for the variable binding (VB)
 * class. The VB class is an encapsulation of a SNMP VB. A VB object is
 * composed of one SNMP++ Oid and one SMI value. The Vb class utilizes Oid
 * objects and thus requires the Oid class. To use this class,
 * set oid, value then call valid() to be sure object was constructed correctly.
 *
 *
 *  @author Peter E MellquistMichael R MacFaden  mrm@cisco.com - rework & ACE port
 */
//=============================================================================

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
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"

//---------------[ Vb::Vb( void) ]--------------------------------------
// constructor with no arguments
// makes an vb, unitialized
Vb::Vb( void): output_(0), iv_vb_value_(0),
    exception_status_(SNMP_CLASS_SUCCESS)
{
}

//---------------[ Vb::Vb( const Oid &oid) ]-----------------------------
// constructor to initialize the oid
// makes a vb with oid portion initialized
Vb::Vb( const Oid &oid):  output_(0), iv_vb_oid_(oid), iv_vb_value_(0),
 exception_status_(SNMP_CLASS_SUCCESS)
{
}

//---------------[ Vb::Vb( const Oid &oid, const SmiSyntax &val) ]-------
Vb::Vb( const Oid &oid, const SnmpSyntax &val, const SmiUINT32 status):
  output_(0), iv_vb_oid_(oid), iv_vb_value_(0), exception_status_(status)
{
  // iv_vb_value_ = recast_smi_object(val);  // allocate and construct object
  iv_vb_value_ = val.clone();
}

//---------------[ Vb::Vb( const Vb &vb) ]-----------------------------
// copy constructor
Vb::Vb( const Vb &vb): output_(0), iv_vb_value_(0)
{
   *this = vb;
}

//---------------[ Vb::~Vb() ]------------------------------------------
// destructor
// if the vb has a oid or an octect string then
// the associated memory needs to be freed
Vb::~Vb()
{
  free_vb();
  delete [] output_; // formatting buffer if it exists
}


//--------------[ Vb::valid() ]-----------------------------------------
// returns validity of a Vb object
// must have a valid oid and value
int Vb::valid() const
{
   if ( iv_vb_oid_.valid() && (iv_vb_value_ && iv_vb_value_->valid()) )
     return 1;
   else
     return 0;
}


//---------------[ Vb& Vb::operator=( const Vb &vb) ]--------------------
// overloaded assignment allows assigning one Vb to another
// this involves deep memory thus target vb needs to be freed
// before assigning source
Vb& Vb::operator=( const Vb &vb)
{
   free_vb();    // free up target to begin with

   //-----[ reassign the Oid portion 1st ]
   vb.get_oid( iv_vb_oid_);

   //-----[ next set the vb value portion ]
   if (vb.iv_vb_value_ == 0) {
     iv_vb_value_ = 0;
   }
   else {
     iv_vb_value_ = vb.iv_vb_value_->clone();
   }
   exception_status_ = vb.exception_status_;

   return *this; // return self reference
}

 // set a Vb null, if its not already
void Vb::set_null()
{
   free_vb();
}

//---------------[ Vb::set_oid( const Oid oid ) ]-----------------------
// set value oid only with another oid
void Vb::set_oid( const Oid& oid)
{
  iv_vb_oid_ = oid;
}

//---------------[ Vb::get_oid( Oid &oid) ]-----------------------------
// get oid portion
void Vb::get_oid( Oid &oid) const
{
  oid = iv_vb_oid_;
}

//----------------[ void Vb::free_vb() ]--------------------------------
// protected method to free memory
// this methos is used to free memory when assigning new vbs
// or destructing
// in the case of oids and octets, we need to do a deep free
void Vb::free_vb()
{
  if (iv_vb_value_)
    delete iv_vb_value_;
  exception_status_ = SNMP_CLASS_SUCCESS;
  iv_vb_value_ = 0;
}

void Vb::set_value( const SnmpInt32& i)
{
  free_vb();
  iv_vb_value_ = (SnmpSyntax *)new SnmpInt32(i);
}

void Vb::set_value( const SnmpUInt32& u)
{
  free_vb();
  iv_vb_value_ = (SnmpSyntax *)new SnmpUInt32(u);
}

void Vb::set_value( const Gauge32& g)
{
  free_vb();
  iv_vb_value_ = (SnmpSyntax *)new Gauge32(g);
}

void Vb::set_value( const Counter32& c)
{
  free_vb();
  iv_vb_value_ = (SnmpSyntax *)new Counter32(c);
}

void Vb::set_value( const Counter64& c)
{
  free_vb();
  iv_vb_value_ = (SnmpSyntax *)new Counter64(c);
}

void Vb::set_value( const TimeTicks& t)
{
  free_vb();
  iv_vb_value_ = (SnmpSyntax *)new TimeTicks(t);
}

void Vb::set_value( const OctetStr& s)
{
  free_vb();
  iv_vb_value_ = (SnmpSyntax *)new OctetStr(s);
}

void Vb::set_value( const Oid& o)
{
  free_vb();
  iv_vb_value_ = (SnmpSyntax *)new Oid(o);
}

void Vb::set_value ( const SnmpSyntax &val)
{
  free_vb();
  iv_vb_value_ = val.clone();
}

int Vb::get_value( SnmpInt32 &i)
{
   if (iv_vb_value_ &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_INT32 )) {
     i = *((SnmpInt32 *) iv_vb_value_);
     return SNMP_CLASS_SUCCESS;
   }
   else
     return SNMP_CLASS_INVALID;
}

int Vb::get_value( SnmpUInt32 &u)
{
   if (iv_vb_value_ && iv_vb_value_->valid())
   {
       SmiUINT32 syntax = iv_vb_value_->get_syntax();
       if (syntax == sNMP_SYNTAX_GAUGE32 ||
           syntax == sNMP_SYNTAX_CNTR32 ||
           syntax == sNMP_SYNTAX_TIMETICKS ||
           syntax == sNMP_SYNTAX_UINT32)
       {
           u = *((SnmpUInt32 *) iv_vb_value_);
           return SNMP_CLASS_SUCCESS;
       }
   }
   return SNMP_CLASS_INVALID;
}

/* return a uint or a gauge. this is casting, but no semantic difference
 * at this level
 */
int Vb::get_value( Gauge32 &g)
{
   if (iv_vb_value_ &&
       iv_vb_value_->valid() &&
       ((iv_vb_value_->get_syntax() == sNMP_SYNTAX_GAUGE32) ||
            iv_vb_value_->get_syntax() == sNMP_SYNTAX_UINT32) ) {
     g = *((Gauge32 *) iv_vb_value_);
     return SNMP_CLASS_SUCCESS;
   }
   else
     return SNMP_CLASS_INVALID;
}

int Vb::get_value( Counter32 &c)
{
   if (iv_vb_value_ &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_CNTR32 )) {
     c = *((Counter32 *) iv_vb_value_);
     return SNMP_CLASS_SUCCESS;
   }
   else
     return SNMP_CLASS_INVALID;
}

int Vb::get_value( Counter64 &c)
{
   if (iv_vb_value_ &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_CNTR64 )) {
     c = *((Counter32 *) iv_vb_value_);
     return SNMP_CLASS_SUCCESS;
   }
   else
     return SNMP_CLASS_INVALID;
}

int Vb::get_value( TimeTicks &t)
{
   if (iv_vb_value_ &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_TIMETICKS )) {
     t = *((TimeTicks *) iv_vb_value_);
     return SNMP_CLASS_SUCCESS;
   }
   else
     return SNMP_CLASS_INVALID;
}

int Vb::get_value( OctetStr &s)
{
   if (iv_vb_value_ &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_OCTETS )) {
     s = *((OctetStr *) iv_vb_value_);
     return SNMP_CLASS_SUCCESS;
   }
   else
     return SNMP_CLASS_INVALID;
}

int Vb::get_value( Oid &s)
{
   if (iv_vb_value_ &&
       iv_vb_value_->valid() &&
       (iv_vb_value_->get_syntax() == sNMP_SYNTAX_OID )) {
     s = *((Oid *) iv_vb_value_);
     return SNMP_CLASS_SUCCESS;
   }
   else
     return SNMP_CLASS_INVALID;
}


//---------------[ Vb::get_value( Value &val) ]--------
int Vb::get_value( SnmpSyntax &val)
{
  if (iv_vb_value_) {
    val = *iv_vb_value_;
    if (val.valid())
        return SNMP_CLASS_SUCCESS;
    else
        return SNMP_CLASS_INVALID;
  }
  else
  {
//TM: should set val to be invalid
    return SNMP_CLASS_INVALID;
  }
}



//-----[ misc]--------------------------------------------------------

// return the current syntax
// This method violates Object Orientation but may be useful if
// the caller has a vb object and does not know what it is.
// This would be useful in the implementation of a browser.
SmiUINT32 Vb::get_syntax()
{
  if ( exception_status_ != SNMP_CLASS_SUCCESS)
    return exception_status_;
  else
    return ( iv_vb_value_ ? iv_vb_value_->get_syntax() : sNMP_SYNTAX_NULL);
}

// return the printabel value
const char  *Vb::to_string_value()
{
  if (iv_vb_value_)
    return iv_vb_value_->to_string();
  else
    return "";
}

// return the printable oid
const char  *Vb::to_string_oid()
{
  return iv_vb_oid_.to_string();
}

// generate string with name/ value format
const char  *Vb::to_string()
{
  int len = ACE_OS::strlen(iv_vb_oid_.to_string());
  const char *ptr = iv_vb_value_ ? iv_vb_value_->to_string() : "";
  len += ACE_OS::strlen(ptr) + 3 + 1; // " / " + null
  ACE_NEW_RETURN(output_, char[len], "");
  ACE_OS::sprintf(output_, "%s / %s", iv_vb_oid_.to_string(), ptr);
  return output_;
}

// friend function to set exception status
void set_exception_status( Vb *vb, const SmiUINT32 status)
{
   vb->exception_status_ = status;
}

// equivlence operator overloaded
// hack, by side effect, compare based on string formatting output_
bool operator==( const Vb &lhs, const Vb &rhs)
{
  if ( lhs.iv_vb_oid_ != rhs.iv_vb_oid_)
       return false;

  if (lhs.iv_vb_value_ != 0 && rhs.iv_vb_value_ != 0)
    {
      const int val =
        ACE_OS::strcmp (lhs.iv_vb_value_->to_string(),
                        rhs.iv_vb_value_->to_string());
      return !val;
    }
  else
    return false;
}
