/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Capabilities.h
//
// = AUTHOR
//    Arturo Montes <mitosys@colomsat.net.co>
//
// ============================================================================

#ifndef ACE_CAPABILITIES_H
#define ACE_CAPABILITIES_H
#include "ace/pre.h"

#include "ace/OS.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Containers.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_CapEntry
{
  // = TITLE
  //   This class is the base class for all ACE Capabilities entry
  //   subclasses.
  //
  // = DESCRIPTION
  //   This class is not instantiable and does not provide accessors
  //   or methods.  If you want to add a new kind of attribute you
  //   subclasses of this class and dynamic cast to proper subclass.
public:
   virtual ~ACE_CapEntry (void);

protected:
  enum
  {
    ACE_INTCAP = 0,
    ACE_STRINGCAP = 1,
    ACE_BOOLCAP = 2
  };

  ACE_CapEntry (int captype);

  int captype_;
};

class ACE_Export ACE_IntCapEntry : public ACE_CapEntry
{
  // = TITLE
  //   This class implement the ACE Integer Capability subclass.
  //
  // = DESCRIPTION
  //   This is a container class for ACE Capabilities integer container
  //   values.
public:
  ACE_IntCapEntry (int val);
  int getval (void) const;

protected:
  int val_;
};

class ACE_Export ACE_StringCapEntry : public ACE_CapEntry
{
  // = TITLE
  //   This class implement the ACE String Capability subclass.
  //
  // = DESCRIPTION
  //   This is a container class for ACE Capabilities String container
  //   values.
public:
  ACE_StringCapEntry (const ACE_CString &val);
  ACE_CString getval (void) const;

protected:
  ACE_CString val_;
};

class ACE_Export ACE_BoolCapEntry : public ACE_CapEntry
{
  // = TITLE
  //   This class implement the ACE Bool Capability subclass.
  //
  // = DESCRIPTION
  // This is a container class for ACE Capabilities bool container
  // values.
public:
  ACE_BoolCapEntry (int val);
  int getval (void) const;

protected:
  int val_;
};

class ACE_Export ACE_Capabilities
{
  // = TITLE
  //   This class implement the ACE Capabilities.
  //
  // = DESCRIPTION
  //   This is a container class for ACE Capabilities
  //   values. Currently exist three different capability values:
  //   <ACE_IntCapEntry> (integer), <ACE_BoolCapEntry> (bool) and
  //   <ACE_StringCapEntry> (String).  An <ACE_Capabilities> is a
  //   unordered set of pair = (<String>, <ACE_CapEntry> *).  Where
  //   the first component is the name of capability and the second
  //   component is a pointer to the capability value container.  A
  //   <FILE> is a container for <ACE_Capabilities>, the
  //   <ACE_Capabilities> has a name in the file, as a termcap file.
public:
  ACE_Capabilities (void);
  // The Constructor

  ~ACE_Capabilities(void);
  // The Destructor

public:
  int getval (const char *ent,
              ACE_CString &val);
  // Get a string entry.

  int getval (const char *ent,
              int &val);
  // Get an integer entry.

  int getent (const char *fname,
              const char *name);
  // Get the ACE_Capabilities name from FILE fname and load the
  // associated capabitily entries in map.

protected:
  // Parse an integer property
  const char *parse (const char *buf,
                     int &cap);
  // Parse a string property

  const char *parse (const char *buf,
                     ACE_CString &cap);
  // Fill the ACE_Capabilities with description in ent.

  int fillent(const char *ent);
  // Parse a cap entry

  int parseent (const char *name,
                char *line);
  // Get a line from FILE input stream

  int getline (FILE* fp,
               ACE_CString &line);
  // Is a valid entry

  int is_entry (const char *name,
                const char *line);
  // Reset the set of capabilities

  void resetcaps (void);
  // Atributes.

private:
  ACE_Hash_Map_Manager<ACE_CString, ACE_CapEntry *, ACE_Null_Mutex> caps_;
  // This is the set of ACE_CapEntry.
};

#if defined (__ACE_INLINE__)
#include "ace/Capabilities.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* __ACE_CAPABILITIES_H__ */
