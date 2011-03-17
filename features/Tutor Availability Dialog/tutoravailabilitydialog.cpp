#include "tutoravailabilitydialog.h"

#include <QMessageBox>
#include <QTableWidget>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

TutorAvailabilityDialog::TutorAvailabilityDialog(QWidget *parent)
	: QDialog(parent)
{
	okButton = new QPushButton(tr("OK"));
	cancelButton = new QPushButton(tr("Cancel"));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(true);
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(createTutorInfoGroupBox(), 0, 0);
	layout->addWidget(createSubjectsGroupBox(), 0, 1);
	layout->addWidget(createAvailabilityGroupBox(), 1, 0, 1, -1);
	layout->addLayout(buttonLayout, 2, 1, 1, -1);
	
	//Added in the unofficial-official CSC meeting on thursday by Witold & Xavier
	QAbstractButton *cornerButton = availabilityTableWidget->findChild<QAbstractButton*>();
	// Since it's not part of the API, there is no guarantee it exists
	if (cornerButton) 
	{
		cornerButton->disconnect();
		connect(cornerButton, SIGNAL(clicked()), this, SLOT(cornerButtonClicked()));
	}
	connect(availabilityTableWidget, SIGNAL(itemSelectionChanged()), this,SLOT(tableSelectionChanged()));
	//END
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
	connect(availabilityTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(timeSlotClicked(int, int)));
		connect(reinterpret_cast<const QObject *>(availabilityTableWidget->horizontalHeader()), SIGNAL(sectionClicked(int)),
		this, SLOT(horizontalHeaderClicked(int)));
	connect(reinterpret_cast<const QObject *>(availabilityTableWidget->verticalHeader()), SIGNAL(sectionClicked(int)),
		this, SLOT(verticalHeaderClicked(int)));

	setWindowTitle(tr("Tutor Availability"));
	setLayout(layout);
}

TutorAvailabilityDialog::~TutorAvailabilityDialog()
{

}
//Added in the unofficial-official CSC meeting on thursday by Witold & Xavier
void TutorAvailabilityDialog::tableSelectionChanged()
{
	int numHours = availabilityTableWidget->selectedIndexes().size();
	hoursCountLabel->setText(QString().setNum(numHours));
	if (numHours > 24) {
		QMessageBox::warning(this, tr("Hours limit reached"), tr("You cannot work more than 24 hours per week."));
		//availabilityTableWidget->setRangeSelected(QTableWidgetSelectionRange(row, column, row, column), false);
		//availabilityTableWidget->selectedItems
		return;
	}
}

void TutorAvailabilityDialog::cornerButtonClicked()
{
	availabilityTableWidget->clearSelection();
	
}


void TutorAvailabilityDialog::verticalHeaderClicked(int row)
{
	//int numHours = availabilityTableWidget->selectedIndexes().size();
	//if (numHours > 24) {
	//	QMessageBox::warning(this, tr("Hours limit reached"), tr("You cannot work more than 24 hours per week."));
	//	availabilityTableWidget->setRangeSelected(QTableWidgetSelectionRange(row, 0, row, 5), false);
	//	return;
	//}
	
}

void TutorAvailabilityDialog::horizontalHeaderClicked(int column)
{
	//int numHours = availabilityTableWidget->selectedIndexes().size();
	//if (numHours > 24) {
	//	QMessageBox::warning(this, tr("Hours limit reached"), tr("You cannot work more than 24 hours per week."));
	//	availabilityTableWidget->setRangeSelected(QTableWidgetSelectionRange(0, column, 11, column), false);
	//	return;
	//}
	//hoursCountLabel->setText(QString().setNum(numHours));
}
//END
void TutorAvailabilityDialog::okButtonClicked()
{
	accept();
}

void TutorAvailabilityDialog::cancelButtonClicked()
{
	reject();
}

void TutorAvailabilityDialog::timeSlotClicked(int row, int column)
{	
    
	
	int numHours = availabilityTableWidget->selectedIndexes().size();
	if (numHours > 24) {
		QMessageBox::warning(this, tr("Error"), tr("You cannot work more than 24 hours per week."));
		availabilityTableWidget->setRangeSelected(QTableWidgetSelectionRange(row, column, row, column), false);
		return;
	}
	hoursCountLabel->setText(QString().setNum(numHours));
}

QGroupBox *TutorAvailabilityDialog::createTutorInfoGroupBox()
{
	nameLabel = new QLabel(tr("Tutor Name:"));
	phoneNumberLabel = new QLabel(tr("Phone Number:"));
	nameLineEdit = new QLineEdit;
	phoneNumberLineEdit = new QLineEdit;
	
	QGridLayout *layout = new QGridLayout;
	layout->setHorizontalSpacing(20);
	layout->addWidget(nameLabel, 0, 0);
	layout->addWidget(nameLineEdit, 0, 1);
	layout->addWidget(phoneNumberLabel, 1, 0);
	layout->addWidget(phoneNumberLineEdit, 1, 1);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(layout);
	mainLayout->addStretch(true);
	
	QGroupBox *groupBox = new QGroupBox(tr("Tutor Information"));
	groupBox->setLayout(mainLayout);
	return groupBox;
}

QGroupBox *TutorAvailabilityDialog::createSubjectsGroupBox()
{
	coursesTableWidget = new QTableWidget;
	coursesTableWidget->setMaximumSize(QSize(280, 90));
	addButton = new QPushButton(tr("Add Course"));
	removeButton = new QPushButton(tr("Remove"));

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(removeButton);
	buttonLayout->addStretch(true);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addLayout(buttonLayout);
	layout->addWidget(coursesTableWidget);

	QGroupBox *groupBox = new QGroupBox(tr("Courses"));
	groupBox->setLayout(layout);
	return groupBox;
}

QGroupBox *TutorAvailabilityDialog::createAvailabilityGroupBox()
{
	availabilityTableWidget = new CustomTableWidget(12, 6);
	for (int i = 0; i <= availabilityTableWidget->rowCount(); i++)
		availabilityTableWidget->setRowHeight(i, 21);
	for (int i = 0; i <= availabilityTableWidget->columnCount(); i++)
		availabilityTableWidget->setColumnWidth(i, 86);
	availabilityTableWidget->setHorizontalHeaderLabels(QStringList() << tr("Monday")
																	 << tr("Tuesday")
																	 << tr("Wednesday")
																	 << tr("Thursday")
																	 << tr("Friday")
																	 << tr("Saturday"));
	availabilityTableWidget->setVerticalHeaderLabels(QStringList() << "09:00 - 10:00"
																   << "10:00 - 11:00"
																   << "11:00 - 12:00"
																   << "12:00 - 01:00"
																   << "01:00 - 02:00"
																   << "02:00 - 03:00"
																   << "03:00 - 04:00"
																   << "04:00 - 05:00"
																   << "05:00 - 06:00"
																   << "06:00 - 07:00"
																   << "07:00 - 08:00"
																   << "08:00 - 09:00");
	availabilityTableWidget->setMinimumSize(QSize(600, 278));
	availabilityTableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	availabilityTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//Added in the unofficial-official CSC meeting on thursday by Witold & Xavier
	availabilityTableWidget->setDragEnabled(false);
	//END
	hoursMessageLabel = new QLabel(tr("<b>Total hours:</b>"));
	hoursCountLabel = new QLabel(tr("0"));
	availabilityMessage = new QLabel(tr("<b>Click the hours you are available to work.</b>"));

	QHBoxLayout *messageLayout = new QHBoxLayout;
	messageLayout->addWidget(hoursMessageLabel);
	messageLayout->addWidget(hoursCountLabel);
	messageLayout->addStretch(true);
	
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(availabilityMessage, 0, 0, 1, -1);
	layout->addWidget(availabilityTableWidget, 1, 0, 1, -1);
	layout->addLayout(messageLayout, 2, 0);

	QGroupBox *groupBox = new QGroupBox(tr("Availability"));
	groupBox->setLayout(layout);
	return groupBox;
}

/*QString selectedrows;
	QTableWidgetItem *selectedrow;
	selectedrow=availabilityTableWidget->verticalHeaderItem(row);
	selectedrows=selectedrow->text();
	QMessageBox::information(this, "", selectedrows);*/
//this will handle the retrieving of day and time of tutor availability