#ifndef EMAILSETTINGS_H
#define EMAILSETTINGS_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QTextEdit;

class EmailSettings : public QWidget
{
	Q_OBJECT

public:
	EmailSettings(QWidget *parent = 0);

private:
	QLabel *emailLabel;
	QLabel *passwordLabel;
	QLabel *smtpLabel;
	QLabel *portLabel;
	QLabel *subjectLabel;
	QLabel *messageLabel;

	QLineEdit *emailLineEdit;
	QLineEdit *passwordLineEdit;
	QLineEdit *smtpLineEdit;
	QLineEdit *portLineEdit;
	QLineEdit *subjectLineEdit;
	QTextEdit *messageTextEdit;
};

#endif // EMAILSETTINGS_H