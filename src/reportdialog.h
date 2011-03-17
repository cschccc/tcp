#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include <QLabel>

class QCheckBox;
class QComboBox;
class QTableView;
class QCommandLinkButton;
class QStandardItemModel;
class QPushButton;

class ReportDialog : public QDialog
{
	Q_OBJECT

public:
	ReportDialog(QWidget *parent = 0);

public slots:
	void femaleCheckBoxClicked(bool);
	void studentCheckBoxClicked(bool);
	void marriedCheckBoxClicked(bool);
	void childrenCheckBoxClicked(bool);
	void maleCheckBoxClicked(bool);
	void showGraphicalReport(bool);
	void doneButtonClicked();
	void reject();

private:
	QCheckBox *studentCheckBox;
	QCheckBox *tutorCheckBox;
	QCheckBox *maleCheckBox;
	QCheckBox *femaleCheckBox;
	QCheckBox *marriedCheckBox;
	QCheckBox *childrenCheckBox;
	QTableView *matchesTableView;
	QCommandLinkButton *showGraphicalReportButton;
	QPushButton *doneButton;

	QStandardItemModel *model;
	QString queryString;
	void clearResultsTable();
	void updateQueryStringAndExecuteIt(bool, QString, QString);
	void updateResultsTable();
	void toggleSearchCriteria(bool);
	void generateGraphicalReport();
	QString chartTitle;
	int checkedCriteria;
	QString totalStudents;
	void resetForm();
};

#endif // REPORTDIALOG_H