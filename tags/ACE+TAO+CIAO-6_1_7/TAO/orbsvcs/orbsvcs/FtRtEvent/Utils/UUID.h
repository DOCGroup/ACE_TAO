// -*- C++ -*-
//=============================================================================
/**
 *  @file   UUID.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef UUID_H
#define UUID_H
#include /**/ "ace/pre.h"

#include "orbsvcs/FtRtEvent/Utils/ftrtevent_export.h"
#include "tao/Versioned_Namespace.h"
#include "ace/SString.h"

#if defined(TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO  FtRtEvent_Export

#if defined(TAO_EXPORT_NESTED_CLASSES)
#if defined(TAO_EXPORT_NESTED_MACRO)
#undef TAO_EXPORT_NESTED_MACRO
#endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#define TAO_EXPORT_NESTED_MACRO FtRtEvent_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_FtRt
{
/**
 * This is an implementation of Global Unique Identifier
 * which follows the document draft-leach-uuids-guids-01.txt. The
 * class layout is compatible with the binary representation of
 * UUID specified by that document. The default constructor
 * is used to create an new global unique id, not a NULL UUID.
 * This class does not use dynamic memory allocation and does
 * not throw any exception except to_string(ACE_CString&).
 */
class TAO_FtRtEvent_Export  UUID
{
  public:
    enum
    {
      BINRARY_LENGTH= 16,

      // the required length for the string representation including the termination '\0'
      STRING_LENGTH = 37
    };

    /* Construct an Global Unique Identifier. */
    UUID(void);

    /* This is used to create an uninitialized UUID. */
    UUID(int);

    /*
     * Construct an UUID from the binary represetation. If you want to avoid a memory
     * copy operation for this constructor. You can safely reinterpret_cast the 16
     * bytes buffer into the UUID object and use all the member functions provided in
     * this class except is_valid() which is only useful if a UUID object is
     * constructed from the string representation.
     */
    UUID(const unsigned char *buffer);

    /*
     * Construct an UUID from the string representation. Use is_valid() to test if the
     * construction is successful or not.
     */
    UUID(const ACE_TCHAR *);

    /* Construct an uuid from the string representation. */
    bool from_string(const char *string_rep);

    bool operator==(const UUID &other) const;
    bool operator!=(const UUID &other) const;

    /*
     * Test if the UUID is valid. Used after constructing an UUID from an string
     * representation. This is done by testing whether the time_hi and time_mid fields
     * of the timestamp are 0. By current time, those two field shouldn't be 0 at all.
     */
    bool is_valid(void) const;

    /*
     * convert to a string representation the buffer size provided must be at least
     * STRING_LENGTH
     */
    void to_string(char *string_rep) const;

    void to_string(ACE_CString &string) const;

    /*
     * convert to a string representation the buffer size provided must be at least
     * BINRARY_LENGTH
     */
    void to_binary(unsigned char *binary_rep) const;

    /* create an UUID in the specified buffer */
    static void create(unsigned char *buffer);
  private:
    union
    {
      struct
      {
        ACE_UINT32  low;
        ACE_UINT32  hi;
      } timestamp;
      unsigned char uuid[BINRARY_LENGTH];
    } rep_;
};
}
TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/FtRtEvent/Utils/UUID.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif
