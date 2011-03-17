#ifndef LOOKUPCOURSEDIALOG_H
#define LOOKUPCOURSEDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;
class QTableWidgetItem;
class AdditionalCourseInfoDialog;

class LookupCourseDialog : public QDialog
{
	Q_OBJECT

public:
	LookupCourseDialog(QWidget *parent = 0);
	void setDuplicateCourseFound();
	void resetForm();

public slots:
	void doneButtonClicked();
	void populateCourseTable();
	void courseTableDoubleClicked(QTableWidgetItem *);
	void additionalCourseInfoSubmitted(QStringList &);

signals:
	void addCourseInfoToTable(QStringList &);
	void courseTableDoubleClicked(QString &);

private:
	QLabel *courseNameLabel;
	QLineEdit *courseNameLineEdit;
	QTableWidget *courseTable;
	QLabel *instructionLabel;
	QPushButton *doneButton;
	QStringList courseInfo;
	bool duplicatedCourseFound;

	AdditionalCourseInfoDialog *additionalCourseInfoDialog;
};

#endif // LOOKUPCOURSEDIALOG_H
