/* -*-C++-*- */
#ifndef OID_DEF
#define OID_DEF
/**
 *  @file    oid_def.h
 *
 * Well known Oids defined in RFC 1215
 *
 *  @author Peter E Mellquist
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

/// SMI trap OID definition.
class snmpTrapsOid: public Oid {
public:
  snmpTrapsOid ():Oid("1.3.6.1.6.3.1.1.5"){};
};

/// SMI enterprise trap OID.
class snmpTrapEnterpriseOid: public Oid {
public:
  snmpTrapEnterpriseOid():Oid("1.3.6.1.6.3.1.1.4.3.0"){};
};

/// SMI cold start OID.
class coldStartOid: public snmpTrapsOid {
public:
  coldStartOid(){*this+=".1";};
};

/// SMI warm start OID.
class warmStartOid: public snmpTrapsOid {
public:
  warmStartOid(){*this+=".2";};
};

/// SMI link down OID.
class linkDownOid: public snmpTrapsOid {
public:
  linkDownOid(){*this+=".3";};
};

/// SMI link up OID.
class linkUpOid: public snmpTrapsOid {
public:
  linkUpOid(){*this+=".4";};
};

/// SMI authentication failure OID.
class authenticationFailureOid: public snmpTrapsOid {
public:
  authenticationFailureOid(){*this+=".5";};
};

/// SMI EGP neighbor loss OID.
class egpNeighborLossOid: public snmpTrapsOid {
public:
  egpNeighborLossOid(){*this+=".6";};
};

#endif // OID_DEF
