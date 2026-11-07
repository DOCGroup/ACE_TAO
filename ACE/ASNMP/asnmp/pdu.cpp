
//=============================================================================
/**
 *  @file     pdu.cpp
 *
 *  Pdu class implementation. Encapsulation of an SMI Protocol
 *  Data Unit (PDU) in C++.
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

#include "asnmp/snmp.h"
#include "asnmp/pdu.h"       // include Pdu class definition
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <new>
#include <utility>

//=====================[ constructor no args ]=========================
Pdu::Pdu() = default;

//=====================[ constructor with vbs_ and count ]==============
Pdu::Pdu (Vb *pvbs, int pvb_count)
{
   // zero is ok
   if (pvb_count == 0)
     {
      validity_ = 1;
      return;
     }

   // check for over then max
   if (pvb_count < 0 || pvb_count > MAX_VBS)
     {
       validity_ = 0;
       return;
     }

   // loop through and assign internal vbs_
   for (int z = 0; z < pvb_count; ++z)
     {
       validity_ = 0;
       std::unique_ptr<Vb> value {new (std::nothrow) Vb (pvbs[z])};
       if (!value)
         {
           errno = ENOMEM;
           return;
         }
       vbs_[z] = std::move (value);
       ++vb_count_;
       validity_ = 1;
     }

   // assign the vb count
   vb_count_ = pvb_count;

   validity_ = 1;
}

//=====================[ constructor with another Pdu instance ]========
Pdu::Pdu (Pdu const &pdu)
{
   *this = pdu;
}

//=====================[ destructor ]====================================
Pdu::~Pdu() = default;

//=====================[ assignment to another Pdu object overloaded ]===
Pdu &Pdu::operator= (Pdu const &pdu)
{
   if (this == &pdu)
     return *this;

   // Initialize all mv's
   error_status_ = pdu.error_status_;
   error_index_ = pdu.error_index_;
   request_id_ = pdu.request_id_;
   pdu_type_ = pdu.pdu_type_;
   notify_id_ = pdu.notify_id_;
   notify_timestamp_ = pdu.notify_timestamp_;
   notify_enterprise_ = pdu.notify_enterprise_;
   validity_ = 1;

   delete_all_vbs ();

   // check for zero case
   if ( pdu.vb_count_ == 0) {
      return *this;
   }

   // loop through and fill em up
   for (int z = 0; z < pdu.vb_count_; ++z)
     {
       validity_ = 0;
       std::unique_ptr<Vb> value {new (std::nothrow) Vb (*pdu.vbs_[z])};
       if (!value)
         {
           errno = ENOMEM;
           return *this;
         }
       vbs_[z] = std::move (value);
       ++vb_count_;
       validity_ = 1;
     }

   return *this;
}

// append operator, appends a string
Pdu &Pdu::operator+= (Vb &vb)
{
  // do we have room?
  if ( vb_count_ + 1 > MAX_VBS)
    return *this;

  // add the new one
  validity_ = 0;
  std::unique_ptr<Vb> value {new (std::nothrow) Vb (vb)};
  if (!value)
    {
      errno = ENOMEM;
      return *this;
    }
  vbs_[vb_count_] = std::move (value);
  // set up validity_
  validity_ = 1;

  // up the vb count
  vb_count_++;

  // return self reference
  return *this;
}

// return fomatted version of this object
char const *Pdu::to_string ()
{
  // determine how big a buffer and allocate it
  constexpr std::size_t HEADER_STR = 100;
  std::size_t size = HEADER_STR; // header takes up this much room

   for (int z = 0; z < vb_count_; ++z)
       size += std::strlen (vbs_[z]->to_string ()) + 2;

  std::unique_ptr<char[]> buffer {new (std::nothrow) char[size]};
  if (!buffer)
    {
      errno = ENOMEM;
      return "";
    }
  output_ = std::move (buffer);

  // print pdu header info
  std::sprintf (output_.get (), "pdu: valid: %d type:%d, req:%d, cnt: %d, err stat: %d \
                  err idx: %d\n",     validity_, pdu_type_, static_cast<int> (request_id_),
                  vb_count_, error_status_, error_index_ );

  // now append vb pairs in this object
   for (int z = 0; z < vb_count_; ++z) {
     std::strcat (output_.get (), vbs_[z]->to_string ());
     std::strcat (output_.get (), "\n\t");
   }

  return output_.get ();
}

//=====================[ extract Vbs from Pdu ]==========================
// how do you know that the caler has enough memory???
// should I self allocate this in here and require the
// caller then to free it up at soem later time
int Pdu::get_vblist (Vb *pvbs, int pvb_count)
{
   if (pvbs == nullptr || pvb_count < 0 || pvb_count > vb_count_)
      return 0;

   // loop through all vbs_ and assign to params
   int z;
   for (z = 0; z < pvb_count; z++)
      pvbs[z] = *vbs_[z];

   return 1;
}

//=====================[ deposit Vbs ]===================================
int Pdu::set_vblist (Vb *pvbs, int pvb_count)
{
   // if invalid then don't destroy
   if (pvbs == nullptr || pvb_count < 0 || pvb_count > MAX_VBS)
     return 0;

   // check for zero case
   if ( pvb_count == 0) {
      delete_all_vbs ();
      validity_ = 1;
      error_status_ = 0;
      error_index_ = 0;
      request_id_ = 0;
      return 0;
   }

   // Build the replacement list first so allocation failure leaves this PDU intact.
   std::array<std::unique_ptr<Vb>, MAX_VBS> new_vbs {};
   for (int z = 0; z < pvb_count; ++z)
     {
       std::unique_ptr<Vb> value {new (std::nothrow) Vb (pvbs[z])};
       if (!value)
         {
           errno = ENOMEM;
           return 0;
         }
       new_vbs[z] = std::move (value);
     }

   vbs_ = std::move (new_vbs);
   vb_count_ = pvb_count;

   // clear error status and index since no longer valid
   // request id may still apply so don't reassign it
   error_status_ = 0;
   error_index_ = 0;
   validity_ = 1;

   return 1;
}

//===================[ get a particular vb ]=============================
// here the caller has already instantiated a vb object
// index is zero based
int Pdu::get_vb (Vb &vb, int index) const
{
   // can't have an index less than 0
   if ( index < 0)
     return 0;

   // can't ask for something not there
   if ( index > (vb_count_ - 1))
      return 0;

   // asssign it
   vb = *vbs_[index];

   return 1;
}

//===================[ set a particular vb ]=============================
int Pdu::set_vb (Vb &vb, int index)
{
   // can't set a vb at index less than 0
   if ( index < 0)
     return 0;

   // can't ask for something not there
   if ( index > (vb_count_ - 1))
      return 0;

   std::unique_ptr<Vb> value {new (std::nothrow) Vb (vb)};
   if (!value)
     {
       errno = ENOMEM;
       return 0;
     }
   vbs_[index] = std::move (value);
   validity_ = 1;

   return 1;
}

//=====================[ return number of vbs_ ]==========================
int Pdu::get_vb_count () const
{
   return vb_count_;
}

//=====================[ return the error status ]=======================
int Pdu::get_error_status () const
{
   return error_status_;
}

char const *Pdu::agent_error_reason ()
{
    int const pdu_err = get_error_status ();
    if (pdu_err == 0) // any real error?
        return "not in error state";

    int const n_vbs = get_vb_count ();
    Vb bad;
    get_vb(bad, get_error_index() -1); // not zero based??
    char const * const pmsg = Snmp::error_string (get_error_status ());
    char const * const id = bad.to_string_oid ();
    char const * const val = bad.to_string_value ();
    constexpr std::size_t HDR_SZ = 100;

    std::size_t const size = std::strlen (pmsg) + std::strlen (id) + std::strlen (val);
    std::unique_ptr<char[]> buffer {new (std::nothrow) char[size + HDR_SZ]};
    if (!buffer)
      {
        errno = ENOMEM;
        return "";
      }
    output_ = std::move (buffer);

    std::sprintf (output_.get (),
                    "FAIL PDU REPORT: pdu id: %lu vb cnt: %d vb idx: %d\n"
                    "                 msg: %s vb oid: %s value: %s\n",
                    get_request_id(), n_vbs, get_error_index(),
                    pmsg, id, val);

    return output_.get ();
}

//=====================[ set the error status ]==========================
// friend
void set_error_status (Pdu *pdu, int status)
{
   if (pdu != nullptr)
     pdu->error_status_ = status;
}

//=====================[ return the error index ]========================
int Pdu::get_error_index () const
{
   return error_index_;
}

//=====================[ set the error index ]===========================
// friend
void set_error_index (Pdu *pdu, int index)
{
   if (pdu != nullptr)
     pdu->error_index_ = index;
}

//=====================[ clear error status ]=============================
void clear_error_status (Pdu *pdu)
{
   if (pdu != nullptr)
     pdu->error_status_ = 0;
}

//=====================[ clear error index ]==============================
void clear_error_index (Pdu *pdu)
{
   if (pdu != nullptr)
     pdu->error_index_ = 0;
}

//=====================[ return the request id ]==========================
unsigned long Pdu::get_request_id () const
{
   return request_id_;
}

//=====================[ set the request id ]=============================
// friend function
void set_request_id (Pdu *pdu, unsigned long rid)
{
   if (pdu != nullptr)
     pdu->request_id_ = rid;
}

//=====================[ returns validity_ of Pdu instance ]===============
int Pdu::valid () const
{
   return validity_;
}

//=====================[ get the pdu type ]===============================
unsigned short Pdu::get_type () const
{
   return pdu_type_;
}

// set the pdu type
void Pdu::set_type (unsigned short type)
{
   pdu_type_ = type;
}

// trim off the last vb
int Pdu::trim (int p)
{
   int lp = p;

   // verify that lp is legal
   if ( lp < 0 || lp > vb_count_)
     return 0;

   while ( lp != 0)   {
     if ( vb_count_ > 0) {
         vbs_[vb_count_ - 1].reset ();
         vb_count_--;
     }
     lp--;
   }
   return 1;
}

// delete a Vb anywhere within the Pdu
int Pdu::delete_vb (int p)
{
   // position has to be in range
   if (( p < 0) || ( p > (vb_count_ - 1)))
      return 0;

   // safe to remove it
   for ( int z=p;z < (vb_count_-1);z++) {
      vbs_[z] = std::move (vbs_[z + 1]);
   }
   vbs_[vb_count_ - 1].reset ();
   vb_count_--;

   return 1;
}

void Pdu::delete_all_vbs ()
{
  for ( int z = 0; z < vb_count_; z++)
    vbs_[z].reset ();
   vb_count_ = 0;
}

// set notify timestamp
void Pdu::set_notify_timestamp (TimeTicks const &timestamp)
{
   notify_timestamp_ = timestamp;
}

// get notify timestamp
void Pdu::get_notify_timestamp (TimeTicks &timestamp) const
{
    timestamp = notify_timestamp_;
}

// set the notify id
void Pdu::set_notify_id (Oid id)
{
    notify_id_ = id;
}

// get the notify id
void Pdu::get_notify_id (Oid &id) const
{
   id = notify_id_;
}

// set the notify enterprise
void Pdu::set_notify_enterprise (Oid const &enterprise)
{
   notify_enterprise_ = enterprise;
}

// get the notify enterprise
void Pdu::get_notify_enterprise (Oid &enterprise) const
{
    enterprise = notify_enterprise_;
}

// ------   class VbIter -------------------
VbIter::VbIter (Pdu &pdu)
  : pdu_ (&pdu)
{
}

// returns 1 if ok, else 0 if none left
int VbIter::next (Vb &vb)
{
   if (idx_ == -1) {
    idx_ = 0;
    pdu_->get_vb(vb, idx_++);
    return 1;
   }
  else
  if (idx_ < pdu_->get_vb_count()) {
    pdu_->get_vb(vb, idx_++);
    return 1;
  }
  return 0; // non left
}
