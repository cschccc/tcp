#ifndef REQUESTFORTUTORIALASSISTANCEDIALOG_H
#define REQUESTFORTUTORIALASSISTANCEDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QRadioButton;
class QComboBox;
class QGroupBox;
class QButtonGroup;
class QPushButton;
class QTableWidget;
class LookupCourseDialog;
class QAbstractButton;
class QTableWidgetItem;
class QDateEdit;
class EditAdditionalCourseInfoDialog;
class QProgressDialog;

class RequestForTutorialAssistanceDialog : public QDialog
{
	Q_OBJECT

public:
	RequestForTutorialAssistanceDialog(QWidget *parent = 0);
	void updateWindowTitle();

public slots:
	void lookupCourseButtonClicked();
	void okButtonClicked();
	void cancelButtonClicked();
	void financialAidRadioButtonToggled(bool);
	void addCourseInfoToTable(QStringList &);
	void itemFromTutoringRequestTableSelected(QTableWidgetItem *);
	void itemSelectionChangedInTutoringRequestTable();
	void removeSelectedButtonClicked();
	void editSelectedButtonClicked();
	void checkForDuplicatedCourse(QString &);
	void courseInfoEdited(QStringList &);
	void studentIdLineEditChanged(const QString &);
	void phoneNumberLineEditChanged(const QString &);
	void firstNameLineEditChanged(const QString &);
	void lastNameLineEditChanged(const QString &);
	void dobLineEditChanged(const QString &);
	void addressLineEditChanged(const QString &);
	void cityLineEditChanged(const QString &);
	void zipCodeLineEditEditChanged(const QString &);
	void emailLineEditChanged(const QString &);
	void ethnicityComboBoxChanged(int);
	void majorComboBoxChanged(int);
	void yesMarriedRadioButtonChanged(bool);
	void noMarriedRadioButtonChanged(bool);
	void yesParentRadioButtonChanged(bool);
	void noParentRadioButtonChanged(bool);
	void maleRadioButtonChanged(bool);
	void femaleRadioButtonChanged(bool);
	void yesFinancialAidRadioButtonChanged(bool);
	void noFinancialAidRadioButtonChanged(bool);
	void eofRadioButtonChanged(bool);
	void pellGrantRadioButtonChanged(bool);
	void ssspRadioButtonChanged(bool);

signals:
	void editSelectedButtonClicked(QTableWidgetItem *);

private:
	// contents of the "Student Information" groupbox
	QLabel *studentIdLabel;
	QLineEdit *studentIdLineEdit;
	QLabel *phoneNumberLabel;
	QLineEdit *phoneNumberLineEdit;
	QLabel *firstNameLabel;
	QLineEdit *firstNameLineEdit;
	QLabel *lastNameLabel;
	QLineEdit *lastNameLineEdit;
	QLabel *dobLabel;
	QLineEdit *dobLineEdit;
	QLabel *disclaimerLabel;
	QLabel *addressLabel;
	QLineEdit *addressLineEdit;
	QLabel *cityLabel;
	QLineEdit *cityLineEdit;
	QLabel *stateLabel;
	QComboBox *stateComboBox;
	QLabel *zipCodeLabel;
	QLineEdit *zipCodeLineEdit;
	QLabel *emailLabel;
	QLineEdit *emailLineEdit;

	// contents of the "Additional Information" groupbox
	QLabel *maritalStatusLabel;
	QRadioButton *yesMarriedRadioButton;
	QRadioButton *noMarriedRadioButton;
	QLabel *parentLabel;
	QRadioButton *yesParentRadioButton;
	QRadioButton *noParentRadioButton;
	QLabel *genderLabel;
	QRadioButton *maleRadioButton;
	QRadioButton *femaleRadioButton;
	QLabel *ethnicityLabel;
	QComboBox *ethnicityComboBox;
	QLabel *majorLabel;
	QComboBox *majorComboBox;
	QButtonGroup *maritalStatusButtonGroup;
	QButtonGroup *parentButtonGroup;
	QButtonGroup *genderButtonGroup;

	// contents of the "Language Information" groupbox
	QLabel *spokenLanguagesLabel;
	QLineEdit *spokenLanguagesLineEdit;
	QLabel *writtenLanguagesLabel;
	QLineEdit *writtenLanguagesLineEdit;

	// contents of the "Financial Aid Information" groupbox
	QLabel *financialAidLabel;
	QRadioButton *yesFinancialAidRadioButton;
	QRadioButton *noFinancialAidRadioButton;
	QLabel *programLabel;
	QRadioButton *eofRadioButton;
	QRadioButton *pellGrantRadioButton;
	QRadioButton *ssspRadioButton;
	QButtonGroup *financialAidButtonGroup;
	QButtonGroup *financialAidProgramButtonGroup;
	
	// contents of the "What do you need help with?" groupbox
	QPushButton *lookupCourseButton;
	QPushButton *editSelectedButton;
	QPushButton *removeSelectedButton;
	QTableWidget *tutoringRequestTable;

	QGroupBox *createStudentInformationGroupBox();
	QGroupBox *createAdditionalStudentInformationGroupBox();
	QGroupBox *createLanguageInformationGroupBox();
	QGroupBox *createFinancialInformationGroupBox();
	QGroupBox *createCourseHelpGroupBox();

	LookupCourseDialog *lookupCourseDialog;
	EditAdditionalCourseInfoDialog *editAdditionalCourseInfoDialog;
	
	QPushButton *cancelButton;
	QPushButton *okButton;

	void openDatabase();
	void submitStudentAndFinancialAidInformation();
	void submitLanguageInformation();
	void submitTutoringRequestInformation();
	void addLanguageToLanguageTable(const QString &);
	const QString getLanguageId(const QString &);
	void resetForm();
	void resetStudentInformation();
	void resetAdditionalStudentInformation();
	void resetLanguageInformation();
	void resetFinancialAidInformation();
	void resetTutoringRequestInformation();
	void checkForm();
	int sendEmail();
	
	//######################################################
	// these look the same, factor common stuff out!
	void addLanguageToSpokenLanguageTable(const QString &);
	void addLanguageToWrittenLanguageTable(const QString &);
	void submitSpokenLanguages();
	void submitWrittenLanguages();
	//######################################################

	QString formErrorMessage;
	bool noErrors;

	bool isEmpty(QLineEdit *lineEdit);
	bool isEmpty(QComboBox *comboBox);
	bool isEmpty(QRadioButton *qRadioButton);
	void clearField(QLineEdit *lineEdit);
	void clearField(QComboBox *comboBox);
	void clearField(QRadioButton *radioButton);
	void setAttention(QLineEdit *lineEdit, bool attention);
	void setAttention(QComboBox *comboBox, bool attention);
	void setAttention(QRadioButton *qRadioButton, bool attention);
	bool inputValidation();
	void clearAll();
};

#endif // REQUESTFORTUTORIALASSISTANCEDIALOG_H
