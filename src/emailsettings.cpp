#include "emailsettings.h"

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGridLayout>

EmailSettings::EmailSettings(QWidget *parent)
	: QWidget(parent)
{
	emailLabel = new QLabel(tr("Email Address:"));
	passwordLabel = new QLabel(tr("Password:"));
	smtpLabel = new QLabel(tr("SMTP Server:"));
	portLabel = new QLabel(tr("Port:"));
	subjectLabel = new QLabel(tr("Subject:"));
	messageLabel = new QLabel(tr("Message:"));

	emailLineEdit = new QLineEdit;
	passwordLineEdit = new QLineEdit;
	smtpLineEdit = new QLineEdit;
	portLineEdit = new QLineEdit;
	subjectLineEdit = new QLineEdit;
	messageTextEdit = new QTextEdit;

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(emailLabel, 0, 0);
	layout->addWidget(passwordLabel, 1, 0);
	layout->addWidget(emailLineEdit, 0, 1);
	layout->addWidget(passwordLineEdit, 1, 1);
	layout->addWidget(smtpLabel, 0, 2);
	layout->addWidget(portLabel, 1, 2);
	layout->addWidget(smtpLineEdit, 0, 3);
	layout->addWidget(portLineEdit, 1, 3);
	layout->addWidget(subjectLabel, 2, 0);
	layout->addWidget(subjectLineEdit, 2, 1, 1, -1);
	layout->addWidget(messageLabel, 3, 0, Qt::AlignTop);
	layout->addWidget(messageTextEdit, 3, 1, 1, -1);

	setLayout(layout);
}