#pragma once
#include <string>

// Klasa Property - dowolna posiad³oœæ (pole, trakt, miasto)
class Prop
{
public:
	virtual std::string prop_type() = 0;

};

class Field : public Prop
{
public:
	std::string prop_type()
	{
		return "Field";
	}
};

class Trail : public Prop
{
public:
	std::string prop_type()
	{
		return "Trail";
	}
};

class Town : public Prop
{
public:
	std::string prop_type()
	{
		return "Town";
	}
};