

/**********************************************************************
	�ļ�:		ClientConnOnPlayerDataSrv.h
	���	
	����:		dujiang
	��������:	2012/12/10   15:17

	��Ȩ����(C) 2012

	����޸�����	�޸���		�޸�����
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/
#ifndef ClientConnOnPlayerDataSrv_h__
#define ClientConnOnPlayerDataSrv_h__

#pragma once

#include "ClientConnection.h"

class ClientConnOnPlayerDataSrv :
	public ClientConnection
{
public:
	ClientConnOnPlayerDataSrv(void);
	virtual ~ClientConnOnPlayerDataSrv(void);

	virtual void onClose();
};

#endif // ClientConnOnPlayerDataSrv_h__

