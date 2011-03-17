#ifndef TERMSETTINGS_H
#define TERMSETTINGS_H

#include <QWidget>

class QLabel;
class QListWidget;
class QPushButton;
class QListWidgetItem;

class TermSettings : public QWidget
{
	Q_OBJECT

public:
	TermSettings(QWidget *parent = 0);

public slots:
	void termSelected(QListWidgetItem *);

private:
	QLabel *termLabel;
	QLabel *currentTermLabel;
	QListWidget *termList;
	QPushButton *addButton;
	QPushButton *editButton;
	QPushButton *removeButton;
};

#endif // TERMSETTINGS_H