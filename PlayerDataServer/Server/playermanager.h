/**********************************************************************
	�ļ�:		PlayerMgr.h
	���	
	����:		dujiang
	��������:	2012/12/6   15:37

	��Ȩ����(C) 2012

	����޸�����	�޸���		�޸�����
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/

#ifndef PlayerMgr_h__
#define PlayerMgr_h__

#pragma once

#include "Singleton.h"
#include "Player.h"

//typedef ObjManager<Player, PLAYER_OBJ_ID_BASE> PlayerObjManager;
class PlayerManager : public  ObjManager<Player, PLAYER_OBJ_ID_BASE>, public Singleton<PlayerManager>
{
public:
	Player*		GetPlayerByGuid(Guid_t guid);
	Player*		GetFreePlayer(Guid_t guid);
	void		RecyclePlayer(Player* p);
	void      onGameServerClose(ClientConnection* pGameServerConnection);

private:
	std::map<Guid_t, Player*> m_mapPlayers;
};


#endif // PlayerMgr_h__

