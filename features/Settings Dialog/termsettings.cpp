#include "termsettings.h"

#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidgetItem>

TermSettings::TermSettings(QWidget *parent)
	:QWidget(parent)
{
	termLabel = new QLabel(tr("<b>Current term:</b>"));
	currentTermLabel = new QLabel(tr("not selected"));
	termList = new QListWidget;
	termList->setMaximumSize(QSize(120, 120));
	addButton = new QPushButton(tr("Add"));
	editButton = new QPushButton(tr("Edit"));
	removeButton = new QPushButton(tr("Remove"));

	connect(termList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(termSelected(QListWidgetItem *)));

	QHBoxLayout *termLayout = new QHBoxLayout;
	termLayout->addWidget(termLabel);
	termLayout->addWidget(currentTermLabel);
	termLayout->addStretch(true);

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(editButton);
	buttonLayout->addWidget(removeButton);
	buttonLayout->addStretch(true);

	QHBoxLayout *listLayout = new QHBoxLayout;
	listLayout->addWidget(termList);
	listLayout->addLayout(buttonLayout);
	listLayout->addStretch(true);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(listLayout);
	mainLayout->addLayout(termLayout);
	mainLayout->addStretch(true);

	setLayout(mainLayout);
}

void TermSettings::termSelected(QListWidgetItem *term)
{
	currentTermLabel->setText(term->text());
}