/**********************************************************************
	�ļ�:		WorldServerConnector.h
	���	
	����:		dujiang
	��������:	2012/11/20   15:09

	��Ȩ����(C) 2012

	����޸�����	�޸���		�޸�����
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/


#ifndef WorldServerConnector_h__
#define WorldServerConnector_h__

#pragma once

#include "ServerConnection.h"

class WorldServerConnector :
	public ServerConnection
{
public:
	WorldServerConnector(void);
	virtual ~WorldServerConnector(void);

protected:
	// ��ͨserver��Ĵ���
	virtual	void	onConnected();
};

#endif // WorldServerConnector_h__

