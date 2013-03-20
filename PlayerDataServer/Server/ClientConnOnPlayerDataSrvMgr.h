/**********************************************************************
	�ļ�:		ClientConnOnPlayerDataSrvMgr.h
	���	
	����:		dujiang
	��������:	2012/12/10   15:18

	��Ȩ����(C) 2012

	����޸�����	�޸���		�޸�����
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/

#ifndef ClientConnOnPlayerDataSrvMgr_h__
#define ClientConnOnPlayerDataSrvMgr_h__

#pragma once
#include "ClientConnectionManager.h"
#include "ClientConnOnPlayerDataSrv.h"

class ClientConnOnPlayerDataSrvMgr :
	public ClientConnectionManager
{
public:
	ClientConnOnPlayerDataSrvMgr(void);
	virtual ~ClientConnOnPlayerDataSrvMgr(void);

	virtual ClientConnection* createClientConnection(){
		return new ClientConnOnPlayerDataSrv();
	}

};


#endif // ClientConnOnPlayerDataSrvMgr_h__
