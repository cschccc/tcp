#ifndef ADDITIONALCOURSEINFODIALOG_H
#define ADDITIONALCOURSEINFODIALOG_H

#include <QDialog>

class QLineEdit;
class QTextEdit;
class QLabel;
class QGroupBox;

class AdditionalCourseInfoDialog : public QDialog
{
	Q_OBJECT

public:
	AdditionalCourseInfoDialog(QWidget *parent = 0);
	void setCourseName(const QString);

public slots:
	void okButtonClicked();
	void accept();
	void reject();

signals:
	void additionalCourseInfoSubmitted(QStringList &);
	void doneAddingCourses();

private:
	QString courseName;
	QLabel *messageLabel;
	QLineEdit *instructorNameLineEdit;
	QTextEdit *topicsTextEdit;
	void setMessageLabelText();
	QString formErrorMessage;
	void checkForm();
	QString sanitize(QString &);
};

#endif ADDITIONALCOURSEINFODIALOG_H // ADDITIONALCOURSEINFODIALOG_H