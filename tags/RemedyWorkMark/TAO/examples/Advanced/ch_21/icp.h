// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/examples/Advanced/ch_21
//
// = FILENAME
//    icp.h
//
// = AUTHORS
//   Source code used in TAO has been modified and adapted from the code
//   provided in the book, "Advanced CORBA Programming with C++" by Michi
//   Henning and Steve Vinoski. Copyright 1999. Addison-Wesley, Reading,
//   MA.
//
//   Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
//
// ============================================================================

#ifndef _ICP_H
#define _ICP_H

extern "C" {
    int ICP_online(unsigned long id);   // Add device
    int ICP_offline(unsigned long id);  // Remove device
    int ICP_get(                        // Get attribute
            unsigned long   id,
            const char *    attr,
            void *          value,
            size_t          len
        );
    int ICP_set(                        // Set attribute
            unsigned long   id,
            const char *    attr,
            const void *    value
        );
}

#endif /* _ICP_H */









