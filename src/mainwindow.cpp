#include "mainwindow.h"
#include "requestfortutorialassistancedialog.h"
#include "programsettingsdialog.h"
#include "reportdialog.h"

#include <QPushButton>
#include <QIcon>
#include <QGridLayout>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow()
{
	openDatabase();

	newStudentButton = new QPushButton(QIcon("new_student.png"), tr("New Student"));
	reportButton = new QPushButton(QIcon("report.png"), tr("View Reports"));
	settingsButton = new QPushButton(QIcon("settings.png"), tr("Settings"));
	quitButton = new QPushButton(QIcon("close.png"), tr("Quit"));

	newStudentButton->setIconSize(QSize(48, 48));
	reportButton->setIconSize(QSize(48, 48));
	settingsButton->setIconSize(QSize(48, 48));
	quitButton->setIconSize(QSize(48, 48));

	QFont font;
	font.setPointSize(12);

	newStudentButton->setFont(font);
	reportButton->setFont(font);
	settingsButton->setFont(font);
	quitButton->setFont(font);

	newStudentButton->setMinimumSize(QSize(170, 70));
	reportButton->setMinimumSize(QSize(170, 70));
	settingsButton->setMinimumSize(QSize(170, 70));
	quitButton->setMinimumSize(QSize(170, 70));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(newStudentButton, 0, 0);
	layout->addWidget(settingsButton, 0, 1);
	layout->addWidget(reportButton, 1, 0);
	layout->addWidget(quitButton, 1, 1);
	QLabel *creditsLabel = new QLabel(tr("Presented by the HCCC Computer Science Club - 2011"));
	layout->addWidget(creditsLabel, 2, 0, 1, -1);

	requestForTutorialAssistanceDialog = new RequestForTutorialAssistanceDialog(this);
	reportDialog = new ReportDialog(this);
	programSettingsDialog = new ProgramSettingsDialog(this);

	connect(newStudentButton, SIGNAL(clicked()), this, SLOT(newStudentButtonClicked()));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(reportButton, SIGNAL(clicked()), this, SLOT(reportButtonClicked()));
	connect(settingsButton, SIGNAL(clicked()), this, SLOT(settingsButtonClicked()));
	connect(programSettingsDialog, SIGNAL(programSettingsChanged()), this, SLOT(programSettingsChanged()));

	setLayout(layout);
	setWindowTitle(tr("HCCC Tutoring Center"));
}

void MainWindow::programSettingsChanged()
{
	requestForTutorialAssistanceDialog->updateWindowTitle();
}

void MainWindow::newStudentButtonClicked()
{
	requestForTutorialAssistanceDialog->exec();
}

void MainWindow::reportButtonClicked()
{
	reportDialog->exec();
}

void MainWindow::settingsButtonClicked()
{
	QFile file("settings.dat");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&file);
		QString line;
		line = in.readLine();
		programSettingsDialog->setLocation(line);
		line = in.readLine();
		programSettingsDialog->setTerm(line);
		file.close();
	}
	programSettingsDialog->exec();
}

void MainWindow::openDatabase()
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("tutoring_center.db");
	db.open();
	if (db.isOpenError()) {
		QMessageBox::critical(this, tr("Database Error"), db.lastError().driverText());
		return;
	}

	QSqlQuery query;

	query.exec("select * from course_info");

	// If we run this particular query and it does not become active, that means
	// our course_info table does not exist, which in turn implies that this is the first
	// time the program is being used. In that case, we must create and populate our database
	if (!query.isActive()) {
		query.exec("create table course_info (cid text primary key, course_name text)");
		query.exec("insert into course_info values('CSC-115', 'Programming in C++ for Science and Engineering')");
		query.exec("insert into course_info values('CSC-100', 'Introduction to Computers and Computing')");
		query.exec("insert into course_info values('CSC-101', 'Scientific Programming')");
		query.exec("insert into course_info values('CSC-113', 'Computer Logic and Discrete Mathematics')");
		query.exec("insert into course_info values('CSC-117', 'Java Programming')");
		query.exec("insert into course_info values('CSC-212', 'Computer Organization and Design')");
		query.exec("insert into course_info values('CSC-214', 'Data Structures and Advanced Programming')");
		query.exec("insert into course_info values('CSC-226', 'Database Design and Concepts')");
		query.exec("insert into course_info values('CSC-227', 'Introduction to Operating Systems')");
		query.exec("insert into course_info values('CSC-230', 'Data Communication Concepts')");
		query.exec("insert into course_info values('CSC-231', 'Information Systems Analysis and Design')");
		query.exec("insert into course_info values('CSC-240', 'Introduction to Networks and Networking')");
		query.exec("insert into course_info values('PHY-111', 'Engineering Physics I')");
		query.exec("insert into course_info values('PHY-113', 'Physics I')");
		query.exec("insert into course_info values('PHY-211', 'Engineering Physics II')");
		query.exec("insert into course_info values('PHY-213', 'Physics II')");
		query.exec("insert into course_info values('MAT-070', 'Basic Algebra Workshop')");
		query.exec("insert into course_info values('MAT-071', 'Basic Mathematics')");
		query.exec("insert into course_info values('MAT-073', 'Basic Algebra')");
		query.exec("insert into course_info values('MAT-082', 'Basic Math Express')");
		query.exec("insert into course_info values('MAT-083', 'Basic Algebra Express')");
		query.exec("insert into course_info values('MAT-100', 'College Algebra')");
		query.exec("insert into course_info values('MAT-110', 'Precalculus')");
		query.exec("insert into course_info values('MAT-111', 'Calculus I')");
		query.exec("insert into course_info values('MAT-112', 'Calculus II')");
		query.exec("insert into course_info values('MAT-114', 'Introduction to Statistics and Probability')");
		query.exec("insert into course_info values('MAT-116', 'Precalculus for Business')");
		query.exec("insert into course_info values('MAT-211', 'Calculus III')");
		query.exec("insert into course_info values('MAT-212', 'Differential Equations')");
		query.exec("insert into course_info values('ACC-121', 'Principles of Accounting I')");
		query.exec("insert into course_info values('ACC-211', 'Computerized Accounting')");
		query.exec("insert into course_info values('ACC-221', 'Principles of Accounting II')");
		query.exec("insert into course_info values('ACC-224', 'Federal Taxation')");
		query.exec("insert into course_info values('ACC-240', 'Intermediate Accounting I')");
		query.exec("insert into course_info values('ACC-241', 'Intermediate Accounting II')");
		query.exec("insert into course_info values('AMS-135', 'Introduction to American Studies')");
		query.exec("insert into course_info values('ANT-101', 'Introduction to Cultural Anthropology')");
		query.exec("insert into course_info values('ARC-105', 'Introduction to Computer Arts')");
		query.exec("insert into course_info values('ARC-106', 'Digital Imaging')");
		query.exec("insert into course_info values('ARC-107', 'Print Design')");
		query.exec("insert into course_info values('ARC-109', 'Interative/Web Design')");
		query.exec("insert into course_info values('ARC-201', 'Digital Video')");
		query.exec("insert into course_info values('ARC-202', 'Digital Animation')");
		query.exec("insert into course_info values('ARC-280', 'Computer Arts Portfolio/Presentation')");
		query.exec("insert into course_info values('ART-101', 'Experiencing Art')");
		query.exec("insert into course_info values('ART-103', 'Two Dimensional Design')");
		query.exec("insert into course_info values('ART-106', 'Drawing I')");
		query.exec("insert into course_info values('ART-107', 'Painting I')");
		query.exec("insert into course_info values('ART-113', 'Three Dimensional Design')");
		query.exec("insert into course_info values('ART-114', 'Color Theory')");
		query.exec("insert into course_info values('ART-115', 'Art History I')");
		query.exec("insert into course_info values('ART-116', 'Drawing II')");
		query.exec("insert into course_info values('ART-125', 'Art History II')");
		query.exec("insert into course_info values('ART-126', 'Figure Drawing')");
		query.exec("insert into course_info values('ART-127', 'Water Color')");
		query.exec("insert into course_info values('ART-130', 'Portfolio and Presentation')");
		query.exec("insert into course_info values('ASL-101', 'American Sign Language I')");
		query.exec("insert into course_info values('ASL-102', 'American Sign Language II')");
		query.exec("insert into course_info values('BIO-101', 'General Biology')");
		query.exec("insert into course_info values('BIO-107', 'Human Biology')");
		query.exec("insert into course_info values('BIO-111', 'Anatomy and Physiology I')");
		query.exec("insert into course_info values('BIO-115', 'Principles of Biology I')");
		query.exec("insert into course_info values('BIO-116', 'Principles of Biology II')");
		query.exec("insert into course_info values('BIO-201', 'Practical Nutrition')");
		query.exec("insert into course_info values('BIO-208', 'Ecology')");
		query.exec("insert into course_info values('BIO-211', 'Anatomy and Physiology II')");
		query.exec("insert into course_info values('BIO-230', 'Histology')");
		query.exec("insert into course_info values('BIO-250', 'Microbiology')");
		query.exec("insert into course_info values('BUS-230', 'Business Law')");
		query.exec("insert into course_info values('CAI-113', 'Food Service Sanitation')");
		query.exec("insert into course_info values('CAI-114', 'Table Service I')");
		query.exec("insert into course_info values('CAI-117', 'Production Kitchen Skills I')");
		query.exec("insert into course_info values('CAI-118', 'Pantry and Breakfast Cookery')");
		query.exec("insert into course_info values('CAI-119', 'Bakeshop I')");
		query.exec("insert into course_info values('CAI-123', 'Store Room and Purch Operations')");
		query.exec("insert into course_info values('CAI-124', 'Table Service II')");
		query.exec("insert into course_info values('CAI-125', 'Externship I')");
		query.exec("insert into course_info values('CAI-127', 'Production Kitchen Skills II')");
		query.exec("insert into course_info values('CAI-128', 'Introduction to Garde Manger')");
		query.exec("insert into course_info values('CAI-129', 'Bakeshop II')");
		query.exec("insert into course_info values('CAI-213', 'Menu Planning and Facilities Design')");
		query.exec("insert into course_info values('CAI-214', 'Advanced Table Service III')");
		query.exec("insert into course_info values('CAI-215', 'Externship II')");
		query.exec("insert into course_info values('CAI-216', 'Restaurant Operations Management')");
		query.exec("insert into course_info values('CAI-217', 'Advanced Kitchen - International')");
		query.exec("insert into course_info values('CAI-218', 'Intermediate Garde Manger')");
		query.exec("insert into course_info values('CAI-219', 'Advanced Bakeshop III')");
		query.exec("insert into course_info values('CAI-223', 'Food, Beverage, and Labor Cost Control')");
		query.exec("insert into course_info values('CAI-224', 'Advanced Table Service IV')");
		query.exec("insert into course_info values('CAI-225', 'Externship III')");
		query.exec("insert into course_info values('CAI-227', 'Advanced Kitchen - Classical')");
		query.exec("insert into course_info values('CAI-228', 'Advanced Garde Manger')");
		query.exec("insert into course_info values('CAI-229', 'Advanced Bakeshop IV - Classical')");
		query.exec("insert into course_info values('CHP-100', 'Introduction to Chemistry')");
		query.exec("insert into course_info values('CHP-111', 'College Chemistry I')");
		query.exec("insert into course_info values('CHP-211', 'College Chemistry II')");
		query.exec("insert into course_info values('CRJ-111', 'Introduction to Criminal Justice')");
		query.exec("insert into course_info values('CRJ-120', 'Introduction to Criminal Law')");
		query.exec("insert into course_info values('CRJ-200', 'Constitutional Liberties and Rights')");
		query.exec("insert into course_info values('CRJ-214', 'Corrections')");
		query.exec("insert into course_info values('CRJ-215', 'Juvenile Justice System')");
		query.exec("insert into course_info values('CRJ-220', 'General Police Organization and Administration')");
		query.exec("insert into course_info values('CRJ-222', 'Criminal Investigation')");
		query.exec("insert into course_info values('CRJ-290', 'Criminal Justice Externship')");
		query.exec("insert into course_info values('CSC-109', 'Web Page Design')");
		query.exec("insert into course_info values('CSC-111', 'Computer Science I')");
		query.exec("insert into course_info values('CSS-100', 'College Survival Skills')");
		query.exec("insert into course_info values('CTC-212', 'Computer Organization and Design')");
		query.exec("insert into course_info values('ECE-110', 'Child Development Associate CDA Workshop')");
		query.exec("insert into course_info values('ECE-120', 'Externship for CDA')");
		query.exec("insert into course_info values('ECE-201', 'Introduction to Early Childhood Education')");
		query.exec("insert into course_info values('ECE-211', 'Early Childhood Curriculum')");
		query.exec("insert into course_info values('ECE-214', \"Guiding Young Child's Behaviour\")");
		query.exec("insert into course_info values('ECE-215', 'Emerging Literacy')");
		query.exec("insert into course_info values('ECE-231', 'Early Childhood Education Externship I')");
		query.exec("insert into course_info values('ECE-241', 'Early Childhood Education Externship II')");
		query.exec("insert into course_info values('ECO-201', 'Principles of Macroeconomics')");
		query.exec("insert into course_info values('ECO-202', 'Principles of Microeconomics')");
		query.exec("insert into course_info values('EDU-211', 'Foundations of American Education')");
		query.exec("insert into course_info values('EET-111', 'Electrical Circuits I')");
		query.exec("insert into course_info values('EET-211', 'Electrical Circuits II')");
		query.exec("insert into course_info values('EET-212', 'Active Electronic Devices')");
		query.exec("insert into course_info values('EET-222', 'Analog Integrated Circuits')");
		query.exec("insert into course_info values('EET-228', 'Electronics Project Lab')");
		query.exec("insert into course_info values('EET-229', 'Microprocessor/Microcomputer System Design')");
		query.exec("insert into course_info values('EGS-100', 'Fundamentals of Engineering Design')");
		query.exec("insert into course_info values('EGS-101', 'Engineering Graphics')");
		query.exec("insert into course_info values('EGS-230', 'Statics and Dynamics')");
		query.exec("insert into course_info values('ENG-070', 'Fundamentals of Basic Writing')");
		query.exec("insert into course_info values('ENG-071', 'Basic Writing I')");
		query.exec("insert into course_info values('ENG-072', 'Basic Writing II')");
		query.exec("insert into course_info values('ENG-073', 'Basic Writing III')");
		query.exec("insert into course_info values('ENG-091', 'English Foundations')");
		query.exec("insert into course_info values('ENG-096', 'College Composition Workshop')");
		query.exec("insert into course_info values('ENG-101', 'College Composition I')");
		query.exec("insert into course_info values('ENG-102', 'College Composition II')");
		query.exec("insert into course_info values('ENG-103', 'Technical Report Writing')");
		query.exec("insert into course_info values('ENG-112', 'Speech')");
		query.exec("insert into course_info values('ENG-113', 'Introduction to Journalism')");
		query.exec("insert into course_info values('ENG-210', 'Creative Writing')");
		query.exec("insert into course_info values('ESL-020', 'Introduction to ESL Writing')");
		query.exec("insert into course_info values('ESL-021', 'ESL Writing I')");
		query.exec("insert into course_info values('ESL-022', 'ESL Writing II')");
		query.exec("insert into course_info values('ESL-023', 'ESL Writing III')");
		query.exec("insert into course_info values('ESL-024', 'ESL Writing IV')");
		query.exec("insert into course_info values('ESL-025', 'ESL Writing V')");
		query.exec("insert into course_info values('ESL-030', 'Introduction to Grammar for ESL')");
		query.exec("insert into course_info values('ESL-031', 'Grammar for ESL Writing I')");
		query.exec("insert into course_info values('ESL-032', 'Grammar for ESL Writing II')");
		query.exec("insert into course_info values('ESL-033', 'Grammar for ESL Writing III')");
		query.exec("insert into course_info values('ESL-034', 'Grammar for ESL Writing IV')");
		query.exec("insert into course_info values('ESL-035', 'Editing for ESL Writing V')");
		query.exec("insert into course_info values('ESL-040', 'Introduction to ESL Reading')");
		query.exec("insert into course_info values('ESL-041', 'ESL Reading I')");
		query.exec("insert into course_info values('ESL-042', 'ESL Reading II')");
		query.exec("insert into course_info values('ESL-043', 'ESL Reading III')");
		query.exec("insert into course_info values('ESL-044', 'ESL Reading IV')");
		query.exec("insert into course_info values('ESL-045', 'ESL Reading V')");
		query.exec("insert into course_info values('ESL-060', 'Introduction to ESL Academic Discussion')");
		query.exec("insert into course_info values('ESL-061', 'ESL Academic Discussion I')");
		query.exec("insert into course_info values('ESL-062', 'ESL Academic Discussion II')");
		query.exec("insert into course_info values('ESL-063', 'ESL Academic Discussion III')");
		query.exec("insert into course_info values('ESL-064', 'ESL Academic Discussion IV')");
		query.exec("insert into course_info values('ESL-065', 'ESL Academic Discussion V')");
		query.exec("insert into course_info values('FLM-101', 'Introduction to Film')");
		query.exec("insert into course_info values('FLM-102', 'Latin American Literature in Film')");
		query.exec("insert into course_info values('HIS-105', 'U.S. History I')");
		query.exec("insert into course_info values('HIS-106', 'U.S. History II')");
		query.exec("insert into course_info values('HIS-130', 'African American History')");
		query.exec("insert into course_info values('HIS-210', 'History of Western Civilization I')");
		query.exec("insert into course_info values('HIS-211', 'History of Western Civilization II')");
		query.exec("insert into course_info values('HLT-115', 'Dynamics of Healthcare in Society')");
		query.exec("insert into course_info values('HLT-210', 'Medical Law and Ethics')");
		query.exec("insert into course_info values('HMT-110', 'Introduction to the Hospitality Industry')");
		query.exec("insert into course_info values('HMT-115', 'The Urban Entrepreneur')");
		query.exec("insert into course_info values('HMT-121', 'Hotel Practicum')");
		query.exec("insert into course_info values('HMT-122', 'Front Office Operations')");
		query.exec("insert into course_info values('HMT-202', 'Innovation, Creativity, and Marketing')");
		query.exec("insert into course_info values('HMT-204', 'Fundamentals of Wine and Food')");
		query.exec("insert into course_info values('HMT-206', 'Information Systems in Hospitality and Tourism')");
		query.exec("insert into course_info values('HMT-210', 'Hospitality and Travel Law')");
		query.exec("insert into course_info values('HMT-213', 'Principles of Hospitality Marketing')");
		query.exec("insert into course_info values('HMT-215', 'Housekeeping Management')");
		query.exec("insert into course_info values('HMT-216', 'Restaurant Operations Management II')");
		query.exec("insert into course_info values('HUM-101', 'Cultures and Values')");
		query.exec("insert into course_info values('HUM-120', \"Introduction to Women's Studies\")");
		query.exec("insert into course_info values('HUM-128', 'Food and Culture')");
		query.exec("insert into course_info values('HUS-200', 'Group Work in Human Services')");
		query.exec("insert into course_info values('HUS-221', 'Community Organization')");
		query.exec("insert into course_info values('HUS-230', 'Interviewing Techniques')");
		query.exec("insert into course_info values('HUS-231', 'Human Services Field Placement I')");
		query.exec("insert into course_info values('HUS-241', 'Human Services Field Placement II')");
		query.exec("insert into course_info values('INTD-200', 'Death and the Human Experience')");
		query.exec("insert into course_info values('INTD-235', 'Exploring Multi Studies')");
		query.exec("insert into course_info values('INTD-250', 'The Child, Family, and Community')");
		query.exec("insert into course_info values('LGA-112', 'Civil Litigation')");
		query.exec("insert into course_info values('LGA-113', 'Real Property')");
		query.exec("insert into course_info values('LGA-205', 'Trusts and States')");
		query.exec("insert into course_info values('LGA-211', 'Criminal Law and Procedures')");
		query.exec("insert into course_info values('LGA-231', 'Legal Assistant Externship I')");
		query.exec("insert into course_info values('LGA-241', 'Legal Assistant Externship II')");
		query.exec("insert into course_info values('LIT-201', 'Introduction to Literature')");
		query.exec("insert into course_info values('LIT-207', 'Introduction to Drama')");
		query.exec("insert into course_info values('LIT-209', \"Children's Literature\")");
		query.exec("insert into course_info values('LIT-212', 'Introduction to Latino Literature of the U.S.')");
		query.exec("insert into course_info values('LIT-213', \"Women's Voices: Autobiography\")");
		query.exec("insert into course_info values('LIT-215', 'World Literature I')");
		query.exec("insert into course_info values('MAN-121', 'Principles of Management')");
		query.exec("insert into course_info values('MAN-221', 'Marketing')");
		query.exec("insert into course_info values('MAN-231', 'Management of Small Enterprise')");
		query.exec("insert into course_info values('MAN-232', 'Human Resource Management')");
		query.exec("insert into course_info values('MAN-241', 'Corporate Finance')");
		query.exec("insert into course_info values('MAN-242', 'Labor Relations')");
		query.exec("insert into course_info values('MDA-106', 'Medical Terminology')");
		query.exec("insert into course_info values('MDA-211', 'Clinical Office Procedures')");
		query.exec("insert into course_info values('MDA-214', 'Medical Office Procedures II')");
		query.exec("insert into course_info values('MDA-223', 'Typing Medical Machine Transcripts')");
		query.exec("insert into course_info values('MDA-231', 'Medical Assisting Externship')");
		query.exec("insert into course_info values('MLF-101', 'Basic French I')");
		query.exec("insert into course_info values('MLF-102', 'Basic French II')");
		query.exec("insert into course_info values('MLS-101', 'Basic Spanish I')");
		query.exec("insert into course_info values('MLS-102', 'Basic Spanish II')");
		query.exec("insert into course_info values('MLS-111', 'Spanish for Native Speakers')");
		query.exec("insert into course_info values('MLS-201', 'Intermediate Spanish')");
		query.exec("insert into course_info values('MUS-101', 'Introduction to Music')");
		query.exec("insert into course_info values('MUS-102', 'Introduction to World Music')");
		query.exec("insert into course_info values('MUS-104', 'African American Music Heritage')");
		query.exec("insert into course_info values('NSG-120', 'Nursing II')");
		query.exec("insert into course_info values('NSG-220', 'Nursing IV')");
		query.exec("insert into course_info values('NSG-230', 'Nursing Seminar')");
		query.exec("insert into course_info values('PHL-101', 'Introduction to Philosophy')");
		query.exec("insert into course_info values('PHL-103', 'Religion of the West')");
		query.exec("insert into course_info values('PHL-218', 'Contemporary Moral Issues')");
		query.exec("insert into course_info values('PSC-101', 'Introduction to Political Science')");
		query.exec("insert into course_info values('PSC-102', 'American Government')");
		query.exec("insert into course_info values('PSY-101', 'Introduction to Psychology')");
		query.exec("insert into course_info values('PSY-211', 'Developmental Psychology I')");
		query.exec("insert into course_info values('PSY-215', 'Psychology of Women')");
		query.exec("insert into course_info values('PSY-260', 'Lifespan Development')");
		query.exec("insert into course_info values('PSY-270', 'Psychology Teaching and Learning')");
		query.exec("insert into course_info values('PSY-280', 'Abnormal Psychology I')");
		query.exec("insert into course_info values('RDG-070', 'Fundamentals of Basic Reading')");
		query.exec("insert into course_info values('RDG-071', 'Basic Reading I')");
		query.exec("insert into course_info values('RDG-072', 'Basic Reading II')");
		query.exec("insert into course_info values('RDG-073', 'Basic Reading III')");
		query.exec("insert into course_info values('RDG-074', 'Introduction to Analytical/Critical Reading')");
		query.exec("insert into course_info values('RDG-075', 'Developing Analytical Thought I')");
		query.exec("insert into course_info values('RDG-076', 'Developing Analytical Thought II')");
		query.exec("insert into course_info values('SCI-101', 'Introduction to Physical Science')");
		query.exec("insert into course_info values('SED-235', 'Young Children with Special Needs')");
		query.exec("insert into course_info values('SED-290', 'Introduction to Special Education')");
		query.exec("insert into course_info values('SOC-101', 'Principles of Sociology')");
		query.exec("insert into course_info values('SOC-201', 'Sociology of the Family')");
		query.exec("insert into course_info values('SOC-211', 'Social Problems')");
		query.exec("insert into course_info values('SOC-230', 'Religion and Society')");
		query.exec("insert into course_info values('SOC-260', 'Race and Ethnic')");
		query.exec("insert into course_info values('SOC-280', 'Social Research Methods')");
		query.exec("insert into course_info values('THA-101', 'Introduction to Acting')");
		query.exec("insert into course_info values('THA-102', 'Introduction to Theater')");

		query.exec("create table spoken_language (lid text, sid text)");
		query.exec("create table written_language (lid text, sid text)");
		query.exec("create table student_request (request_id integer primary key autoincrement, "
			"sid text, cid text, instructor_name text, topics text)");
		query.exec("create table language (lid integer primary key autoincrement, language_name text)");
		query.exec("create table student_info (sid text primary key, first_name text, last_name text, phone_number text, "
			"dob text, address text, city text, state text, zip_code text, email text, married text, children text, "
			"gender text, ethnicity text, major text, financial_aid_program text)");
	}
}