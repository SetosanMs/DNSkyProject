enum eChatType
{
	CHATTYPE_NORMAL = 1,	// �Ϲ����� ��ȭ
	CHATTYPE_PARTY,			// ��Ƽ ��ȭ
	CHATTYPE_PRIVATE,		// �ӼӸ�
	CHATTYPE_GUILD,			// ��� ��ȭ - �����Ŷ�� ������ �޶� SCChat����ü ��� S
	CHATTYPE_CHANNEL,		// ä�� ��ȭ(�ŷ�ä�ε��� �߰��� �ɱ��?)
	CHATTYPE_SYSTEM,		// �ý���
	CHATTYPE_WORLD,			// ���� ��ȭ
	CHATTYPE_NOTICE,		// ����
	CHATTYPE_TEAMCHAT,		// ��ä��
	//CHATTYPE_NOTICEZONE,	// �ʰ���
	CHATTYPE_GM,			// ��� ��ȭ
	CHATTYPE_PARTY_GM,		// ��� ��Ƽ ��ȭ
	CHATTYPE_PRIVATE_GM,	// ��� �Ӹ�
	CHATTYPE_DICE,
	CHATTYPE_CHATROOM,		// ä�ù� �޽���

	WORLDCHATTYPE_ENCHANTITEM = 1,	// ��ȭ������
	WORLDCHATTYPE_MISSION,			// �̼�
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