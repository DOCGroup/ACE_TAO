/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Web_Crawler
//
// = FILENAME
//    Iterators.h
//
// = AUTHOR
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#ifndef _ITERATORS_H
#define _ITERATORS_H

#include "URL.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class URL_Iterator
{
  // = TITLE
  //    An abstract base class that defines an iterator.
  //
  // = DESCRIPTION
  //    Subclasses of this base class can define what strings
  //    to return from <next>.  This class decouples higher-level
  //    software from the details of whatever type of URL header or
  //    body we're iterating over.
public:
  // = Initialization and termination methods.
  virtual int destroy (void);
  // "virtual" destructor.

  // = Iterator methods.
  virtual int next (ACE_CString &string) = 0;
  // Pass back the next <string> that hasn't been seen yet.  Returns 0
  // when all items have been seen, else 1.

protected:
  virtual ~URL_Iterator (void);
  // C++ destructor.
};

class HTML_Body_Iterator : public URL_Iterator
{
  // = TITLE
  //    An iterator that returns URLs embedded in HTML files.
public:
  // = Initialization and termination methods.
  HTML_Body_Iterator (URL &url);
  // Constructor.

  // = Iterator methods.
  virtual int next (ACE_CString &url);
  // Pass back the next <url> that hasn't been seen in the
  // memory-mapped file.  Returns 0 when all items have been seen,
  // else 1.

private:
  URL &url_;
  // HTTP URL that we're iterating over.
};

class HTTP_Header_Iterator : public URL_Iterator
{
  // = TITLE
  //    An iterator that iterates over the HTTP header.
public:
  // = Initialization and termination methods.
  HTTP_Header_Iterator (URL &url);
  // Constructor.

  // = Iterator methods.
  virtual int next (ACE_CString &line);
  // Pass back the next <line> that hasn't been seen in the
  // memory-mapped file header.  Returns 0 when we've reached the end
  // of the header.  seen, else 1.

private:
  URL &url_;
  // HTTP URL that we're iterating over.

  int end_of_header_;
  // We've found the end of the header, which means this iterator is
  // finished.
};

class URL_Download_Iterator : public URL_Iterator
{
  // = TITLE
  //    An iterator that iterates over the contents of an entire URL,
  //    i.e., both header and body, and returns it in <BUFSIZ>
  //    <buffer>s.
public:
  // = Initialization and termination methods.
  URL_Download_Iterator (URL &url);
  // Constructor.

  // = Iterator methods.
  virtual int next (ACE_CString &buffer);
  // Pass back the next <buffer> data from the stream, where
  // <buffer.size> <= <BUFSIZ> .  Returns 0 when we've reached the end
  // of the header, else 1.

private:
  URL &url_;
  // HTTP URL that we're iterating over.
};

#endif /* _ITERATORS_H */
