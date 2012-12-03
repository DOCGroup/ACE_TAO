// -*- C++ -*-

//=============================================================================
/**
 *  @file   Storable_Base.h
 *
 *  $Id$
 *
 *  @author Bruce Trask <trask_b@ociweb.com>
 *  @author Chanaka Liyanaarachchi <chanaka@ociweb.com>
 *  @author Byron Harris <harrisb@ociweb.com>
 */
//=============================================================================

#ifndef TAO_STORABLE_BASE_H
#define TAO_STORABLE_BASE_H

#include "tao/TAO_Export.h"
#include "tao/orbconf.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class TAO_Export Storable_Base
  {
  public:
    Storable_Base();

    virtual ~Storable_Base();

    virtual void remove() = 0;

    virtual int exists() = 0;

    virtual int open () = 0;

    virtual int close () = 0;

    virtual int flock (int whence, int start, int len) = 0;

    virtual int funlock (int whence, int start, int len) = 0;

    virtual time_t last_changed(void) = 0;

    // Mimic a portion of the std::ios interface.  We need to be able
    // to indicate error states from the extraction operators below.
    enum Storable_State { goodbit = 0,
                          badbit  = 1,
                          eofbit  = 2,
                          failbit = 4
    };

    void clear (Storable_State state = goodbit);

    void setstate (Storable_State state);

    Storable_State rdstate (void) const;

    bool good (void) const;

    bool bad (void) const;

    bool eof (void) const;

    bool fail (void) const;

    virtual void rewind (void) = 0;

    virtual bool flush (void) = 0;

    virtual Storable_Base& operator << (const ACE_CString& str) = 0;

    virtual Storable_Base& operator >> (ACE_CString& str) = 0;

    virtual Storable_Base& operator << (int i) = 0;

    virtual Storable_Base& operator >> (int &i) = 0;

    virtual Storable_Base& operator << (unsigned int i) = 0;

    virtual Storable_Base& operator >> (unsigned int &i) = 0;

  private:
    Storable_State state_;

  };

}

#if defined (__ACE_INLINE__)
#include "tao/Storable_Base.inl"
#endif

#endif
