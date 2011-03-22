#include "settingsdialog.h"
#include "coursesettings.h"
#include "majorsettings.h"
#include "tutorsettings.h"
#include "emailsettings.h"
#include "termsettings.h"
#include "locationsettings.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	tabWidget = new QTabWidget(this);
	tabWidget->addTab(new TermSettings, tr("Term"));
	tabWidget->addTab(new LocationSettings, tr("Location"));
	tabWidget->addTab(new CourseSettings, tr("Courses"));
	tabWidget->addTab(new MajorSettings, tr("Majors"));
	tabWidget->addTab(new TutorSettings, tr("Tutors"));
	tabWidget->addTab(new EmailSettings, tr("Email"));

	okButton = new QPushButton(tr("OK"));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(true);
	buttonLayout->addWidget(okButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabWidget);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Program Settings"));
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::okButtonClicked()
{
	accept();
}
