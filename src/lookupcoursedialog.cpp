#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>

#include "additionalcourseinfodialog.h"
#include "lookupcoursedialog.h"

LookupCourseDialog::LookupCourseDialog(QWidget *parent)
	: QDialog(parent)
{
	courseNameLabel = new QLabel(tr("Enter a course name or keyword:"));
	courseNameLineEdit = new QLineEdit;
	courseNameLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9a-zA-Z+ -]{30}"), courseNameLineEdit));
	instructionLabel = new QLabel;
	doneButton = new QPushButton(tr("Done Adding Courses"));

	courseTable = new QTableWidget(0, 2);
	courseTable->setHorizontalHeaderLabels(QStringList() << tr("Course Number")
														 << tr("Course Name"));
	courseTable->horizontalHeader()->resizeSection(1, 300);
	courseTable->horizontalHeader()->setStretchLastSection(true);
	courseTable->horizontalHeader()->setHighlightSections(false);
	courseTable->verticalHeader()->hide();
	courseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);

	QHBoxLayout *queryLayout = new QHBoxLayout;
	queryLayout->addWidget(courseNameLabel);
	queryLayout->addWidget(courseNameLineEdit);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(instructionLabel);
	buttonLayout->addStretch(true);
	buttonLayout->addWidget(doneButton);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(queryLayout);
	mainLayout->addWidget(courseTable);
	mainLayout->addLayout(buttonLayout);

	additionalCourseInfoDialog = new AdditionalCourseInfoDialog;

	connect(courseNameLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(populateCourseTable()));
	connect(courseTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),
		this, SLOT(courseTableDoubleClicked(QTableWidgetItem *)));
	connect(doneButton, SIGNAL(clicked()), this, SLOT(doneButtonClicked()));
	connect(additionalCourseInfoDialog, SIGNAL(doneAddingCourses()), this, SLOT(accept()));
	connect(additionalCourseInfoDialog, SIGNAL(additionalCourseInfoSubmitted(QStringList &)),
		this, SLOT(additionalCourseInfoSubmitted(QStringList &)));

	setLayout(mainLayout);
	setWindowTitle(tr("Lookup courses"));
	resize(600, 200);
}

void LookupCourseDialog::setDuplicateCourseFound()
{
	duplicatedCourseFound = true;
}

void LookupCourseDialog::additionalCourseInfoSubmitted(QStringList &additionalCourseInfo)
{
	courseInfo += additionalCourseInfo;
	emit addCourseInfoToTable(courseInfo);
}

void LookupCourseDialog::courseTableDoubleClicked(QTableWidgetItem *cell)
{
	duplicatedCourseFound = false;
	emit(courseTableDoubleClicked(courseTable->item(cell->row(), 1)->text()));
	if (duplicatedCourseFound)
		return;
	courseInfo.clear();
	courseInfo << courseTable->item(cell->row(), 0)->text() << courseTable->item(cell->row(), 1)->text();
	additionalCourseInfoDialog->setCourseName(courseInfo.at(1));
	courseNameLineEdit->clear();
	courseNameLineEdit->setFocus(Qt::OtherFocusReason);
	courseTable->clearContents();
	courseTable->setRowCount(0);
	additionalCourseInfoDialog->exec();
}

void LookupCourseDialog::resetForm()
{
	courseNameLineEdit->clear();
	courseNameLineEdit->setFocus(Qt::OtherFocusReason);
}

void LookupCourseDialog::doneButtonClicked()
{
	courseNameLineEdit->clear();
	courseNameLineEdit->setFocus(Qt::OtherFocusReason);
	accept();
}

void LookupCourseDialog::populateCourseTable()
{
	QSqlQuery query;
	QVector<QTableWidgetItem *> tableItems;

	courseTable->clearContents();
	courseTable->setRowCount(0);
	instructionLabel->hide();
	
	if (courseNameLineEdit->text().length() == 0)
		return;

	query.exec("select distinct cid, course_name from course_info where course_name like '%" 
		+ courseNameLineEdit->text() + "%' or cid like '%" 
		+ courseNameLineEdit->text() + "%' order by course_name");

	if (query.next()) {
		instructionLabel->setText("<font color='red'>" + tr("Double click on a course to add it to your course list.") + "</font>");
		instructionLabel->show();
		query.seek(-1);
	}
	else {
		instructionLabel->setText("<font color='red'>" + tr("No results. Please check your spelling.") + "</font>");
		instructionLabel->show();
		return;
	}

	int itemCount = 0;
	while (query.next()) {
		QTableWidgetItem *courseNumber = new QTableWidgetItem(query.value(0).toString());
		QTableWidgetItem *courseName = new QTableWidgetItem(query.value(1).toString());
		tableItems.push_back(courseNumber);
		tableItems.push_back(courseName);
		itemCount += 2;
	}
	
	const int NCOLUMNS = 2;
	int rowCount = itemCount / NCOLUMNS;
	
	courseTable->setRowCount(rowCount);

	int index = 0;
	for (int i = 0; i < rowCount; i++)
		for (int j = 0; j < NCOLUMNS; j++)
			courseTable->setItem(i, j, tableItems.at(index++));
}
