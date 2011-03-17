#ifndef EDITADDITIONALCOURSEINFODIALOG_H
#define EDITADDITIONALCOURSEINFODIALOG_H

#include <QDialog>

class QTableWidgetItem;
class QLabel;
class QLineEdit;
class QTextEdit;

class EditAdditionalCourseInfoDialog : public QDialog
{
	Q_OBJECT

public:
	EditAdditionalCourseInfoDialog(QWidget *parent = 0);

public slots:
	void initializeDialog(QTableWidgetItem *);
	void okButtonClicked();

signals:
	void courseInfoEdited(QStringList &);

private:
	QLabel *messageLabel;
	QLineEdit *instructorNameLineEdit;
	QTextEdit *topicsTextEdit;
	QString courseNumber;
	QString courseName;
	QString sanitize(QString &);
};

#endif // EDITADDITIONALCOURSEINFODIALOG_H