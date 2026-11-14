/* -*- C++ -*- */
#ifndef MESSAGE_H
#define MESSAGE_H

class RecordingDevice;

class Message
{
public:
  Message () : device_ (nullptr), type_ (nullptr), id_ (nullptr)
  { }

  ~Message () = default;

  RecordingDevice *recorder () const
  {
    return this->device_;
  }

  void recorder (RecordingDevice *device)
  {
    this->device_ = device;
  }

  void type (MessageType *type)
  {
    this->type_ = type;
  }

  MessageType *type () const
  {
    return this->type_;
  }

  void caller_id (CallerId *id)
  {
    this->id_ = id;
  }

  CallerId *caller_id () const
  {
    return this->id_;
  }

  void addr (ACE_FILE_Addr &addr)
  {
    this->addr_ = addr;
  }

  void incoming_message (ACE_FILE_Addr &addr, MessageType *type)
  {
    this->addr_ = addr;
    this->type_ = type;
  }

  ACE_FILE_Addr &addr ()
  {
    return this->addr_;
  }

  bool is_text () const
  {
    return this->type_->is_text ();
  }

  bool is_audio () const
  {
    return this->type_->is_audio ();
  }

  bool is_video () const
  {
    return this->type_->is_video ();
  }

private:
  RecordingDevice *device_;
  MessageType *type_;
  CallerId *id_;
  ACE_FILE_Addr addr_;
};

class AudioMessage : public Message
{ };

class VideoMessage : public Message
{ };

#endif /* MESSAGE_H */
