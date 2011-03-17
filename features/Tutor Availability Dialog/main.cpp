#include "tutoravailabilitydialog.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TutorAvailabilityDialog w;
	w.show();
	return a.exec();
}
