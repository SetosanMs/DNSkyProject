enum eChatType
{
	CHATTYPE_NORMAL = 1,	// 일반적인 대화
	CHATTYPE_PARTY,			// 파티 대화
	CHATTYPE_PRIVATE,		// 귓속말
	CHATTYPE_GUILD,			// 길드 대화 - 길드패킷은 구조가 달라서 SCChat구조체 대신 S
	CHATTYPE_CHANNEL,		// 채널 대화(거래채널등은 추가가 될까요?)
	CHATTYPE_SYSTEM,		// 시스템
	CHATTYPE_WORLD,			// 월드 대화
	CHATTYPE_NOTICE,		// 공지
	CHATTYPE_TEAMCHAT,		// 팀채팅
	//CHATTYPE_NOTICEZONE,	// 맵공지
	CHATTYPE_GM,			// 운영자 대화
	CHATTYPE_PARTY_GM,		// 운영자 파티 대화
	CHATTYPE_PRIVATE_GM,	// 운영자 귓말
	CHATTYPE_DICE,
	CHATTYPE_CHATROOM,		// 채팅방 메시지

	WORLDCHATTYPE_ENCHANTITEM = 1,	// 강화아이템
	WORLDCHATTYPE_MISSION,			// 미션
};


struct SendChatStruct
{
	eChatType type;
	WCHAR CharName[20];
	WORD Size;
	WCHAR Text[50];
};

VOID SendChatNormal(WCHAR *From, WCHAR *Text);
VOID SendChatField(WCHAR *From, WCHAR *Text);
VOID SendChatGM(WCHAR *From, WCHAR *Text);
VOID SendChatNotice(WCHAR *From, WCHAR *Text);