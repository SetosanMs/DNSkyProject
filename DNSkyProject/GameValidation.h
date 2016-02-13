#pragma once
#ifndef GameValid_H
#define GameValid_H
#include <vector>
class GameValidation
{
	bool isValid = FALSE;
public:
	GameValidation();
	~GameValidation();
};


extern std::vector<std::string> AllowList;

void LoadAllowList();

bool checkIP(std::string ip);
bool checkIP(std::wstring wip);


#endif