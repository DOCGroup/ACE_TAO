// $Id$

#include "DetailView.h"
#include <Qt/qlayout.h>
#include <Qt/qlabel.h>
#include <Qt/qspinbox.h>

DetailView::DetailView(QWidget *parent)
: QWidget(parent)
{
  QGridLayout *grid = new QGridLayout(this);
  grid->setHorizontalSpacing (1);
  grid->setColumnMinimumWidth (0, 79);
  grid->setColumnMinimumWidth (1, 30);
  grid->setColumnMinimumWidth (2, 30);
  grid->setColumnMinimumWidth (3, 35);
  grid->setColumnMinimumWidth (4, 80);
  grid->setColumnMinimumWidth (5, 30);
  grid->setColumnMinimumWidth (6, 30);

  QLabel *x_label = new QLabel("X coordinate red GPS:", this);
  grid->addWidget(x_label, 1, 0);
  x_edit = new QSpinBox(this);
  x_edit->setMinimum(0);
  x_edit->setMaximum(65535);
  x_edit->setReadOnly(true);
  grid->addWidget(x_edit, 1, 2);
  QLabel *y_label = new QLabel("Y coordinate red GPS:", this);
  grid->addWidget(y_label, 1, 4);
  y_edit = new QSpinBox(this);
  y_edit->setMinimum(0);
  y_edit->setMaximum(65535);
  y_edit->setReadOnly(true);
  grid->addWidget(y_edit, 1, 6);
}

DetailView::~DetailView()
{
}

void
DetailView::currentNode(NavUnit *unit)
{
  UnitLocation loc = unit->getLocation();
  this->updateLocation(loc);
}

void
DetailView::updateLocation(const UnitLocation &loc)
{
  x_edit->setValue(static_cast<int>(loc.x_));
  y_edit->setValue(static_cast<int>(loc.y_));
}
