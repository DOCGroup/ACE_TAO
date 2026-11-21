#ifndef HTTPU_HTTP_STATUS_HPP
#define HTTPU_HTTP_STATUS_HPP

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "HTTPU/http_export.h"
#include "ace/Synch_Traits.h"

class HTTP_SCode_Base;

/// A status code and its non-owning reason string.
/// The reason string passed to the constructor must remain valid for
/// the lifetime of the node.
class HTTPU_Export HTTP_SCode_Node
{
  friend class HTTP_SCode_Base;

public:
  operator int () const;
  operator const char * () const;

private:
  HTTP_SCode_Node (int code, const char *code_str);

private:
  int code_;
  const char *code_str_;
};

class HTTPU_Export HTTP_SCode_Base
{
public:
  static HTTP_SCode_Node STATUS_OK;
  static HTTP_SCode_Node STATUS_CREATED;
  static HTTP_SCode_Node STATUS_ACCEPTED;
  static HTTP_SCode_Node STATUS_NO_CONTENT;
  static HTTP_SCode_Node STATUS_MULTIPLE_CHOICES;
  static HTTP_SCode_Node STATUS_MOVED_PERMANENTLY;
  static HTTP_SCode_Node STATUS_MOVED_TEMPORARILY;
  static HTTP_SCode_Node STATUS_NOT_MODIFIED;
  static HTTP_SCode_Node STATUS_INSUFFICIENT_DATA;
  static HTTP_SCode_Node STATUS_BAD_REQUEST;
  static HTTP_SCode_Node STATUS_UNAUTHORIZED;
  static HTTP_SCode_Node STATUS_FORBIDDEN;
  static HTTP_SCode_Node STATUS_NOT_FOUND;
  static HTTP_SCode_Node STATUS_INTERNAL_SERVER_ERROR;
  static HTTP_SCode_Node STATUS_NOT_IMPLEMENTED;
  static HTTP_SCode_Node STATUS_BAD_GATEWAY;
  static HTTP_SCode_Node STATUS_SERVICE_UNAVAILABLE;
  static HTTP_SCode_Node STATUS_QUIT;

  enum
  {
    MIN_STATUS_CODE = 200,
    MAX_STATUS_CODE = 599
  };

private:
  static HTTP_SCode_Node DUMMY;
};

/// Maps numeric status codes to descriptive strings.
class HTTPU_Export HTTP_SCode : public HTTP_SCode_Base
{
  friend class HTTP_SCode_Node;
  friend class ACE_Singleton<HTTP_SCode, ACE_SYNCH_MUTEX>;

protected:
  HTTP_SCode ();
  ~HTTP_SCode ();

public:
  /// Return the reason string corresponding to a status code number.
  const char * operator[] (int) const;

  /// Return the singleton instance.
  static HTTP_SCode *instance ();

  enum
  {
    SC_TABLE_SIZE = MAX_STATUS_CODE - MIN_STATUS_CODE + 1
  };

  void dump ();

private:
  static const char *table_[SC_TABLE_SIZE];
};

using HTTP_SCode_Singleton = ACE_Singleton<HTTP_SCode, ACE_SYNCH_MUTEX>;

#if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "HTTPU/http_status.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#endif /* HTTPU_HTTP_STATUS_HPP */
