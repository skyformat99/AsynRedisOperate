#include "PersonalBase.h"
#include "TimeSystem.h"
#include "AssertDefine.h"
#include "BuildingConfig.h"
#include "Player.h"
#include "ObjManager.h"
#include "../../../Public/AllPacketInclude.h"
#include "PlayerManager.h"
#include "DataTable.h"
#include "TableSystem.h"
#include "PublicTableDefine.h"
#include "AchievementsConfig.h"


PersonalBase::PersonalBase(void)
{
	__ENTER_FUNCTION__
		m_nId = -1;
	m_nOwnerGUID = -1;
	m_llLatestUpdateTime = 0;
	m_nMaxBuildingObjID = 0;
	m_PersonalBaseInfo.clear();

	__LEAVE_FUNCTION__
}

PersonalBase::~PersonalBase(void)
{
	__ENTER_FUNCTION__

		__LEAVE_FUNCTION__
}

void PersonalBase::HeartBeat( float nowTime, float dt )
{
	__ENTER_FUNCTION__
		LONGLONG llnow = CTimeSystem::Instance()->GetBigSecond();
	if (llnow - m_llLatestUpdateTime < 1 /*����ʱ���ݶ�1��*/)
	{
		return;
	}
	m_llLatestUpdateTime = llnow;

	unsigned int count = m_vecBuildings.size();
	unsigned int i = 0;
	for (i = 0; i < count; i++)
	{
		Building& b = *(m_vecBuildings[i]);
		// �����߼�
		logicBuild(b, llnow);
		// �����߼�
		logicUpgrade(b, llnow);
		// �����߼�
		logicProduce(b, llnow);
		// ��Դ����
		logicResourceCollect(b, llnow);
	}

	// �������Ա仯�����ͻ���
	bool bDirty = false;
	for (i = 0; i < m_DirtyFlagsForClient.BIT_SIZE; i++)
	{
		if (m_DirtyFlagsForClient.test(i))
		{
			bDirty = true;
			break;
		}
	}
	if (bDirty)
	{
		SCSendPersonalBaseInfo msg;
		msg.targetGuid(this->GetOwnerGUID());
		msg.SetGemCount(GetGemCount());
		msg.SetCoin(GetGoldCoin());
		msg.SetMaxCoin(GetMaxGoldCoin());
		msg.SetEnergy(GetEnergy());
		msg.SetMaxEnergy(GetMaxEnergy());
		msg.SetGemCount(GetGemCount());
		msg.SetBuilderCount(GetBuilderCount());
		msg.SetWorkingBuilderCount(GetWorkingBuilderCount());

		BroadcastMsg(&msg);

		m_DirtyFlagsForClient.reset();
	}
	__LEAVE_FUNCTION__
}

bool PersonalBase::load(const PersonalBaseInfo& personalbaseinfo)
{
	__ENTER_FUNCTION__
		// TODO:??? ����ʱ���ж��ϴθ�������ʲôʱ��ͳһ�������ǰ��״̬��Ϣ
	LONGLONG llnow = CTimeSystem::Instance()->GetBigSecond();
	m_llLatestUpdateTime = llnow;
	m_PersonalBaseInfo = personalbaseinfo;

	// TODO �������ݿ��е���Ϣ��������
	CreateInitBase();

	unsigned int count = m_vecBuildings.size();
	for (unsigned int i = 0; i < count; i++)
	{
		Building& b = *(m_vecBuildings[i]);
		OnBuildingEffect(b);
	}

// ��ʼ������
	AddGold(5000);
	AddEnergy(5000);

    AddTreasure(0);
    AddTreasure(1);
    AddTreasure(1);
    AddTreasure(2);

    //AddTreasure(0, 0);
    //AddTreasure(0, 1);

    return true;

	__LEAVE_FUNCTION__
}

void PersonalBase::BeginNewVistor(ClientConnection* pClientConnectoin, Guid_t nVisitorGuid)
{
	Assert(pClientConnectoin != NULL);
	Assert(nVisitorGuid > 0);

	this->AddVisitor(nVisitorGuid, pClientConnectoin);

	SCSendPersonalBaseInfo msg;
	msg.targetGuid(this->GetOwnerGUID());
	msg.visitorGuid(nVisitorGuid);
	msg.SetCoin(this->GetGoldCoin());
	msg.SetMaxCoin(this->GetMaxGoldCoin());
	msg.SetEnergy(this->GetEnergy());
	msg.SetMaxEnergy(this->GetMaxEnergy());
	msg.SetGemCount(this->GetGemCount());
	msg.SetBuilderCount(this->GetBuilderCount());
	msg.SetWorkingBuilderCount(this->GetWorkingBuilderCount());
	pClientConnectoin->SendMsg(&msg);


	SCSendAllBuildingData msgBuilding;
	msgBuilding.targetGuid(this->GetOwnerGUID());
	msgBuilding.visitorGuid(nVisitorGuid);
	int nCount = this->GetBuildingCount();
	for (int i = 0; i < nCount; i++)
	{
		msgBuilding.AddBuildingData(this->GetBuidingByIndex(i));
	}
	pClientConnectoin->SendMsg(&msgBuilding);

	// �·����б���
	SCSendAllTreasures msgSendAllTreasures;
	msgSendAllTreasures.visitorGuid(nVisitorGuid);
	msgSendAllTreasures.targetGuid(this->GetOwnerGUID());

    msgSendAllTreasures.setTreasuresLst(this->getAllTreasures());

	pClientConnectoin->SendMsg(&msgSendAllTreasures);
}

int PersonalBase::GetBuildingCount()
{
	__ENTER_FUNCTION__
		return m_vecBuildings.size();
	__LEAVE_FUNCTION__
}

Building* PersonalBase::GetBuidingByIndex( int idx )
{
	__ENTER_FUNCTION__
		Assert( idx >= 0 && idx < (int)m_vecBuildings.size() );
	return m_vecBuildings[idx];
	__LEAVE_FUNCTION__
}


Building* PersonalBase::CreateNewBuilding( int nTblId, int nTag, Guid_t nHelperGuid )
{ 
	__ENTER_FUNCTION__
		if (nTag > 0)
		{
			if (FindBuidingByTag(nTag) != NULL)
			{
				//TODO: ֪ͨ�ͻ��˴˴��Ѿ��н���
				return NULL;
			}               
		}
		_DBC_T_BASEBUILDING * pCfg = BuildingConfig::getSingleton()->getBaseCfg(nTblId);
		if (pCfg == NULL)
		{
			return NULL;
		}

		if (pCfg->nLogicType == BTYPE_MAINBUILDING)  // ���������ܽ��� 
		{
			return NULL;
		}

		// ���صȼ�Լ�� (??? �Ƿ�ŵ�������������)


		// TODO:??? ����Լ�� �ͻ����Ѿ�ʵ�֣�������Ҳ��Ҫ�ж�

		// TODO:??? ��ӶҪͬʱ�жϹ�Ӷ���ĺͽ�������
		if (pCfg->nCostGoldForBuild > GetGoldCoin() || pCfg->nCostEnergyForBuild > GetEnergy() || pCfg->nCostGemForBuild > GetGemCount())
		{
			//TODO: ??? ���ͻ�����ʾ
			return NULL;
		}

		// ����ǹ�Ӷ���ѣ���ֱ�ӽ����Ӷ��ص��ж�
		if (nHelperGuid != invalid_guid)
		{
			if ( !CanHireFriend(nHelperGuid) )
			{
				return NULL;
			}

			HireFriend(nHelperGuid);
		}        
		else 
		{
			if (GetWorkingBuilderCount() < GetBuilderCount())// ������������
			{
				SetWorkingBuilderCount(GetWorkingBuilderCount()+1);
			}
			else
			{
				//TODO: ��ʾ
				return NULL;
			}
		}

		CostGold(pCfg->nCostGoldForBuild);
		CostEnergy(pCfg->nCostEnergyForBuild);
		CostGem(pCfg->nCostGemForBuild);

		int nTotalBuildTime = pCfg->nTimeForBuild;
		Building *b = new Building();
		b->nObjID = m_nMaxBuildingObjID++;
		b->nLogicType = pCfg->nLogicType;
		b->nBeginTime = (int)CTimeSystem::Instance()->GetBigSecond();
		b->nState = Building::BS_Build;
		b->nLeftTime = nTotalBuildTime;
		b->nLevel = 1;
		b->nTag = nTag;
		b->nTableId = nTblId;
		b->nHelperGuid = nHelperGuid;

		FillBuildingTableCfg(b, nTblId, 1);

		m_vecBuildings.push_back(b); 

		OnBuildingEffect(*b);

		return b;
		__LEAVE_FUNCTION__
}

Building* PersonalBase::FindBuidingByTag( int nTag )
{
	__ENTER_FUNCTION__
		for (int i = 0; i < (int)m_vecBuildings.size(); i++)
		{
			if (m_vecBuildings[i]->nTag == nTag)
			{
				return m_vecBuildings[i];
			}
		}
		return NULL;
		__LEAVE_FUNCTION__
}

bool PersonalBase::DestroyBuildingByID( int nObjId )
{
	__ENTER_FUNCTION__
		std::vector<Building *>::iterator it = m_vecBuildings.begin();
	for (; it != m_vecBuildings.end(); it++)
	{
		if ((*it)->nObjID == nObjId)
		{
			m_vecBuildings.erase(it);
			return true;
		}
	}
	return false;
	__LEAVE_FUNCTION__
}

Building* PersonalBase::GetBuildingById( int nObjId )
{
	__ENTER_FUNCTION__
		std::vector<Building *>::iterator it = m_vecBuildings.begin();
	for (; it != m_vecBuildings.end(); it++)
	{
		if ((*it)->nObjID == nObjId)
		{
			return *it;
		}
	}
	return NULL;
	__LEAVE_FUNCTION__
}

void PersonalBase::logicBuild( Building &b, LONGLONG llnow )
{
	__ENTER_FUNCTION__
		if (b.nState != Building::BS_Build)
		{
			return;
		}
		int nTotalNeedTime = BuildingConfig::getSingleton()->getBaseCfg(b.nTableId)->nTimeForBuild; //??? ����ֱ��д�뽨��������,���ټ�����
		int nElapseTime = llnow - b.nBeginTime;
		b.nLeftTime = nTotalNeedTime - nElapseTime;
		if (b.nLeftTime <= 0)
		{
			b.nState = Building::BS_Idle;
			b.nLeftTime = 0;   

			OnBuildingBuildCompleted(b);

		}
		__LEAVE_FUNCTION__
}

void PersonalBase::logicUpgrade( Building &b, LONGLONG llnow )
{
	__ENTER_FUNCTION__
		if (b.nState == Building::BS_Upgrade)
		{
			int nTotalNeedTime = BuildingConfig::getSingleton()->getLevelCfg(b.nTableId, b.nLevel)->nUpgradeTime; //TODO:??? ����ֱ��д�뽨��������,���ټ����� ����������Ⱦ�����ʱ��
			int nElapseTime = llnow - b.nBeginTime;
			b.nLeftTime = nTotalNeedTime - nElapseTime;
			if (b.nLeftTime <= 0)
			{
				b.nState = Building::BS_Idle;
				b.nLevel++;
				b.nLeftTime = 0;

				OnBuildingUpgradeCompleted(b);
			}
		}
		__LEAVE_FUNCTION__
}

void PersonalBase::logicProduce( Building &b, LONGLONG llnow )
{
	__ENTER_FUNCTION__
		if (b.nLogicType == BTYPE_PUBLICHOUSE) //�ƹ�һֱˢ��, ʿ��ֻ�������������������
		{
			if (b.nState == Building::BS_Idle)
			{
				b.nState = Building::BS_Produce; 
				b.nBeginTime = llnow;
			}
		}
		if (b.nState != Building::BS_Produce)
		{
			return;
		}
		__LEAVE_FUNCTION__

}

void PersonalBase::logicResourceCollect( Building &b, LONGLONG llnow )
{
	if (b.nLogicType == BTYPE_ENERGY_PRODUCE_HOUSE || b.nLogicType == BTYPE_GOLDMINE ) //������Դ�ջ���Զ����뿪��
	{
		if (b.nState == Building::BS_Idle)
		{
			b.nState = Building::BS_Collect; 
			b.nBeginTime = llnow;
		}
	}
	if (b.nState != Building::BS_Collect)
	{
		return;
	}

	// �洢����Ҫ���
	int nTimeForResFullInSeconds = (float)b.CollectionExt.nCapacity / (float)b.CollectionExt.nProductionRate * 3600.f;
	if (llnow - b.nBeginTime < nTimeForResFullInSeconds)
	{
		b.CollectionExtDataRT.nResNumber = (float)b.CollectionExt.nProductionRate/3600.f * (float)(llnow - b.nBeginTime);
		b.nLeftTime = nTimeForResFullInSeconds - (llnow - b.nBeginTime);
	}
	else
	{
		b.nLeftTime = 0;
		b.nState = Building::BS_ResFull;
		//֪ͨ�ͻ���
		SCSendBuildInfo msg;
		msg.SetObjID(b.nObjID);
		msg.SetInfoType(SCSendBuildInfo::SBI_ResFull);
		BroadcastMsg(&msg);
	}
}

void PersonalBase::AddVisitor( Guid_t playerguid, ClientConnection* pConnection )
{
	__ENTER_FUNCTION__
		if (playerguid == invalid_guid || pConnection == NULL){
			CacheLog_DEBUG(LOG_DEBUG, "wrong parameter(guid:%d, pConnection:%d in PersonalBase::AddVisitor()",
				playerguid, pConnection);
			return;
		}
		VisitorOnClient theVisitor;
		theVisitor.nVisitorGuid = playerguid;
		theVisitor.pClientConn = pConnection;
		m_lstVisitors.push_back(theVisitor);
		__LEAVE_FUNCTION__
}

void PersonalBase::RemoveVisitor( Guid_t playerguid, ClientConnection* pConnection )
{
	__ENTER_FUNCTION__
		if (playerguid == invalid_guid || pConnection == NULL){
			CacheLog_DEBUG(LOG_DEBUG, "wrong parameter(guid:%d, pConnection:%d in PersonalBase::RemoveVisitor()",
				playerguid, pConnection);
			return;
		}
		VisitorOnClient theVisitor;
		theVisitor.nVisitorGuid = playerguid;
		theVisitor.pClientConn = pConnection;
		m_lstVisitors.remove(theVisitor);
		__LEAVE_FUNCTION__
}

void PersonalBase::OnBuildingUpgradeCompleted( Building &b )
{
	__ENTER_FUNCTION__
	SCSendBuildInfo msg;
	msg.SetObjID(b.nObjID);
	msg.SetInfoType(SCSendBuildInfo::SBI_UpgradeCompleted);
	BroadcastMsg(&msg);

	if (b.nHelperGuid != invalid_guid)
	{
		m_PersonalBaseInfo.m_lstHiredFriends.erase(std::remove(m_PersonalBaseInfo.m_lstHiredFriends.begin(), m_PersonalBaseInfo.m_lstHiredFriends.end(), b.nHelperGuid), m_PersonalBaseInfo.m_lstHiredFriends.end());
	}
	else
	{
		SetWorkingBuilderCount(GetWorkingBuilderCount()-1);
	}

	FillBuildingTableCfg(&b, b.nTableId, b.nLevel);

	// ��������߼� 
	OnBuildingEffect(b);

	// ������Ի������ݵ�Ӱ�췢���ͻ��ˣ���洢������
	// TODO:???

	__LEAVE_FUNCTION__
}

void PersonalBase::OnBuildingBuildCompleted( Building &b )
{
	SCSendBuildInfo msg;
	msg.SetObjID(b.nObjID);
	msg.SetInfoType(SCSendBuildInfo::SBI_BuildCompleted);
	BroadcastMsg(&msg);

	if (b.nHelperGuid != invalid_guid)
	{
		m_PersonalBaseInfo.m_lstHiredFriends.erase(std::remove(m_PersonalBaseInfo.m_lstHiredFriends.begin(), m_PersonalBaseInfo.m_lstHiredFriends.end(), b.nHelperGuid), m_PersonalBaseInfo.m_lstHiredFriends.end());
	}
	else
	{
		SetWorkingBuilderCount(GetWorkingBuilderCount()-1);
	}
}

void PersonalBase::CreateInitBase()
{
	__ENTER_FUNCTION__

	SetEnergy(0);
	SetGoldCoin(0);

	Building* pTownHall = CreateInitBuilding(0);   // �������
	pTownHall->nPosX = 25;
	pTownHall->nPosY = 25;

	Building* pClanHall = CreateInitBuilding(7);    // ���Ὠ��
	pClanHall->nPosX = 16;
	pClanHall->nPosY = 22;

	Building* pItemCenter = CreateInitBuilding(9); // ��������
	pItemCenter->nPosX = 36;
	pItemCenter->nPosY = 18;

	__LEAVE_FUNCTION__
}

void PersonalBase::OnBuildingEffect( Building &b )
{
	__ENTER_FUNCTION__
	_DBC_T_BUILDINGLEVELCFG* pCfg = BuildingConfig::getSingleton()->getLevelCfg(b.nTableId, b.nLevel);
	if (pCfg == NULL)
	{
		return;
	}

	switch(b.nLogicType)
	{
	case BTYPE_GOLD_WAREHOUSE: //�ֿ����������Դ�ͽ������ 
		{
			ReCalcGoldCapacity();
		}
		break;
	case BTYPE_ENERGY_WAREHOUSE:
		{
			ReCalcEnergyCapacity();
		}
		break;
	case BTYPE_MAINBUILDING:
		{
			m_PersonalBaseInfo.m_nHallLevel = b.nLevel;
			ReCalcGoldCapacity();
			ReCalcEnergyCapacity();

			// �ɾͱ仯
			int nProgress = b.nLevel;
			int nAchieveId = AchievementId_TownHallLevel;

			onAchievementProgressChanged(nAchieveId, nProgress);

		}
		break;
	default:
		break;
	}
	__LEAVE_FUNCTION__
}

bool PersonalBase::AddGold( int nVal )
{
	__ENTER_FUNCTION__
		if (nVal < 0)
		{
			return false;
		}
		int nAddVal = nVal;
		if (GetGoldCoin() + nAddVal > GetMaxGoldCoin())
		{
			nAddVal = GetMaxGoldCoin()-GetGoldCoin();
		}
		SetGoldCoin(GetGoldCoin() + nAddVal);
		//// �����洢����������
		//AddGoldToWareHouse(nAddVal);

		return true;
		__LEAVE_FUNCTION__
}

bool PersonalBase::AddEnergy( int nVal )
{
	__ENTER_FUNCTION__
		if (nVal < 0)
		{
			return false;
		}
		int nAddVal = nVal;
		if (GetEnergy() + nAddVal > GetMaxEnergy())
		{
			nAddVal = GetMaxEnergy()-GetEnergy();
		}
		SetEnergy(GetEnergy() + nAddVal);
		SetDirtyFlag(BASE_DATA_FLAG_Energy);

		AddEnergyToWareHouse(nAddVal);

		return true;
		__LEAVE_FUNCTION__
}

bool PersonalBase::CostGold( int nVal )
{
	__ENTER_FUNCTION__
		if (nVal < 0)
		{
			return false;
		}
		if (nVal > GetGoldCoin())
		{
			return false;
		}
		SetGoldCoin(GetGoldCoin() - nVal);
		SetDirtyFlag(BASE_DATA_FLAG_Gold);
		//// �������н���
		//CostGoldFromWarehouse(nVal);
		return true;
		__LEAVE_FUNCTION__
}

bool PersonalBase::CostEnergy( int nVal )
{
	__ENTER_FUNCTION__
		if (nVal < 0)
		{
			return false;
		}
		if (nVal > GetEnergy())
		{
			return false;
		}
		SetEnergy(GetEnergy() - nVal);
		SetDirtyFlag(BASE_DATA_FLAG_Energy);

		CostEnergyFromWarehouse(nVal);

		return true;
		__LEAVE_FUNCTION__
}

bool PersonalBase::CostGem( int nVal )
{
__ENTER_FUNCTION__
	if (nVal < 0)
	{
		return false;
	}
	if (nVal > GetGemCount())
	{
		return false;
	}
	m_PersonalBaseInfo.m_nGemCount -= nVal; 
	return true;
__LEAVE_FUNCTION__
}

void PersonalBase::SetBuilderCount( int val )
{
	m_PersonalBaseInfo.m_nBuilderCount = val;
	SetDirtyFlag(BASE_DATA_FLAG_BuilderCount);
}

void PersonalBase::SetWorkingBuilderCount( int val )
{
	m_PersonalBaseInfo.m_nWorkingBuilderCount = val;
	SetDirtyFlag(BASE_DATA_FLAG_WorkingBuilderCount);
}

void PersonalBase::SetDirtyFlag( enum_PBASE_DATA_FLAG flag )
{
	__ENTER_FUNCTION__
		m_DirtyFlagsForClient.set((int)flag);
	__LEAVE_FUNCTION__
}

void PersonalBase::FillBuildingTableCfg( Building* pData, int nTblId, int nLevel )
{
	__ENTER_FUNCTION__
	_DBC_T_BUILDINGLEVELCFG* pLCfg = BuildingConfig::getSingleton()->getLevelCfg(nTblId, nLevel);
	if (pLCfg != NULL)
	{
		for (int i = 0; i < BUILDING_EXT_DATA_SIZE; i++)
		{
			pData->raw[i] = pLCfg->arrParams[i];
		}
	}
	__LEAVE_FUNCTION__
}

void PersonalBase::RemoveVisitorFromConnection( ClientConnection * pConnection )
{
	__ENTER_FUNCTION__
		std::list<VisitorOnClient>	m_lstVisitors;
	VisitoContainer::iterator it, end(m_lstVisitors.end());
	for (it = m_lstVisitors.begin(); it != end;){
		const VisitorOnClient& theVisior = *it;
		if (theVisior.pClientConn == pConnection){
			m_lstVisitors.erase( it++ );
		}
		else{
			++it;
		}
	}
	__LEAVE_FUNCTION__
}

Building* PersonalBase::CreateInitBuilding( int nTblId )
{
	_DBC_T_BASEBUILDING * pCfg = BuildingConfig::getSingleton()->getBaseCfg(nTblId);
	if (pCfg == NULL)
	{
		return NULL;
	}

	int nTotalBuildTime = pCfg->nTimeForBuild;
	Building *b = new Building();
	b->nObjID = m_nMaxBuildingObjID++;
	b->nLogicType = pCfg->nLogicType;
	b->nBeginTime = 0;
	b->nState = Building::BS_Idle;
	b->nLeftTime = 0;
	b->nLevel = 1;
	b->nTag = pCfg->nFoundationTagId;
	b->nTableId = nTblId;

	FillBuildingTableCfg(b, nTblId, 1);

	m_vecBuildings.push_back(b); 

	return b;
}

ClientConnection* PersonalBase::GetOwnerClientConnection() const
{
	__ENTER_FUNCTION__
	ClientConnection* pConnection = NULL;
	VisitoContainer::const_iterator it, end(m_lstVisitors.end());
	for (it = m_lstVisitors.begin(); it != end; ++it){
		const VisitorOnClient& theVistor = *it;
		if (theVistor.nVisitorGuid == m_nOwnerGUID){
			pConnection = theVistor.pClientConn;
			break;
		}
	}
	return pConnection;

		__LEAVE_FUNCTION__
}

bool PersonalBase::CanHireFriend( Guid_t nHelperGuid )
{
	if (m_PersonalBaseInfo.m_lstHiredFriends.size() == m_PersonalBaseInfo.m_nMaxCanHiredFriendsCount)
	{
		// �ﵽ��Ӷ����
		return false;
	}
	if (std::find(m_PersonalBaseInfo.m_lstHiredFriends.begin(), m_PersonalBaseInfo.m_lstHiredFriends.end(), nHelperGuid) != m_PersonalBaseInfo.m_lstHiredFriends.end())
	{
		// �����ظ���Ӷͬһ������
		return false;
	}
	return true;
}

void PersonalBase::HireFriend( Guid_t nHelperGuid )
{
	// TODO:??? ��Ӷ�ʽ��ж������ �������ʼ�������

	//
	m_PersonalBaseInfo.m_lstHiredFriends.push_back(nHelperGuid);
}

// ���ܣ� ��һ��������Լ����ֵ��һ��ֵ������ʣ��ֵ
int _StoreResTo(int & dest, int capacity, int nAddVal)
{
	int nLeft = 0;
	if (nAddVal + dest > capacity)
	{
		nLeft = nAddVal - (capacity-dest);
		dest = capacity;
	}
	else
	{
		dest += nAddVal;
	}
	return nLeft;
}

void PersonalBase::AddGoldToWareHouse( int nVal )
{
   /* SCSendAllBuildingData msgBuilding;
	msgBuilding.targetGuid(GetOwnerGUID());
	msgBuilding.SetReson(SCSendAllBuildingData::SABD_REASON_DATA_CHANGED);

	int nValLeft = nVal;
	for (int i = 0; i < m_vecBuildings.size(); i++)
	{
		bool bAddOK = false;
		Building* pBuilding = m_vecBuildings[i];
		if (pBuilding->nLogicType == BTYPE_MAINBUILDING && pBuilding->MainBuildingExtDataRT.nGoldStore < pBuilding->MainBuildingExt.nGoldCapacity)
		{
			nValLeft = _StoreResTo(pBuilding->MainBuildingExtDataRT.nGoldStore, pBuilding->MainBuildingExt.nGoldCapacity, nValLeft);
			bAddOK = true;
		}
		else if (pBuilding->nLogicType == BTYPE_GOLD_WAREHOUSE && pBuilding->WareHouseExtDataRT.nResNumber < pBuilding->WareHouseExt.nCapacity)
		{
			nValLeft = _StoreResTo(pBuilding->WareHouseExtDataRT.nResNumber, pBuilding->WareHouseExt.nCapacity, nValLeft);
			bAddOK = true;
		}

		if (bAddOK)
		{
			msgBuilding.AddBuildingData(pBuilding);
		}
		
		if (nValLeft == 0)
		{
			break;
		}
	}
	if (msgBuilding.GetCount() > 0)
	{
		BroadcastMsg(&msgBuilding);
	}*/
}

void PersonalBase::AddEnergyToWareHouse( int nVal )
{
	SCSendAllBuildingData msgBuilding;
	msgBuilding.targetGuid(GetOwnerGUID());
	msgBuilding.SetReson(SCSendAllBuildingData::SABD_REASON_DATA_CHANGED);

	int nValLeft = nVal;
	for (int i = 0; i < m_vecBuildings.size(); i++)
	{
		bool bAddOK = false;
		Building* pBuilding = m_vecBuildings[i];
		if (pBuilding->nLogicType == BTYPE_MAINBUILDING && pBuilding->MainBuildingExtDataRT.nEnergyStore < pBuilding->MainBuildingExt.nEnergyCapacity)
		{
			nValLeft = _StoreResTo(pBuilding->MainBuildingExtDataRT.nEnergyStore, pBuilding->MainBuildingExt.nEnergyCapacity, nValLeft);
			bAddOK = true;
		}
		else if (pBuilding->nLogicType == BTYPE_ENERGY_WAREHOUSE && pBuilding->WareHouseExtDataRT.nResNumber < pBuilding->WareHouseExt.nCapacity)
		{
			nValLeft = _StoreResTo(pBuilding->WareHouseExtDataRT.nResNumber, pBuilding->WareHouseExt.nCapacity, nValLeft);
			bAddOK = true;
		}

		if (bAddOK)
		{
			msgBuilding.AddBuildingData(pBuilding);
		}

		if (nValLeft == 0)
		{
			break;
		}
	}
	if (msgBuilding.GetCount() > 0)
	{
		BroadcastMsg(&msgBuilding);
	}
}

void PersonalBase::ReCalcGoldCapacity()
{
	int nCapacity = 0x7fffffff;
	//for (int i = 0; i<m_vecBuildings.size(); i++)
	//{
	//	Building* pData = m_vecBuildings[i];
	//	if (pData->nLogicType == BTYPE_MAINBUILDING)
	//	{
	//		nCapacity += pData->MainBuildingExt.nGoldCapacity;
	//	}
	//	else if (pData->nLogicType == BTYPE_GOLD_WAREHOUSE)
	//	{
	//		nCapacity += pData->WareHouseExt.nCapacity;
	//	}
	//}
	SetMaxGoldCoin(nCapacity);
}

void PersonalBase::ReCalcEnergyCapacity()
{
	int nCapacity = 0;
	for (int i = 0; i<m_vecBuildings.size(); i++)
	{
		Building* pData = m_vecBuildings[i];
		if (pData->nLogicType == BTYPE_MAINBUILDING)
		{
			nCapacity += pData->MainBuildingExt.nEnergyCapacity;
		}
		else if (pData->nLogicType == BTYPE_ENERGY_WAREHOUSE)
		{
			nCapacity += pData->WareHouseExt.nCapacity;
		}
	}
	SetMaxEnergy(nCapacity);
}

// ���ܣ� �Ӵ洢��ȡ��Դ�����ػ�����ٲ���ȡ��
int _GetResFrom(int & src, int nGetVal)
{
	int nLeft = 0;
	if (src - nGetVal >= 0)
	{
		src = src - nGetVal;
	}
	else
	{
		nLeft = nGetVal - src;
		src = 0;
	}
	return nLeft;
}

void PersonalBase::CostGoldFromWarehouse( int nVal )
{
	SCSendAllBuildingData msgBuilding;
	msgBuilding.targetGuid(GetOwnerGUID());
	msgBuilding.SetReson(SCSendAllBuildingData::SABD_REASON_DATA_CHANGED);

	int nValLeft = nVal;
	for (int i = m_vecBuildings.size()-1; i >= 0; i--)
	{
		bool bResChange = false;
		Building* pBuilding = m_vecBuildings[i];
		if (pBuilding->nLogicType == BTYPE_MAINBUILDING && pBuilding->MainBuildingExtDataRT.nGoldStore > 0)
		{
			nValLeft = _GetResFrom(pBuilding->MainBuildingExtDataRT.nGoldStore, nValLeft);
			bResChange = true;
		}
		else if (pBuilding->nLogicType == BTYPE_GOLD_WAREHOUSE && pBuilding->WareHouseExtDataRT.nResNumber > 0)
		{
			nValLeft = _GetResFrom(pBuilding->WareHouseExtDataRT.nResNumber, nValLeft);
			bResChange = true;
		}

		if (bResChange)
		{
			msgBuilding.AddBuildingData(pBuilding);
		}

		if (nValLeft == 0)
		{
			break;
		}
	}
	if (msgBuilding.GetCount() > 0)
	{
		BroadcastMsg(&msgBuilding);
	}
}

void PersonalBase::CostEnergyFromWarehouse( int nVal )
{
	SCSendAllBuildingData msgBuilding;
	msgBuilding.targetGuid(GetOwnerGUID());
	msgBuilding.SetReson(SCSendAllBuildingData::SABD_REASON_DATA_CHANGED);

	int nValLeft = nVal;
	for (int i = m_vecBuildings.size()-1; i >= 0; i--)
	{
		bool bResChange = false;
		Building* pBuilding = m_vecBuildings[i];
		if (pBuilding->nLogicType == BTYPE_MAINBUILDING && pBuilding->MainBuildingExtDataRT.nEnergyStore > 0)
		{
			nValLeft = _GetResFrom(pBuilding->MainBuildingExtDataRT.nEnergyStore, nValLeft);
			bResChange = true;
		}
		else if (pBuilding->nLogicType == BTYPE_ENERGY_WAREHOUSE && pBuilding->WareHouseExtDataRT.nResNumber > 0)
		{
			nValLeft = _GetResFrom(pBuilding->WareHouseExtDataRT.nResNumber, nValLeft);
			bResChange = true;
		}

		if (bResChange)
		{
			msgBuilding.AddBuildingData(pBuilding);
		}

		if (nValLeft == 0)
		{
			break;
		}
	}
	if (msgBuilding.GetCount() > 0)
	{
		BroadcastMsg(&msgBuilding);
	}
}

void PersonalBase::onAchievementProgressChanged( int nAchieveId, int nProgress)
{
	Player* pOwner = PlayerManager::GetInstancePtr()->GetPlayerByGuid(GetOwnerGUID());
	if (pOwner != NULL)
	{
		AchievementItem *pAItem = pOwner->getAchievementData().GetAchievementItemById(nAchieveId);
		if (pAItem == NULL || pAItem->progress == nProgress)
		{
			return;
		}

		if (pAItem->level == MAX_ACHIEVEMENT_LEVEL)
		{
			return;
		}

		pAItem->progress = nProgress;

		SCSendAchievementDataChanged adcMsg;
		adcMsg.visitorGuid(GetOwnerGUID());
		adcMsg.SetAchieveId(nAchieveId);
		adcMsg.SetInfoType(SCSendAchievementDataChanged::SADC_Progress);
		adcMsg.SetParam1(pAItem->progress);
		ClientConnection* pOwnerClientConn = GetOwnerClientConnection();
		if (pOwnerClientConn != NULL)
		{
			pOwnerClientConn->SendMsg(&adcMsg);
		}

		// �ж��Ƿ�ﵽ������׼
		_DBC_T_ACHIEVEMENTS *pACfg = AchievementsConfig::getSingleton()->getCfg(nAchieveId);
		if (nProgress >= pACfg->aCfg[pAItem->level].nNum)
		{
			pAItem->level++;

			SCSendAchievementDataChanged adcLvMsg;
			adcLvMsg.visitorGuid(GetOwnerGUID());
			adcLvMsg.SetAchieveId(nAchieveId);
			adcLvMsg.SetInfoType(SCSendAchievementDataChanged::SADC_Level);
			adcLvMsg.SetParam1(pAItem->level);
			pOwnerClientConn->SendMsg(&adcLvMsg);
		}
	}
}

std::vector< std::vector<_TreasureData> >  PersonalBase::GetAllTreasures()
{
    std::vector< std::vector<_TreasureData> > ret;
    //for (int i = 0; i < m_PersonalBaseInfo.m_nTreasureBagCount; i++)
    //{
    //    std::vector<_TreasureData> bag;
    //    for (int j = 0; j < m_PersonalBaseInfo.m_nTreasureBagSize; j++)
    //    {
    //        _TreasureData & td = m_PersonalBaseInfo.m_allTresureData[i][j];
    //        if (td.id == invalid_card_id)
    //        {
    //            break;
    //        }
    //        bag.push_back(td);
    //    }
    //    ret.push_back(bag);
    //}
	return ret;
}

std::map<ID_t, int> PersonalBase::getAllTreasures()
{
    return m_PersonalBaseInfo.m_aTreasures;
}

void PersonalBase::AddTreasure(ID_t nTreasureId){
	Assert(nTreasureId >= 0);
	std::map<ID_t, int>::iterator iter = m_PersonalBaseInfo.m_aTreasures.find(nTreasureId);
	if (iter == m_PersonalBaseInfo.m_aTreasures.end())
	{
		m_PersonalBaseInfo.m_aTreasures[nTreasureId] = 1;
	}
	else{
		m_PersonalBaseInfo.m_aTreasures[nTreasureId]++;
	}
    //֪ͨ�ͻ��˱���仯
    OnTreasureChanged();
}

//void PersonalBase::AddTreasure( int nBagIdx, ID_t nTreasureId )
//{
//    for (int i = 0; i < MAX_TREASURE_BAG_SIZE; i++)
//    {
//        _TreasureData& refTreasure = m_PersonalBaseInfo.m_allTresureData[nBagIdx][i];
//        if (refTreasure.id == invalid_card_id)
//        {
//            refTreasure.id = nTreasureId;
//            refTreasure.state = _TreasureData::TreasureState_Raw; // Ӧ����nTreasureId�жϳ��Ƿ��Ѿ��������
//            break;
//        }
//    }
//}

void PersonalBase::RemoveTreasure( ID_t nTreasureId )
{
    Assert(nTreasureId >= 0);
    std::map<ID_t, int>::iterator iter = m_PersonalBaseInfo.m_aTreasures.find(nTreasureId);
    if (iter != m_PersonalBaseInfo.m_aTreasures.end())
    {
        if (iter->second == 1)
        {
            m_PersonalBaseInfo.m_aTreasures.erase(iter);
        }
        else
        {
            m_PersonalBaseInfo.m_aTreasures[nTreasureId]--;
        }
    }
    //֪ͨ�ͻ��˱���仯
    OnTreasureChanged();
}

void PersonalBase::OnTreasureChanged()
{
    // �·����б���
    SCSendAllTreasures msgSendAllTreasures;
    msgSendAllTreasures.targetGuid(this->GetOwnerGUID());
    msgSendAllTreasures.setTreasuresLst(this->getAllTreasures());
    BroadcastMsg(&msgSendAllTreasures);
}