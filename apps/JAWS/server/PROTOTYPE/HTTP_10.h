/* -*- c++ -*- */

#if !defined (JAWS_HTTP_10_H)
#define JAWS_HTTP_10_H

#include "JAWS/IO.h"

class JAWS_HTTP_IO_Handler : protected JAWS_IO_Handler
{
public:
  JAWS_HTTP_IO_Handler (JAWS_IO *io);
  ~JAWS_HTTP_IO_Handler (void);

  ACE_Message_Block *state (void);

protected:
  // The following methods are inherited from JAWS_IO_Handler

  virtual void accept_complete (void);
  virtual void accept_error (void);
  virtual void read_complete (ACE_Message_Block &data);
  virtual void read_error (void);
  virtual void transmit_file_complete (void);
  virtual void transmit_file_error (int result);
  virtual void receive_file_complete (void);
  virtual void receive_file_error (int result);
  virtual void write_error (void);
  virtual void confirmation_message_complete (void);
  virtual void error_message_complete (void);

private:
  ACE_Message_Block *state_;
  // This maintains the state of the request.

  JAWS_IO *io_;
  // The reference to our IO interface (synch vs. asynch)

  JAWS_Pipeline *pipeline_;
  // This is a reference to the next stage of the pipeline when the IO
  // request completes.

  JAWS_HTTP_IO_Handler_Factory *factory_;
};

class JAWS_HTTP_Data_Block : public ACE_Data_Block
{
public:
  JAWS_HTTP_Data_Block (void);
  virtual ~JAWS_HTTP_Data_Block (void);

private:
};


#endif /* !defined (JAWS_HTTP_10_H) */
