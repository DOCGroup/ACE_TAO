// $Id$

#include "ior-handler.h"

IorHandler::IorHandler (void)
{
}

u_long
IorHandler::getOctet8Field (char *readPtr, int *hexCharsRead)
{
  char octet8Holder[8];

  for (int i = 0; i < 8; i++)
    octet8Holder[i] = readPtr[i];

  hexCharsRead[0] = 8;

  u_long value =
    16 * ACE::hex2byte (octet8Holder[6])
    + ACE::hex2byte (octet8Holder[7]);

  return value;
}

u_long
IorHandler::getOctet4Field (char *readPtr, int *hexCharsRead)
{
  char octet4Holder[4];

  for (int i = 0; i < 4; i++)
    octet4Holder[i] = readPtr[i];

  hexCharsRead[0] = 4;

  u_long value =
    16 * 16 * 16 * ACE::hex2byte (octet4Holder[0])
    + 16 * 16 * ACE::hex2byte (octet4Holder[1])
    + 16 * ACE::hex2byte (octet4Holder[2])
    + ACE::hex2byte (octet4Holder[3]);

  return value;
}

u_long
IorHandler::getOctet2Field (char *readPtr, int *hexCharsRead)
{
  char octet2Holder[2];

  for (int i = 0; i < 2; i++)
    octet2Holder[i] = readPtr[i];

  hexCharsRead[0] = 2;

  u_long value =
    16 * ACE::hex2byte (octet2Holder[0])
    + ACE::hex2byte (octet2Holder[1]);

  return value;
}

void
IorHandler::skipNullOctets (char *readPtr, int *hexCharsRead)
{
  char nullOctet[2];
  int offset;

  hexCharsRead[0] = 0;
  offset = 0;

  // There sometimes occurs a null padding of 2 octets after strings
  // such as the type_id in order to ensure even number of octets.

  while (1)
    {
      nullOctet[0] = readPtr[offset];
      nullOctet[1] = readPtr[offset + 1];
      if (nullOctet[0] == '0' && nullOctet[1] == '0')
        offset += 2;
      else
        break;
    }

  hexCharsRead[0] = offset;
}

char *
IorHandler::getString (char *readPtr, int givenLen)
{
  int j = 0;

  // i indexes hexChars while j indexes octet pairs

  for (int i = 0;
       i <= givenLen - 2;
       i += 2)
    {
      char octetPair[2];
      octetPair[0] = readPtr[i];
      octetPair[1] = readPtr[i + 1];

      int intEquiv =
        16 * ACE::hex2byte (octetPair[0])
        + ACE::hex2byte (octetPair[1]);

      char parsedOctetPair[2];
      sprintf (parsedOctetPair,
               "%c",
               intEquiv);

      parsedStr[j] = parsedOctetPair[0];
      j++;
    }

  return parsedStr;
}

void
IorHandler::prettyPrintIOR (IOR thisIor)
{
  ACE_DEBUG ((LM_DEBUG,
              "TypeIdLen\t: %lu bytes\n",
              thisIor.typeIdLen));
  ACE_DEBUG ((LM_DEBUG,
              "TypeId\t\t: %s\n",
              thisIor.typeId));
  ACE_DEBUG ((LM_DEBUG,
              "IDL Interface\t: %s\n",
              thisIor.idlInterface));
  ACE_DEBUG ((LM_DEBUG,
              "ProfileBodyLen\t: %lu bytes\n",
              thisIor.profileBodyLen));
  ACE_DEBUG ((LM_DEBUG,
              "HostLen\t\t: %lu bytes\n",
              thisIor.hostLen));
  ACE_DEBUG ((LM_DEBUG,
              "HostName\t: %s\n",
              thisIor.HostName));
  ACE_DEBUG ((LM_DEBUG,
              "Port Number\t: %d\n",
              thisIor.portNum));
  ACE_DEBUG ((LM_DEBUG,
              "ObjectKeyLen\t: %lu bytes\n",
              thisIor.objectKeyLen));
  ACE_DEBUG ((LM_DEBUG,
              "ObjectKey\t: %s\n",
              thisIor.objectKey));
}

void
IorHandler::interpretIor (char *thisIor, IOR *thisIorInfo)
{
  int numCharsToSkip;

  // Skip the prefix "IOR:"
  int numHexCharsRead = 4;

  skipNullOctets ((char *) (thisIor + numHexCharsRead), &numCharsToSkip);
  numHexCharsRead += numCharsToSkip;

  int ulongValue = getOctet2Field ((char *) (thisIor + numHexCharsRead),
                                   &numCharsToSkip);

  // Read the length of the type_id field
  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<%d hexChars read>: type_id len seems to be NULL \n",
                  numHexCharsRead));
      ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;
  thisIorInfo->typeIdLen = ulongValue;

  // Read the type_id and store it
  ACE_OS::strncpy (thisIorInfo->typeId,
                   getString ((char *) (thisIor + numHexCharsRead),
                              2 * thisIorInfo->typeIdLen),
                   thisIorInfo->typeIdLen);
  numHexCharsRead += 2 * thisIorInfo->typeIdLen;

  // While we have the type_id, we may as well extract the IDL
  // interface name from it.
  ACE_OS::strcpy (thisIorInfo->idlInterface,
                  getIdlInterface (thisIorInfo->typeId));
  ACE_DEBUG ((LM_DEBUG,
              "\nTypeId\t\t: %s \n",
              thisIorInfo->typeId));
  ACE_DEBUG ((LM_DEBUG,
              "IDL Interface\t: %s\n",
              thisIorInfo->idlInterface));

  skipNullOctets ((char *) (thisIor + numHexCharsRead), &numCharsToSkip);
  numHexCharsRead += numCharsToSkip;

  ulongValue = getOctet2Field ((char *) (thisIor + numHexCharsRead),
                               &numCharsToSkip);

  // Read the 4 octets, which should equal 1 (numTaggedProfiles = 1)
  if (ulongValue != 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<%d hexChars read>: numTaggedProfiles != 1\n",
                  numHexCharsRead));
      ACE_OS::exit (1);
    }
  numHexCharsRead += numCharsToSkip;

  ulongValue = getOctet8Field ((char *) (thisIor + numHexCharsRead),
                               &numCharsToSkip);

  // Read the 4 octets, which should equal 0 (TAG_INTERNET_IOP = 0)

  if (ulongValue != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<%d hexChars read>: TAG_INTERNET_IOP != 0\n",
                  numHexCharsRead));
      ACE_OS::exit (1);
    }
  numHexCharsRead += numCharsToSkip;

  skipNullOctets ((char *) (thisIor + numHexCharsRead),
                  &numCharsToSkip);
  numHexCharsRead += numCharsToSkip;

  // Since the object_key and the hostname are part of the ProfileBody
  // field of the IOR, and this is the part that needs to be changed,
  // the IOR should be cut here typically if we want to fake it.
  cutAndPasteHere = numHexCharsRead;

  ulongValue = getOctet2Field ((char *) (thisIor + numHexCharsRead),
                               &numCharsToSkip);

  // Read the 4 octets, which represent the length of the ProfileBody
  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<%d hexChars read>: ProfileBody len equals NULL\n",
                  numHexCharsRead));
      ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;
  thisIorInfo->profileBodyLen = ulongValue;

  ACE_DEBUG ((LM_DEBUG,
              "\nTAG_INTERNET_IOP Profile:\n"));

  ulongValue = getOctet4Field ((char *) (thisIor + numHexCharsRead),
                               &numCharsToSkip);

  // Read the 4 octets, which represent the IIOP version number = 1
  if (ulongValue != 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<%d hexChars read>: IIOP version != 1\n",
                  numHexCharsRead));
      ACE_OS::exit (1);
    }
  numHexCharsRead += numCharsToSkip;

  skipNullOctets ((char *) (thisIor + numHexCharsRead), &numCharsToSkip);
  numHexCharsRead += numCharsToSkip;

  ulongValue = getOctet2Field ((char *) (thisIor + numHexCharsRead),
                               &numCharsToSkip);

  // Read the 2 octets, which represent the length of the hostname
  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<%d hexChars read>: hostLen equals NULL\n",
                  numHexCharsRead));
      ACE_OS::exit (1);
    }
  numHexCharsRead += numCharsToSkip;
  thisIorInfo->hostLen = ulongValue;

  // Read the hostname and store it
  ACE_OS::strncpy (thisIorInfo->HostName,
                   getString ((char *) (thisIor + numHexCharsRead),
                              2 * thisIorInfo->hostLen),
                   thisIorInfo->hostLen);
  numHexCharsRead += 2 * thisIorInfo->hostLen;

  ACE_DEBUG ((LM_DEBUG,
              "    HostName   : %s\n",
              thisIorInfo->HostName));

  skipNullOctets ((char *) (thisIor + numHexCharsRead),
                  &numCharsToSkip);
  numHexCharsRead += numCharsToSkip;

  ulongValue = getOctet4Field ((char *) (thisIor + numHexCharsRead),
                               &numCharsToSkip);

  // Read the port number and store it
  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<%d hexChars read>: port number equals NULL\n",
                  numHexCharsRead));
      ACE_OS::exit (1);
    }
  numHexCharsRead += numCharsToSkip;
  thisIorInfo->portNum = ulongValue;
  ACE_DEBUG ((LM_DEBUG,
              "    Port Number: %d\n",
              thisIorInfo->portNum));

  skipNullOctets ((char *) (thisIor + numHexCharsRead),
                  &numCharsToSkip);
  numHexCharsRead += numCharsToSkip;

  ulongValue = getOctet2Field ((char *) (thisIor + numHexCharsRead),
                               &numCharsToSkip);

  // Read the object key length and store it
  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "<%d hexChars read>: objectKeyLen equals NULL\n",
                  numHexCharsRead));
      ACE_OS::exit (1);
    }
  numHexCharsRead += numCharsToSkip;
  thisIorInfo->objectKeyLen = ulongValue;

  // Read the object_key and store it
  ACE_OS::strncpy (thisIorInfo->objectKey,
                   getString ((char *) (thisIor + numHexCharsRead),
                              2 * thisIorInfo->objectKeyLen),
                   thisIorInfo->objectKeyLen);
  numHexCharsRead += 2 * thisIorInfo->objectKeyLen;

  ACE_DEBUG ((LM_DEBUG,
              "    ObjectKey  : %s \n\n",
              thisIorInfo->objectKey));

  // Pretty print the IOR with more debugging information
  // prettyPrintIOR (*thisIorInfo);
}

char *
IorHandler::getIdlInterface (char *typeId)
{
  int lenInterface;

  char *readStart = strchr (typeId, ':');

   // A sample type_id for an IDL interface name "EchoTests" is
   // IDL:EchoTests:1.0 => the trick is to isolate the parts between
   // the two colons.

  if (readStart == NULL)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "getIdlInterface: type_id contains no starting :\n"));
      ACE_OS::exit (1);
    }

  char *readEnd = strrchr (typeId, ':');

  if (readEnd == NULL)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "getIdlInterface: type_id contains no ending:\n"));
      ACE_OS::exit (1);
    }

  // Now, count the number of bytes between the two colons.
  lenInterface = readEnd - readStart - 1;

  // Copy the IDL interface part of the type_id.
  ACE_OS::strncpy ((char *) idlInterface,
                   readStart + 1,
                   lenInterface);
  idlInterface[lenInterface] = '\0';

  return (char *) idlInterface;
}

void
IorHandler::readIorFromFile (char *filename)
{
  FILE *fp = ACE_OS::fopen (filename, "r");

  // Read the real IOR from the file REAL_IOR_FILE.
  if (fp == NULL)
    {
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "Unable to open file"));
      ACE_OS::exit (1);
    }

  fscanf (fp,
          "%s",
          stringRealIOR);
  ACE_OS::fclose (fp);

  interpretIor (stringRealIOR, &parsedRealIOR);
}
