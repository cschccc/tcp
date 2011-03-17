#include "reportdialog.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QTableView>
#include <QHBoxLayout>
#include <QCommandLinkButton>
#include <QPushButton>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>

ReportDialog::ReportDialog(QWidget *parent)
	: QDialog(parent)
{
	chartTitle = "Students who:<br>";
	checkedCriteria = 0;

	studentCheckBox = new QCheckBox(tr("Students"));
	tutorCheckBox = new QCheckBox(tr("Tutors"));
	tutorCheckBox->setEnabled(false);
	maleCheckBox = new QCheckBox(tr("are male"));
	femaleCheckBox = new QCheckBox(tr("are female"));
	marriedCheckBox = new QCheckBox(tr("are married"));
	childrenCheckBox = new QCheckBox(tr("have children"));
	showGraphicalReportButton = new QCommandLinkButton(tr("View Graphical Report  "));
	doneButton = new QPushButton(tr("Done"));
	toggleSearchCriteria(false); // disables all of the above...

	model = new QStandardItemModel;

	// I added tr() in case we ever need to translate our program to another language.
	model->setHorizontalHeaderLabels(QStringList() << tr("Student ID")
												   << tr("First Name")
												   << tr("Last Name")
												   << tr("Phone Number")
												   << tr("Date of Birth")
												   << tr("Address")
												   << tr("City")
												   << tr("State")
												   << tr("Zip Code")
												   << tr("Email")
												   << tr("Marital Status")
												   << tr("Children")
												   << tr("Gender")
												   << tr("Ethnicity")
												   << tr("Major")
												   << tr("F.A. Program"));
	matchesTableView = new QTableView;
	matchesTableView->setModel(model);
	matchesTableView->horizontalHeader()->setStretchLastSection(true);
	matchesTableView->horizontalHeader()->setHighlightSections(false);
	matchesTableView->verticalHeader()->hide();
	matchesTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	matchesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

	// I changed the names from checkBox, checkBox_1, ... to more descriptive ones.
	connect(femaleCheckBox, SIGNAL(toggled(bool)), this, SLOT(femaleCheckBoxClicked(bool)));
	connect(doneButton,SIGNAL(clicked()),this,SLOT(doneButtonClicked()));
	connect(studentCheckBox,SIGNAL(toggled(bool)),this,SLOT(studentCheckBoxClicked(bool)));
	connect(marriedCheckBox,SIGNAL(toggled(bool)),this,SLOT(marriedCheckBoxClicked(bool)));
    connect(maleCheckBox,SIGNAL(toggled(bool)),this,SLOT(maleCheckBoxClicked(bool)));
	connect(childrenCheckBox,SIGNAL(toggled(bool)),this,SLOT(childrenCheckBoxClicked(bool)));
	connect(showGraphicalReportButton, SIGNAL(clicked(bool)), this, SLOT(showGraphicalReport(bool)));

	QVBoxLayout *categoryLayout = new QVBoxLayout;
	categoryLayout->addWidget(studentCheckBox);
	categoryLayout->addWidget(tutorCheckBox);

	QGridLayout *criteriaLayout = new QGridLayout;
	criteriaLayout->addWidget(maleCheckBox, 0, 0);
	criteriaLayout->addWidget(femaleCheckBox, 1, 0);
	criteriaLayout->addWidget(marriedCheckBox, 0, 1);
	criteriaLayout->addWidget(childrenCheckBox, 1, 1);

	QGroupBox *criteriaGroupBox = new QGroupBox(tr("Who..."));
	criteriaGroupBox->setLayout(criteriaLayout);

	QGroupBox *categoryGroupBox = new QGroupBox(tr("I want to see information about..."));
	categoryGroupBox->setLayout(categoryLayout);

	QHBoxLayout *matchesLayout = new QHBoxLayout;
	matchesLayout->addWidget(matchesTableView);
	QGroupBox *matchesGroupBox = new QGroupBox(tr("The following individuals match your search criteria"));
	matchesGroupBox->setLayout(matchesLayout);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(showGraphicalReportButton);
	buttonLayout->addStretch(true);
	buttonLayout->addWidget(doneButton);

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(categoryGroupBox, 0, 0);
	mainLayout->addWidget(criteriaGroupBox, 0, 1);
	mainLayout->addWidget(matchesGroupBox, 1, 0, 1, -1);
	mainLayout->addLayout(buttonLayout, 2, 0, 1, -1);

	setLayout(mainLayout);
	setWindowTitle(tr("Reports"));
	resize(700, 400);
}

void ReportDialog::doneButtonClicked()
{
	resetForm();
	accept();
}

void ReportDialog::reject()
{
	resetForm();
	QDialog::reject();
}

void ReportDialog::resetForm()
{
	studentCheckBox->setChecked(false);
	tutorCheckBox->setChecked(false);
	maleCheckBox->setChecked(false);
	femaleCheckBox->setChecked(false);
	marriedCheckBox->setChecked(false);
	childrenCheckBox->setChecked(false);
	toggleSearchCriteria(false);
}

void ReportDialog::showGraphicalReport(bool checked)
{
	generateGraphicalReport();
	QDesktopServices::openUrl(QUrl("report.html"));
}

void ReportDialog::generateGraphicalReport()
{
	QString html = QString("") + 
		"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n" +
		"<html>\n" +
		"    <head>\n" +
		"        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n" + 
		"        <title>Report</title>\n" + 
		"		 <script type=\"text/javascript\" src=\"jquery-1.5.min.js\"></script>\n" + 
		"		 <script type=\"text/javascript\" src=\"highcharts.js\"></script>\n\n" +
		"        <script type=\"text/javascript\">\n" + 
		"            Highcharts.setOptions({\n" +
		"    		     colors: ['#ED561B', '#50B432']});\n" +
		"		     var chart;\n" +
		"			 $(document).ready(function() {\n" +
		"			     chart = new Highcharts.Chart({\n" +
		"			         chart: {\n" + 
		"				         renderTo: 'container',\n" + 
		"				         plotBackgroundColor: null,\n" +
		"				         plotBorderWidth: null,\n" +
		"				         plotShadow: false\n" +
		"			         },\n" +
		"			         title: {\n" +
		"				         text: 'charttitle',\n" + 
		"					     margin: 50\n" +
		"			         },\n" +
		"       			 tooltip: {\n" +
		"					     formatter: function() {\n" +
		"						     return this.y +'%';\n" +
		"						 }\n" +
		"				     },\n" +
		"					 plotOptions: {\n" +
		"					     pie: {\n" + 
		"						     allowPointSelect: false,\n" +
		"							 cursor: 'pointer',\n" +
		"							 dataLabels: {\n" +
		"							     enabled: true\n" +
		"							 },\n" +
		"							 showInLegend: false\n" +
		"						 }\n" +
		"			         },\n" +
		"					 series: [{\n" +
		"					     type: 'pie',\n" +
		"						 name: 'Browser share',\n" +
		"						 data: [\n" +
		"						     ['data1',   quantity1],\n" +
		"							 ['data2',   quantity2]\n" +
		"                        ]\n" + 
		"					 }]\n" +
		"                });\n" +
		"            });\n" +
		"        </script>\n" +
		"    </head>\n" + 
		"    <body>\n" + 
		"        <div id=\"container\" style=\"width: 800px; height: 400px; margin: 0 auto\"></div>\n" +
		"    </body>\n" +
		"</html>\n";

	QString matchCriteria = QString().setNum(model->rowCount());
	double percentMatchCriteria;
	double percentAllOthers;

	percentMatchCriteria = matchCriteria.toDouble() / totalStudents.toDouble() * 100;
	percentAllOthers = 100 - percentMatchCriteria;

	html.replace("charttitle", chartTitle);
	html.replace("data1", "All other students");
	html.replace("quantity1", QString().setNum(percentAllOthers)); 
	html.replace("data2", "Students who meet your search criteria");
	html.replace("quantity2", QString().setNum(percentMatchCriteria));

	QFile file("report.html");
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&file);
		out << html;
		file.close();
	}
}


void ReportDialog::clearResultsTable()
{
	int rowCount = model->rowCount();
	model->removeRows(0, rowCount);
}

void ReportDialog::toggleSearchCriteria(bool trueOrFalse)
{
	childrenCheckBox->setEnabled(trueOrFalse);
	femaleCheckBox->setEnabled(trueOrFalse);
	maleCheckBox->setEnabled(trueOrFalse);
	marriedCheckBox->setEnabled(trueOrFalse);
	showGraphicalReportButton->setEnabled(trueOrFalse && checkedCriteria);
}

// This method updates the results table, so let's call it that.
// Also, we don't need to pass the query string that we build because
// it is a class variable.
// void reportwindow::selectAllFunction(QString parameter)
void ReportDialog::updateResultsTable()
{	
	// I deleted the code that was opening another connection to the database
	// because that was already done in the constructor.

	QSqlQuery query;
	QList<QStringList> rows;

	/*
	rows << (QStringList() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString()
		                                                    << query.value(3).toString() << query.value(4).toString()
															<< query.value(5).toString() << query.value(6).toString()
															<< query.value(7).toString() << query.value(8).toString()
															<< query.value(9).toString() << query.value(10).toString()
	                                                        << query.value(11).toString() << query.value(12).toString()
															<< query.value(13).toString() << query.value(14).toString()
	                                                        << query.value(15).toString());*/

	// Same as above, just less typing.
	query.exec(queryString);
	while (query.next()) {
		QStringList columns;
		for (int i = 0; i < 16; i++) // we have 16 columns...
			columns << query.value(i).toString();
		rows << columns;
	}

	foreach (QStringList row, rows) {
		QList<QStandardItem *> items;
		foreach (QString text, row)
			items.append(new QStandardItem(text));
         model->appendRow(items);
	}

	matchesTableView->setModel(model);
}
void ReportDialog::studentCheckBoxClicked(bool checked)
{
	toggleSearchCriteria(checked);
	if(checked){
		queryString="select * from student_info";
		// No need to write the name of the class. When you write the name of the method only,
		// the compiler knows what you are refering to.
		//reportwindow::selectAllFunction(Sqlquery);
		updateResultsTable();
		totalStudents = QString().setNum(model->rowCount());
			}
	else {
		clearResultsTable();
		queryString="";
	}
}

// There is a pattern here with all the methods that handle a
// click on one of the check boxes or a change in the combo boxes. Let's factor it out...
void ReportDialog::updateQueryStringAndExecuteIt(bool checked, QString s1, QString s2)
{
	if (checked) {
		if (queryString.contains("where", Qt::CaseInsensitive))
			queryString += s1;
		else
			queryString += s2;
	}
	else {
		queryString.replace(s2, QString(""));
		queryString.replace(s1, QString(""));
	}

	clearResultsTable();
	updateResultsTable();
}

void ReportDialog::childrenCheckBoxClicked(bool checked)
{
	if (checked) {
		chartTitle += "have children<br>";
		checkedCriteria++;
	}
	else {
		chartTitle.replace("have children<br>", "");
		checkedCriteria--;
	}
	toggleSearchCriteria(true);
	// Now we can do this...
	updateQueryStringAndExecuteIt(checked, " and children='Yes'", " where children='Yes'");
}
void ReportDialog::marriedCheckBoxClicked(bool checked)
{
	if (checked) {
		chartTitle += "are married<br>";
		checkedCriteria++;
	}
	else {
		chartTitle.replace("are married<br>", "");
		checkedCriteria--;
	}
	toggleSearchCriteria(true);
	updateQueryStringAndExecuteIt(checked, " and married='Yes'", " where married='Yes'");
}

void ReportDialog::maleCheckBoxClicked(bool checked)
{
	if (checked) {
		chartTitle += "are male<br>";
		checkedCriteria++;
	}
	else {
		chartTitle.replace("are male<br>", "");
		checkedCriteria--;
	}
	toggleSearchCriteria(true);
	updateQueryStringAndExecuteIt(checked, " and gender='Male'", " where gender='Male'");
}

void ReportDialog::femaleCheckBoxClicked(bool checked)
{
	if (checked) {
		chartTitle += "are female<br>";
		checkedCriteria++;
	}
	else {
		chartTitle.replace("are female<br>", "");
		checkedCriteria--;
	}
	toggleSearchCriteria(true);
	updateQueryStringAndExecuteIt(checked, " and gender='Female'", " where gender='Female'");
}