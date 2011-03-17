#ifndef MAJORSETTINGS_H
#define MAJORSETTINGS_H

#include <QWidget>

class QListWidget;
class QPushButton;

class MajorSettings : public QWidget
{
	Q_OBJECT

public:
	MajorSettings(QWidget *parent = 0);

private:
	QListWidget *majorList;
	QPushButton *addButton;
	QPushButton *editButton;
	QPushButton *removeButton;
};

#endif // MAJORSETTINGS_H