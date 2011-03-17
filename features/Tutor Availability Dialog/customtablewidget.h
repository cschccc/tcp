#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QTableWidget>

class CustomTableWidget : public QTableWidget
{
public:
	CustomTableWidget(QWidget *parent);
	CustomTableWidget(int, int);

public:
	QModelIndexList selectedIndexes () const;
};

#endif // CUSTOMTABLEWIDGET_H