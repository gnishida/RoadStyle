#pragma once

#include <QVector3D>
#include <qhash.h>
#include <qstring.h>
#include <qvariant.h>

class Attributes {
protected:
	QHash<QString, QVariant> attributes;

public:
	Attributes();

	bool getBool(QString name);
	int getInt(QString name);
	float getFloat(QString name);
	QString getString(QString name);
	void set(QString name, QVariant value);
};

