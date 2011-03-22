#ifndef TUTORSETTINGS_H
#define TUTORSETTINGS_H

#include <QWidget>

class QLabel;
class QSpinBox;

class TutorSettings : public QWidget
{
	Q_OBJECT

public:
	TutorSettings(QWidget *parent = 0);

private:
	QLabel *hourLabel;
	QSpinBox *hourSpinBox;
};

#endif // TUTORSETTINGS_H