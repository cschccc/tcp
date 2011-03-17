#include "tutorsettings.h"

#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

TutorSettings::TutorSettings(QWidget *parent)
	:QWidget(parent)
{
	hourLabel = new QLabel(tr("Maximum hours allowed:"));
	hourSpinBox = new QSpinBox;
	hourSpinBox->setRange(0, 99);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(hourLabel);
	layout->addWidget(hourSpinBox);
	layout->addStretch(true);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(layout);
	mainLayout->addStretch(true);

	setLayout(mainLayout);
}