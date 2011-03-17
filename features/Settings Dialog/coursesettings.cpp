#include "coursesettings.h"

#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>

CourseSettings::CourseSettings(QWidget *parent)
	: QWidget(parent)
{
	courseTable = new QTableWidget(0, 2);
	courseTable->setHorizontalHeaderLabels(QStringList() << tr("Course Number") << tr("Course Name"));
	courseTable->setMinimumSize(QSize(350, 100));
	courseTable->horizontalHeader()->setStretchLastSection(true);

	addButton = new QPushButton(tr("Add"));
	editButton = new QPushButton(tr("Edit"));
	removeButton = new QPushButton(tr("Remove"));

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(editButton);
	buttonLayout->addWidget(removeButton);
	buttonLayout->addStretch(true);
	
	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(courseTable);
	mainLayout->addLayout(buttonLayout);

	setLayout(mainLayout);
}