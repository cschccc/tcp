#include "majorsettings.h"

#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

MajorSettings::MajorSettings(QWidget *parent)
	: QWidget(parent)
{
	majorList = new QListWidget;
	majorList->setMaximumSize(QSize(150, 150));
	addButton = new QPushButton(tr("Add"));
	editButton = new QPushButton(tr("Edit"));
	removeButton = new QPushButton(tr("Remove"));

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(editButton);
	buttonLayout->addWidget(removeButton);
	buttonLayout->addStretch(true);

	QVBoxLayout *majorLayout = new QVBoxLayout;
	majorLayout->addWidget(majorList);
	majorLayout->addStretch(true);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(majorLayout);
	mainLayout->addLayout(buttonLayout);
	mainLayout->addStretch(true);

	setLayout(mainLayout);
}