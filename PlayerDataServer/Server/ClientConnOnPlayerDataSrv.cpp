#include "ClientConnOnPlayerDataSrv.h"
#include "PersonalBaseManager.h"
#include "PlayerManager.h"

ClientConnOnPlayerDataSrv::ClientConnOnPlayerDataSrv(void)
{
}

ClientConnOnPlayerDataSrv::~ClientConnOnPlayerDataSrv(void)
{
}

void ClientConnOnPlayerDataSrv::onClose()
{
	// !! TO DO [12/10/2012 dujiang] ����������������ע�����Һ�һ�����ӵ�ҲҪ����Ӵ���
	PersonalBaseManager::GetInstancePtr()->removeVisitorFromConneciton(this);
	PlayerManager::GetInstancePtr()->onGameServerClose(this);
}
