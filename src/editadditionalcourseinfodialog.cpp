#include "editadditionalcourseinfodialog.h"

#include <QTableWidgetItem>
#include <QMessageBox>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGridLayout>

EditAdditionalCourseInfoDialog::EditAdditionalCourseInfoDialog(QWidget *parent)
	: QDialog(parent)
{
	messageLabel = new QLabel;
	QLabel *instructorNameLabel = new QLabel(tr("Instructor's Name:"));
	QLabel *topicsLabel = new QLabel(tr("What topics do you need help with?"));
	
	instructorNameLineEdit = new QLineEdit;
	topicsTextEdit = new QTextEdit;

	instructorNameLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-z A-Z]{25}"), instructorNameLineEdit));

	QPushButton *cancelButton = new QPushButton(tr("Cancel"));
	QPushButton *okButton = new QPushButton(tr("OK"));

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(true);
	buttonLayout->addWidget(cancelButton);
	buttonLayout->addWidget(okButton);

	QGridLayout *courseInfoLayout = new QGridLayout;
	courseInfoLayout->addWidget(messageLabel, 0, 0, 1, -1);
	courseInfoLayout->addWidget(instructorNameLabel, 1, 0, Qt::AlignRight);
	courseInfoLayout->addWidget(instructorNameLineEdit, 1, 1);
	courseInfoLayout->addWidget(topicsLabel, 2, 0, Qt::AlignTop);
	courseInfoLayout->addWidget(topicsTextEdit, 2, 1);

	QGroupBox *groupBox = new QGroupBox(tr("Additional Course Information"));
	groupBox->setLayout(courseInfoLayout);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(groupBox);
	layout->addLayout(buttonLayout);

	setLayout(layout);
	setWindowTitle(tr("Edit Additional Course Information"));
}

QString EditAdditionalCourseInfoDialog::sanitize(QString &dirty)
{
	QString clean;

	for (int i = 0; i < dirty.length(); i++)
		if (dirty.at(i).isLetterOrNumber() || dirty.at(i).isSpace() || dirty.at(i) == ',')
			clean += dirty.at(i);
	return clean;
}

void EditAdditionalCourseInfoDialog::initializeDialog(QTableWidgetItem *item)
{
	courseNumber = item->tableWidget()->item(item->row(), 0)->text();
	courseName = item->tableWidget()->item(item->row(), 1)->text();
	QString message = "You told us that you need help with " + courseName
		+ ".\nHere you can edit the information you provided about this course.\n"
		+ "If you don't know the instructor's name, you may leave it blank.\n";
	messageLabel->setText(message);
	instructorNameLineEdit->setText(item->tableWidget()->item(item->row(), 2)->text());
	topicsTextEdit->setText(item->tableWidget()->item(item->row(), 3)->text());
}

void EditAdditionalCourseInfoDialog::okButtonClicked()
{
	QStringList courseInfo;
	courseInfo << courseNumber << courseName;
	if (instructorNameLineEdit->text().length() == 0)
		courseInfo << "Unknown";
	else
		courseInfo << instructorNameLineEdit->text();
	
	courseInfo << sanitize(topicsTextEdit->toPlainText());
	emit(courseInfoEdited(courseInfo));
	QMessageBox::information(this, tr("Edit Additional Course Information"),
		tr("The changes you made were saved successfully."));
	accept();
}