/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_OS.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_OS_H
#include /**/ "ace/pre.h"
#define ACE_CLASSIX_OS_H

/* ------------------------------------------------------------------------- */
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Select_Reactor.h"
#include "ace/Singleton.h"
#include "ace/CLASSIX/CLASSIX_Port_Core.h"

#include <ipc/chIpc.h>


#if 0
class ACE_Export ACE_CLASSIX_Mgr
    // = TITLE
    //   Class to manage resouces, especially static resources
    //
    // = DESCRIPTION
    //   It is intended that one <ACE_CLASSIX_Mgr> per actor
    //
{
 public:
    friend ACE_Singleton<ACE_CLASSIX_Mgr, ACE_SYNCH_NULL_MUTEX>;

    ACE_SYNCH_RECURSIVE_MUTEXT& ref_lock(const) const;

 protected:
    // = Initialization
    ACE_CLASSIX_Mgr(void);
    // Only allow its fried to instantiate, since it is meant as a
    // singleton class.
    // This class shall be instanti

private:
    ACE_SYNCH_RECURSIVE_MUTEX   *port_lock_;
};
#endif

class ACE_Export ACE_CLASSIX_OS
{
public:

    ACE_CLASSIX_OS();

    static const KnUniqueId&  null_KnUniqueId();
    static ssize_t convert_io_error (ssize_t /* CLASSIX' error no */);
    // conver CLASSIX's errno no to Posix no.

protected:

private:
    friend class ACE_CLASSIX_Port_Core;
    friend class ACE_CLASSIX_Port;
    static ACE_Recursive_Thread_Mutex& get_lock_(void);
    // Returns CLASSIX environment-wide lock

    static KnUniqueId                        null_KnUniqueId_;
    // Null-filled KnUniqueId

    static ACE_Recursive_Thread_Mutex        lock_;
};

/* ------------------------------------------------------------------------- */


class ACE_CLASSIX_Msg
    // = TITLE
    //   Class wrapper over Chorus's KnMsgDesc structure
    //
    // = DESCRIPTION
    //   Provides initialization and reset methods to construct a KnMsgDesc.
    //
    //   The flags in the <flags> field of the KnMsgDesc structure is always
    //   set to 0. Therefore, the message body will always be copied and
    //   the message is assumed to be part of the user(kernel) address
    //   space when the caller is a USER(SUPERVISOR) thread.
    //   (Chorus may not support the options defined in the flag, so we
    //   choose not to use the flag, i.e. set it to null).
    //
    //   The message annex is not used.

    // = SEE ALSO
    //   ipcSend(2)
{
 public:
    // = initialization
    ACE_CLASSIX_Msg();
    // default constructor
    ACE_CLASSIX_Msg(const void * /* body address */, int /* msg length */);

    KnMsgDesc* get(void);
    // get CLASSIX' message structure
    void set (const void* /* body address */, int /* msg length*/);
    // Reset the message.  The original message will be overwritten.

    void dump(void) const;

    ACE_ALLOC_HOOK_DECLARE;
    // declare the dynamic allocation hooks

 private:
    KnMsgDesc    msg_;
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_OS.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_CLASSIX_OS_H */
