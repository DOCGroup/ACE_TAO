
//=============================================================================
/**
 *  @file     pdu.cpp
 *
 *  $Id$
 *
 *  Pdu class implementation. Encapsulation of an SMI Protocol
 *  Data Unit (PDU) in C++.
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

#include "asnmp/snmp.h"
#include "asnmp/pdu.h"       // include Pdu class definition
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

//=====================[ constructor no args ]=========================
Pdu::Pdu( void): vb_count_(0), error_status_(0), error_index_(0),
validity_(0), request_id_(0), pdu_type_(0), notify_timestamp_(0),
output_(0)
{
}

//=====================[ constructor with vbs_ and count ]==============
Pdu::Pdu( Vb* pvbs, const int pvb_count): vb_count_(0), error_index_(0),
validity_(0), request_id_(0), pdu_type_(0), notify_timestamp_(0),
output_(0)
{
   int z = 0;  // looping variable

   // zero is ok
   if ( pvb_count == 0) {
      validity_ = 1;
      return;
   }

   // check for over then max
   if ( pvb_count > MAX_VBS) {
       validity_ = 0;
       return;
   }

   // loop through and assign internal vbs_
   for (z = 0;z < pvb_count; z++) {
     validity_ = 0;
     ACE_NEW(vbs_[z], Vb( pvbs[z]));
     validity_ = 1;
   }

   // assign the vb count
   vb_count_ = pvb_count;

   validity_ = 1;
}

//=====================[ constructor with another Pdu instance ]========
Pdu::Pdu( const Pdu &pdu): vb_count_(0),
error_index_(0), validity_(0), request_id_(0), pdu_type_(0),
notify_timestamp_(0), output_(0)
{
   *this = pdu;
   return;
}

//=====================[ destructor ]====================================
Pdu::~Pdu()
{
  delete_all_vbs();
  delete [] output_;
}


//=====================[ assignment to another Pdu object overloaded ]===
Pdu& Pdu::operator=( const Pdu &pdu)
{
   if (this == &pdu)
     return *this;

   int z;   // looping variable

   // Initialize all mv's
   error_status_ = pdu.error_status_;
   error_index_ = pdu.error_index_;
   request_id_ = pdu.request_id_;
   pdu_type_ = pdu.pdu_type_;
   notify_id_ = pdu.notify_id_;
   notify_timestamp_ = pdu.notify_timestamp_;
   notify_enterprise_ = pdu.notify_enterprise_;
   validity_ = 1;

   // free up old vbs_
   for ( z = 0;z < vb_count_; z++)
     delete vbs_[z];
   vb_count_ = 0;

   // check for zero case
   if ( pdu.vb_count_ == 0) {
      return *this;
   }

   // loop through and fill em up
   for (z = 0; z < pdu.vb_count_; z++) {
     validity_ = 0;
     ACE_NEW_RETURN(vbs_[z], Vb ( *(pdu.vbs_[z])), *this);
     validity_ = 1;
   }

   vb_count_ = pdu.vb_count_;
   return *this;
}

// append operator, appends a string
Pdu& Pdu::operator+=( Vb &vb)
{

  // do we have room?
  if ( vb_count_ + 1 > MAX_VBS)
    return *this;

  // add the new one
  validity_ = 0;
  ACE_NEW_RETURN(vbs_[vb_count_], Vb (vb), *this);
  // set up validity_
  validity_ = 1;

  // up the vb count
  vb_count_++;

  // return self reference
  return *this;

}

// return fomatted version of this object
const char * Pdu::to_string()
{
  // determine how big a buffer and allocate it
  const int HEADER_STR = 100;
  unsigned size = HEADER_STR; // header takes up this much room
  int z;

   for ( z = 0; z < vb_count_; z++)
       size += ACE_OS::strlen(vbs_[z]->to_string());

  ACE_NEW_RETURN(output_, char[size], "");

  // print pdu header info
  ACE_OS::sprintf(output_, "pdu: valid: %d type:%d, req:%d, cnt: %d, err stat: %d \
                  err idx: %d\n",     validity_, pdu_type_, (int) request_id_,
                  vb_count_, error_status_, error_index_ );

  // now append vb pairs in this object
   for ( z = 0; z < vb_count_; z++) {
     ACE_OS::strcat(output_, vbs_[z]->to_string());
     ACE_OS::strcat(output_, "\n\t");
   }

  return output_;
}


//=====================[ extract Vbs from Pdu ]==========================
// how do you know that the caler has enough memory???
// should I self allocate this in here and require the
// caller then to free it up at soem later time
int Pdu::get_vblist( Vb* pvbs, const int pvb_count)
{
   if ((!pvbs) || ( pvb_count < 0) || ( pvb_count > vb_count_))
      return 0;

   // loop through all vbs_ and assign to params
   int z;
   for (z = 0; z < pvb_count; z++)
      pvbs[z] = *vbs_[z];

   return 1;

}

//=====================[ deposit Vbs ]===================================
int Pdu::set_vblist( Vb* pvbs, const int pvb_count)
{

   // if invalid then don't destroy
   if ((!pvbs) || ( pvb_count < 0) || ( pvb_count > MAX_VBS))
     return 0;

   // free up current vbs_
   int z;
   for ( z = 0; z < vb_count_; z++)
     delete vbs_[z];
   vb_count_ = 0;

   // check for zero case
   if ( pvb_count == 0) {
      validity_ = 1;
      error_status_ = 0;
      error_index_ = 0;
      request_id_ = 0;
      return 0;
   }


   // loop through all vbs_ and reassign them
   for ( z = 0; z < pvb_count; z++) {
     validity_ = 0;
     ACE_NEW_RETURN(vbs_[z], Vb (pvbs[z]), 0);
     validity_ = 1;
   }

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
int Pdu::get_vb( Vb &vb, const int index) const
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
int Pdu::set_vb( Vb &vb, const int index)
{
   // can't set a vb at index less than 0
   if ( index < 0)
     return 0;

   // can't ask for something not there
   if ( index > (vb_count_ - 1))
      return 0;

   // delete what is there
   delete vbs_[index];

   // assign it
   validity_ = 0;
   ACE_NEW_RETURN(vbs_[index], Vb (vb), 0);
   validity_ = 1;

   return 1;

}

//=====================[ return number of vbs_ ]==========================
int Pdu::get_vb_count() const
{
   return vb_count_;
}

//=====================[ return the error status ]=======================
int Pdu::get_error_status() const
{
   return error_status_;
}

const char *Pdu::agent_error_reason()
{
    int pdu_err = get_error_status();
    if (pdu_err == 0) // any real error?
        return "not in error state";

    int n_vbs = get_vb_count();
    Vb bad;
    get_vb(bad, get_error_index() -1); // not zero based??
    const char *pmsg =   Snmp::error_string(get_error_status());
    const char *id =   bad.to_string_oid();
    const char *val =  bad.to_string_value();
    const int HDR_SZ = 100;

    if (!output_) {
      int size = ACE_OS::strlen(pmsg) + ACE_OS::strlen(id) +
           ACE_OS::strlen(val);
      ACE_NEW_RETURN(output_, char[size + HDR_SZ], "");
    }

    ACE_OS::sprintf(output_,
                    "FAIL PDU REPORT: pdu id: %lu vb cnt: %d vb idx: %d\n"
                    "                 msg: %s vb oid: %s value: %s\n",
                    get_request_id(), n_vbs, get_error_index(),
                    pmsg, id, val);

    return output_;
}

//=====================[ set the error status ]==========================
// friend
void set_error_status( Pdu *pdu, const int status)
{
   if (pdu)
     pdu->error_status_ = status;
}

//=====================[ return the error index ]========================
int Pdu::get_error_index() const
{
   return error_index_;
}

//=====================[ set the error index ]===========================
// friend
void set_error_index( Pdu *pdu, const int index)
{
   if (pdu)
     pdu->error_index_ = index;
}

//=====================[ clear error status ]=============================
void clear_error_status( Pdu *pdu)
{
   if (pdu)
     pdu->error_status_ = 0;
}

//=====================[ clear error index ]==============================
void clear_error_index( Pdu *pdu)
{
   if (pdu)
     pdu->error_index_ = 0;
}

//=====================[ return the request id ]==========================
unsigned long Pdu::get_request_id() const
{
   return request_id_;
}

//=====================[ set the request id ]=============================
// friend function
void set_request_id( Pdu *pdu, const unsigned long rid)
{
   if (pdu)
     pdu->request_id_ = rid;
}

//=====================[ returns validity_ of Pdu instance ]===============
int Pdu::valid() const
{
   return validity_;
}

//=====================[ get the pdu type ]===============================
unsigned short Pdu::get_type()const
{
   return pdu_type_;
}

// set the pdu type
void Pdu::set_type( unsigned short type)
{
   pdu_type_ = type;
}


// trim off the last vb
int Pdu::trim(const int p)
{
   int lp = p;

   // verify that lp is legal
   if ( lp < 0 || lp > vb_count_)
     return 0;

   while ( lp != 0)   {
     if ( vb_count_ > 0) {
         delete vbs_[vb_count_ - 1];
         vb_count_--;
     }
     lp--;
   }
   return 1;
}



// delete a Vb anywhere within the Pdu
int Pdu::delete_vb( const int p)
{
   // position has to be in range
   if (( p < 0) || ( p > (vb_count_ - 1)))
      return 0;

   // safe to remove it
   delete vbs_[ p];

   for ( int z=p;z < (vb_count_-1);z++) {
      vbs_[z] = vbs_[z+1];
   }
   vb_count_--;

   return 1;
}

void Pdu::delete_all_vbs()
{
  for ( int z = 0; z < vb_count_; z++)
     delete vbs_[z];
   vb_count_ = 0;
}


// set notify timestamp
void Pdu::set_notify_timestamp( const TimeTicks & timestamp)
{
   notify_timestamp_ = timestamp;
}


// get notify timestamp
void Pdu::get_notify_timestamp( TimeTicks & timestamp) const
{
    timestamp = notify_timestamp_;
}

// set the notify id
void Pdu::set_notify_id( const Oid id)
{
    notify_id_ = id;
}

// get the notify id
void Pdu::get_notify_id( Oid &id) const
{
   id = notify_id_;
}

// set the notify enterprise
void Pdu::set_notify_enterprise( const Oid &enterprise)
{
   notify_enterprise_ = enterprise;
}

// get the notify enterprise
void Pdu::get_notify_enterprise( Oid & enterprise) const
{
    enterprise = notify_enterprise_;
}

// ------   class VbIter -------------------
VbIter::VbIter(Pdu& pdu): idx_(-1), pdu_(&pdu)
{
}

// returns 1 if ok, else 0 if none left
int VbIter::next(Vb& vb)
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
