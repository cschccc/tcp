#ifndef COURSESETTINGS_H
#define COURSESETTINGS_H

#include <QWidget>

class QTableWidget;
class QPushButton;

class CourseSettings : public QWidget
{
	Q_OBJECT

public:
	CourseSettings(QWidget *parent = 0);

private:
	QTableWidget *courseTable;
	QPushButton *addButton;
	QPushButton *editButton;
	QPushButton *removeButton;
};

#endif // COURSESETTINGS_H