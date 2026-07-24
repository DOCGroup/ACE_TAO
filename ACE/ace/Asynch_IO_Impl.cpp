#include "ace/Asynch_IO_Impl.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)
// This only works on Win32 platforms and on Unix platforms supporting
// aio calls.

#include "ace/os_include/os_errno.h"

#if !defined (__ACE_INLINE__)
#include "ace/Asynch_IO_Impl.inl"
#endif /* __ACE_INLINE__ */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_Asynch_Result_Impl::~ACE_Asynch_Result_Impl (void)
{
}

ACE_Asynch_Operation_Impl::~ACE_Asynch_Operation_Impl (void)
{
}

ACE_Asynch_Read_Stream_Impl::~ACE_Asynch_Read_Stream_Impl (void)
{
}

int
ACE_Asynch_Read_Stream_Impl::readv (ACE_Message_Block &/*message_block*/,
                                    size_t /*bytes_to_read*/,
                                    const void */*act*/,
                                    int /*priority*/,
                                    int /*signal_number*/)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_Asynch_Read_Stream_Result_Impl::~ACE_Asynch_Read_Stream_Result_Impl (void)
{
}

ACE_Asynch_Write_Stream_Impl::~ACE_Asynch_Write_Stream_Impl (void)
{
}

int
ACE_Asynch_Write_Stream_Impl::writev (ACE_Message_Block &/*message_block*/,
                                      size_t /*bytes_to_write*/,
                                      const void */*act*/,
                                      int /*priority*/,
                                      int /*signal_number*/)
{
  ACE_NOTSUP_RETURN (-1);
}

ACE_Asynch_Write_Stream_Result_Impl::~ACE_Asynch_Write_Stream_Result_Impl (void)
{
}

ACE_Asynch_Read_File_Impl::~ACE_Asynch_Read_File_Impl (void)
{
}

int
ACE_Asynch_Read_File_Impl::readv (ACE_Message_Block &/*message_block*/,
                                  size_t /*bytes_to_read*/,
                                  u_long /*offset*/,
                                  u_long /*offset_high*/,
                                  const void */*act*/,
                                  int /*priority*/,
                                  int /*signal_number*/)
{
  ACE_NOTSUP_RETURN (-1);
}

int
ACE_Asynch_Read_File_Impl::readv (ACE_Message_Block &message_block,
                                  size_t bytes_to_read,
                                  const void *act,
                                  int priority,
                                  int signal_number)
{
  return this->readv (message_block,
                      bytes_to_read,
                      0,
                      0,
                      act,
                      priority,
                      signal_number);
}

ACE_Asynch_Write_File_Impl::~ACE_Asynch_Write_File_Impl (void)
{
}

int
ACE_Asynch_Write_File_Impl::writev (ACE_Message_Block &/*message_block*/,
                                    size_t /*bytes_to_write*/,
                                    u_long /*offset*/,
                                    u_long /*offset_high*/,
                                    const void */*act*/,
                                    int /*priority*/,
                                    int /*signal_number*/)
{
  ACE_NOTSUP_RETURN (-1);
}

int
ACE_Asynch_Write_File_Impl::writev (ACE_Message_Block &message_block,
                                    size_t bytes_to_write,
                                    const void *act,
                                    int priority,
                                    int signal_number)
{
  return this->writev (message_block,
                       bytes_to_write,
                       0,
                       0,
                       act,
                       priority,
                       signal_number);
}

ACE_Asynch_Read_File_Result_Impl::~ACE_Asynch_Read_File_Result_Impl (void)
{
}

ACE_Asynch_Write_File_Result_Impl::~ACE_Asynch_Write_File_Result_Impl (void)
{
}

ACE_Asynch_Accept_Result_Impl::~ACE_Asynch_Accept_Result_Impl (void)
{
}

ACE_Asynch_Connect_Result_Impl::~ACE_Asynch_Connect_Result_Impl (void)
{
}

ACE_Asynch_Accept_Impl::~ACE_Asynch_Accept_Impl (void)
{
}

ACE_Asynch_Connect_Impl::~ACE_Asynch_Connect_Impl (void)
{
}

ACE_Asynch_Transmit_File_Impl::~ACE_Asynch_Transmit_File_Impl (void)
{
}

ACE_Asynch_Transmit_File_Result_Impl::~ACE_Asynch_Transmit_File_Result_Impl (void)
{
}

ACE_Asynch_Read_Dgram_Impl::~ACE_Asynch_Read_Dgram_Impl (void)
{
}

ACE_Asynch_Read_Dgram_Impl::ACE_Asynch_Read_Dgram_Impl (void)
{
}

ACE_Asynch_Write_Dgram_Impl::~ACE_Asynch_Write_Dgram_Impl (void)
{
}

ACE_Asynch_Write_Dgram_Impl::ACE_Asynch_Write_Dgram_Impl (void)
{
}

//***********************************************

ACE_Asynch_Read_Dgram_Result_Impl::~ACE_Asynch_Read_Dgram_Result_Impl (void)
{
}

ACE_Asynch_Read_Dgram_Result_Impl::ACE_Asynch_Read_Dgram_Result_Impl (void)
{
}

//***********************************************

ACE_Asynch_Write_Dgram_Result_Impl::~ACE_Asynch_Write_Dgram_Result_Impl (void)
{
}

ACE_Asynch_Write_Dgram_Result_Impl::ACE_Asynch_Write_Dgram_Result_Impl (void)
{
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
