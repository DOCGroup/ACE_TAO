/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_Stream.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_STREAM_H
#include /**/ "ace/pre.h"
#define ACE_CLASSIX_STREAM_H

/* ------------------------------------------------------------------------- */
#include "ace/CLASSIX/CLASSIX_SAP.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CLASSIX/CLASSIX_Peer_SAP.h"
#include "ace/CLASSIX/CLASSIX_Port_Default.h"

class ACE_CLASSIX_Stream
    // = TITLE
    //   Provides the stream interface for CLASSIX IPC communications.
    //
    // = DESCRIPTION
    //   This classes provides a stream wrapper over CLASSIX'
    //   IPC. It is designed to be an alternative to Stream type of
    //   communication in the UNIX AF_INET environment.
    //
    //   To the user, the stream is connection-oriented.  The stream
    //   is implemented on top of IPC, it is connectionless and its
    //   reliability is as good as what  the raw IPC can offer.
    //
    //   Since CLASSIX IPC is not intended to go across wide area network,
    //   it can be seen as an
    //   alternative to AF_INET's connection oriented stream, even though
    //   the CLASSIX IPC is connectionless.
    //
    //   This class  contains a local sap, peer sap and data holding
    //   delivery mode information.
    //
    //   When a stream is created, the peer SAP information must be provided.
    //   The local SAP information may be provided in the constructor or
    //   open().  The local SAP information given in open() overrides the
    //   existing one. If the local SAP address is not given at all, it will
    //   allocate a port and use it as the local SAP.
    //
{
 public:
    ACE_CLASSIX_Stream(void);
    // Default constructor, does not setup anything
    ACE_CLASSIX_Stream(const ACE_CLASSIX_Port_Core&, /* remote port */
                        const ACE_CLASSIX_Port& =
                  *ACE_CLASSIX_DEFAULT_PORT::instance()); /* local address */

    ACE_CLASSIX_Stream(const ACE_CLASSIX_Port&,    /* remote port address */
                       const ACE_CLASSIX_Port& =
                   *ACE_CLASSIX_DEFAULT_PORT::instance());/* local address */

    ACE_CLASSIX_Stream(const KnUniqueId&,          /* remote port address */
                       const ACE_CLASSIX_Port& =
                   *ACE_CLASSIX_DEFAULT_PORT::instance());/* local address */

    // = IO
    // Returns -1, if fault has occured during receive.  errno contains
    // the error condition.
    // Returns no of bytes received/sent,  otherwise.
    //
    virtual ssize_t send_n (const void *, /* buf */
                            size_t, /* n */
                            int /* flags */);
    // Sends exactly n bytes (keep trying until n bytes are sent)
    virtual ssize_t send_n (const void *, /* buf */ size_t /* n */) const;
    // Sends n bytes(keep trying until n bytes are sent)
    // Uses the current addressing mode
    virtual ssize_t send (const void *, /* buf */
                          size_t, /* n */
                          int /* flags */);
    // Sends n bytes. Same as send_n(buf, n, flag)
    virtual ssize_t send (const void *, /* buf */
                          size_t /* n */) const;
    // Sends n bytes. same as send_n(buf, n).
    virtual ssize_t reply (const void *, /* buf */
                           size_t /* n */) const;
    // Sends n bytes to the sender of the cureent thread's current message.

    ssize_t ipcRecv (void *,             /* buffer */
                     size_t,             /* length */
                     int  = 0,           /* flag: 0 or MSG_PEEK  */
                     ACE_Time_Value* = 0) const; /* timeout */
    // Recv an <n> byte buffer
    // Wait up to <timeout> amount of time to receive up to <n> bytes
    // into <buf> from <handle> (uses the <recv> call).  If <recv> times
    // out a -1 is returned with <errno == ETIMEOUT>.  If it succeeds the
    // number of bytes received is returned.
    // If <timeout> == 0, the caller
    // will block until action is possible, else will wait until the
    // relative time specified in *<timeout> elapses).
    ssize_t ipcRecv_n (void *,             /* buffer */
                       size_t,             /* length */
                       int  = 0,           /* flag: 0 or MSG_PEEK  */
                       ACE_Time_Value* = 0) const; /* timeout */
    // Continue until receives n bytes of data or an error has occurred.
    // Retruns the no of bytes actually received.
    // If it is less than expected, errno contains error info.

    ssize_t peek(ACE_Time_Value* = 0 /* timeout */) const;
    // It is the same as recv(0, 0, MSG_PEEK, timeout)
    // This must be followd by get_data() in order to read the peeked message
    ssize_t recv(void* /*buf */, int /* len */) const;
    // Gets data which have been peeked
    ssize_t recv_n(void* /*buf */, int /* len */) const;
    // Gets data which have been peeked

    // = ACCESS
    virtual ACE_HANDLE get_handle (void) const;
    // virtual void set_handle (ACE_HANDLE);
    virtual void set_handle(ACE_HANDLE);
    // set the local port's address according to the supplied handle

    const ACE_CLASSIX_SAP&  local_sap(void) const;
    // Returns read-only local sap
    const ACE_CLASSIX_Peer_SAP& peer_sap(void) const;
    // Returns read-only peer sap
    int get_remote_addr (ACE_Addr & /* dest */) const;
    // Returns 0, if the remote address has been setup, else -1.
    // The <dest> parameter is not changed.
    // This method is here for compatability with SOCK_Stream.
    // Since the remote addr may be a port or a group and since the
    // address formats are different between a port and a group, the caller
    // cannot provide an appropriate buffer for recording the address.  The
    // <dest> parameter is therefore not used.


    const ACE_CLASSIX_Port& local_addr(void) const;
    // Returns local address in <ACE_CLASSIX_Port> format

    int is_selectable(void) const;
    // Retruns 1, if the local SAP is enabled.
    int is_writer_closed(void) const;
    // Retruns 1, if writer side is closed.
    int is_reader_closed(void) const;
    // Retruns 1, if reader side is closed.


    // = CONTROL
    int  set_peer_sap(const ACE_Addr&);
    // Returns 0, if successful; returns -1, otherwise
    // same as set_addr(const ACE_Addr&);
    int  set_peer_sap(const ACE_CLASSIX_Port_Core&);
    // Returns 0, if successful; returns -1, otherwise
    // same as void set_addr(const ACE_CLASSIX_Port_Core&);
    int set_peer_sap (const KnUniqueId&);
    // Returns 0, if successful; returns -1, otherwise
    int set_peer_sap (const KnCap&);
    // Returns 0, if successful; returns -1, otherwise

    virtual int enable(int) const;
    virtual int disable(int) const;
    // These two methods do nothing. They are here to maintain the same
    // interface as other type of Stream class, e.g. ACE_SOCK_Stream

    int selectable(int = ACE_CLASSIX_SAP::DEFAULT_PRIORITY);
    int unselectable(void);
    // Enables/Disables the local port to be monitored (by a CLASSIX reactor)
    // Default = DISABLED
    virtual void open_writer(void);
    virtual int close_writer(void);
    // Closes down the writer
    // Returns -1, if it fails; returns 0, otherwise
    // Subclass can overwrite this with sophisticated mechanism, such as
    // exchange protocol information with the peer.
    virtual int close_reader(void);
    // Closes down the writer
    // Returns -1, if it fails; returns 0, otherwise
    // Subclass can overwrite this with sophisticated mechanism, such as
    // exchange protocol information with the peer.

    virtual int open(void);
    // for local SAP...
    // Creates a port and uses it as a SAP.
    // A port can be created if the local_sap is not configured or
    // it is configured and the local_sap uses the actor's default port.
    // Returns -1, if it fails; returns 0, otherwise
    virtual int open(const ACE_Addr&);
    // for local SAP...
    // Uses the supplied port information for the local SAP.
    virtual int close(void);
    // for local SAP...
    // Deletes the optional local port and clears the local SAP infomration.

    // = OPTIONS
    int control(u_int = K_BROADMODE);
    int control(u_int, /* mode */ int /* site */ );
    // Interface for CLASSIX' send mode

    // = HELPER
    void dump (void) const;
    // Dump the state of an object.

    ACE_ALLOC_HOOK_DECLARE;
    // Declare the dynamic allocation hooks.

 protected:
    friend class ACE_CLASSIX_CLD_Connector;
    friend class ACE_CLASSIX_CO_Connector;
    friend class ACE_CLASSIX_CO_Acceptor;
    ACE_CLASSIX_SAP&  get_local_sap(void);
    // Returns local sap
    ACE_CLASSIX_Peer_SAP& get_peer_sap(void);
    // Returns peer sap
    int set_local_port(ACE_CLASSIX_Port_Core*);
    // Uses the supplied port as the local port and assumes the ownership
    // of the port. The existing local port, if any, will be deleted.
    // Returns 0, if it is successful.
    // Returns -1, otherwise.  The setting of the original local port
    // will be erased.

    void set_target_(void);
    // set the delivery target structure using the existing peer information

    virtual int set_mode_(u_int = K_BROADMODE); /* theMode */
    // Sets the addressing mode with the current setting of coTarget
    virtual int set_mode_(u_int, /* mode */ int /* site */);
    // Sets the addressing mode as well as the coTarget

    ACE_CLASSIX_SAP          local_sap_;
    int                      local_sap_closed_;
    // 0 if it's not closed(default),
    // 1 if it's closed.
    ACE_CLASSIX_Port_Core   *local_port_;
    // if non-zero, it will be
    // deleted on exit.

    ACE_CLASSIX_Peer_SAP     peer_sap_;
    int                      peer_sap_closed_;
    // 0 if it's not closed(default),
    // 1 if it's closed.

    // Mode applied destination address
    // Can be changed by control()/set_mode_()
    // Mode is applicable in IPC classes other than the Stream class.
    // It is here since it is the destination format used by ipcSend.

    // with/without additional site information
    // int                 dest_site_;
    KnIpcDest           dest_;

 private:
   // Disable copy constructor
  ACE_CLASSIX_Stream(ACE_CLASSIX_Stream const&);
  ACE_CLASSIX_Stream const& operator=(ACE_CLASSIX_Stream const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Stream.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_CLASSIX_Stream_H */
