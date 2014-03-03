#pragma once

class AbstractListener
{
public:
	virtual void trigger(float x, float y);
};

void AbstractListener::trigger(float x, float y){}