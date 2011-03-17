#include "customtablewidget.h"

CustomTableWidget::CustomTableWidget(QWidget *parent)
	: QTableWidget(parent)
{

}

CustomTableWidget::CustomTableWidget(int rows, int columns)
	: QTableWidget(rows, columns)
{

}

QModelIndexList CustomTableWidget::selectedIndexes () const
{
	return QTableWidget::selectedIndexes();
}