/**
 *  @file    Capabilities.h
 *
 *  @author Arturo Montes <mitosys@colomsat.net.co>
 */

#ifndef ACE_CAPABILITIES_H
#define ACE_CAPABILITIES_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Containers.h"
#include "ace/SString.h"
#include "ace/Functor_String.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_CapEntry
 *
 * @brief This class is the base class for all ACE Capabilities entry
 * subclasses.
 *
 * This class is not instantiable and does not provide accessors
 * or methods.  If you want to add a new kind of attribute subclass
 * this class and dynamic_cast to proper subclass.
 */
class ACE_Export ACE_CapEntry
{
public:
   ACE_CapEntry () = delete;
   ACE_CapEntry (ACE_CapEntry const &) = default;
   ACE_CapEntry (ACE_CapEntry &&) = default;
   ACE_CapEntry &operator= (ACE_CapEntry const &) = default;
   ACE_CapEntry &operator= (ACE_CapEntry &&) = default;
   virtual ~ACE_CapEntry () = default;

protected:
  enum
  {
    ACE_INTCAP = 0,
    ACE_STRINGCAP = 1,
    ACE_BOOLCAP = 2
  };

  explicit ACE_CapEntry (int captype);

protected:
  int captype_;
};

/**
 * @class ACE_IntCapEntry
 *
 * @brief This class implement the ACE Integer Capability subclass.
 *
 * This is a container class for ACE Capabilities integer container
 * values.
 */
class ACE_Export ACE_IntCapEntry : public ACE_CapEntry
{
public:
  ACE_IntCapEntry () = delete;
  explicit ACE_IntCapEntry (int val);
  int getval () const;
  ACE_ALLOC_HOOK_DECLARE;

protected:
  int val_;
};

/**
 * @class ACE_StringCapEntry
 *
 * @brief This class implement the ACE String Capability subclass.
 *
 * This is a container class for ACE Capabilities String container
 * values.
 */
class ACE_Export ACE_StringCapEntry : public ACE_CapEntry
{
public:
  ACE_StringCapEntry () = delete;
  explicit ACE_StringCapEntry (ACE_TString const &val);
  ACE_TString getval () const;
  ACE_ALLOC_HOOK_DECLARE;

protected:
  ACE_TString val_;
};

/**
 * @class ACE_BoolCapEntry
 *
 * @brief This class implement the ACE Bool Capability subclass.
 *
 * This is a container class for ACE Capabilities bool container
 * values.
 */
class ACE_Export ACE_BoolCapEntry : public ACE_CapEntry
{
public:
  ACE_BoolCapEntry () = delete;
  explicit ACE_BoolCapEntry (int val);
  int getval () const;
  ACE_ALLOC_HOOK_DECLARE;

protected:
  int val_;
};

/**
 * @class ACE_Capabilities
 *
 * @brief
 * This class implement the ACE Capabilities.
 *
 * This is a container class for ACE Capabilities
 * values. Currently exist three different capability values:
 * ACE_IntCapEntry (integer), ACE_BoolCapEntry (bool) and
 * ACE_StringCapEntry (String).  An ACE_Capabilities is a
 * unordered set of pair = (String, ACE_CapEntry *).  Where
 * the first component is the name of capability and the second
 * component is a pointer to the capability value container.  A
 * FILE is a container for ACE_Capabilities, the
 * ACE_Capabilities has a name in the file, as a termcap file.
 */
class ACE_Export ACE_Capabilities
{
public:
  using CAPABILITIES_MAP = ACE_Hash_Map_Manager_Ex<ACE_TString, ACE_CapEntry *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;

  ACE_Capabilities ();

  ACE_Capabilities (ACE_Capabilities const &) = delete;
  ACE_Capabilities (ACE_Capabilities &&) = delete;
  ACE_Capabilities &operator= (ACE_Capabilities const &) = delete;
  ACE_Capabilities &operator= (ACE_Capabilities &&) = delete;
  ~ACE_Capabilities ();

public:
  /// Get a string entry.
  int getval (ACE_TCHAR const *ent, ACE_TString &val);

  /// Get an integer entry.
  int getval (ACE_TCHAR const *ent, int &val);

  /// Get the ACE_Capabilities name from FILE fname and load the
  /// associated capabitily entries in map.
  int getent (ACE_TCHAR const *fname, ACE_TCHAR const *name);

protected:
  /// Parse an integer property
  ACE_TCHAR const *parse (ACE_TCHAR const *buf, int &cap);

  /// Parse a string property
  ACE_TCHAR const *parse (ACE_TCHAR const *buf, ACE_TString &cap);

  /// Fill the ACE_Capabilities with description in ent.
  int fillent(ACE_TCHAR const *ent);

  /// Parse a cap entry
  int parseent (ACE_TCHAR const *name, ACE_TCHAR *line);

  /// Get a line from FILE input stream
  int getline (FILE* fp,
               ACE_TString &line);

  /// Is a valid entry
  bool is_entry (ACE_TCHAR const *name, ACE_TCHAR const *line);

  /// Reset the set of capabilities
  void resetcaps ();

private:
  /// This is the set of ACE_CapEntry.
  CAPABILITIES_MAP caps_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Capabilities.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* __ACE_CAPABILITIES_H__ */
