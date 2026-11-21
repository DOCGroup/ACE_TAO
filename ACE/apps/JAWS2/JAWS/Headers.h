/* -*- c++ -*- */
#ifndef JAWS_HEADERS_H
#define JAWS_HEADERS_H

#include "JAWS/Export.h"
#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * Owns a parsed HTTP header's name and value.
 *
 * Parse_Headers builds the name and value in Header_Info, then inserts the
 * completed header into the header table.
 */
class JAWS_Export JAWS_Header_Data
{
public:
  JAWS_Header_Data (const char *name, const char *value = nullptr, int type = 0);
  JAWS_Header_Data (const char *name, int type, const char *value = nullptr);
  ~JAWS_Header_Data ();
  JAWS_Header_Data (JAWS_Header_Data const &) = delete;
  JAWS_Header_Data (JAWS_Header_Data &&) = delete;
  JAWS_Header_Data &operator= (JAWS_Header_Data const &) = delete;
  JAWS_Header_Data &operator= (JAWS_Header_Data &&) = delete;

  const char * header_name () const;
  const char * header_value () const;
  int header_type () const;

  void header_name (const char *name);
  void header_value (const char *value);
  void header_type (int type);

private:
  /// Owns the buffers returned by ACE_OS::strdup.
  char *header_name_;
  char *header_value_;
  int header_type_;
};

using JAWS_Header_Table = ACE_DLList<JAWS_Header_Data>;
using JAWS_Header_Table_Iterator = ACE_DLList_Iterator<JAWS_Header_Data>;

class JAWS_Export JAWS_Headers : public JAWS_Header_Table
{
public:
  JAWS_Headers ();
  ~JAWS_Headers ();

  /// Insert the new data; return 0 on success and -1 on failure.
  int insert (JAWS_Header_Data *new_data);

  /// Find a header from the beginning of the list; return nullptr if absent.
  JAWS_Header_Data * find (const char *const &header_name);

  /// Continue searching from the previous find position; return nullptr if
  /// absent.
  JAWS_Header_Data * find_next (const char *const &header_name);

  /// Remove all headers matching header_name.
  void remove_all (const char *const &header_name);

  /// Return an iterator over the header table.
  JAWS_Header_Table_Iterator &iter ();

private:
  JAWS_Header_Table_Iterator iter_;
};

#endif /* !defined (JAWS_HEADERS_H) */
