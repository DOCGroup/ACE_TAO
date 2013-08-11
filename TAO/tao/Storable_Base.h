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
#include "tao/CDR.h"

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

    Storable_Base (bool use_backup);

    virtual ~Storable_Base ();

    /// The process-wide default policy
    /// for doing a backup when close ()
    /// is called.
    /// The backup can then be restored if
    /// restore_backup () is called.
    /// The initial value for the default is false.
    static bool use_backup_default;

    bool use_backup ();

    /// Remove the file that is assumed to not be open.
    /// If backup are used, the backup will also be removed.
    void remove();

    virtual int create_backup () = 0;

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

    static ACE_CString state_as_string (Storable_State state);

    virtual void rewind (void) = 0;

    virtual bool flush (void) = 0;

    /// Force write of storable data to storage.
    /// Returns 0 on success, otherwise EOF
    virtual int sync (void) = 0;

    virtual Storable_Base& operator << (const ACE_CString& str) = 0;

    virtual Storable_Base& operator >> (ACE_CString& str) = 0;

    virtual Storable_Base& operator << (int i) = 0;

    virtual Storable_Base& operator >> (int &i) = 0;

    virtual Storable_Base& operator << (unsigned int i) = 0;

    virtual Storable_Base& operator >> (unsigned int &i) = 0;

    virtual Storable_Base& operator << (const TAO_OutputCDR & cdr) = 0;

    virtual size_t write (size_t size, const char * bytes) = 0;

    virtual size_t read (size_t size, char * bytes) = 0;

    virtual int restore_backup () = 0;

  protected:

    virtual void do_remove () = 0;

    /// If a backup file exists, remove it.
    virtual void remove_backup () = 0;

    bool use_backup_;

  private:
    Storable_State state_;

  };

  /// Base class for exceptions thrown when encountering
  /// errors working with persistent files.
  class TAO_Export Storable_Exception
  {
  public:
    Storable_Exception (const ACE_CString & file_name);

    virtual ~Storable_Exception ();

    const ACE_CString & get_file_name () const;

  private:
    ACE_CString file_name_;
  };

  /// Exception thrown when an error is encountered
  /// during reading of the persistent store.
  class TAO_Export Storable_Read_Exception : public Storable_Exception
  {
  public:
    Storable_Read_Exception (Storable_Base::Storable_State state,
                             const ACE_CString & file_name);

    Storable_Base::Storable_State get_state () const;

  private:
    TAO::Storable_Base::Storable_State storable_state_;
  };

  /// Exception thrown when an error is encountered
  /// during writing to the persistent store.
  class TAO_Export Storable_Write_Exception : public Storable_Exception
  {
  public:
    Storable_Write_Exception (Storable_Base::Storable_State state,
                              const ACE_CString & file_name);

    Storable_Base::Storable_State get_state () const;

  private:
    TAO::Storable_Base::Storable_State storable_state_;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Storable_Base.inl"
#endif


#endif
