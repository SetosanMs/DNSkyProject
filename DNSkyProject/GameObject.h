void LoadGameObject();
DWORD getPointer(const DWORD Pointer, const DWORD Offset);
char *getIPPointer();

struct PlayerDataStruct{
	bool isConnected = 0;
	int Map;
};

extern PlayerDataStruct PlayerData;