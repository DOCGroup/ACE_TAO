/**
 *  @file    Iterators.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */

#ifndef _ITERATORS_H
#define _ITERATORS_H

#include "URL.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class URL_Iterator
 *
 * @brief An abstract base class that defines an iterator.
 *
 * Subclasses of this base class can define what strings
 * to return from @a next.  This class decouples higher-level
 * software from the details of whatever type of URL header or
 * body we're iterating over.
 */
class URL_Iterator
{
public:
  /// @name Iterator operations
  /// @{

  virtual int destroy ();

  /// Pass back the next @a string that hasn't been seen yet.  Returns 0
  /// when all items have been seen, else 1.
  virtual int next (ACE_CString &string) = 0;

  /// @}

protected:
  virtual ~URL_Iterator ();
};

/**
 * @class HTML_Body_Iterator
 *
 * @brief An iterator that returns URLs embedded in HTML files.
 */
class HTML_Body_Iterator : public URL_Iterator
{
public:
  explicit HTML_Body_Iterator (URL &url);

  /// @name Iterator methods
  /// @{

  /**
   * Pass back the next @a url that hasn't been seen in the
   * memory-mapped file.  Returns 0 when all items have been seen,
   * else 1.
   */
  int next (ACE_CString &url) override;

  /// @}

private:
  /// HTTP URL that we're iterating over.
  URL &url_;
};

/**
 * @class HTTP_Header_Iterator
 *
 * @brief An iterator that iterates over the HTTP header.
 */
class HTTP_Header_Iterator : public URL_Iterator
{
public:
  explicit HTTP_Header_Iterator (URL &url);

  /// @name Iterator methods
  /// @{

  /**
   * Pass back the next @a line that hasn't been seen in the
   * memory-mapped file header.  Returns 0 when we've reached the end
   * of the header.  seen, else 1.
   */
  int next (ACE_CString &line) override;

  /// @}

private:
  /// HTTP URL that we're iterating over.
  URL &url_;

  /// We've found the end of the header, which means this iterator is
  /// finished.
  bool end_of_header_ {};
};

/**
 * @class URL_Download_Iterator
 *
 * @brief An iterator that iterates over the contents of an entire URL,
 * i.e., both header and body, and returns it in @c BUFSIZ
 * @a buffers.
 */
class URL_Download_Iterator : public URL_Iterator
{
public:
  explicit URL_Download_Iterator (URL &url);

  /// @name Iterator methods
  /// @{

  /**
   * Pass back the next @a buffer data from the stream, where
   * <buffer.size> <= @c BUFSIZ .  Returns 0 when we've reached the end
   * of the header, else 1.
   */
  int next (ACE_CString &buffer) override;

  /// @}

private:
  /// HTTP URL that we're iterating over.
  URL &url_;
};

#endif /* _ITERATORS_H */
