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
	CS_LOGIN = 1,			// �α伭����
	CS_SYSTEM,				// ������ ��Ʈ �� ������ ���� ���� ���...
	CS_CHAR,				// Enter, Leave ��� ����
	CS_ACTOR,				// ���Ͱ���
	CS_PROP,				// ��������
	CS_PARTY,				// ��Ƽ����
	CS_ITEM,				// �����۰���
	CS_ITEMGOODS,			// �����ۿ��� ��ǰ(?)���õȰ� ��� �������� �ű��
	CS_NPC,					// ���Ǿ�����
	CS_QUEST,				// ����Ʈ����
	CS_CHAT,				// ä��
	CS_ROOM,				// ��� ���õ� ����
	CS_SKILL,				// ��ų����
	CS_TRADE,				// ����, ����, ���λ��� ���
	CS_FRIEND,				// ģ�������̿�ȿ
	CS_TRIGGER,				// Trigger
	CS_ACTORBUNDLE,			// Actor 
	CS_GUILD,				// guild
	CS_PVP,					// PvP
	CS_ISOLATE,				// ����!
	CS_MISSION,				// �̼�
	CS_APPELLATION,			// ȣĪ
	CS_GAMEOPTION,			// ���ӿɼ�
	CS_RADIO,				// �����޼���
	CS_GESTURE,				// ����ó
	CS_VOICECHAT,			// ���̽�ä��
	CS_CASHSHOP,
	CS_CHATROOM,			// ä�ù�
#if defined( PRE_ADD_NPC_REPUTATION_SYSTEM )
	CS_REPUTATION,
#endif // #if defined( PRE_ADD_NPC_REPUTATION_SYSTEM )
#if defined( PRE_ADD_MASTERSYSTEM )
	CS_MASTERSYSTEM,
#endif // #if defined( PRE_ADD_MASTERSYSTEM )
	CS_VEHICLE,				// Ż��

	//#####################################################
	// ���� ������ �߰����ּ���~!
	//#####################################################

#if defined( PRE_ADD_FARMSYSTEM )
	CS_FARM,
#endif // #if defined( PRE_ADD_FARMSYSTEM )
};
