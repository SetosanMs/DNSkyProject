#pragma once
class Metrics
{
public:
	Metrics();
	~Metrics();
};

DWORD WINAPI MetricsCollectThread(LPVOID lpParam);

