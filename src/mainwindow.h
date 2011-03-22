#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QFile>
#include <QSqlDatabase>

class RequestForTutorialAssistanceDialog;
class QPushButton;
class SettingsDialog;
class ReportDialog;

class MainWindow : public QDialog
{
	Q_OBJECT

public:
	MainWindow();

public slots:
	void newStudentButtonClicked();
	void reportButtonClicked();
	void settingsButtonClicked();
	void programSettingsChanged();

private:
	RequestForTutorialAssistanceDialog *requestForTutorialAssistanceDialog;
	SettingsDialog *settingsDialog;
	ReportDialog *reportDialog;
	QPushButton *newStudentButton;
	QPushButton *reportButton;
	QPushButton *settingsButton;
	QPushButton *quitButton;
	QSqlDatabase db;
	void openDatabase();
};

#endif // MAINWINDOW_H