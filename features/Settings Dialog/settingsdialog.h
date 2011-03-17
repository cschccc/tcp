#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QTabWidget;
class QPushButton;

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

public slots:
	void okButtonClicked();

private:
	QTabWidget *tabWidget;
	QPushButton *okButton;
};

#endif // SETTINGSDIALOG_H
