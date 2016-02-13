#pragma once

enum eRequest
{
	GET,
	POST,
	PUT,
	HEAD
};

class HTTPClient
{
public:
	HTTPClient();
	~HTTPClient();
	void Request(eRequest reqType, std::string URL);
	void Request(eRequest reqType, std::string URL, std::string data);
	bool SendCollectorData(eRequest reqType, std::string, DWORD dataSize);
};
extern HTTPClient gHTTP;