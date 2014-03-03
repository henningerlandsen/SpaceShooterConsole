#pragma once

class IUpgradable
{
public:
	IUpgradable();
	~IUpgradable();
	virtual void setLevel(int level);
};

IUpgradable::IUpgradable(){}
IUpgradable::~IUpgradable(){}
void IUpgradable::setLevel(int level){}