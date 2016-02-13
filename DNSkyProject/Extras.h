bool FileExists(std::string path);
std::string capital(std::string name);
std::string stolower(std::string name);
std::string getFileName(std::string);
int getClientVer();
void print(char* mesaj);
const char *GetFileNameFromHandle(HANDLE hFile);
bool has_suffix(const std::string& s, const  std::string& suffix);

struct SPackingFileHeader
{
	char szHeaderString[256];
	int nVersion;
	int nFileCount;
	DWORD dwFileInfoOffset;
	bool bRequireHeaderWrite;
	char cPadding[3];
};

struct SPackingFileInfo
{
	char szFileName[_MAX_FNAME];
	DWORD dwCompressSize;
	DWORD dwOriginalSize;
	DWORD dwAllocSize;
	DWORD dwOffset;
	DWORD dwSeedValue;
	DWORD dwChecksum;
	char cReserved[36];
};


enum eCSMainCmd
{
	CS_LOGIN = 1,			// 로긴서버쪽
	CS_SYSTEM,				// 아이피 포트 및 서버간 접속 관련 등등...
	CS_CHAR,				// Enter, Leave 등등 모든것
	CS_ACTOR,				// 액터관련
	CS_PROP,				// 프랍관련
	CS_PARTY,				// 파티관련
	CS_ITEM,				// 아이템관련
	CS_ITEMGOODS,			// 아이템에서 상품(?)관련된건 모두 이쪽으로 옮긴다
	CS_NPC,					// 엔피씨관련
	CS_QUEST,				// 퀘스트관련
	CS_CHAT,				// 채팅
	CS_ROOM,				// 룸과 관련된 모든것
	CS_SKILL,				// 스킬관련
	CS_TRADE,				// 상점, 우편, 무인상점 등등
	CS_FRIEND,				// 친구관련이에효
	CS_TRIGGER,				// Trigger
	CS_ACTORBUNDLE,			// Actor 
	CS_GUILD,				// guild
	CS_PVP,					// PvP
	CS_ISOLATE,				// 차단!
	CS_MISSION,				// 미션
	CS_APPELLATION,			// 호칭
	CS_GAMEOPTION,			// 게임옵션
	CS_RADIO,				// 라디오메세지
	CS_GESTURE,				// 제스처
	CS_VOICECHAT,			// 보이스채팅
	CS_CASHSHOP,
	CS_CHATROOM,			// 채팅방
#if defined( PRE_ADD_NPC_REPUTATION_SYSTEM )
	CS_REPUTATION,
#endif // #if defined( PRE_ADD_NPC_REPUTATION_SYSTEM )
#if defined( PRE_ADD_MASTERSYSTEM )
	CS_MASTERSYSTEM,
#endif // #if defined( PRE_ADD_MASTERSYSTEM )
	CS_VEHICLE,				// 탈것

	//#####################################################
	// 여기 위에다 추가해주세용~!
	//#####################################################

#if defined( PRE_ADD_FARMSYSTEM )
	CS_FARM,
#endif // #if defined( PRE_ADD_FARMSYSTEM )
};
