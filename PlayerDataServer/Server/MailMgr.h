/**********************************************************************
	�ļ�:		MailMgr.h
	���	
	����:		dujiang
	��������:	2013/1/21   11:01

	��Ȩ����(C) 2012

	����޸�����	�޸���		�޸�����
	------------------------------------------------------------------

	------------------------------------------------------------------
**********************************************************************/

#ifndef MailMgr_h__
#define MailMgr_h__

#pragma once

#include "MailBox.h"
#include "DataDefine.h"

class Player;

class MailMgr
{
public:
	//
	MailMgr(Player* pOwner);
	~MailMgr(void);


	void	add(const MessageMail& mail);
	void	removeRequestMail(	unsigned char byType, Guid_t nRequestPlayerGuid);
	void	removeAllRequestMail( unsigned char byType );
protected:
	MailMgr(void);


private:
	Player*		m_pOwnerPlayer;

	// �������Ϣ
	MailBox<MessageMail, MAX_MESSAGE_MAIL_IN_MAILBOX_COUNT>	m_mailMessage;
	// �ȴ���Ҵ������Ϣ
	MailBox<MessageMail, MAX_MESSAGE_MAIL_IN_MAILBOX_COUNT> m_mailRequest;

};

#endif // MailMgr_h__

