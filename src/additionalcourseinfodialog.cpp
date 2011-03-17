#include "additionalcourseinfodialog.h"

#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QRegExpValidator>

AdditionalCourseInfoDialog::AdditionalCourseInfoDialog(QWidget *parent)
	: QDialog(parent)
{	
	messageLabel = new QLabel;
	QLabel *instructorNameLabel = new QLabel(tr("Instructor's Name:"));
	QLabel *topicsLabel = new QLabel(tr("What topics do you need help with?"));
	
	instructorNameLineEdit = new QLineEdit;
	topicsTextEdit = new QTextEdit;

	instructorNameLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z ]{25}"), instructorNameLineEdit));

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

	setWindowTitle(tr("Additional Course Information"));
	setLayout(layout);
}

QString AdditionalCourseInfoDialog::sanitize(QString &dirty)
{
	QString clean;

	for (int i = 0; i < dirty.length(); i++)
		if (dirty.at(i).isLetterOrNumber() || dirty.at(i).isSpace() || dirty.at(i) == ',')
			clean += dirty.at(i);
	return clean;
}

void AdditionalCourseInfoDialog::okButtonClicked()
{
	QStringList additionalCourseInfo;

	checkForm();
	if (formErrorMessage == "") {
		if (instructorNameLineEdit->text().length() == 0)
			additionalCourseInfo << "Unknown";
		else
			additionalCourseInfo << instructorNameLineEdit->text();

		additionalCourseInfo << sanitize(topicsTextEdit->toPlainText());
		emit additionalCourseInfoSubmitted(additionalCourseInfo);
		accept();
	}
	else
		QMessageBox::critical(this, tr("Required information missing"), formErrorMessage);
}

void AdditionalCourseInfoDialog::setCourseName(const QString name)
{
	courseName = name;
	setMessageLabelText();
}

void AdditionalCourseInfoDialog::accept()
{
	instructorNameLineEdit->clear();
	instructorNameLineEdit->setFocus(Qt::OtherFocusReason);
	topicsTextEdit->clear();
	
	QMessageBox *messageBox = new QMessageBox(this);
	messageBox->setWindowTitle(tr("Course Added"));
	messageBox->setText(tr("%1 has been added to your course list.\nDo you need help with another course?").arg(courseName));
	messageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	
	if (messageBox->exec() == QMessageBox::No)
		emit(doneAddingCourses());
	QDialog::accept();
}

void AdditionalCourseInfoDialog::reject()
{
	instructorNameLineEdit->clear();
	instructorNameLineEdit->setFocus(Qt::OtherFocusReason);
	topicsTextEdit->clear();
	QDialog::reject();
}

void AdditionalCourseInfoDialog::setMessageLabelText()
{
	QString message = "You told us that you need help with " + courseName
		+ ".\nPlease give us more details about this course.\n"
		+ "If you don't know the instructor's name, you may leave it blank.\n";
	messageLabel->setText(message);
}

void AdditionalCourseInfoDialog::checkForm()
{
	formErrorMessage = "";

	if (topicsTextEdit->toPlainText().length() == 0)
		formErrorMessage += "Please enter the topic(s) that you need help with.\n";
}