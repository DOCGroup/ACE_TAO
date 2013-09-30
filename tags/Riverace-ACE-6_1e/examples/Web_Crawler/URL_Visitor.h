/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    URL_Visitor.h
 *
 *  $Id$
 *
 *  @author Douglas C.Schmidt <schmidt@cs.wustl.edu> Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef _URL_VISITOR_H
#define _URL_VISITOR_H
#include /**/ "ace/pre.h"

#include "ace/Strategies_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "HTTP_URL.h"
#include "Iterators.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Caching_Strategies_T.h"
#include "ace/Cached_Connect_Strategy_T.h"
#include "Options.h"

// Forward declarations.
class URL_Validation_Visitor;

/**
 * @class URL_Processing_Strategy
 *
 * @brief Abstract base class for the URL processing strategy.
 *
 */
class URL_Processing_Strategy
{
public:
  /// Constructor.
  URL_Processing_Strategy (URL &,
                           URL_Iterator &);

  virtual ~URL_Processing_Strategy (void);

  /// Perform the strategy.
  virtual int execute (void) = 0;

  virtual int destroy (void);

  // Close down the resources.

protected:
  /// A reference to the URL "context" that we're processing.
  URL &url_;

  /// Iterator for the URL that we're processing.
  URL_Iterator &iterator_;
};

/**
 * @class HTTP_Header_Processing_Strategy
 *
 * @brief Defines the HTTP header processing strategy.
 *
 */
class HTTP_Header_Processing_Strategy : public URL_Processing_Strategy
{
public:
  /// Constructor.
  HTTP_Header_Processing_Strategy (URL &,
                                   URL_Iterator &);

  /// Perform the strategy for processing an HTTP header.
  virtual int execute (void);
};

/**
 * @class HTML_Body_Validation_Strategy
 *
 * @brief Defines the HTML body processing strategy.
 *
 * This class iterates through the body of an HTML file and
 * recursively visits embedded links.
 */
class HTML_Body_Validation_Strategy : public URL_Processing_Strategy
{
public:
  /// Constructor.
  HTML_Body_Validation_Strategy (URL &,
                                 URL_Iterator &,
                                 URL_Validation_Visitor &);

  /**
   * Perform the strategy for processing an HTML file.  This strategy
   * iterates over the HTML file and recursively visits embedded links
   * to process them, as well.
   */
  virtual int execute (void);

private:
  /// This is the context of the visit.
  URL_Validation_Visitor &visitor_context_;
};

/**
 * @class URL_Download_Strategy
 *
 * @brief Defines a URL downloading strategy.
 *
 * This class downloads a URL's contents into a temporary file.
 */
class URL_Download_Strategy : public URL_Processing_Strategy
{
public:
  /// Constructor.
  URL_Download_Strategy (URL &,
                         URL_Iterator &);

  /// Perform the strategy for downloading a URL to a temporary file.
  virtual int execute (void);
};

/**
 * @class URL_Visitation_Strategy_Factory
 *
 * @brief Abstract Factory for the URL visitation strategy.
 *
 */
class URL_Visitation_Strategy_Factory
{
public:
  URL_Visitation_Strategy_Factory (URL *);

  /// Destructor.
  virtual ~URL_Visitation_Strategy_Factory (void);

  // = Factory Methods.
  /// Factory Method that makes the header iterator.
  virtual URL_Iterator *make_header_iterator (void) = 0;

  /// Factory Method that makes the body iterator.
  virtual URL_Iterator *make_body_iterator (void) = 0;

  /// Factory Method that makes the header processing strategy.
  virtual URL_Processing_Strategy *make_header_strategy (URL_Iterator &) = 0;

  /// Factory Method that makes the body processing strategy .
  virtual URL_Processing_Strategy *make_body_strategy (URL_Iterator &) = 0;

  /// Close down the resources.
  virtual int destroy (void) = 0;

protected:
  /// Stash the URL so we don't have to pass it around.
  URL *url_;
};

/**
 * @class URL_Download_Visitation_Strategy_Factory
 *
 * @brief Concrete Factory for the URL validation visitation strategy.
 *
 */
class URL_Download_Visitation_Strategy_Factory : public URL_Visitation_Strategy_Factory
{
public:
  /// Constructor.
  URL_Download_Visitation_Strategy_Factory (URL *);

  // = Factory Methods.
  /// Factory Method that makes an <HTTP_Header_Iterator>.
  virtual URL_Iterator *make_header_iterator (void);

  /// Factory Method that makes an <HTML_Body_Iterator>.
  virtual URL_Iterator *make_body_iterator (void);

  /// Factory Method that makes the header processing strategy.
  virtual URL_Processing_Strategy *make_header_strategy (URL_Iterator &);

  /// Factory Method that makes the body processing strategy .
  virtual URL_Processing_Strategy *make_body_strategy (URL_Iterator &);

  /// Close down the resources.
  virtual int destroy (void);
};

/**
 * @class URL_Validation_Visitation_Strategy_Factory
 *
 * @brief Concrete Factory for the URL validation visitation strategy.
 *
 */
class URL_Validation_Visitation_Strategy_Factory : public URL_Visitation_Strategy_Factory
{
public:
  /// Constructor.
  URL_Validation_Visitation_Strategy_Factory (URL *,
                                              URL_Validation_Visitor &);

  // = Factory Methods.
  /// Factory Method that makes an <HTTP_Header_Iterator>.
  virtual URL_Iterator *make_header_iterator (void);

  /// Factory Method that makes an <HTML_Body_Iterator>.
  virtual URL_Iterator *make_body_iterator (void);

  /// Factory Method that makes the header processing strategy.
  virtual URL_Processing_Strategy *make_header_strategy (URL_Iterator &);

  /// Factory Method that makes the body processing strategy .
  virtual URL_Processing_Strategy *make_body_strategy (URL_Iterator &);

  /// Close down the resources.
  virtual int destroy (void);

private:
  /// Context of the visitor.
  URL_Validation_Visitor &visitor_context_;
};

/**
 * @class URL_Visitor
 *
 * @brief Base class for the URL Visitor.
 *
 * This class plays the "visitor" role in the Visitor pattern.
 */
class URL_Visitor
{
public:

  virtual ~URL_Visitor (void);

  /// Visit an <HTTP_URL>.
  virtual int visit (HTTP_URL &http_url) = 0;

  // @@
  // virtual int visit (FTP_URL &http_url) = 0;

  /// Cleanup the resources.
  virtual int destroy (void) = 0;

protected:
  /// Make the appropriate <URL_Visitation_Strategy_Factory>.
  virtual URL_Visitation_Strategy_Factory *make_visitation_strategy_factory (URL &) = 0;
};

typedef int ATTRIBUTES;
typedef ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH>
        Client_Svc_Handler;
typedef std::pair<Client_Svc_Handler *, ATTRIBUTES>
        CACHED_HANDLER;
typedef ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
        ACE_ADDR;
typedef ACE_Hash<ACE_ADDR> H_KEY;
typedef ACE_Equal_To<ACE_ADDR> C_KEYS;

typedef ACE_Hash_Map_Manager_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP;
typedef ACE_Hash_Map_Iterator_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP_ITERATOR;
typedef ACE_Hash_Map_Reverse_Iterator_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP_REVERSE_ITERATOR;

typedef ACE_Recyclable_Handler_Cleanup_Strategy<ACE_ADDR, CACHED_HANDLER, HASH_MAP>
        CLEANUP_STRATEGY;
typedef ACE_Recyclable_Handler_Caching_Utility<ACE_ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTRIBUTES>
        CACHING_UTILITY;

typedef ACE_LRU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
        LRU_CACHING_STRATEGY;

typedef LRU_CACHING_STRATEGY
        CACHING_STRATEGY;

typedef ACE_Strategy_Connector<Client_Svc_Handler, ACE_SOCK_CONNECTOR>
        STRATEGY_CONNECTOR;

typedef ACE_NOOP_Creation_Strategy<Client_Svc_Handler>
        NULL_CREATION_STRATEGY;

typedef ACE_NOOP_Concurrency_Strategy<Client_Svc_Handler>
        NULL_ACTIVATION_STRATEGY;

typedef ACE_Cached_Connect_Strategy_Ex<Client_Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES, ACE_SYNCH_NULL_MUTEX>
        CACHED_CONNECT_STRATEGY;

/**
 * @class URL_Validation_Visitor
 *
 * @brief Subclass that defines the URL validation visitor.
 *
 * This class checks to make sure that the <HTTP_URL> is valid.
 * If the <HTTP_URL> is an <HTML> file, it can also be used to
 * recursively check that all embedded links in this file are
 * valid.
 */
class URL_Validation_Visitor : public URL_Visitor
{
public:
  typedef ACE_Hash_Map_Manager <ACE_URL_Addr, URL_Status, ACE_Null_Mutex>
          URL_CACHE;

  /**
   * Visit an <HTTP_URL> to make sure that it's valid.  If the content
   * type of the <HTTP_URL> is "text/html" and the <recursion> option
   * is enabled then <visit> recursively checks each link embedded in
   * the HTML page.
   */
  virtual int visit (HTTP_URL &http_url);

  // @@
  // virtual int visit (FTP_URL &http_url);

  /// Cleanup the resources.
  URL_Validation_Visitor (void);
  virtual int destroy (void);

  /// Returns a reference to the URL cache.
  URL_CACHE &url_cache (void);

protected:
  /// Factory Method that makes a
  /// <URL_Validation_Visitation_Strategy_Factory>.
  virtual ~URL_Validation_Visitor (void);
  virtual URL_Visitation_Strategy_Factory *make_visitation_strategy_factory (URL &);

  /// Cache the status of URLs we've already validated.
  URL_CACHE url_cache_;

  /// Check to see if the reply status of this <url_addr> is in the
  /// cache.  Returns 1 if so, 0 if not.
  int in_cache (const ACE_URL_Addr &url_addr);

  NULL_CREATION_STRATEGY creation_strategy_;
  NULL_ACTIVATION_STRATEGY activation_strategy_;

  // Configure the Strategy Connector with a strategy that caches
  // connection.
  CACHED_CONNECT_STRATEGY *caching_connect_strategy_;

  STRATEGY_CONNECTOR *strat_connector_;

  CACHING_STRATEGY caching_strategy_;
};


/**
 * @class URL_Download_Visitor
 *
 * @brief Subclass for the URL validtion visitor.
 *
 * This class checks to make sure that the <HTTP_URL> is valid.
 */
class URL_Download_Visitor : public URL_Visitor
{
public:
  /**
   * Visit an <HTTP_URL> to make sure that it's valid.  If the content
   * type of the <HTTP_URL> is "text/html" and the <recursion> option
   * is enabled then <visit> recursively checks each link embedded in
   * the HTML page.
   */
  virtual int visit (HTTP_URL &http_url);

  // @@
  // virtual int visit (FTP_URL &http_url);

  /// Cleanup the resources.
  virtual int destroy (void);

protected:
  /// Factory Method that makes a <URL_Download_Visitation_Strategy_Factory>.
  URL_Visitation_Strategy_Factory *make_visitation_strategy_factory (URL &);
};

/**
 * @class Auto_Destroyer
 *
 * @brief Simple class that ensures the <destroy> method is called on our
 * <URL_*> objects when they go out of scope.
 *
 * This class is similar to an auto_ptr<> and should be used to
 * simplify blocks of code that must create/destroy pointers to
 * various <URL_*> related strategies and iterators.
 */
template <class T>
class Auto_Destroyer
{
public:
  Auto_Destroyer (T *t): t_ (t) {}
  T *operator-> (void) { return this->t_; }
  T *operator *(void) { return this->t_; }
  void operator= (T *t)
  {
    if (this->t_ != 0)
      this->t_->destroy ();
    this->t_ = t;
  }
  ~Auto_Destroyer (void)
  {
    if (this->t_ != 0)
      t_->destroy ();
  }
private:
  T *t_;
};

#include /**/ "ace/post.h"
#endif /* _URL_VISITOR_H */
