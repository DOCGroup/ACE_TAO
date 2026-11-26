/**
 *  @file    Parse_Headers.h
 *
 *  @author James Hu
 */

#ifndef PARSE_HEADERS_H
#define PARSE_HEADERS_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Headers_Map_Item
{
friend class Headers_Map;
friend class Headers;

private:
  Headers_Map_Item ();
  Headers_Map_Item (Headers_Map_Item const &) = delete;
  Headers_Map_Item (Headers_Map_Item &&) = delete;
  ~Headers_Map_Item ();

  // operator const char * () const;
  Headers_Map_Item &operator= (char *);
  Headers_Map_Item &operator= (char const *);
  Headers_Map_Item &operator= (Headers_Map_Item const &);
  Headers_Map_Item &operator= (Headers_Map_Item &&) = delete;

public:
  char const *header () const;
  char const *value () const;

private:
  char const *header_ {};
  char const *value_ {};
};

/**
 * @class Headers_Map
 *
 * @brief Map textual headings to header values (e.g. "Subject:" maps to
 * "Re: My left foot"
 */
class Headers_Map
{
public:
  Headers_Map ();
  Headers_Map (Headers_Map const &) = delete;
  Headers_Map (Headers_Map &&) = delete;
  Headers_Map &operator= (Headers_Map const &) = delete;
  Headers_Map &operator= (Headers_Map &&) = delete;
  ~Headers_Map () = default;

  Headers_Map_Item &operator[] (char const *const header);
  Headers_Map_Item const &operator[] (char const *const header) const;

  enum
  {
    MAX_HEADERS = 100
  };

  int mapped (char const *const header) const;

private:
  Headers_Map_Item *find (char const *const header) const;
  Headers_Map_Item *place (char const *const header);
  static int compare (void const *item1, void const *item2);

private:
  Headers_Map_Item map_[MAX_HEADERS];
  Headers_Map_Item garbage_;

  int num_headers_ {};
};

/**
 * @class Headers
 *
 * @brief A general mechanism to parse headers of Internet text headers.
 *
 * Allow interesting headers to be inserted and later associated
 * with values.  This implementation assumes the parsing of headers
 * will be done from ACE_Message_Blocks.
 */
class Headers
{
public:
  Headers ();
  Headers (Headers const &) = delete;
  Headers (Headers &&) = delete;
  Headers &operator= (Headers const &) = delete;
  Headers &operator= (Headers &&) = delete;
  ~Headers () = default;

  void recognize (char const *const header);

  void parse_header_line (char *const header_line);

  /**
   * -1 -> end of line but not complete header line
   *  0 -> no end of line marker
   *  1 -> complete header line
   */
  int complete_header_line (char *const header_line);

  int end_of_headers () const;

  enum
  {
    MAX_HEADER_LINE_LENGTH = 8192
  };

  Headers_Map_Item &operator[] (char const *const header);
  Headers_Map_Item const &operator[] (char const *const header) const;

private:
  int end_of_line (char *&line, int &offset) const;

private:
  Headers_Map map_;
  bool done_ {};
};

#endif /* PARSE_HEADERS_H */
