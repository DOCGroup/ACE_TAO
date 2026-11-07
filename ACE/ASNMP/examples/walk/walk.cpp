
//=============================================================================
/**
 *  @file    walk.cpp
 *
 *  Sample application demonstrating synchronous Snmp::get, get_next API
 *  to access an SNMP Version 1 agent.
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
#include "ace/Argv_Type_Converter.h"
#include "ace/Get_Opt.h"
// FUZZ: disable check_for_streams_include
#include "ace/streams.h"

//
// SNMPv1 Walk Mib Application
//
class walkapp
{
public:
  walkapp (int argc, char **argv); // process command line args
  bool valid () const;            // verify transaction can proceed
  int run ();                     // issue transaction
  static void usage ();           // operator help message

private:
  walkapp (walkapp const &) = delete;
  walkapp (walkapp &&) = delete;
  walkapp &operator= (walkapp const &) = delete;
  walkapp &operator= (walkapp &&) = delete;

  UdpAddress address_;
  Pdu pdu_;                                // construct a request Pdu
  Oid oid_;
  OctetStr community_;
  Snmp snmp_;
  UdpTarget target_;
  bool valid_ {};
};

// main entry point
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Argv_Type_Converter atc (argc, argv);
  walkapp get (atc.get_argc (), atc.get_ASCII_argv ());
  if (get.valid ())
     return get.run ();
  else
    walkapp::usage();
  return 1;
}

bool walkapp::valid () const
{
  return this->valid_;
}

walkapp::walkapp (int argc, char *argv[])
{
   Oid req {};
   Oid def_oid ("1.3.6.1.2.1.1.1.0"); // default begin walk with MIBII
   if ( argc < 2)
     return;

   address_ = argv[argc - 1];
   if (!address_.valid ()) {
      cout << "ERROR: Invalid IPv4 address or DNS hostname: " \
     << argv[argc - 1] << "\n";
      return;
   }

   ACE_Argv_Type_Converter to_tchar (argc, argv);
   ACE_Get_Opt get_opt (argc,
                        to_tchar.get_TCHAR_argv (),
                        ACE_TEXT ("o:c:r:t:"));
   for (int option = get_opt (); option != -1; option = get_opt ())
     switch (option)
       {
       case 'o':
         req = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
         if (req.valid () == 0)
         cout << "ERROR: oid value: "
              << ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ())
              << "is not valid. using default.\n";
         break;

       case 'c':
         community_ = ACE_TEXT_ALWAYS_CHAR (get_opt.opt_arg ());
         target_.set_read_community (community_);
         break;

       case 'r':
         target_.set_retry (ACE_OS::atoi (get_opt.opt_arg ()));
         break;

       case 't':
         target_.set_timeout (ACE_OS::atoi (get_opt.opt_arg ()));
         break;

       default:
         break;
       }

  Vb vb {}; // construct a Vb object
  if (req.valid ())
     vb.set_oid (req); // set the Oid portion of the Vb
  else {
     vb.set_oid (def_oid); // set the Oid portion of the Vb
  }
  pdu_ += vb;
  vb.get_oid (oid_); // store for later use
  this->valid_ = true;
}

void walkapp::usage ()
{
  cout << "Usage:\n";
  cout << "walk [options] dotted-quad | DNSName[:port]\n";
  cout << "      -o OID starts with oid after 1.3.6.1.2.1.1.1.0 (mibII sysDescr.0)\n";
  cout << "      -c Community_name, default is 'public'\n";
  cout << "      -r N  retries default is N = 1 retry\n";
  cout << "      -t N  timeout in seconds default is 1 second" << endl;
}

//
// simple mib iterator class
//
class MibIter
{
public:
   // Pdu must contain initial oid to begin with
   MibIter (Snmp *snmp, Pdu& pdu, UdpTarget *target);
   int next (Vb& vb, char const *&err_reason); // return next oid in mib

 private:
  MibIter (MibIter const &) = delete;
  MibIter (MibIter &&) = delete;
  MibIter &operator= (MibIter const &) = delete;
  MibIter &operator= (MibIter &&) = delete;

  // These pointers refer to the session and target owned by walkapp.
  Snmp * const snmp_;
  UdpTarget * const target_;
  Pdu pdu_;
  Vb vb_;
  bool first_ {}; // flag to obtain first entry
  bool valid_ {}; // flag to obtain first entry
};

MibIter::MibIter (Snmp *snmp, Pdu& pdu, UdpTarget *target)
  : snmp_ (snmp), target_ (target), pdu_ (pdu)
{
  // verify we have a valid oid to begin iterating with
  Oid oid {};
  Vb vb {};
  pdu.get_vb (vb, 0);
  vb.get_oid (oid);
  this->valid_ = oid.valid ();
}

// return vb of next oid in agent tree, return 1 else return 0, reason set
int MibIter::next (Vb& vb, char const *&reason)
{
  int rc {};

  if (!this->valid_) // not valid object
     return -1;

  // 1. poll for value
  if (!this->first_) {
    rc = this->snmp_->get (pdu_, *this->target_);
    this->first_ = true;
  }
  else {
   rc = this->snmp_->get_next (pdu_, *this->target_);
  }

  if (rc != SNMP_CLASS_SUCCESS) {
       reason = this->snmp_->error_string ();
       return 0;
  }

  // 2. check for problems
  if (this->pdu_.get_error_status ()) {
     reason = this->pdu_.agent_error_reason ();
     return 0;
  }

  // 3. return vb to caller
  this->pdu_.get_vb (vb, 0);
  Oid nextoid {};
  vb.get_oid (nextoid); // and setup next oid to get
  Vb nextvb (nextoid);
  this->pdu_.delete_all_vbs ();
  this->pdu_ += nextvb; // can't do set_vb as there are no entries to replace

  return 1; // ok
}

int walkapp::run ()
{
   //----------[ create a ASNMP session ]-----------------------------------
   if (snmp_.valid () != SNMP_CLASS_SUCCESS) {
      cout << "\nASNMP:ERROR:Create session failed: "<<
          snmp_.error_string()<< "\n";
      return 1;
   }

   //--------[ build up ASNMP object needed ]-------------------------------
   if (address_.get_port () == 0)
     address_.set_port (DEF_AGENT_PORT);
   target_.set_address (address_); // make a target using the address

   //-------[ issue the request, blocked mode ]-----------------------------
   cout << "\nASNMP:INFO:SNMP Version " << (target_.get_version () + 1) << \
       " WALK SAMPLE PROGRAM \nOID: " << oid_.to_string() << "\n";
   target_.get_address (address_); // target updates port used
   int rc {};
   char const * const name = address_.resolve_hostname (rc);

   cout << "Device: " << address_ << " ";

   //FUZZ: disable check_for_lack_ACE_OS
   cout << (rc ? "<< did not resolve via gethostbyname() >>" : name) << "\n";
   //FUZZ: enable check_for_lack_ACE_OS

   cout << "[ Retries=" << target_.get_retry() << " \
        Timeout=" << target_.get_timeout() <<" ms " << "Community=" << \
         community_.to_string() << " ]"<< endl;

   MibIter iter (&snmp_, pdu_, &target_);
   char const *err_str {};
   Vb vb {};
   while (iter.next (vb, err_str))
     {
     cout << "\tOid = " << vb.to_string_oid() << "\n";
     cout << "\tValue = " << vb.to_string_value() << "\n";
     }

   if (err_str)
     {
     cout << "ERROR: walk: " << err_str << endl;
     return 0;
     }

  cout << "ASNMP:INFO:command completed normally. ACE Rocks...\n"<< endl;
  return 0;
}

