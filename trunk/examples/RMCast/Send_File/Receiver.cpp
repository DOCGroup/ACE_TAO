// $Id$

#include "ace/RMCast/RMCast_UDP_Reliable_Receiver.h"
#include "ace/INET_Addr.h"
#include "ace/FILE_IO.h"
#include "ace/Message_Block.h"

ACE_RCSID(tests, RMCast_Examples_Receiver, "$Id$")

class File_Module : public ACE_RMCast_Module
{
public:
  File_Module (void);

  /// Return 1 if all the data has been received
  int status (void) const;

  /// Initialize the module
  int open (const char *filename);

  int close (void);
  int data (ACE_RMCast::Data &data);
  int ack_join (ACE_RMCast::Ack_Join &ack_join);
  int ack_leave (ACE_RMCast::Ack_Leave &ack_leave);

private:
  /// Set to 1 when the last block is received
  int status_;

  /// Used to dump the received data into a file
  ACE_FILE_IO file_io_;
};

int
main (int argc, char *argv[])
{
  if (argc != 3)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Usage: %s <filename> <mcastgroup:port>\n",
                         argv[0]),
                        1);
    }

  const char *filename = argv[1];

  File_Module file_module;
  if (file_module.open (filename) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open file module\n"),
                        1);
    }

  ACE_RMCast_UDP_Reliable_Receiver receiver (&file_module);

  ACE_INET_Addr mcast_group;
  if (mcast_group.set (argv[2]) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot setup multicast group <%s>\n",
                         argv[2]),
                        1);
    }

  if (receiver.open (mcast_group) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open UDP I/O at <%s:%d> %p\n",
                         mcast_group.get_host_name (),
                         mcast_group.get_port_number (),
                         ""),
                        1);
    }

  // Use the Reactor to demultiplex all the messages
  ACE_Reactor *reactor = ACE_Reactor::instance ();
  receiver.reactive_incoming_messages (reactor);

  // Wait until all the messages are successfully delivered
  do
    {
      // Try for 50 milliseconds...
      ACE_Time_Value tv (5, 0); // 0, 50000);
      int r = reactor->handle_events (&tv);
      if (r == -1)
        break;
    }
  while (file_module.status () != 2);

  ACE_DEBUG ((LM_DEBUG, "event loop completed\n"));

  return 0;
}

// ****************************************************************

File_Module::File_Module (void)
  :  status_ (0)
{
}

int
File_Module::status (void) const
{
  return this->status_;
}

int
File_Module::open (const char * filename)
{
  ACE_HANDLE handle = ACE_OS::open (filename,
                                    O_WRONLY|O_BINARY|O_CREAT,
                                    0644);
  if (handle == ACE_INVALID_HANDLE)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open file <%s> %p\n", filename, ""),
                        -1);
    }
  this->file_io_.set_handle (handle);
  return 0;
}

int
File_Module::close (void)
{
  ACE_DEBUG ((LM_DEBUG, "File_Module closed\n"));
  (void) this->file_io_.close ();
  return 0;
}

int
File_Module::data (ACE_RMCast::Data &data)
{
  if (this->status_ == 1)
    return -1;

  size_t length = data.payload->length () - 1;
  (void) this->file_io_.send (data.payload->rd_ptr () + 1, length);

  if (*(data.payload->rd_ptr ()) == 'E')
    {
      this->status_ = 1;
      return -1;
    }

  return 0;
}

int
File_Module::ack_join (ACE_RMCast::Ack_Join &)
{
  ACE_DEBUG ((LM_DEBUG,
              "File_Module::ack_join\n"));
  return 0;
}

int
File_Module::ack_leave (ACE_RMCast::Ack_Leave &)
{
  ACE_DEBUG ((LM_DEBUG,
              "File_Module::ack_leave\n"));
  this->status_ = 2;
  return 0;
}
