// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/utils/IOR-parser
//
// = FILENAME
//    ior-handler.h
//
// = DESCRIPTION
//    Provides the definition of a class that parses real (valid) IORs.
//
// = AUTHORS
//    Priya Narasimhan <priya@lambda.ece.ucsb.edu>
//
// ============================================================================

#if !defined (__IORPARSER_H__)
#define __IORPARSER_H__

#include "ace/OS.h"

// Maximum length of either the type_id, the ProfileBody or the
// object_key fields. Tentatively assigned.
#define MAX_IOR_FIELD_LEN   200
#define MAX_TYPE_ID_LEN     100

// These are CDR-encoed sequences of hexChars (note, not octets) that
// are useful in creating IORs
#define NULL_HEXCHARS       "00000000"
#define NUM_TAG_PROFS       "0001"
#define IIOP_VERSION        "0001"

class IOR
{
  // = TITLE
  //     This is the useful information obtained from parsing an IOR.
  //
  // = DESCRIPTION
  //     This structure assumes that the profile_id is
  //     TAG_INTERNET_IOP and that there is only one TaggedProfile in 
  //     the IOR.
public:
  // @@ Priya, can you please add comments to these fields?

  u_long typeIdLen;

  char typeId[MAX_TYPE_ID_LEN];

  char idlInterface[MAX_TYPE_ID_LEN];

  u_long profileBodyLen;

  u_long hostLen;

  char HostName[32];

  u_long portNum;

  u_long objectKeyLen;

  char objectKey[100];
};

class IorHandler
{
  // = TITLE 
  //     This is the class that takes in a real (valid) IOR from a
  //     server and patches it with the Replication Manager's port
  //     number and IP address, while leaving the IDL interface part
  //     unchanged.
  //
  // = DESCRIPTION
  //     This class ensures that the Replication Manager will become
  //     the point of contact when a client wishes to contact the
  //     actual server.
public:
  // @@ Priya, can you please add comments to these methods?
  IorHandler (void);
  void prettyPrintIOR (struct IOR thisIor);
  void interpretIor (char *thisIor, struct IOR *thisIorInfo);
  char *getIdlInterface (char *typeId);
  void readIorFromFile (char *filename);

  int cutAndPasteHere;
  char stringRealIOR[400];
  struct IOR parsedRealIOR;

private:
  int hexChar2int (char thisChar);
  u_long getOctet8Field (char *readPtr, int *hexCharsRead);
  u_long getOctet4Field (char *readPtr, int *hexCharsRead);
  u_long getOctet2Field (char *readPtr, int *hexCharsRead);
  void skipNullOctets (char *readPtr, int *hexCharsRead);
  char *getString (char *readPtr, int givenLen);
};

#endif /* __IORPARSER_H__ */
