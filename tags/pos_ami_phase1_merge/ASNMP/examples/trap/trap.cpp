// $Id$

// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    trap.cpp
//
// = DESCRIPTION
//  Sample application demonstrating synchronous Snmp::trap API  
//  to send to an SNMP Version 1 trap listener app.
//
// = AUTHOR
//  Peter E. Mellquist original code
//  Michael R MacFaden mrm@cisco.com rework OO, API/ACE integration
//
// ============================================================================
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
#define DEFINE_TRAP_CONSTANTS_
#include "asnmp/enttraps.h" // enterprise standard traps
#include "ace/Get_Opt.h"

ACE_RCSID(trap, trap, "$Id$")

//
// SNMPv1 Trap Application
//
class trapapp {
  public:
  trapapp(int argc, char **argv); // process command line args
  int valid() const;             // verify transaction can proceed
  int run();                     //  issue transaction
  static void usage();           // operator help message

  private: 
  trapapp(const trapapp&);

  UdpAddress address_;
  Pdu pdu_;                                // construct a request Pdu
  Oid oid_;
  OctetStr community_;
  Snmp snmp_;
  UdpTarget target_;
  int valid_;
};


// main entry point 
int main( int argc, char *argv[])  
{
  trapapp get(argc, argv);
  if (get.valid())
     return get.run();
  else
    trapapp::usage();
  return 1;
}

trapapp::valid() const 
{ 
 return valid_; 
}
trapapp::trapapp(int argc, char *argv[]): valid_(0)
{
   Oid def_ent_oid("1.3.6.1.2.1.1.1.2.0.1");      // def enterprise oid 
   Oid ent, trap; // user specified values

   if ( argc < 2)  // hostname mandatory
     return; 
    
   address_ = argv[argc - 1];
   if ( !address_.valid()) {
      cout << "ERROR: Invalid IPv4 address or DNS hostname: " \
     << argv[argc] << "\n";
      return;
   }

   ACE_Get_Opt get_opt (argc, argv, "c:e:t:");
   for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'c': // community string
         community_ = get_opt.optarg;
         target_.set_read_community(community_);
         break;

       case 'e': // trap oid to send
         ent = get_opt.optarg;
         break;

       case 't': // trap oid
         trap = get_opt.optarg;
         break;;

       default:
         break;
       }

   if (ent.valid())
      pdu_.set_notify_enterprise( ent); // set up the enterprise of the trap
   else
      pdu_.set_notify_enterprise( def_ent_oid);

  if (trap.valid())
     pdu_.set_notify_id( trap);          // set the id of the trap
  else
     pdu_.set_notify_id( coldStart);      // set the id of the trap

  Oid detail_oid("1.3.6.1.4.1.11.2.16.2");
  OctetStr detail_value("SNMP++ Trap Send Test");
  Vb vb(detail_oid, detail_value);
  pdu_ += vb;

  pdu_.get_notify_id(oid_); // store for later use
  valid_ = 1;
}

void trapapp::usage()
{
  cout << "Usage:\n";
  cout << "trap [options] dotted-quad | DNSName[:port]\n";
  cout << "      -c Community_name, default is 'public' \n";
  cout << "      -r N  retries default is N = 1 retry\n";
  cout << "      -t N  timeout in seconds default is 1 second" << endl;
  cout << "      -e oid  enterprise oid default is 1.3.6.1.2.1.1.1.2.0.1\n";
  cout << "      -O oid  trap id default is coldStart 1.3.6.1.6.3.1.1.5.1\n";
}

int trapapp::run()
{ 
   if ( snmp_.valid() != SNMP_CLASS_SUCCESS) {
      cout << "\nASNMP:ERROR:Create session failed: "<< 
          snmp_.error_string()<< "\n";
      return 1;
   }

   if (address_.get_port() == 0)
     address_.set_port(DEF_TRAP_PORT);
   target_.set_address( address_);         // make a target using the address

   //-------[ issue the request, blocked mode ]-----------------------------
   cout << "\nASNMP:INFO:SNMP Version " << (target_.get_version()+ 1) << \
       " TRAP GENERATOR SAMPLE PROGRAM \nOID: " << oid_.to_string() << "\n";
   target_.get_address(address_); // target updates port used
   int rc;
   char *name = address_.resolve_hostname(rc);
   if (rc)
      name = "<< did not resolve via gethostbyname() >>";

   cout << "Device: " << address_ << " " << name << "\n"; 
   cout << "[ Community=" <<  community_.to_string() << " ]"<< endl;

   if (snmp_.trap( pdu_, target_) == SNMP_CLASS_SUCCESS) {
     cout << "Trap was written to network...\n";
   }
   else {
    char *ptr = snmp_.error_string();
    cout << "ASNMP:ERROR: trap command failed reason: " << ptr << endl;
  }

  cout << "ASNMP:INFO:command completed normally.\n"<< endl;
  return 0;
} 

