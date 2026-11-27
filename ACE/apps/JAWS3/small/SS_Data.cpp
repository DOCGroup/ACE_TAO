#include "SS_Data.h"
#include "SS_Service_Handler.h"

TeraSS_Data::TeraSS_Data (TeraSS_Service_Handler &handler)
  : mb_ (8 * 1024)
  , handler_ (handler)
{
}

ACE_SOCK_Stream &
TeraSS_Data::peer ()
{
  return this->handler_.peer ();
}

ACE_Message_Block &
TeraSS_Data::mb ()
{
  return this->mb_;
}

ACE_FILE_IO &
TeraSS_Data::file_io ()
{
  return this->file_io_;
}

