// $Id$

#ifndef __DETAILVIEW_H
#define __DETAILVIEW_H


#include "NavUnit.h"
#include <Qt/qwidget.h>
#include <Qt/qmovie.h>
#include <Qt/q3table.h>

class QLabel;
class QLineEdit;
class QSpinBox;

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
  Q3Table *child_list;
  Q3Table *parent_list;
  QLabel *movielabel;
  QMovie *movie;
  QLabel *separatorlabel;
  QMovie separator;


  NavUnit *current_unit;
};


#endif // __DETAILVIEW_H
