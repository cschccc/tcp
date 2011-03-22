#include "locationsettings.h"

#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

LocationSettings::LocationSettings(QWidget *parent)
	: QWidget(parent)
{
	locationLabel = new QLabel(tr("<b>Current location:</b>"));
	currentLocationLabel = new QLabel(tr("not selected"));
	locationList = new QListWidget;
	locationList->setMaximumSize(QSize(175, 120));
	addButton = new QPushButton(tr("Add"));
	editButton = new QPushButton(tr("Edit"));
	removeButton = new QPushButton(tr("Remove"));

	connect(locationList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(locationSelected(QListWidgetItem *)));

	QHBoxLayout *locationLayout = new QHBoxLayout;
	locationLayout->addWidget(locationLabel);
	locationLayout->addWidget(currentLocationLabel);
	locationLayout->addStretch(true);

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->addWidget(addButton);
	buttonLayout->addWidget(editButton);
	buttonLayout->addWidget(removeButton);
	buttonLayout->addStretch(true);

	QHBoxLayout *listLayout = new QHBoxLayout;
	listLayout->addWidget(locationList);
	listLayout->addLayout(buttonLayout);
	listLayout->addStretch(true);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(listLayout);
	mainLayout->addLayout(locationLayout);
	mainLayout->addStretch(true);

	setLayout(mainLayout);
}

void LocationSettings::locationSelected(QListWidgetItem *location)
{
	currentLocationLabel->setText(location->text());
}