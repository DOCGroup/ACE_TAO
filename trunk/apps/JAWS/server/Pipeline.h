/* -*- c++ -*- */
// $Id$

#include "ace/Message_Block.h"
#include "ace/Service_Object.h"


class JAWS_Protocol_Pipeline : public ACE_Service_Object
// = TITLE
//     This is intended to be the base class of pipeline
//     components.
//
{
public:
  JAWS_Protocol_Pipeline (void);
  JAWS_Protocol_Pipeline (JAWS_Protocol_Pipeline *pp);

  virtual ~JAWS_Protocol_Pipeline (void);

  int pull (ACE_Message_Block &mb);
  // Method that initiates the work.

  virtual int hook (ACE_Message_Block &mb) = 0;
  // Required method that finishes the work.


  virtual int init (int, char *[]);
  virtual int fini (void);
  // Service Object initialization and finalization

protected:
  void insert (JAWS_Protocol_Pipeline *pp);
  void append (JAWS_Protocol_Pipeline *pp);

  virtual JAWS_Protocol_Pipeline * pipeline (JAWS_Protocol_Pipeline *pp = 0);
  // Virtual accessor to the preceding pipeline component

};

class JAWS_Protocol_Filter : public JAWS_Protocol_Pipeline
// = TITLE
//     This is intended to be the task skeleton of pipeline
//     components.
//
{
public:
  JAWS_Protocol_Filter (void);
  JAWS_Protocol_Filter (JAWS_Protocol_Pipeline *pp);
  // Constructors

  virtual ~JAWS_Protocol_Filter (void);
  // Destructor

  void add (JAWS_Protocol_Pipeline *component);
  // Insert a pipeline component into the pipeline chain

  virtual int hook (ACE_Message_Block &mb) = 0;
  // Required method that finishes the work.

private:
  virtual JAWS_Protocol_Pipeline * pipeline (JAWS_Protocol_Pipeline * = 0);
  // Accessor to the preceding pipeline component

protected:
  JAWS_Protocol_Pipeline *pipeline_;
  // The preceeding element in the pipeline chain.
};


/*
  This file describes the interfaces to the protocol pipeline
  abstraction for JAWS.  To understand how this is to work, let's
  examine the processing path of a Web server.

  CLIENT                          SERVER
  +--------------.--------------+ +---------------.----------------+
  |Issue Request |Display File  | |Retrieve File  |Receive Request |
  +--------------^--------------+ +---------------^----------------+
   11111111111111 66666666666666   333333333333333 2222222222222222
                 +--------------+ +---------------+
                 |Receive File  | |Send File      |
                 +--------------+ +---------------+
                  55555555555555   444444444444444

  That is, a client issues a request.  The server receives it, returns
  content back to the client.  The client then displays the content.

  But, this ignores issues which surround processing of the content.
  As a simple example, consider the case when the client has limited
  screen real-estate and limited colors (as might be the case with
  PDAs).  Then, when the content involves an image, the processing
  path involve the following additional stages:

  CLIENT                          SERVER
  +--------------.--------------+ +---------------.----------------+
  |Issue Request |Display File  | |Retreive File  |Receive Request |
  +--------------^--------------+ +---------------^----------------+
   11111111111111 88888888888888   333333333333333 2222222222222222
                 +--------------+ +---------------+
                 |Dither Image  | |Send File      |
                 +--------------+ +---------------+
                  77777777777777   444444444444444
                 +--------------+
                 |Scale Image   |
                 +--------------+
                  66666666666666
                 +--------------+
                 |Receive File  |
                 +--------------+
                  55555555555555

  After the server returns the content of the image to the client, the
  client must scale the image to fit on its display device and then
  dither the image to the number of colors which its display device
  can support.

  However, for a PDA with limited resources for battery life and
  computation time, this may not be the optimal path for the file to
  follow to achieve the best possible performance.  For instance, due
  to the bandwidth available to the PDA client, it may be better if
  the server were to scale the image instead.  In this case, the path
  for the requested image would be viewed as the following:

  CLIENT                          SERVER
  +--------------.--------------+ +---------------.----------------+
  |Issue Request |Display File  | |Retreive File  |Receive Request |
  +--------------^--------------+ +---------------^----------------+
   11111111111111 88888888888888   333333333333333 2222222222222222
                 +--------------+ +---------------+
                 |Dither Image  | |Scale File     |
                 +--------------+ +---------------+
                  77777777777777   444444444444444
                 +--------------+ +---------------+
                 |Receive File  | |Send File      |
                 +--------------+ +---------------+
                  66666666666666   555555555555555

  The purpose of Pipeline is to provide a framework which allows for
  this kind of negotiation to occur between a client and the server.
  The protocol by which the client and server negotiate with each
  other is left as an open issue.  Solutions that can be used may
  include use of special headers which identify which actions a client
  would like the server to perform, or through PEP, the protocol
  negotiation protocol.

 */

// Ok, let's figure out an interface that's going to work.

// These pipeline components should provide a method that allows me to
// chain a bunch of them together and then with a single method call,
// have data.

// The usual semantics of programatic pipelines like ours is that the
// first component makes a complete pass over the input before sending
// it to the next component.  This seems like a reasonable semantic,
// but we can do some refinements.

// First, we do not want the hand-off of data to involve copying the
// data from the previous component to the next.  This implies that
// between two consecutive components is a shared buffer.  However, it
// is wasteful to have N+1 buffers for N pipeline components if the
// pipelines are going to be processing the input serially.  This
// leads to the conclusion that only two buffers are necessary for the
// pipeline.

// Second, we need to decide how to specify the makeup or
// functionality of the protocol pipeline.  In UNIX, this is done by
// the command line with "|" symbols between components.  We too could
// use strings.

// Third, we need to decide how to build of these protocol pipelines.
// We may wish for static definitions to be constructed ala the
// Service Configurator.  This implies that the Protocol Pipeline
// Factory is a Service Object, and it can accept a string which
// describes the components of the pipeline.  Since we are using
// strings, it seems that each of the pipeline components themselves
// could also be considered Service Objects to be dynamically loaded.
// However, to maintain some notion of type safety, these pipeline
// components should probably have their own separate Configurator, so
// a Protocol Pipeline Configurator should be created.
