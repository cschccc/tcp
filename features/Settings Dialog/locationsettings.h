#ifndef LOCATIONSETTINGS_H
#define LOCATIONSETTINGS_H

#include <QWidget>

class QLabel;
class QListWidget;
class QPushButton;
class QListWidgetItem;

class LocationSettings : public QWidget
{
	Q_OBJECT

public:
	LocationSettings(QWidget *parent = 0);

public slots:
	void locationSelected(QListWidgetItem *);

private:
	QLabel *locationLabel;
	QLabel *currentLocationLabel;
	QListWidget *locationList;
	QPushButton *addButton;
	QPushButton *editButton;
	QPushButton *removeButton;
};

#endif // LOCATIONSETTINGS_H