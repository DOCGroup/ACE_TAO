// $Id$

namespace CUTS
{
  //
  // collection_threads
  //
  CUTS_INLINE
  size_t BDC_Task::collection_threads (void) const
  {
    return this->collection_threads_;
  }

  //
  // collection_threads
  //
  CUTS_INLINE
  void BDC_Task::collection_threads (size_t threads)
  {
    this->collection_threads_ = threads;
  }

  //
  // timeout
  //
  CUTS_INLINE
  long BDC_Task::timeout (void) const
  {
    return this->timeout_;
  }

  //
  // timeout
  //
  CUTS_INLINE
  void BDC_Task::timeout (long timeout)
  {
    this->timeout_ = timeout;
  }

  //
  // testing_service
  //
  CUTS_INLINE
  void BDC_Task::testing_service (Testing_Service_exec_i * testing_service)
  {
    this->testing_service_ = testing_service;
  }

  //
  // metrics_outfile
  //
  CUTS_INLINE
  void BDC_Task::metrics_outfile (const std::string & filename)
  {
    this->metrics_outfile_ = filename;
  }

  //
  // metrics_outfile
  //
  CUTS_INLINE
  const std::string & BDC_Task::metrics_outfile (void) const
  {
    return this->metrics_outfile_;
  }
}
