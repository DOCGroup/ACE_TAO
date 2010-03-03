// $Id$

#include "Convert_Plan_Impl.h"
#include "ace/FILE_Connector.h"
#include "tao/CDR.h"
#include "Config_Handlers/XML_File_Intf.h"
#include "Logger/Log_Macros.h"

namespace DAnCE
{
  Deployment::DeploymentPlan * 
  Convert_Plan::read_xml_plan (const char *filename)
  {
    CIAO::Config_Handlers::XML_File_Intf xml_intf (filename);
    xml_intf.add_search_path ("ACE_ROOT", "/docs/schema/");
    xml_intf.add_search_path ("TAO_ROOT", "/docs/schema/");
    xml_intf.add_search_path ("CIAO_ROOT", "/docs/schema/");
    xml_intf.add_search_path ("DANCE_ROOT", "/docs/schema/");
  
    return xml_intf.release_plan ();  
  }

  Deployment::DeploymentPlan * 
  Convert_Plan::read_cdr_plan (const char *filename)
  {
    ACE_FILE_Addr file_addr (ACE_sap_any_cast (ACE_FILE_Addr &));
  
    if (filename)
      file_addr.set (filename);
    else
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::read_cdr_plan - passed nil file name\n"));
        return 0;
      }
      
    ACE_FILE_Connector conn;
    ACE_FILE_IO file;
  
    if (conn.connect (file,
                      file_addr,
                      0,
                      ACE_Addr::sap_any,
                      0,
                      O_RDONLY,
                      ACE_DEFAULT_FILE_PERMS == -1))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::read_cdr_plan - unable to read file %p\n",
                         filename));
        return 0;
      }
  
    ACE_FILE_Info info;
    if (file.get_info (info) == -1)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::read_cdr_plan - unable to determine file size %p\n",
                         filename));
        return 0;
      }
  
    ACE_OFF_T bufsize = info.size_ - 1;
  
    char *buffer = 0;
    ACE_NEW_RETURN (buffer,
                    char[bufsize],
                    0);
  
#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
    ACE_OS::memset(buffer, 0, sizeof (buffer));
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */
  
    // Make sure <buffer> is released automagically.
    ACE_Auto_Basic_Array_Ptr<char> b (buffer);
  
    if (file.seek (0, SEEK_SET) == -1)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::read_cdr_plan - seek failed %p\n",
                         filename));
        return 0;
      }
  
    char byte_order;
    ssize_t size = file.recv (&byte_order, 1);
    size = file.recv (buffer, bufsize);
  
    if (size != bufsize)
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::read_cdr_plan - read %u bytes "
                         "instead of %u bytes\n", size, bufsize));
        return 0;
      }
  
    ACE_Message_Block mb (ACE_CDR::MAX_ALIGNMENT + bufsize);
    ACE_CDR::mb_align (&mb);
    mb.copy (buffer, bufsize);
  
    TAO_InputCDR input_cdr (&mb);
    input_cdr.reset_byte_order ((int) byte_order);
  
    DANCE_DEBUG (9, (LM_TRACE, DLINFO "Convert_Plan_Impl::read_cdr_plan - "
                     "Reading file %C in %s endian format\n",
                     filename, ACE_CDR_BYTE_ORDER ? ACE_TEXT("little") : ACE_TEXT("big")));
  
  
    Deployment::DeploymentPlan *retval;
  
    ACE_NEW_RETURN (retval, Deployment::DeploymentPlan (), 0);
  
    input_cdr >> *retval;
  
    file.close ();
  
    return retval;
  }

  bool
  Convert_Plan::write_cdr_plan (const char *output_filename, Deployment::DeploymentPlan &plan)
  {
    ACE_FILE_Addr file_addr (ACE_sap_any_cast (ACE_FILE_Addr &));
  
    if (output_filename)
      file_addr.set (output_filename);
    else
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::write_cdr_plan - passed nil file name\n"));
        return 0;
      }
      
    ACE_FILE_Connector conn;
    ACE_FILE_IO file;
  
    if (conn.connect (file,
                      file_addr,
                      0,
                      ACE_Addr::sap_any,
                      0,
                      O_RDWR | O_CREAT,
                      ACE_DEFAULT_FILE_PERMS == -1))
      {
        DANCE_ERROR (1, (LM_ERROR, DLINFO "Convert_Plan_Impl::write_cdr_plan - unable to write file %p\n",
                         output_filename));
        return 0;
      }
  
    char byte_order = ACE_CDR_BYTE_ORDER;
    size_t n = file.send (&byte_order, 1);
  
    TAO_OutputCDR output_cdr (0, ACE_CDR_BYTE_ORDER, 0, 0, 0, 
                              ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                              ACE_CDR_GIOP_MAJOR_VERSION,
                              ACE_CDR_GIOP_MINOR_VERSION);
  
    output_cdr << plan;
  
    const ACE_Message_Block *output_mb = output_cdr.begin ();
  
    DANCE_DEBUG (9, (LM_TRACE, DLINFO  "Convert_Plan_Impl::write_cdr_plan - "
                     ACE_TEXT ("Writing plan to file %s in %s endian format\n"),
                     output_filename,
                     ACE_CDR_BYTE_ORDER ? ACE_TEXT("little") : ACE_TEXT("big")));
  
    n = file.send (output_mb->rd_ptr (),
                   output_mb->length ());
  
    file.close ();
  
    return true;    
  }
}
