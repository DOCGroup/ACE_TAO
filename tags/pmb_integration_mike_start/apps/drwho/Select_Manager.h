/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    drwho
//
// = FILENAME
//    Select_Manager.h
//
// = AUTHOR
//    Douglas C. Schmidt
//
// ============================================================================

#ifndef _SELECT_MANAGER_H
#define _SELECT_MANAGER_H

class Select_Manager
{
public:
  virtual int mux (char *packet,
                   int &packet_length) = 0 ;

  virtual int demux (char *packet,
                     int &packet_length) = 0;
};

#endif /* _SELECT_MANAGER_H */
