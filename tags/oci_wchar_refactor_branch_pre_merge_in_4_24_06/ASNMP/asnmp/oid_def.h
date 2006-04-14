/* -*-C++-*- */
// $Id$
#ifndef OID_DEF
#define OID_DEF
// ============================================================================
//
// = LIBRARY
//    asnmp
//
// = FILENAME
//    address.cpp
//
// = DESCRIPTION
//   Well known Oids defined in RFC 1215
//
// = AUTHOR
//    Peter E Mellquist
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

// SMI trap oid def
class snmpTrapsOid: public Oid {
   public:
   snmpTrapsOid (void):Oid("1.3.6.1.6.3.1.1.5"){};
};

// SMI Enterprose Oid
class snmpTrapEnterpriseOid: public Oid {
   public:
   snmpTrapEnterpriseOid(void):Oid("1.3.6.1.6.3.1.1.4.3.0"){};
};

// SMI Cold Start Oid
class coldStartOid: public snmpTrapsOid {
   public:
   coldStartOid( void){*this+=".1";};
};

// SMI WarmStart Oid
class warmStartOid: public snmpTrapsOid {
   public:
   warmStartOid( void){*this+=".2";};
};

// SMI LinkDown Oid
class linkDownOid: public snmpTrapsOid {
   public:
   linkDownOid( void){*this+=".3";};
};


// SMI LinkUp Oid
class linkUpOid: public snmpTrapsOid {
   public:
   linkUpOid( void){*this+=".4";};
};

// SMI Authentication Failure Oid
class authenticationFailureOid: public snmpTrapsOid {
   public:
   authenticationFailureOid( void){*this+=".5";};
};

// SMI egpneighborloss Oid
class egpNeighborLossOid: public snmpTrapsOid {
   public:
   egpNeighborLossOid( void){*this+=".6";};
};


#endif // OID_DEF
