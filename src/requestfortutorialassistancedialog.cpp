#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QDateEdit>
#include <QTimer>
#include <QProgressDialog>

#include "Poco/Net/MailMessage.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/SecureSMTPClientSession.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Exception.h"
#include "Poco/Net/SSLManager.h"

using Poco::Net::MailMessage;
using Poco::Net::MailRecipient;
using Poco::Net::SecureSMTPClientSession;
using Poco::Net::StringPartSource;
using Poco::Exception;
using Poco::Net::SSLManager;
using Poco::Net::Context;

#include "requestfortutorialassistancedialog.h"
#include "lookupcoursedialog.h"
#include "editadditionalcourseinfodialog.h"

RequestForTutorialAssistanceDialog::RequestForTutorialAssistanceDialog(QWidget *parent)
	: QDialog(parent)
{
	noErrors = true;

	cancelButton = new QPushButton(tr("Cancel"));
	okButton = new QPushButton(tr("OK"));
	okButton->setDefault(true);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addStretch(true);
	buttonLayout->addWidget(cancelButton);
	buttonLayout->addWidget(okButton);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(createStudentInformationGroupBox(), 0, 0);
	layout->addWidget(createAdditionalStudentInformationGroupBox(), 0, 1);
	layout->addWidget(createLanguageInformationGroupBox(), 1, 0);
	layout->addWidget(createFinancialInformationGroupBox(), 1, 1);
	layout->addWidget(createCourseHelpGroupBox(), 2, 0, 1, -1);
	layout->addLayout(buttonLayout, 3, 1);

	lookupCourseDialog = new LookupCourseDialog(this);
	editAdditionalCourseInfoDialog = new EditAdditionalCourseInfoDialog(this);

	connect(this, SIGNAL(editSelectedButtonClicked(QTableWidgetItem *)),
		editAdditionalCourseInfoDialog, SLOT(initializeDialog(QTableWidgetItem *)));
	connect(lookupCourseDialog, SIGNAL(courseTableDoubleClicked(QString &)), this,
		SLOT(checkForDuplicatedCourse(QString &)));
	connect(lookupCourseButton, SIGNAL(clicked()), this, SLOT(lookupCourseButtonClicked()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
	connect(yesFinancialAidRadioButton, SIGNAL(toggled(bool)), 
		this, SLOT(financialAidRadioButtonToggled(bool)));
	connect(lookupCourseDialog, SIGNAL(addCourseInfoToTable(QStringList &)), 
		this, SLOT(addCourseInfoToTable(QStringList &)));
	connect(tutoringRequestTable, SIGNAL(itemClicked(QTableWidgetItem *)),
		this, SLOT(itemFromTutoringRequestTableSelected(QTableWidgetItem *)));
	connect(tutoringRequestTable, SIGNAL(itemSelectionChanged()),
		this, SLOT(itemSelectionChangedInTutoringRequestTable()));
	connect(editSelectedButton, SIGNAL(clicked()), this, SLOT(editSelectedButtonClicked()));
	connect(removeSelectedButton, SIGNAL(clicked()), this, SLOT(removeSelectedButtonClicked()));
	connect(editAdditionalCourseInfoDialog, SIGNAL(courseInfoEdited(QStringList &)),
		this, SLOT(courseInfoEdited(QStringList &)));
	connect(studentIdLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(studentIdLineEditChanged(const QString &)));
	connect(phoneNumberLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(phoneNumberLineEditChanged(const QString &)));
	connect(firstNameLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(firstNameLineEditChanged(const QString &)));
	connect(lastNameLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(lastNameLineEditChanged(const QString &)));
	connect(dobLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(dobLineEditChanged(const QString &)));
	connect(addressLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(addressLineEditChanged(const QString &)));
	connect(cityLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(cityLineEditChanged(const QString &)));
	connect(zipCodeLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(zipCodeLineEditEditChanged(const QString &)));
	connect(emailLineEdit, SIGNAL(textChanged(const QString &)), 
		this, SLOT(emailLineEditChanged(const QString &)));
	connect(ethnicityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ethnicityComboBoxChanged(int)));
	connect(majorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(majorComboBoxChanged(int)));
	connect(yesMarriedRadioButton, SIGNAL(toggled(bool)), this, SLOT(yesMarriedRadioButtonChanged(bool)));
	connect(noMarriedRadioButton, SIGNAL(toggled(bool)), this, SLOT(noMarriedRadioButtonChanged(bool)));
	connect(yesParentRadioButton, SIGNAL(toggled(bool)), this, SLOT(yesParentRadioButtonChanged(bool)));
	connect(noParentRadioButton, SIGNAL(toggled(bool)), this, SLOT(noParentRadioButtonChanged(bool)));
	connect(maleRadioButton, SIGNAL(toggled(bool)), this, SLOT(maleRadioButtonChanged(bool)));
	connect(femaleRadioButton, SIGNAL(toggled(bool)), this, SLOT(femaleRadioButtonChanged(bool)));
	connect(yesFinancialAidRadioButton, SIGNAL(toggled(bool)), this, SLOT(yesFinancialAidRadioButtonChanged(bool)));
	connect(noFinancialAidRadioButton, SIGNAL(toggled(bool)), this, SLOT(noFinancialAidRadioButtonChanged(bool)));
	connect(eofRadioButton, SIGNAL(toggled(bool)), this, SLOT(eofRadioButtonChanged(bool)));
	connect(pellGrantRadioButton, SIGNAL(toggled(bool)), this, SLOT(pellGrantRadioButtonChanged(bool)));
	connect(ssspRadioButton, SIGNAL(toggled(bool)), this, SLOT(ssspRadioButtonChanged(bool)));

	updateWindowTitle();
	setLayout(layout);
}

void RequestForTutorialAssistanceDialog::courseInfoEdited(QStringList &courseInfo)
{
	QList<QTableWidgetItem *> matches = tutoringRequestTable->findItems(courseInfo.at(0), Qt::MatchFixedString);

	tutoringRequestTable->removeRow(matches.at(0)->row());
	addCourseInfoToTable(courseInfo);
}

void RequestForTutorialAssistanceDialog::editSelectedButtonClicked()
{
	emit(editSelectedButtonClicked(tutoringRequestTable->currentItem()));
	editAdditionalCourseInfoDialog->exec();
}

void RequestForTutorialAssistanceDialog::checkForDuplicatedCourse(QString &course)
{
	int rowCount = tutoringRequestTable->rowCount();
	for (int i = 0; i < rowCount; i++) {
		if (course == tutoringRequestTable->item(i, 1)->text()) {
			QMessageBox::critical(this, tr("Duplicated course"),
				course + " is already in your course list.");
			lookupCourseDialog->setDuplicateCourseFound();
			lookupCourseDialog->resetForm();
			return;
		}
	}
}

void RequestForTutorialAssistanceDialog::updateWindowTitle()
{
	QString windowTitle = "Request for Tutorial Assistance - ";
	QFile file("settings.dat");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&file);
		QString line;
		line = in.readLine();
		windowTitle += (line + ", ");
		line = in.readLine();
		windowTitle += (line + " ");
		file.close();
	}
	windowTitle += QString().setNum(QDate::currentDate().year());
	setWindowTitle(windowTitle);
}

void RequestForTutorialAssistanceDialog::removeSelectedButtonClicked()
{
	int row = tutoringRequestTable->selectedItems().at(0)->row();
	tutoringRequestTable->removeRow(row);
}

void RequestForTutorialAssistanceDialog::itemSelectionChangedInTutoringRequestTable()
{
	if (tutoringRequestTable->selectedItems().size() == 0) {
		editSelectedButton->setEnabled(false);
		removeSelectedButton->setEnabled(false);
	}
}

void RequestForTutorialAssistanceDialog::itemFromTutoringRequestTableSelected(QTableWidgetItem *item)
{
	editSelectedButton->setEnabled(true);
	removeSelectedButton->setEnabled(true);
}

void RequestForTutorialAssistanceDialog::addCourseInfoToTable(QStringList &courseInfo)
{
	int row = tutoringRequestTable->rowCount();
	tutoringRequestTable->setRowCount(row + 1);

	for (int i = 0; i < courseInfo.size(); i++) {
		QTableWidgetItem *cell = new QTableWidgetItem(courseInfo.at(i));
		tutoringRequestTable->setItem(row, i, cell);
	}
}

int RequestForTutorialAssistanceDialog::sendEmail()
{
	QProgressDialog *progressDialog = new QProgressDialog(tr("Please wait..."), tr("Cancel"), 0, 100, this);
	progressDialog->setWindowTitle(tr("Processing request"));

	QString content = "HCCC TUTORIAL SERVICES REMINDER EMAIL\n\n"
					  "Dear " + firstNameLineEdit->text() + ",\n\n"
					  "The Assistant Dean, Head Tutors, and Staff of Tutorial Services sincerely hope that you will find your association with us congenial, helpful, and successful.\n"
					  "What to expect:\n"
					  "-In most cases, you will begin with 1 hour/per subject/per week of tutoring. Your appointment is for the entire semester. If you wish to cancel or change your appointment, please inform one of the Head Tutors. Please see the Head Tutor to request additional hours.\n"
					  "-Tutors work WITH students not for students. The tutor will go over examples of similar problems in order to develop your understanding of concepts. You must be an active participant in your learning experience.\n"
					  "-Tutors will not correct your homework or paper. They will provide useful comments and suggestions and serve as guides not as instructors or editors. This method fosters independent learning.\n"
					  "-Tutors will not proofread your papers. They will help you develop proofreading and writing skills.\n"
					  "-Tutors will not predict what your grade will be on your assignments. Tutors cannot speak for your instructor. Please ask your instructor for clarification about your coursework.\n"
					  "-Identify your objectives and concerns: Be as specific as possible about what you want to achieve, how you hope to use the tutoring session, and what you want the session to focus on. Ask questions and listen to suggestions. Be sure to bring all necessary materials (class notes, assignment guidelines, textbooks, drafts, dictionary, thumb drive, etc.) to the session.\n"
					  "-Group Tutoring: At times it may be necessary to share your session with other students; tutors work hard to serve the entire college community. Please be considerate of each other's needs and work.\n"
					  "-Please be on time: If you are 15 minutes late, you will lose fifteen minutes of your session.\n"
					  "-Call in advance: Two (2) consecutive missed sessions will cost you your tutoring slot. If you must misss a tutoring session, call us. You may be able to reschedule a makeup time for that missed session by contacting the Head Tutor.\n"
					  "\n\nThe Writing Center\nJ204\n(201) 360-4370"
					  "\n\nThe Tutoring Center\nB312/B317\n(201) 360-4187"
					  "\n\nNorth Hudson Tutoring Center\nLower Level\n(201) 360-4623\n";

	QString sender = "your gmail email address here";
	QString recipient = emailLineEdit->text();
	QString mailhost = "smtp.gmail.com"; 
	const int port = 587;                    
	QString password = "your email password here";

	Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	SSLManager::instance().initializeClient(0, 0, ptrContext);

	try {
		progressDialog->setValue(10);
		MailMessage message;
		message.setSender(sender.toStdString());
		message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, recipient.toStdString()));
		progressDialog->setValue(30);
		message.setSubject("Your request for tutorial services at HCCC");
		message.addContent(new StringPartSource(content.toStdString()));
		progressDialog->setValue(50);
		SecureSMTPClientSession session(mailhost.toStdString(), port);
		session.login();
		session.startTLS();
		progressDialog->setValue(70);
		session.login(SecureSMTPClientSession::AUTH_LOGIN, sender.toStdString(), password.toStdString());
		session.sendMessage(message);
		progressDialog->setValue(90);
		session.close();
		progressDialog->setValue(100);
	} 
	catch (Exception& exc)
	{
		return 1;
	}

	return 0;
}

void RequestForTutorialAssistanceDialog::okButtonClicked()
{
	checkForm();
	inputValidation();
	if (formErrorMessage == "") {
		submitStudentAndFinancialAidInformation();
		submitLanguageInformation();
		submitTutoringRequestInformation();
		if (noErrors && sendEmail() == 0) {
			QMessageBox::information(this, tr("Request processed successfully"), 
				tr("Thank you. Your request for tutorial assistance was processed successfully.\nA confirmation has been sent to your email."));
			resetForm();
			clearAll();
			accept();
		}
	}
	else
		QMessageBox::critical(this, tr("Required information missing"), formErrorMessage);
}

void RequestForTutorialAssistanceDialog::submitTutoringRequestInformation()
{
	QSqlQuery query;
	QString courseNumber, courseName, instructorName, topics;

	int rowCount = tutoringRequestTable->rowCount();
	for (int i = 0; i < rowCount; i++) {
		courseNumber = tutoringRequestTable->item(i, 0)->text();
		courseName = tutoringRequestTable->item(i, 1)->text();
		instructorName = tutoringRequestTable->item(i, 2)->text();
		topics = tutoringRequestTable->item(i, 3)->text();

		query.prepare("insert into student_request (request_id, sid, cid, instructor_name, topics) "
			"values (:request_id, :sid, :cid, :instructor_name, :topics)");
		query.bindValue(":request_id", QVariant(QVariant::String));
		query.bindValue(":sid", studentIdLineEdit->text());
		query.bindValue(":cid", courseNumber);
		query.bindValue(":instructor_name", instructorName);
		query.bindValue(":topics", topics);
		query.exec();
	}
}

void RequestForTutorialAssistanceDialog::submitLanguageInformation()
{
	if (spokenLanguagesLineEdit->text().length() != 0)
		submitSpokenLanguages();
	if (writtenLanguagesLineEdit->text().length() != 0)
		submitWrittenLanguages();
}

const QString RequestForTutorialAssistanceDialog::getLanguageId(const QString &language)
{
	QSqlQuery query;

	query.exec("select lid from language where language_name = '" + language + "'");
	if (query.next())
		return query.value(0).toString();
	else
		return QString();
}

void RequestForTutorialAssistanceDialog::addLanguageToSpokenLanguageTable(const QString &languageId)
{
	QSqlQuery query;

	query.exec("insert into spoken_language (lid, sid) values(:lid, :sid)");
	query.bindValue(":lid", languageId);
	query.bindValue(":sid", studentIdLineEdit->text());
	query.exec();
}

void RequestForTutorialAssistanceDialog::addLanguageToWrittenLanguageTable(const QString &languageId)
{
	QSqlQuery query;

	query.exec("insert into written_language (lid, sid) values(:lid, :sid)");
	query.bindValue(":lid", languageId);
	query.bindValue(":sid", studentIdLineEdit->text());
	query.exec();
}

void RequestForTutorialAssistanceDialog::addLanguageToLanguageTable(const QString &language)
{
	QSqlQuery query;
	if (!query.exec("insert into language values(null, '" + language + "')")) {
		QMessageBox::critical(this, tr("Language Information"), 
			tr("Error while adding language to main language table.\n") + query.lastError().text());
		noErrors = false;
	}
}

void RequestForTutorialAssistanceDialog::submitSpokenLanguages()
{
	QStringList languageList = spokenLanguagesLineEdit->text().split(",", QString::SkipEmptyParts);
	QString languageId;

	for (int i = 0; i < languageList.size(); i++) {
		languageId = getLanguageId(languageList.at(i));
		
		if (languageId.length() == 0) { // we don't have this language in the database
			addLanguageToLanguageTable(languageList.at(i));
			languageId = getLanguageId(languageList.at(i));
		}
		
		addLanguageToSpokenLanguageTable(languageId);
	}
}

void RequestForTutorialAssistanceDialog::submitWrittenLanguages()
{
	QStringList languageList = writtenLanguagesLineEdit->text().split(",", QString::SkipEmptyParts);
	QString languageId;

	for (int i = 0; i < languageList.size(); i++) {
		languageId = getLanguageId(languageList.at(i));
		
		if (languageId.length() == 0) { // we don't have this language in the database
			addLanguageToLanguageTable(languageList.at(i));
			languageId = getLanguageId(languageList.at(i));
		}
		
		addLanguageToWrittenLanguageTable(languageId);
	}
}

void RequestForTutorialAssistanceDialog::submitStudentAndFinancialAidInformation()
{
	QSqlQuery query;

	query.prepare("insert into student_info (sid, first_name, last_name, phone_number, dob, address, city, state, zip_code, "
		"email, married, children, gender, ethnicity, major, financial_aid_program) values(:sid, :first_name, :last_name, "
		":phone_number, :dob, :address, :city, :state, :zip_code, :email, :married, :children, :gender, :ethnicity, "
		":major, :financial_aid_program)");
	query.bindValue(":sid", studentIdLineEdit->text());
	query.bindValue(":first_name", firstNameLineEdit->text());
	query.bindValue(":last_name", lastNameLineEdit->text());
	query.bindValue(":phone_number", phoneNumberLineEdit->text());
	query.bindValue(":dob", dobLineEdit->text());
	query.bindValue(":address", addressLineEdit->text());
	query.bindValue(":city", cityLineEdit->text());
	query.bindValue(":state", stateComboBox->itemText(stateComboBox->currentIndex()));
	query.bindValue(":zip_code", zipCodeLineEdit->text());
	query.bindValue(":email", emailLineEdit->text());
	query.bindValue(":married", maritalStatusButtonGroup->checkedButton()->text());
	query.bindValue(":children", parentButtonGroup->checkedButton()->text());
	query.bindValue(":gender", genderButtonGroup->checkedButton()->text());
	query.bindValue(":ethnicity", ethnicityComboBox->itemText(ethnicityComboBox->currentIndex()));
	query.bindValue(":major", majorComboBox->itemText(majorComboBox->currentIndex()));
	if (yesFinancialAidRadioButton->isChecked())
		query.bindValue(":financial_aid_program", financialAidProgramButtonGroup->checkedButton()->text());
	else
		query.bindValue(":financial_aid_program", "None");

	if (!query.exec()) {
		QMessageBox::critical(this, tr("Student and Financial Aid Information"),
			tr("A database error ocurred while submitting student and financial aid information.\n") + query.lastError().text());
		noErrors = false;
	}
}

void RequestForTutorialAssistanceDialog::cancelButtonClicked()
{
	resetForm();
	clearAll();
	reject();
}

void RequestForTutorialAssistanceDialog::lookupCourseButtonClicked()
{
	lookupCourseDialog->exec();
}

void RequestForTutorialAssistanceDialog::financialAidRadioButtonToggled(bool receivesFinancialAid)
{
	if (receivesFinancialAid) {
		programLabel->setEnabled(true);
		eofRadioButton->setEnabled(true);
		pellGrantRadioButton->setEnabled(true);
		ssspRadioButton->setEnabled(true);
	}
	else {
		// in order to clear the radio buttons, we must first turn off
		//the exclusive property of the button group
		financialAidProgramButtonGroup->setExclusive(false);
		eofRadioButton->setChecked(false);
		pellGrantRadioButton->setChecked(false);
		ssspRadioButton->setChecked(false);
		financialAidProgramButtonGroup->setExclusive(true);

		programLabel->setEnabled(false);
		eofRadioButton->setEnabled(false);
		pellGrantRadioButton->setEnabled(false);
		ssspRadioButton->setEnabled(false);
	}
}

void RequestForTutorialAssistanceDialog::resetStudentInformation()
{
	studentIdLineEdit->clear();
	phoneNumberLineEdit->clear();
	firstNameLineEdit->clear();
	lastNameLineEdit->clear();
	dobLineEdit->clear();
	addressLineEdit->clear();
	cityLineEdit->clear();
	stateComboBox->setCurrentIndex(stateComboBox->findText("NJ"));
	zipCodeLineEdit->clear();
	emailLineEdit->clear();
}

void RequestForTutorialAssistanceDialog::resetForm()
{
	resetStudentInformation();
	resetAdditionalStudentInformation();
	resetLanguageInformation();
	resetFinancialAidInformation();
	resetTutoringRequestInformation();
	studentIdLineEdit->setFocus(Qt::OtherFocusReason);
}

void RequestForTutorialAssistanceDialog::resetLanguageInformation()
{
	spokenLanguagesLineEdit->clear();
	writtenLanguagesLineEdit->clear();
}

void RequestForTutorialAssistanceDialog::resetFinancialAidInformation()
{
	// in order to clear the radio buttons, we must first turn off
	//the exclusive property of the button groups

	financialAidButtonGroup->setExclusive(false);
	yesFinancialAidRadioButton->setChecked(false);
	noFinancialAidRadioButton->setChecked(false);
	financialAidButtonGroup->setExclusive(true);
	
	financialAidProgramButtonGroup->setExclusive(false);
	eofRadioButton->setChecked(false);
	pellGrantRadioButton->setChecked(false);
	ssspRadioButton->setChecked(false);
	financialAidProgramButtonGroup->setExclusive(true);
}

void RequestForTutorialAssistanceDialog::resetAdditionalStudentInformation()
{
	// in order to clear the radio buttons, we must first turn off
	//the exclusive property of the button groups

	maritalStatusButtonGroup->setExclusive(false);
	yesMarriedRadioButton->setChecked(false);
	noMarriedRadioButton->setChecked(false);
	maritalStatusButtonGroup->setExclusive(true);
	
	parentButtonGroup->setExclusive(false);
	yesParentRadioButton->setChecked(false);
	noParentRadioButton->setChecked(false);
	parentButtonGroup->setExclusive(true);
	
	genderButtonGroup->setExclusive(false);
	maleRadioButton->setChecked(false);
	femaleRadioButton->setChecked(false);
	genderButtonGroup->setExclusive(true);
	
	ethnicityComboBox->setCurrentIndex(0);
	majorComboBox->setCurrentIndex(0);
}

void RequestForTutorialAssistanceDialog::resetTutoringRequestInformation()
{
	int rowCount = tutoringRequestTable->rowCount();

	// we have to go from the bottom up
	for (int i = rowCount; i >= 0; i--)
		tutoringRequestTable->removeRow(i);
}

QGroupBox *RequestForTutorialAssistanceDialog::createStudentInformationGroupBox()
{
	studentIdLabel = new QLabel(tr("Student ID:"));
	studentIdLineEdit = new QLineEdit;
	phoneNumberLabel = new QLabel(tr("Phone Number:"));
	phoneNumberLineEdit = new QLineEdit;
	phoneNumberLineEdit->setInputMask("(999) 999-9999");
	firstNameLabel = new QLabel(tr("First Name:"));
	firstNameLineEdit = new QLineEdit;
	lastNameLabel = new QLabel(tr("Last Name:"));
	lastNameLineEdit = new QLineEdit;
	dobLabel = new QLabel(tr("Date of Birth:"));
	dobLineEdit = new QLineEdit;
	dobLineEdit->setInputMask("99/99/D999");
	disclaimerLabel = new QLabel(tr("(required solely for identification purposes)"));
	addressLabel = new QLabel(tr("Address:"));
	addressLineEdit = new QLineEdit;
	cityLabel = new QLabel(tr("City:"));
	cityLineEdit = new QLineEdit;
	stateLabel = new QLabel(tr("State:"));
	stateComboBox = new QComboBox;
	zipCodeLabel = new QLabel(tr("Zip Code:"));
	zipCodeLineEdit = new QLineEdit;
	emailLabel = new QLabel(tr("Email:"));
	emailLineEdit = new QLineEdit;

	// input constraints (we need WAY more, these are just to see how they could work)
	studentIdLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{7,8}"), studentIdLineEdit));
	//phoneNumberLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{10}"), phoneNumberLineEdit));
	firstNameLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z ]{15}"), firstNameLineEdit));
	lastNameLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z ]{15}"), lastNameLineEdit));
	//dobLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{2}/[0-9]{2}/[0-9]{4}"), dobLineEdit));
	addressLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,5}[a-zA-Z0-9 ]{30}"), addressLineEdit));
	cityLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z ]{25}"), cityLineEdit));
	zipCodeLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{5,5}"), zipCodeLineEdit));
	emailLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9._@]{30}"), emailLineEdit));

	QStringList states = (QStringList() << "AK" << "AL" << "AZ" << "AR" << "CA" << "CO" << "CT" << "DE" << "FL" << "GA" 
										<< "HI" << "ID" << "IL" << "IN" << "IA" << "KS" << "KY" << "LA" << "ME" << "MD" 
										<< "MA" << "MI" << "MN" << "MS" << "MO" << "MT" << "NE" << "NV" << "NH" << "NJ" 
										<< "NM" << "NY" << "NC" << "ND" << "OH" << "OK" << "OR" << "PA" << "RI" << "SC" 
										<< "SD" << "TN" << "TX" << "UT" << "VT" << "VA" << "WA" << "WV" << "WI" << "WY");
	stateComboBox->addItems(states);
	stateComboBox->setCurrentIndex(stateComboBox->findText("NJ"));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(studentIdLabel, 0, 0);
	mainLayout->addWidget(studentIdLineEdit, 0, 1);
	mainLayout->addWidget(phoneNumberLabel, 0, 2);
	mainLayout->addWidget(phoneNumberLineEdit, 0, 3);
	mainLayout->addWidget(firstNameLabel, 1, 0);
	mainLayout->addWidget(firstNameLineEdit, 1, 1);
	mainLayout->addWidget(lastNameLabel, 1, 2);
	mainLayout->addWidget(lastNameLineEdit, 1, 3);
	mainLayout->addWidget(dobLabel, 2, 0);
	mainLayout->addWidget(dobLineEdit, 2, 1);
	mainLayout->addWidget(disclaimerLabel, 2, 2, 1, -1);
	mainLayout->addWidget(addressLabel, 3, 0);
	mainLayout->addWidget(addressLineEdit, 3, 1, 1, -1);
	QHBoxLayout *stateAndZipCodeLayout = new QHBoxLayout;
	stateAndZipCodeLayout->addWidget(stateLabel);
	stateAndZipCodeLayout->addWidget(stateComboBox);
	stateAndZipCodeLayout->addWidget(zipCodeLabel);
	stateAndZipCodeLayout->addWidget(zipCodeLineEdit);
	mainLayout->addWidget(cityLabel, 4, 0);
	mainLayout->addWidget(cityLineEdit, 4, 1);
	mainLayout->addLayout(stateAndZipCodeLayout, 4, 2, 1, -1);
	mainLayout->addWidget(emailLabel, 5, 0);
	mainLayout->addWidget(emailLineEdit, 5, 1, 1, -1);

	QGroupBox *groupBox = new QGroupBox(tr("Student Information"));
	groupBox->setLayout(mainLayout);
	return groupBox;
}

void RequestForTutorialAssistanceDialog::checkForm()
{
	formErrorMessage = "";

	if (studentIdLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your student ID.\n";
	if (phoneNumberLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your phone number.\n";
	if (firstNameLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your first name.\n";
	if (lastNameLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your last name.\n";
	if (dobLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your date of birth.\n";
	if (addressLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your address.\n";
	if (cityLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your city of residence.\n";
	if (zipCodeLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your zip code.\n";
	if (emailLineEdit->text().length() == 0)
		formErrorMessage += "Please enter your email address.\n";
	if (!yesMarriedRadioButton->isChecked() && !noMarriedRadioButton->isChecked())
		formErrorMessage += "Please specify your marital status.\n";
	if (!yesParentRadioButton->isChecked() && !noParentRadioButton->isChecked())
		formErrorMessage += "Please specify whether or not you are a parent.\n";
	if (!maleRadioButton->isChecked() && !femaleRadioButton->isChecked())
		formErrorMessage += "Please specify your gender.\n";
	if (ethnicityComboBox->currentText() == "Select one")
		formErrorMessage += "Please specify your ethnicity.\n";
	if (majorComboBox->currentText() == "Select one")
		formErrorMessage += "Please specify your major.\n";
	if (!yesFinancialAidRadioButton->isChecked() && !noFinancialAidRadioButton->isChecked())
		formErrorMessage += "Please specify wheter or not you receive financial aid.\n";
	if (yesFinancialAidRadioButton->isChecked() && !eofRadioButton->isChecked()
		&& !pellGrantRadioButton->isChecked() && !ssspRadioButton->isChecked())
		formErrorMessage += "Please identify your financial aid program.\n";
	if (tutoringRequestTable->rowCount() == 0)
		formErrorMessage += "Please specify the course(s) that you need help with.\n";
}

QGroupBox *RequestForTutorialAssistanceDialog::createAdditionalStudentInformationGroupBox()
{
	maritalStatusLabel = new QLabel(tr("Are you married?"));
	yesMarriedRadioButton = new QRadioButton(tr("Yes"));
	noMarriedRadioButton = new QRadioButton(tr("No"));
	parentLabel = new QLabel(tr("Are you a parent?"));
	yesParentRadioButton = new QRadioButton(tr("Yes"));
	noParentRadioButton = new QRadioButton(tr("No"));
	genderLabel = new QLabel(tr("Gender:"));
	maleRadioButton = new QRadioButton(tr("Male"));
	femaleRadioButton = new QRadioButton(tr("Female"));
	ethnicityLabel = new QLabel(tr("Ethnicity:"));
	ethnicityComboBox = new QComboBox;
	majorLabel = new QLabel(tr("What is your major?"));
	majorComboBox = new QComboBox;

	// no yellow people?
	QStringList ethnicityList = (QStringList() << "Select one" 
											   << "American Indian" 
											   << "Asian" 
											   << "Black"
											   << "Caucasian" 
											   << "Hispanic" 
											   << "Other");
	ethnicityComboBox->addItems(ethnicityList);

	// Instead of hard coding the major list here, we could read it from a file. That way, in case a major
	// is discontinued or a new one is introduced, our users could easily make changes.
	QStringList majorList = (QStringList() << "Select one" 
										   << "Accounting" 
										   << "American Studies" 
										   << "Baking" 
										   << "Biology" 
										   << "Business" 
										   << "Chemistry" 
										   << "Childcare" 
										   << "Cold Food Production" 
										   << "Computer Science" 
										   << "Computer Technology" 
										   << "Cooperative Nursing" 
										   << "Criminal Justice" 
										   << "Culinary Arts" 
										   << "Early Childhood Education" 
										   << "Electronic Engineering Technology" 
										   << "Electronics Technology" 
										   << "Elementary and Secondary Education" 
										   << "Engineering Science" 
										   << "Health Science" 
										   << "History" 
										   << "Hospitality Management" 
										   << "Hot Food Production" 
										   << "Human Services" 
										   << "Latino Studies" 
										   << "Legal Assisting" 
										   << "Liberal Arts" 
										   << "Management" 
										   << "Management Funeral Services" 
										   << "Mathematics" 
										   << "Medical Assisting" 
										   << "Medical Transcription" 
										   << "Paramedic Science" 
										   << "Physics" 
										   << "Practical Nursing"
										   << "Psychology" 
										   << "Respiratory Care" 
										   << "Science and Mathematics" 
										   << "Social Services" 
										   << "Sociology"
										   << "Special Education" 
										   << "Studio Arts" 
										   << "Technical Studies" 
										   << "Women's Studies");
	majorComboBox->addItems(majorList);

	// we must somehow connect the two buttons so that only one can be checked at a time
	maritalStatusButtonGroup = new QButtonGroup;
	maritalStatusButtonGroup->addButton(yesMarriedRadioButton);
	maritalStatusButtonGroup->addButton(noMarriedRadioButton);

	parentButtonGroup = new QButtonGroup;
	parentButtonGroup->addButton(yesParentRadioButton);
	parentButtonGroup->addButton(noParentRadioButton);

	genderButtonGroup = new QButtonGroup;
	genderButtonGroup->addButton(maleRadioButton);
	genderButtonGroup->addButton(femaleRadioButton);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(maritalStatusLabel, 0, 0);
	layout->addWidget(yesMarriedRadioButton, 0, 1);
	layout->addWidget(noMarriedRadioButton, 0, 2);
	layout->addWidget(parentLabel, 1, 0);
	layout->addWidget(yesParentRadioButton, 1, 1);
	layout->addWidget(noParentRadioButton, 1, 2);
	layout->addWidget(genderLabel, 2, 0);
	layout->addWidget(maleRadioButton, 2, 1);
	layout->addWidget(femaleRadioButton, 2, 2);
	layout->addWidget(ethnicityLabel, 3, 0);
	layout->addWidget(ethnicityComboBox, 3, 1, 1, -1);
	layout->addWidget(majorLabel, 4, 0);
	layout->addWidget(majorComboBox, 4, 1, 1, -1);

	QGroupBox *groupBox = new QGroupBox(tr("Additional Student Information"));
	groupBox->setLayout(layout);
	return groupBox;
}

QGroupBox *RequestForTutorialAssistanceDialog::createLanguageInformationGroupBox()
{
	spokenLanguagesLabel = new QLabel(tr("Which languages other than English do you speak?"));
	spokenLanguagesLineEdit = new QLineEdit;
	spokenLanguagesLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-z,A-Z]{20}"), spokenLanguagesLineEdit));
	writtenLanguagesLabel = new QLabel(tr("Which languages other than English do you read?"));
	writtenLanguagesLineEdit = new QLineEdit;
	writtenLanguagesLineEdit->setValidator(new QRegExpValidator(QRegExp("[a-z,A-Z]{20}"), writtenLanguagesLineEdit));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(spokenLanguagesLabel, 0, 0);
	layout->addWidget(spokenLanguagesLineEdit, 0, 1);
	layout->addWidget(writtenLanguagesLabel, 1, 0);
	layout->addWidget(writtenLanguagesLineEdit, 1, 1);

	QGroupBox *groupBox = new QGroupBox(tr("Language Information"));
	groupBox->setLayout(layout);
	return groupBox;
}

QGroupBox *RequestForTutorialAssistanceDialog::createFinancialInformationGroupBox()
{
	financialAidLabel = new QLabel(tr("Do you receive financial aid?"));
	yesFinancialAidRadioButton = new QRadioButton(tr("Yes"));
	noFinancialAidRadioButton = new QRadioButton(tr("No"));
	programLabel = new QLabel(tr("If yes, which program?"));
	eofRadioButton = new QRadioButton(tr("EOF Program"));
	pellGrantRadioButton = new QRadioButton(tr("Pell Grant"));
	ssspRadioButton = new QRadioButton(tr("SSSP Program"));

	programLabel->setEnabled(false);
	eofRadioButton->setEnabled(false);
	pellGrantRadioButton->setEnabled(false);
	ssspRadioButton->setEnabled(false);

	financialAidButtonGroup = new QButtonGroup;
	financialAidButtonGroup->addButton(yesFinancialAidRadioButton);
	financialAidButtonGroup->addButton(noFinancialAidRadioButton);

	financialAidProgramButtonGroup = new QButtonGroup;
	financialAidProgramButtonGroup->addButton(eofRadioButton);
	financialAidProgramButtonGroup->addButton(pellGrantRadioButton);
	financialAidProgramButtonGroup->addButton(ssspRadioButton);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(financialAidLabel, 0, 0);
	layout->addWidget(yesFinancialAidRadioButton, 0, 1);
	layout->addWidget(noFinancialAidRadioButton, 0, 2);
	layout->addWidget(programLabel, 1, 0);
	layout->addWidget(eofRadioButton, 1, 1);
	layout->addWidget(pellGrantRadioButton, 1, 2);
	layout->addWidget(ssspRadioButton, 1, 3);

	QGroupBox *groupBox = new QGroupBox(tr("Financial Aid Information"));
	groupBox->setLayout(layout);
	return groupBox;
}

QGroupBox *RequestForTutorialAssistanceDialog::createCourseHelpGroupBox()
{
	lookupCourseButton = new QPushButton(tr("Lookup Courses"));
	editSelectedButton = new QPushButton(tr("Edit Selected"));
	removeSelectedButton = new QPushButton(tr("Remove Selected"));

	tutoringRequestTable = new QTableWidget(0, 4);
	tutoringRequestTable->setHorizontalHeaderLabels(QStringList() << tr("Course Number")
														 << tr("Course Name")
														 << tr("Instructor")
														 << tr("Topics"));
	tutoringRequestTable->horizontalHeader()->resizeSection(1, 300);
	tutoringRequestTable->horizontalHeader()->setStretchLastSection(true);
	tutoringRequestTable->horizontalHeader()->setHighlightSections(false);
	tutoringRequestTable->verticalHeader()->hide();
	tutoringRequestTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tutoringRequestTable->setSelectionBehavior(QAbstractItemView::SelectRows);

	removeSelectedButton->setEnabled(false);
	editSelectedButton->setEnabled(false);

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->addWidget(lookupCourseButton);
	buttonLayout->addWidget(editSelectedButton);
	buttonLayout->addWidget(removeSelectedButton);
	buttonLayout->addStretch(true); // moves all the buttons to the top of the layout

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addLayout(buttonLayout);
	layout->addWidget(tutoringRequestTable);

	QGroupBox *groupBox = new QGroupBox(tr("What do you need help with?"));
	groupBox->setLayout(layout);
	return groupBox;
}

void RequestForTutorialAssistanceDialog::studentIdLineEditChanged(const QString &)
{
	setAttention(studentIdLineEdit, false);
}
void RequestForTutorialAssistanceDialog::phoneNumberLineEditChanged(const QString &)
{
	setAttention(phoneNumberLineEdit, false);
}
void RequestForTutorialAssistanceDialog::firstNameLineEditChanged(const QString &)
{
	setAttention(firstNameLineEdit, false);
}
void RequestForTutorialAssistanceDialog::lastNameLineEditChanged(const QString &)
{
	setAttention(lastNameLineEdit, false);
}
void RequestForTutorialAssistanceDialog::dobLineEditChanged(const QString &)
{
	setAttention(dobLineEdit, false);
}
void RequestForTutorialAssistanceDialog::addressLineEditChanged(const QString &)
{
	setAttention(addressLineEdit, false);
}
void RequestForTutorialAssistanceDialog::cityLineEditChanged(const QString &)
{
	setAttention(cityLineEdit, false);
}
void RequestForTutorialAssistanceDialog::zipCodeLineEditEditChanged(const QString &)
{
	setAttention(zipCodeLineEdit, false);
}
void RequestForTutorialAssistanceDialog::emailLineEditChanged(const QString &)
{
	setAttention(emailLineEdit, false);
}
void RequestForTutorialAssistanceDialog::ethnicityComboBoxChanged(int)
{
	setAttention(ethnicityComboBox, false);
}
void RequestForTutorialAssistanceDialog::majorComboBoxChanged(int)
{
	setAttention(majorComboBox, false);
}
void RequestForTutorialAssistanceDialog::yesMarriedRadioButtonChanged(bool)
{
	setAttention(yesMarriedRadioButton, false);
	setAttention(noMarriedRadioButton, false);
}
void RequestForTutorialAssistanceDialog::noMarriedRadioButtonChanged(bool)
{
	setAttention(yesMarriedRadioButton, false);
	setAttention(noMarriedRadioButton, false);
}
void RequestForTutorialAssistanceDialog::yesParentRadioButtonChanged(bool)
{
	setAttention(yesParentRadioButton, false);
	setAttention(noParentRadioButton, false);
}
void RequestForTutorialAssistanceDialog::noParentRadioButtonChanged(bool)
{
	setAttention(yesParentRadioButton, false);
	setAttention(noParentRadioButton, false);
}
void RequestForTutorialAssistanceDialog::maleRadioButtonChanged(bool)
{
	setAttention(maleRadioButton, false);
	setAttention(femaleRadioButton, false);
}
void RequestForTutorialAssistanceDialog::femaleRadioButtonChanged(bool)
{
	setAttention(maleRadioButton, false);
	setAttention(femaleRadioButton, false);
}
void RequestForTutorialAssistanceDialog::yesFinancialAidRadioButtonChanged(bool)
{
	setAttention(yesFinancialAidRadioButton, false);
	setAttention(noFinancialAidRadioButton, false);
}
void RequestForTutorialAssistanceDialog::noFinancialAidRadioButtonChanged(bool)
{
	setAttention(yesFinancialAidRadioButton, false);
	setAttention(noFinancialAidRadioButton, false);
}
void RequestForTutorialAssistanceDialog::eofRadioButtonChanged(bool)
{
	setAttention(eofRadioButton, false);
	setAttention(pellGrantRadioButton, false);
	setAttention(noFinancialAidRadioButton, false);
}
void RequestForTutorialAssistanceDialog::pellGrantRadioButtonChanged(bool)
{
	setAttention(eofRadioButton, false);
	setAttention(pellGrantRadioButton, false);
	setAttention(ssspRadioButton, false);
}
void RequestForTutorialAssistanceDialog::ssspRadioButtonChanged(bool)
{
	setAttention(eofRadioButton, false);
	setAttention(pellGrantRadioButton, false);
	setAttention(ssspRadioButton, false);
}

bool RequestForTutorialAssistanceDialog::inputValidation()
{
	bool check = false;
	bool checked = false;

	//lineEdits
	checked = isEmpty(studentIdLineEdit);
	setAttention(studentIdLineEdit, checked);
	check = (check || checked);
	checked = isEmpty(phoneNumberLineEdit);
	setAttention(phoneNumberLineEdit, checked);
	check = (check || checked);
	checked = isEmpty(firstNameLineEdit);
	setAttention(firstNameLineEdit, checked);
	check = (check || checked);
	checked = isEmpty(lastNameLineEdit);
	setAttention(lastNameLineEdit, checked);
	check = (check || checked);
	checked = isEmpty(dobLineEdit);
	setAttention(dobLineEdit, checked);
	check = (check || checked);
	checked = isEmpty(addressLineEdit);
	setAttention(addressLineEdit, checked);
	check = (check || checked);
	checked = isEmpty(cityLineEdit);
	setAttention(cityLineEdit, checked);
	check = (check || checked);
	checked = isEmpty(zipCodeLineEdit);
	setAttention(zipCodeLineEdit, checked);
	check = (check || checked);
	checked = isEmpty(emailLineEdit);
	setAttention(emailLineEdit, checked);
	check = (check || checked);

	//comboBoxes
	checked = isEmpty(ethnicityComboBox);
	setAttention(ethnicityComboBox, checked);
	check = (check || checked);
	checked = isEmpty(majorComboBox);
	setAttention(majorComboBox, checked);
	check = (check || checked);

	//radioButtons
	checked = (isEmpty(yesMarriedRadioButton) && isEmpty(noMarriedRadioButton));
	setAttention(yesMarriedRadioButton, checked);
	setAttention(noMarriedRadioButton, checked);
	check = (check || checked);
	checked = (isEmpty(yesParentRadioButton) && isEmpty(noParentRadioButton));
	setAttention(yesParentRadioButton, checked);
	setAttention(noParentRadioButton, checked);
	check = (check || checked);
	checked = (isEmpty(maleRadioButton) && isEmpty(femaleRadioButton));
	setAttention(maleRadioButton, checked);
	setAttention(femaleRadioButton, checked);
	check = (check || checked);
	checked = (isEmpty(yesFinancialAidRadioButton) && isEmpty(noFinancialAidRadioButton));
	setAttention(yesFinancialAidRadioButton, checked);
	setAttention(noFinancialAidRadioButton, checked);
	check = (check || checked);
	//special case, only check if the aboved answered as yes
	if (!isEmpty(yesFinancialAidRadioButton))
	{
		checked = (isEmpty(eofRadioButton) && isEmpty(pellGrantRadioButton) && isEmpty(ssspRadioButton));
		setAttention(eofRadioButton, checked);
		setAttention(pellGrantRadioButton, checked);
		setAttention(ssspRadioButton, checked);
		check = (check || checked);
	}

	return (!check);
}

bool RequestForTutorialAssistanceDialog::isEmpty(QLineEdit *lineEdit)
{
	QString lineEditText = lineEdit->text();
	return (lineEditText.isEmpty());
}
//overload for QComboBox
bool RequestForTutorialAssistanceDialog::isEmpty(QComboBox *comboBox)
{
	QString comboBoxText = comboBox->currentText();
	if (comboBoxText.isEmpty() || comboBoxText == "Select one")
	{
		return (true);
	}
	return (false);
}
//overload for QRadioButton
bool RequestForTutorialAssistanceDialog::isEmpty(QRadioButton *qRadioButton)
{
	if (qRadioButton->isChecked())
	{
		return (false);
	}
	return (true);
}
//defualt implementatino for QLineEdit
void RequestForTutorialAssistanceDialog::setAttention(QLineEdit *lineEdit, bool attention)
{
	QPalette palette = lineEdit->palette();
	if (attention)
	{
		palette.setColor(lineEdit->backgroundRole(), Qt::yellow);
		lineEdit->setPalette(palette);	
	}
	else
	{
		palette.setColor(lineEdit->backgroundRole(), Qt::white);
		lineEdit->setPalette(palette);	
	}
	
}
//overload for QComboBox
void RequestForTutorialAssistanceDialog::setAttention(QComboBox *comboBox, bool attention)
{
	QPalette palette = comboBox->palette();
	if (attention)
	{
		palette.setColor(comboBox->backgroundRole(), Qt::yellow);
		palette.setColor(QPalette::Base, palette.color(QPalette::Window));
		comboBox->setPalette(palette);	
	}
	else
	{
		palette.setColor(comboBox->backgroundRole(), Qt::white);
		palette.setColor(QPalette::Base, palette.color(QPalette::Window));
		comboBox->setPalette(palette);	
	}
	
}
//overload for QRadioButton
void RequestForTutorialAssistanceDialog::setAttention(QRadioButton *qRadioButton, bool attention)
{
	QPalette palette = qRadioButton->palette();
	if (attention)
	{
		
		//palette.setBrush(QPalette::Active,QPalette: :Text, QBrush(Qt::red));
		//qRadioButton->foregroundRole
		palette.setColor(palette.WindowText, Qt::red);
		qRadioButton->setPalette(palette);
	}
	else
	{
		palette.setColor(palette.WindowText, Qt::black);
		qRadioButton->setPalette(palette);		
	}
}

void RequestForTutorialAssistanceDialog::clearAll()
{
	//lineEdits
	clearField(studentIdLineEdit);
	clearField(phoneNumberLineEdit);
	clearField(firstNameLineEdit);
	clearField(lastNameLineEdit);
	clearField(dobLineEdit);
	clearField(addressLineEdit);
	clearField(cityLineEdit);
	clearField(zipCodeLineEdit);
	clearField(emailLineEdit);

	//comboBoxes
	clearField(ethnicityComboBox);
	clearField(majorComboBox);

	//radioButtons
	clearField(yesMarriedRadioButton);
	clearField(noMarriedRadioButton);
	clearField(yesParentRadioButton);
	clearField(noParentRadioButton);
	clearField(maleRadioButton);
	clearField(femaleRadioButton);
	clearField(yesFinancialAidRadioButton);
	clearField(noFinancialAidRadioButton);
	clearField(eofRadioButton);
	clearField(pellGrantRadioButton);
	clearField(ssspRadioButton);
}

//default for QLineEdit
void RequestForTutorialAssistanceDialog::clearField(QLineEdit *lineEdit)
{
	setAttention(lineEdit, false);
}
//Overload for QComboBox
void RequestForTutorialAssistanceDialog::clearField(QComboBox *comboBox)
{
	setAttention(comboBox, false);
}
void RequestForTutorialAssistanceDialog::clearField(QRadioButton *radioButton)
{
	setAttention(radioButton, false);
}