#include "programsettingsdialog.h"

#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>

ProgramSettingsDialog::ProgramSettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	QLabel *locationLabel = new QLabel(tr("Location:"));
	locationComboBox = new QComboBox;
	QLabel *termLabel = new QLabel(tr("Term:"));
	termComboBox = new QComboBox;
	okButton = new QPushButton(tr("OK"));
	cancelButton = new QPushButton(tr("Cancel"));

	QStringList locationList = (QStringList() << tr("Select one") 
											  << tr("Path Side") 
											  << tr("North Hudson Center")
											  << tr("Writing Center"));
	locationComboBox->addItems(locationList);

	QStringList termList = (QStringList() << tr("Select one")
										  << tr("Spring")
										  << tr("Fall")
										  << tr("Summer I")
										  << tr("Summer II")
										  << tr("Winter"));
	termComboBox->addItems(termList);

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(locationLabel, 0, 0);
	layout->addWidget(locationComboBox, 0, 1);
	layout->addWidget(termLabel, 1, 0);
	layout->addWidget(termComboBox, 1, 1);

	QGroupBox *groupBox = new QGroupBox(tr("Program Settings"));
	groupBox->setLayout(layout);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch();
	buttonLayout->addWidget(cancelButton);
	buttonLayout->addWidget(okButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(groupBox);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Configure Program Settings"));
	resize(260, 100);
}

void ProgramSettingsDialog::okButtonClicked()
{
	QFile file("settings.dat");
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&file);
		out << locationComboBox->currentText() << endl << termComboBox->currentText();
		file.close();
	}
	QMessageBox::information(this, tr("Settings Saved"),
		tr("Program settings saved successfully."));
	emit(programSettingsChanged());
	accept();
}

void ProgramSettingsDialog::cancelButtonClicked()
{
	reject();
}

void ProgramSettingsDialog::setLocation(const QString &location)
{
	locationComboBox->setCurrentIndex(locationComboBox->findText(location));
}

void ProgramSettingsDialog::setTerm(const QString &term)
{
	termComboBox->setCurrentIndex(termComboBox->findText(term));
}