// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_helper.h
//
// = DESCRIPTION
//    Helper utilities and global singleton "params" class
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (TAO_BE_HELPER_H)
#define TAO_BE_HELPER_H

#include        "iostream.h"
#include        "fstream.h"
#include        "ace/ACE.h"
#include        "ace/OS.h"
#include        "ace/Singleton.h"
#include        "ace/Synch.h"

class TAO_BE_Params
// = TITLE
//   Holds global parameters for the Back End
//
// = DESCRIPTION
//
{
public:
  TAO_BE_Params();
  // Constructor

  void client_header(streambuf *sbuf);
  // set the client header stream

  streambuf* client_header();
  // get the client header stream

  void client_stubs(streambuf* f);
  // set the client stub stream

  streambuf* client_stubs();
  // get the client stubs stream

  void server_header(streambuf* f);
  // set the server header stream

  streambuf* server_header();
  // get the server header stream

  void server_skeletons(streambuf* f);
  // set the server skeletons stream

  streambuf* server_skeletons();
  // get the server skeletons stream
private:
  streambuf *pd_client_header;
  // client header stream

  streambuf *pd_client_stubs;
  // client stub file stream

  streambuf *pd_server_header;
  // server header stream

  streambuf *pd_server_skeletons;
  // server skeleton stream
};

typedef ACE_Singleton<TAO_BE_Params,ACE_Thread_Mutex> TAO_BE_PARAMS;
// Singleton instance of BE parameters

const char* be_get_client_hdr_fname();
const char* be_get_client_stub_fname();
const char* be_get_server_hdr_fname();
const char* be_get_server_skeleton_fname();


#endif
