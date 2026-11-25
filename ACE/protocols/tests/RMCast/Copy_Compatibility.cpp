/**
 * @file Copy_Compatibility.cpp
 *
 * Check that derived protocol classes can copy their base from an rvalue.
 */

#include "ace/RMCast/Protocol.h"

#include <utility>

namespace
{
  struct Profile_Test : ACE_RMCast::Profile
  {
    Profile_Test () : Profile (1) {}
    Profile_Test (Profile_Test &&other) : Profile (std::move (other)) {}

    ACE_RMCast::Profile_ptr clone_ () override
    {
      return ACE_RMCast::Profile_ptr ();
    }
  };

  struct Message_Test : ACE_RMCast::Message
  {
    Message_Test () = default;
    Message_Test (Message_Test &&other) : Message (std::move (other)) {}
  };

  struct Data_Test : ACE_RMCast::Data
  {
    Data_Test () : Data (nullptr, 0) {}
    Data_Test (Data_Test &&other) : Data (std::move (other)) {}
  };
}
