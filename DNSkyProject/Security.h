#pragma once
class security
{
public:
	security();
	~security();
	void crash();
	void hideModule();
	void IntegrityCheck();
};

extern security s;

