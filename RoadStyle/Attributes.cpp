#include "Attributes.h"
#include <assert.h>

Attributes::Attributes() {
}

bool Attributes::getBool(QString name) {
	assert(attributes.contains(name));
	return attributes[name].toBool();
}

int Attributes::getInt(QString name) {
	assert(attributes.contains(name));
	return attributes[name].toInt();
}

float Attributes::getFloat(QString name) {
	assert(attributes.contains(name));
	return attributes[name].toFloat();
}

QString Attributes::getString(QString name) {
	assert(attributes.contains(name));
	return attributes[name].toString();
}

void Attributes::set(QString name, QVariant value) {
	attributes[name] = value;
}