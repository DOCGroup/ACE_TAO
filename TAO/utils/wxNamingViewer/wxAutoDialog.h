// wxAutoDialog.h

#ifndef i_wxAutoDialog_h
#define i_wxAutoDialog_h

template <class D>
class WxAutoDialog
{
public:
  explicit WxAutoDialog( D* dialog) noexcept:dialog( dialog) {}
  ~WxAutoDialog() noexcept { dialog->Destroy(); }

  D* operator->() const noexcept { return dialog; }

protected:
  D* dialog;

private:
  // Unimplemented
  WxAutoDialog( const WxAutoDialog<D>&);
  WxAutoDialog<D>& operator=( const WxAutoDialog<D>&);
};

#endif
