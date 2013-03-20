/**********************************************************************
	�ļ�:		Player.h
	���	
	����:		dujiang
	��������:	2012/12/10   13:05

	��Ȩ����(C) 2012

	����޸�����	�޸���		�޸�����
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/
#ifndef Player_h__
#define Player_h__

#pragma once
#include "Obj.h"
#include "../../Public/DataDefine.h"
#include "MailMgr.h"

class Player : public Obj
{
public:
	Player(void);
	virtual ~Player(void);
	virtual void CleanUp();

public:
	bool InitWithData(const PlayerInfo& playerinfo);
	void SendPlayerInfo(ClientConnection* pClientConnection, Guid_t nVisitorGuid);
	ClientConnection* GetSelfGameServerConnection() const { return m_pGameServerConnection; };
	void SetSelfGameServerConnection(ClientConnection* pConnection);
	void SendMsgToSelf(Packet* pPacket);

	bool		inClan() const;
	bool		isOnline() const { return m_bOnline; }

	GameState	gameState() const { return m_nGameState; }
	void		gameState(GameState val);

	virtual void HeartBeat(float nowTime, float dt);
	
	const PlayerInfo&	playerInfo() const { return m_playerInfo; }
	//PlayerInfo&	playerInfo() { return m_playerInfo; }   // �޸���ҵ�exp��money����Ϣ���Ƚ����У����һ�������㣨������ݾ���ֵ����ȼ�����ȣ����Բ����ṩ�������ṩ��ȥ��

	Guid_t		getGuid() const { return m_playerInfo.m_nPlayerGuid; }
	void		setGuid(Guid_t val) { m_playerInfo.m_nPlayerGuid = val; }

	const char*	name() const { return m_playerInfo.m_szName; }
	void 		name(const char* szPlayerName);

	void		clanGuid(Guid_t clanGuid) { m_playerInfo.m_nClanGuid = clanGuid; }
	Guid_t		clanGuid() const { return m_playerInfo.m_nClanGuid; }
	int	    clanSymbolID() const { return m_playerInfo.m_nClanSymbolID; }
	void	    clanSymbolID(int val) { m_playerInfo.m_nClanSymbolID = val; }
	const char* clanName() const { return m_playerInfo.m_szClanName; }
	void	    clanName(const char* val);

	const int getExp() const;
	void addExp(const int nExp);
	const int getNowLevel() const;
	const int getNextLevelNeedExp() const;

	const int getIconID() const;
	void setNowIconID(const int nIconID);

	const int getMoneyNum() const;
	void addMoneyNum(const int nNum);
	bool consumeMoneyNum(const int nNum); 

	void UseTitleOfAchievementId(const int nAchieveId); 

	AchievementData& getAchievementData(){ return m_playerInfo.m_achievements; }

	//char          m_szClanName[CLAN_NAME_WITH_CLOSING];  

	void		addMail(const MessageMail& mail);
	void		removeRequestMail(unsigned char byType, Guid_t nRequestPlayerGuid);
	void		removeAllRequestMail( unsigned char byType );

	void    loadAchievementsFromDB();

private:
	void setPlayerInfoChange(const PLAYER_INFO_CHANGE_INDEX dataindex);
	void Levelup();
	void validatePlayerInfoChange(float dt);

private:
	bool			m_bOnline;
	//Guid_t			m_nGuid;

	// ����Ϸ�е�ʲô״̬��
	GameState		m_nGameState;
	PlayerInfo		m_playerInfo;
	unsigned int    m_nPlayerInfoChangeFlag;

	ClientConnection* m_pGameServerConnection;

	// ����clan�еĹ���ȼ�
	//ClanManageLevel m_nClanManageLevel;
	MailMgr			m_mailMgr;	


};

#endif // Player_h__
