// $Id$

#include "ior-handler.h"
#include "ace/Log_Msg.h"

IorHandler::IorHandler (void)
{
}

int
IorHandler::hexChar2int (char thisChar)
{
  switch (thisChar)
    {
    case '0': return (0);
    case '1': return (1);
    case '2': return (2);
    case '3': return (3);
    case '4': return (4);
    case '5': return (5);
    case '6': return (6);
    case '7': return (7);
    case '8': return (8);
    case '9': return (9);
    case 'a': return (10);
    case 'b': return (11);
    case 'c': return (12);
    case 'd': return (13);
    case 'e': return (14);
    case 'f': return (15);
    }
  return -1;
}

u_long
IorHandler::getOctet8Field (char *readPtr, int *hexCharsRead)
{
  char octet8Holder[8];
  u_long value;
  int i;

  for (i = 0; i < 8; i++)
    octet8Holder[i] = * (readPtr + i);

  *hexCharsRead = 8;

  value = 16*hexChar2int (octet8Holder[6]) + hexChar2int (octet8Holder[7]);
  return (value);
}

u_long
IorHandler::getOctet4Field (char *readPtr, int *hexCharsRead)
{
  char octet4Holder[4];
  u_long value;
  int i;

  for (i = 0; i < 4; i++)
    octet4Holder[i] = * (readPtr + i);

  *hexCharsRead = 4;

#ifdef ACE_BIG_ENDIAN
  // On a big endian platform, the MSB is first and the LSB is next
  value = 16*16*16* hexChar2int (octet4Holder[0]) +
    16*16* hexChar2int (octet4Holder[1]) +
    16* hexChar2int (octet4Holder[2]) +
    hexChar2int (octet4Holder[3]);
#endif


#ifdef ACE_LITTLE_ENDIAN
  // On a little endian platform, the LSB is first and the MSB is next
  value = 16*16*16* hexChar2int (octet4Holder[2]) +
    16*16* hexChar2int (octet4Holder[3]) +
    16* hexChar2int (octet4Holder[0]) +
    hexChar2int (octet4Holder[1]);
#endif


  return (value);
}

u_long
IorHandler::getOctet2Field (char *readPtr, int *hexCharsRead)
{
  char octet2Holder[2];
  u_long value;
  int i;

  for (i = 0; i < 2; i++)
    octet2Holder[i] = * (readPtr + i);

  *hexCharsRead = 2;

  value = 16 * hexChar2int (octet2Holder[0]) +  hexChar2int (octet2Holder[1]);
  return (value);
}

void
IorHandler::skipSpaceIfAny(char *readPtr, int *hexCharsRead)
{
  char nullOctet[2];
  int offset;

  *hexCharsRead = 0;
  offset = 0;

  // Some ORBs tend to insert a single space character, ie, ASCII
  // equivalent 20, at the end of certain fields in the IOR. Use
  // skipSpaceIfAny() with care since in certain cases, it might
  // actually represent an unsigned long.

  nullOctet[0] = *(readPtr + offset);
  nullOctet[1] = *(readPtr + offset + 1);

  if ((nullOctet[0] == '2') && (nullOctet[1] == '0'))
    offset += 2;

  *hexCharsRead = offset;
}

void
IorHandler::skipNullOctets (char *readPtr, int *hexCharsRead, int expectingStr)
{
  char nullOctet[4];
  int offset;

  *hexCharsRead = 0;
  offset = 0;

  // There sometimes occurs a null padding of 2 octets after strings
  // such as the type_id in order to ensure even number of octets.

  while (1)
    {
      nullOctet[0] = *(readPtr + offset);
      nullOctet[1] = *(readPtr + offset + 1);
      nullOctet[2] = *(readPtr + offset + 2);
      nullOctet[3] = *(readPtr + offset + 3);

      // Some ORBs tend to pad the IORs with double space characters,
      // ie., 2020 in ASCII equivalent

      if ((nullOctet[0] == '2') && (nullOctet[1] == '0') &&
          (nullOctet[2] == '2') && (nullOctet[3] == '0'))
        {
          offset += 4;
          continue;
        }

      if ((nullOctet[0] == '0') && (nullOctet[1] == '0'))
        offset += 2;
      else
        if ((expectingStr) && (hexChar2int(nullOctet[0]) >= 8))
          offset += 2;
        else
          break;
    }

  *hexCharsRead = offset;
}

int
IorHandler::findIfVisiIor(char *readPtr, int *hexCharsRead)
{
  int offset;
  int hasVisiPreamble = 0;

  *hexCharsRead = 0;
  offset = 0;

  if ((getCharacter(readPtr, &offset) == 'P') &&
      (getCharacter(readPtr, &offset) == 'M') &&
      (getCharacter(readPtr, &offset) == 'C'))
    {
      hasVisiPreamble = 1;
      *hexCharsRead = offset;
    }

  return (hasVisiPreamble);
}

char
IorHandler::getCharacter (char *readPtr, int *offset)
{
  char octetPair[2];
  int intEquiv;

  octetPair[0] = *(readPtr + *offset);
  octetPair[1] = *(readPtr + *offset + 1);

  intEquiv = 16 * hexChar2int(octetPair[0]) + hexChar2int(octetPair[1]);
  *offset += 2;

  return ((char) intEquiv);
}

char *
IorHandler::getString (char *readPtr, int givenLen)
{
  static char parsedStr[MAX_IOR_FIELD_LEN];
  char octetPair[2];
  char parsedOctetPair[2];
  int intEquiv;
  int i = 0;
  int j = 0;

  // i indexes hexChars while j indexes octet pairs

  while (i <= (givenLen - 2))
    {
      octetPair[0] = * (readPtr + i);
      octetPair[1] = * (readPtr + i + 1);

      intEquiv = 16 * hexChar2int (octetPair[0]) + hexChar2int (octetPair[1]);
      sprintf (parsedOctetPair, "%c", intEquiv);
      parsedStr[j] = parsedOctetPair[0];
      j ++;

      i += 2;
    }

  return (parsedStr);
}

void
IorHandler::interpretIor (char *thisIor, struct IOR_Manager *thisIorInfo)
{
  int numCharsToSkip;
  int validTypeId = 0;
  int isVisiIor = 0;
  int ulongValue;

  // Skip the prefix "IOR:"
  int numHexCharsRead = 4;

  // Type ID must be at least 4 bytes in length since type_id is prefixed
  // by the string "IDL:"
  int validTypeIdLen = 0;

  while (!validTypeIdLen)
    {
      skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 1);
      numHexCharsRead += numCharsToSkip;

      // Read the length of the type_id field
      ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                                  &numCharsToSkip);

      if (ulongValue == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "type_id len seems to be 0 \n"));
          ACE_OS::exit (1);
        }

      numHexCharsRead += numCharsToSkip;

      if (ulongValue > 4)
        {
          validTypeIdLen = 1;
          thisIorInfo->typeIdLen = ulongValue;
        }

      skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 1);
      numHexCharsRead += numCharsToSkip;

      // Read the Type ID
      ACE_OS::strncpy (thisIorInfo->typeId,
                       getString((char *)(thisIor + numHexCharsRead),
                                 2 * thisIorInfo->typeIdLen),
                       thisIorInfo->typeIdLen);

      // Confirm that this is a valid TypeId by looking for the IDL
      // interface. Additionally, the IDL interface could be extracted
      // and used for other purposes.
      getIdlInterface(thisIorInfo->typeId, &validTypeId);

      if (!validTypeId)
        {
          validTypeIdLen = 0;
          numHexCharsRead -= numCharsToSkip;
        }
    }

  numHexCharsRead += 2 * thisIorInfo->typeIdLen;

  ACE_DEBUG ((LM_DEBUG,
              "\nTypeId\t\t: %s (%d bytes)\n",
              thisIorInfo->typeId,
              thisIorInfo->typeIdLen));

  skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 0);
  numHexCharsRead += numCharsToSkip;

  // Read the 4 octets, which should equal 1 (numTaggedProfiles = 1)
  // for a single-component profile.
  ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                              &numCharsToSkip);

  if (ulongValue != 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "numTaggedProfiles = %d\n",
                  ulongValue));
     ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;

  // Read the 4 octets, which should equal 0 (TAG_INTERNET_IOP = 0)
  ulongValue = getOctet8Field((char *)(thisIor + numHexCharsRead),
                              &numCharsToSkip);

  if (ulongValue != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAG_INTERNET_IOP != 0\n"));
      ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;

  skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 0);
  numHexCharsRead += numCharsToSkip;

  // Read the 4 octets, which represent the length of the ProfileBody
  ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                              &numCharsToSkip);

  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ProfileBody len equals 0\n"));
      ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;
  thisIorInfo->profileBodyLen = ulongValue;

  ACE_DEBUG ((LM_DEBUG,
              "\nTAG_INTERNET_IOP Profile (%d bytes):\n",
              ulongValue));

  skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 1);
  numHexCharsRead += numCharsToSkip;

  // Read the 4 octets, which represent the IIOP version number = 1
  // Read the major number of the IIOP Version (should be 1)
  ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                              &numCharsToSkip);

  if (ulongValue != 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "IIOP major version != 1\n"));
      ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;
  ACE_DEBUG ((LM_DEBUG,
              "IIOP Version: %d.",
              ulongValue));

  // Read the minor number of the IIOP Version (can be 0 or 1)
  ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                              &numCharsToSkip);

  if ((ulongValue != 0) && (ulongValue != 1) && (ulongValue != 2))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "IIOP minor version != 0, 1, or 2\n"));
     ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;
  ACE_DEBUG ((LM_DEBUG,
              "%d\n",
              ulongValue));

  skipSpaceIfAny((char *)(thisIor + numHexCharsRead), &numCharsToSkip);
  numHexCharsRead += numCharsToSkip;
  skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 0);
  numHexCharsRead += numCharsToSkip;

  // Read the 2 octets, which represent the length of the hostname
  ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                              &numCharsToSkip);

  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "hostLen equals 0\n"));
      ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;
  thisIorInfo->hostLen = ulongValue;
  skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 1);
  numHexCharsRead += numCharsToSkip;

  // Read the hostname and store it
  ACE_OS::strncpy (thisIorInfo->HostName,
                   getString((char *)(thisIor + numHexCharsRead),
                             2 * thisIorInfo->hostLen), thisIorInfo->hostLen);
  numHexCharsRead += 2 * thisIorInfo->hostLen;

  ACE_DEBUG ((LM_DEBUG,
              "    HostName   : %s (%d bytes)\n",
              thisIorInfo->HostName,
              thisIorInfo->hostLen));

  skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 0);
  numHexCharsRead += numCharsToSkip;

  // Read the port number and store it
  ulongValue = getOctet4Field((char *)(thisIor + numHexCharsRead),
                              &numCharsToSkip);

  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "port number equals 0\n"));
      ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;
  thisIorInfo->portNum = ulongValue;

  ACE_DEBUG ((LM_DEBUG,
              "    Port Number: %d\n",
              thisIorInfo->portNum));

  skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 0);
  numHexCharsRead += numCharsToSkip;

  // Read the object key length
  ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                              &numCharsToSkip);

  if (ulongValue == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "objectKeyLen equals 0\n"));
      ACE_OS::exit (1);
    }

  numHexCharsRead += numCharsToSkip;
  thisIorInfo->objectKeyLen = ulongValue;

  skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 1);
  numHexCharsRead += numCharsToSkip;

  // VisiBroker tends to insert the preamble "PMC" (=> Post Modern
  // Computing), before the "real" object key actually starts. Identify
  // if this is the case.
  isVisiIor = findIfVisiIor((char *)(thisIor + numHexCharsRead),
                            &numCharsToSkip);
  if (isVisiIor)
    {
      numHexCharsRead += numCharsToSkip;
      skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 0);
      numHexCharsRead += numCharsToSkip;

      ACE_DEBUG ((LM_DEBUG,
                  "    ObjectKey  : PMC"));

      skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 0);
      numHexCharsRead += numCharsToSkip;

      // Read the real object key length and then the real object key
      ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                                  &numCharsToSkip);

      if (ulongValue == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "VisiBroker IOR - real objectKeyLen equals 0\n"));
          ACE_OS::exit (1);
        }

      numHexCharsRead += numCharsToSkip;

      ACE_OS::strncpy (thisIorInfo->objectKey,
                       getString((char *)(thisIor + numHexCharsRead),
                                 2 * ulongValue), ulongValue);

      numHexCharsRead += 2 * ulongValue;

      ACE_DEBUG ((LM_DEBUG,
                  "%s",
                  thisIorInfo->objectKey,
                  ulongValue));

      skipNullOctets((char *)(thisIor + numHexCharsRead), &numCharsToSkip, 1);
      numHexCharsRead += numCharsToSkip;

      // Read the object name length and then the object name
      ulongValue = getOctet2Field((char *)(thisIor + numHexCharsRead),
                                  &numCharsToSkip);

      if (ulongValue == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "\n[No object name] (%d bytes)\n\n",
                      thisIorInfo->objectKeyLen));
          ACE_OS::exit (1);
        }

      numHexCharsRead += numCharsToSkip;

      ACE_OS::strncpy (thisIorInfo->objectKey,
                       getString((char *)(thisIor + numHexCharsRead),
                                 2 * ulongValue), ulongValue);

      numHexCharsRead += 2 * ulongValue;

      ACE_DEBUG ((LM_DEBUG,
                  "[object name: %s] (%d bytes)\n\n",
                  thisIorInfo->objectKey,
                  thisIorInfo->objectKeyLen));
    }

  else
    {
      // Non-VisiBroker IOR - Read the object_key and store it
      ACE_OS::strncpy (thisIorInfo->objectKey,
                       getString((char *)(thisIor + numHexCharsRead),
                                 2 * thisIorInfo->objectKeyLen),
                       thisIorInfo->objectKeyLen);

      numHexCharsRead += 2 * thisIorInfo->objectKeyLen;

      ACE_DEBUG ((LM_DEBUG,
                  "    ObjectKey  : %s (%d bytes)\n\n",
                  thisIorInfo->objectKey,
                  thisIorInfo->objectKeyLen));
    }

}


char *
IorHandler::getIdlInterface (char *typeId, int *validTypeId)
{
  static char idlInterface[MAX_TYPE_ID_LEN];
  int lenInterface;

  // A sample type_id for an IDL interface name "EchoTests" is
  // IDL:EchoTests:1.0 => the trick is to isolate the parts between
  // the two colons.
  if ((*typeId != 'I') && (*(typeId+1) != 'D') && (*(typeId+2) != 'L'))
    {
      *validTypeId = 0;
      return ((char *)typeId);
    }

  char *readStart = strchr(typeId, ':');

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
  ACE_OS::strncpy ((char *)idlInterface,
                   readStart+1,
                   lenInterface);
  idlInterface[lenInterface] = '\0';

  *validTypeId = 1;
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
          stringIOR);
  ACE_OS::fclose (fp);

  interpretIor (stringIOR, &parsedIOR);
}
