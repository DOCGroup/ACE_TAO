// $Id$

#ifndef __DETAILVIEW_H
#define __DETAILVIEW_H


#include "NavUnit.h"
#include <qwidget.h>
#include <qmovie.h>


class QLabel;
class QLineEdit;
class QSpinBox;
class QTable;


class DetailView : public QWidget
{
  Q_OBJECT

public:
  DetailView(QWidget *parent, const char *name = 0);
  virtual ~DetailView();

public slots:
  void movieStatus(int);
  void movieUpdate(const QRect&);
  void currentNode(NavUnit *unit);
  void updateLocation(const UnitLocation &loc);
  void apply();

private:
  QLineEdit *sem_id_val;
  QLineEdit *descr_edit;
  QSpinBox *x_edit;
  QSpinBox *y_edit;
  QSpinBox *z_edit;
  QTable *child_list;
  QTable *parent_list;
  QLabel *movielabel;
  QMovie movie;
  QLabel *separatorlabel;
  QMovie separator;


  NavUnit *current_unit;
};


#endif // __DETAILVIEW_H
