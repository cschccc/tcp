#ifndef PROGRAMSETTINGSDIALOG_H
#define PROGRAMSETTINGSDIALOG_H

#include <QDialog>

class QComboBox;
class QPushButton;

class ProgramSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	ProgramSettingsDialog(QWidget *parent = 0);
	void setLocation(const QString &);
	void setTerm(const QString &);

public slots:
	void okButtonClicked();
	void cancelButtonClicked();

signals:
	void programSettingsChanged();

private:
	QComboBox *locationComboBox;
	QComboBox *termComboBox;
	QPushButton *okButton;
	QPushButton *cancelButton;
	//void resetForm();
};

#endif // PROGRAMSETTINGSDIALOG_H