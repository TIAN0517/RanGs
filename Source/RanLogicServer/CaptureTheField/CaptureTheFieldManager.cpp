#include "../pch.h"
#include "../../SigmaCore/Util/SystemInfo.h"

#include "./CaptureTheFieldManager.h"
#include "./CaptureTheFieldScriptMan.h"

#include "../../RanLogic/GLUseFeatures.h"
#include "../../RanLogic/GLogicDataMan.h"

#include "../Land/GLLandMan.h"
#include "../FieldServer/GLGaeaServer.h"
#include "../../EngineLib/DxTools/DebugSet.h"


// ----------------------------------------------------------------------------
#include "../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------



class GLLandMan;


CaptureTheFieldMan::CaptureTheFieldMan()
:m_dwID(SNATIVEID(false).dwID)
,m_state(CTF_STATE_STOP)
,m_dwPVEMapID(SNATIVEID(false).dwID)
,m_dwPVPMapID(SNATIVEID(false).dwID)
,m_dwCertificationID1(SNATIVEID(false).dwID)
,m_dwCertificationID2(SNATIVEID(false).dwID)
,m_dwCertificationID3(SNATIVEID(false).dwID)
,m_dwGateStart(0)
,m_dwGateExit1(0)
,m_dwGateExit2(0)
,m_nPlayTimeM(30)
,m_nMaxRebirthTime(0)
,m_nChannel(0)
,m_nUpdatePlayersFreq(10000)
,m_fUpdatePlayerPositionFreq(1.0f)
,m_nContributionPoint4Winner(0)
,m_nContributionPoint4Loser(0)
,m_nContributionPointMax(50)
,m_wSchoolAuthed(SCHOOL_NONE)
,m_wRequiredLevel(1)
,m_bConsecutivelyCapturel(TRUE)
,m_dwConsecutivelyNumber(0)
,m_bConsecutively(FALSE)
,m_bPrevConsecutively(FALSE)
,m_dwDamage(0)
,m_dwRecovery(0)
,m_wLastBattleID(USHRT_MAX)
,m_bStartCycle(false)
{
	m_pScriptMan = new CaptureTheFieldScriptMan();

	for (int i=0; i<SCHOOL_NUM; i++)
		m_dwGateRevival[i] = 0;

	
}


CaptureTheFieldMan::~CaptureTheFieldMan()
{
	SAFE_DELETE(m_pScriptMan);
}


BOOL CaptureTheFieldMan::Initialize()
{
	if (!GLUseFeatures::GetInstance().IsUsingCaptureTheField())
		return FALSE;

	//
	//Load Script
	//
	std::string strScriptPath(sc::getAppPath());
	strScriptPath.append("\\Data\\GLogicServer\\");
	strScriptPath.append("scripts\\");

	std::string strCTFScriptPath(strScriptPath);    
    strCTFScriptPath.append(GLogicData::GetInstance().GetCaptureTheFieldScriptName());

	m_pScriptMan->LoadScript(strCTFScriptPath.c_str());
	
	if (!m_pScriptMan->IsLoaded())
	{
		sc::writeLogError(std::string("CaptureTheFieldMan::Initialize() failure. Script MUST be loaded to run this feature."));

		return FALSE;
	}


	//
	// Initialization using script.
	//
	m_dwID					= SNATIVEID(1, 0).dwID;		//NOT USED YET!
	
	//
	//MapID
	//
	m_dwPVEMapID			= SNATIVEID(m_pScriptMan->GetConfigWORD("PvEMapMID"), m_pScriptMan->GetConfigWORD("PvEMapSID")).dwID;
	m_dwPVPMapID			= SNATIVEID(m_pScriptMan->GetConfigWORD("PvPMapMID"), m_pScriptMan->GetConfigWORD("PvPMapSID")).dwID;
	
	//
	//NPCs
	//
	m_dwCertificationID1	= SNATIVEID(m_pScriptMan->GetConfigWORD("Certification1MID"), m_pScriptMan->GetConfigWORD("Certification1SID")).dwID;
	m_dwCertificationID2	= SNATIVEID(m_pScriptMan->GetConfigWORD("Certification2MID"), m_pScriptMan->GetConfigWORD("Certification2SID")).dwID;
	m_dwCertificationID3	= SNATIVEID(m_pScriptMan->GetConfigWORD("Certification3MID"), m_pScriptMan->GetConfigWORD("Certification3SID")).dwID;
	
	m_nChannel				= m_pScriptMan->GetConfigUINT("Channel");

	//
	//Gates
	//
	m_dwGateStart			= m_pScriptMan->GetConfigUINT("GateStart");

	m_dwGateExit1			= m_pScriptMan->GetConfigUINT("GateExit1");
	m_dwGateExit2			= m_pScriptMan->GetConfigUINT("GateExit2");


	m_dwGateRevival[SCHOOL_SM]	= m_pScriptMan->GetConfigUINT("GateRevivalSM");
	m_dwGateRevival[SCHOOL_HA]	= m_pScriptMan->GetConfigUINT("GateRevivalHA");
	m_dwGateRevival[SCHOOL_BH]	= m_pScriptMan->GetConfigUINT("GateRevivalBH");

	m_dwMapIDKickOut[SCHOOL_SM]		= SNATIVEID(m_pScriptMan->GetConfigWORD("KickOutMapMIDSM"), m_pScriptMan->GetConfigWORD("KickOutMapSIDSM")).dwID;
	m_dwGateKickOut[SCHOOL_SM]		= m_pScriptMan->GetConfigUINT("KickOutGateIDSM");

	m_dwMapIDKickOut[SCHOOL_HA]		= SNATIVEID(m_pScriptMan->GetConfigWORD("KickOutMapMIDHA"), m_pScriptMan->GetConfigWORD("KickOutMapSIDHA")).dwID;
	m_dwGateKickOut[SCHOOL_HA]		= m_pScriptMan->GetConfigUINT("KickOutGateIDHA");

	m_dwMapIDKickOut[SCHOOL_BH]		= SNATIVEID(m_pScriptMan->GetConfigWORD("KickOutMapMIDBH"), m_pScriptMan->GetConfigWORD("KickOutMapSIDBH")).dwID;
	m_dwGateKickOut[SCHOOL_BH]		= m_pScriptMan->GetConfigUINT("KickOutGateIDBH");

	
	m_nReadyDurationM		= m_pScriptMan->GetConfigUINT("ReadyDurationM");
	GASSERT(m_nReadyDurationM > 0);
	m_ctsReadyDurationM		= CTimeSpan(0, 0, m_nReadyDurationM, 0);


	m_nRewardTimeM			= m_pScriptMan->GetConfigUINT("RewardTimeM");
	GASSERT(m_nRewardTimeM > 0);
	m_ctsRewardTimeM		= CTimeSpan(0, 0, m_nRewardTimeM, 0);
	
	m_nStartCycleH			= m_pScriptMan->GetConfigUINT("StartCycleH");	
	GASSERT(m_nStartCycleH > 0);
	m_nStartCycleS			= m_nStartCycleH * 3600;
	m_ctsStartCycleH		= CTimeSpan(0, m_nStartCycleH, 0, 0);
	

	m_nPlayTimeM			= m_pScriptMan->GetConfigUINT("PlayTimeM");
	GASSERT(m_nPlayTimeM > 0);
	m_ctsPlayTimeM			= CTimeSpan(0, 0, m_nPlayTimeM, 0);


	m_ctStartTime			= CTime::GetCurrentTime() + m_ctsStartCycleH;

	m_bStartCycle				= m_pScriptMan->GetConfigbool("bStartCycle");


	m_sItemRewardWin.wMainID = m_pScriptMan->GetConfigWORD("RewardWinner_MID");
	m_sItemRewardWin.wSubID = m_pScriptMan->GetConfigWORD("RewardWinner_SID");

	m_sItemRewardLost.wMainID = m_pScriptMan->GetConfigWORD("RewardLoser_MID");
	m_sItemRewardLost.wSubID = m_pScriptMan->GetConfigWORD("RewardLoser_SID");

	//schedule day-hour-minute
	m_vecSchedule.clear();
	if (!m_bStartCycle)
	{
		TYRANNY_SCHED sched;
		sched.wStartHour = m_pScriptMan->GetConfigWORD("BattleTime_H_1");
		sched.wStartMinute = m_pScriptMan->GetConfigWORD("BattleTime_M_1");
		CTime timecur = CTime::GetCurrentTime();
		for ( int i = 1; i <= 7; ++i )
		{
			sched.wStartDay = timecur.GetDay();
			sched.wStartDayOfWeek = timecur.GetDayOfWeek();
			sched.wStartMonth = timecur.GetMonth();
			sched.wStartYear = timecur.GetYear();
			m_vecSchedule.push_back( sched );
			CTimeSpan span( 1, 0, 0, 0 );
			timecur += span;
			/*sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime 1 Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", 
				sched.wStartYear, sched.wStartMonth, sched.wStartDay, sched.wStartHour, sched.wStartMinute ));*/
		}

		timecur = CTime::GetCurrentTime();
		sched.wStartHour = m_pScriptMan->GetConfigWORD("BattleTime_H_2");
		sched.wStartMinute = m_pScriptMan->GetConfigWORD("BattleTime_M_2");
		for ( int i = 1; i <= 7; ++i )
		{
			sched.wStartDay = timecur.GetDay();
			sched.wStartDayOfWeek = timecur.GetDayOfWeek();
			sched.wStartMonth = timecur.GetMonth();
			sched.wStartYear = timecur.GetYear();
			m_vecSchedule.push_back( sched );
			CTimeSpan span( 1, 0, 0, 0 );
			timecur += span;
			/*sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime 2 Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", 
				sched.wStartYear, sched.wStartMonth, sched.wStartDay, sched.wStartHour, sched.wStartMinute ));*/
		}

		timecur = CTime::GetCurrentTime();
		sched.wStartHour = m_pScriptMan->GetConfigWORD("BattleTime_H_3");
		sched.wStartMinute = m_pScriptMan->GetConfigWORD("BattleTime_M_3");
		for ( int i = 1; i <= 7; ++i )
		{
			sched.wStartDay = timecur.GetDay();
			sched.wStartDayOfWeek = timecur.GetDayOfWeek();
			sched.wStartMonth = timecur.GetMonth();
			sched.wStartYear = timecur.GetYear();
			m_vecSchedule.push_back( sched );
			CTimeSpan span( 1, 0, 0, 0 );
			timecur += span;
			/*sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime 3 Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", 
				sched.wStartYear, sched.wStartMonth, sched.wStartDay, sched.wStartHour, sched.wStartMinute ));*/
		}

		timecur = CTime::GetCurrentTime();
		sched.wStartHour = m_pScriptMan->GetConfigWORD("BattleTime_H_4");
		sched.wStartMinute = m_pScriptMan->GetConfigWORD("BattleTime_M_4");
		for ( int i = 1; i <= 7; ++i )
		{
			sched.wStartDay = timecur.GetDay();
			sched.wStartDayOfWeek = timecur.GetDayOfWeek();
			sched.wStartMonth = timecur.GetMonth();
			sched.wStartYear = timecur.GetYear();
			m_vecSchedule.push_back( sched );
			CTimeSpan span( 1, 0, 0, 0 );
			timecur += span;
			/*sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime 4 Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", 
				sched.wStartYear, sched.wStartMonth, sched.wStartDay, sched.wStartHour, sched.wStartMinute ));*/
		}

		timecur = CTime::GetCurrentTime();
		sched.wStartHour = m_pScriptMan->GetConfigWORD("BattleTime_H_5");
		sched.wStartMinute = m_pScriptMan->GetConfigWORD("BattleTime_M_5");
		for ( int i = 1; i <= 7; ++i )
		{
			sched.wStartDay = timecur.GetDay();
			sched.wStartDayOfWeek = timecur.GetDayOfWeek();
			sched.wStartMonth = timecur.GetMonth();
			sched.wStartYear = timecur.GetYear();
			m_vecSchedule.push_back( sched );
			CTimeSpan span( 1, 0, 0, 0 );
			timecur += span;
			/*sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime 5 Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", 
				sched.wStartYear, sched.wStartMonth, sched.wStartDay, sched.wStartHour, sched.wStartMinute ));*/
		}

		timecur = CTime::GetCurrentTime();
		sched.wStartHour = m_pScriptMan->GetConfigWORD("BattleTime_H_6");
		sched.wStartMinute = m_pScriptMan->GetConfigWORD("BattleTime_M_6");
		for ( int i = 1; i <= 7; ++i )
		{
			sched.wStartDay = timecur.GetDay();
			sched.wStartDayOfWeek = timecur.GetDayOfWeek();
			sched.wStartMonth = timecur.GetMonth();
			sched.wStartYear = timecur.GetYear();
			m_vecSchedule.push_back( sched );
			CTimeSpan span( 1, 0, 0, 0 );
			timecur += span;
			/*sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime 6 Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", 
				sched.wStartYear, sched.wStartMonth, sched.wStartDay, sched.wStartHour, sched.wStartMinute ));*/
		}

		timecur = CTime::GetCurrentTime();
		sched.wStartHour = m_pScriptMan->GetConfigWORD("BattleTime_H_7");
		sched.wStartMinute = m_pScriptMan->GetConfigWORD("BattleTime_M_7");
		for ( int i = 1; i <= 7; ++i )
		{
			sched.wStartDay = timecur.GetDay();
			sched.wStartDayOfWeek = timecur.GetDayOfWeek();
			sched.wStartMonth = timecur.GetMonth();
			sched.wStartYear = timecur.GetYear();
			m_vecSchedule.push_back( sched );
			CTimeSpan span( 1, 0, 0, 0 );
			timecur += span;
			/*sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime 7 Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", 
				sched.wStartYear, sched.wStartMonth, sched.wStartDay, sched.wStartHour, sched.wStartMinute ));*/
		}

		timecur = CTime::GetCurrentTime();
		sched.wStartHour = m_pScriptMan->GetConfigWORD("BattleTime_H_8");
		sched.wStartMinute = m_pScriptMan->GetConfigWORD("BattleTime_M_8");
		for ( int i = 1; i <= 7; ++i )
		{
			sched.wStartDay = timecur.GetDay();
			sched.wStartDayOfWeek = timecur.GetDayOfWeek();
			sched.wStartMonth = timecur.GetMonth();
			sched.wStartYear = timecur.GetYear();
			m_vecSchedule.push_back( sched );
			CTimeSpan span( 1, 0, 0, 0 );
			timecur += span;
			/*sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime 8 Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", 
				sched.wStartYear, sched.wStartMonth, sched.wStartDay, sched.wStartHour, sched.wStartMinute ));*/
		}

		std::sort( m_vecSchedule.begin(), m_vecSchedule.end() );

		if ( m_vecSchedule.empty() )
		{
			sc::writeLogInfo( "CaptureTheFieldMan::Load no schedules loaded" );
		}

		/*for ( int iz=0; iz<m_vecSchedule.size(); ++iz )
		{
		sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::BattleTime Index (%d) Year(%d) Month(%d) Day(%d) Hour(%d) Minute(%d)", iz,
		m_vecSchedule[iz].wStartYear, m_vecSchedule[iz].wStartMonth, m_vecSchedule[iz].wStartDay, m_vecSchedule[iz].wStartHour, m_vecSchedule[iz].wStartMinute ));
		}*/

		FindScheduleFirst();
		if ( m_sScheduleNext.dwIndex != UINT_MAX )
		{
			m_nStartTimeIndexPrev = m_sScheduleNext.dwIndex;

			m_ctStartTime		= CTime(m_sScheduleNext.sStartSched.wStartYear,m_sScheduleNext.sStartSched.wStartMonth,m_sScheduleNext.sStartSched.wStartDay,m_sScheduleNext.sStartSched.wStartHour,m_sScheduleNext.sStartSched.wStartMinute,0);//CTimeSpan(m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute, 0);

			sc::writeLogInfo(sc::string::format("[CTF] CaptureTheFieldMan::Initialize BattleTime Size %d", m_vecBattleTime.size()));
		}

		/*
			CTime t(1999, 3, 19, 22, 15, 0); 
			10:15 PM March 19, 1999
		*/
	}

	m_ctStopTime			= m_ctStartTime + m_ctsPlayTimeM;
	m_ctExtraTime			= m_ctStopTime + m_ctsRewardTimeM;

	m_ctReadyTime			= m_ctStartTime - m_ctsReadyDurationM;

	m_nMaxNumOfJoiners		= m_pScriptMan->GetConfigUINT("MaxNumOfJoiners");
	
	//
	// nMaxNumOfPlayers < CTF_PLAYER_NUM_LIMIT (60) <-- 이 값보다 큰 값으로 설정하고자 할 경우네는 프로그래머에게 문의
	//
	m_nMaxNumOfPlayers		= m_pScriptMan->GetConfigUINT("MaxNumOfPlayers");

	//
	// Script에서 설정하는 각 학원 별 CTF Player의 인원 수는 이 값을 넘어서는 안된다.
	//
	GASSERT( m_nMaxNumOfPlayers >= SCHOOL_NUM );
	GASSERT(m_nMaxNumOfPlayers <= CTF_PLAYER_NUM_LIMIT_PER_SCHOOL);


	m_nMaxRebirthTime		= m_pScriptMan->GetConfigUINT("MaxRebirthTimeS");
	
	m_nUpdatePlayersFreq	= m_pScriptMan->GetConfigUINT("UpdatePlayersFreq");		//msec

	m_fUpdatePlayerPositionFreq	= m_pScriptMan->GetConfigfloat("UpdateMinimapFreq");//msec

	m_nContributionPoint4Winner	= m_pScriptMan->GetConfigUINT("ContributionPoint4Winner");
	m_nContributionPoint4Loser	= m_pScriptMan->GetConfigUINT("ContributionPoint4Loser");
	m_nContributionPointMax		= m_pScriptMan->GetConfigUINT("ContributionPointMax");

	m_wRequiredLevel			= m_pScriptMan->GetConfigUINT("RequiredLevel");

	m_bConsecutivelyCapturel	= m_pScriptMan->GetConfigbool("ConsecutivelyCapturel") == true ? TRUE : FALSE;

	m_dwConsecutivelyNumber		= m_pScriptMan->GetConfigUINT("ConsecutivelyNumber");

	m_dwDamage					= m_pScriptMan->GetConfigUINT("Damage");
	m_dwRecovery				= m_pScriptMan->GetConfigUINT("Recovery");


	// I will make vector size to limited number of consecutive
	// because of it doesn't need to save over count 
	// in this case if 100 numbers saved than it can do
	// ex) if 3
	// if AAA, A cannot join
	// if ABC, AAB etc.. everyone can join
	for(DWORD i= 0; i <m_dwConsecutivelyNumber; i++)
		SetConsecutively(SCHOOL_NONE);
	
	//
	// Some Log Structures HERE .
	//
	HookAfterInit();

	m_mapRewarded.clear();

	return TRUE;
}


void CaptureTheFieldMan::AddRewardBuff(SNATIVEID id, WORD wLevel)
{   
	m_vecRewardBuffs.push_back(CTF_REWARD_BUFF(id.dwID, wLevel));
}


void CaptureTheFieldMan::GetRewardBuff(UINT idx, OUT CTF_REWARD_BUFF &rewardBuff)
{
    GASSERT( idx < (UINT)m_vecRewardBuffs.size() );

    rewardBuff.dwID     = m_vecRewardBuffs[idx].dwID;
    rewardBuff.wLevel   = m_vecRewardBuffs[idx].wLevel;
}


void CaptureTheFieldMan::FindScheduleFirst()
{
	if ( m_vecSchedule.empty() )	return;

	m_sScheduleNext.dwIndex = UINT_MAX;

	if ( m_vecSchedule.size() == 1 )
	{
		m_sScheduleNext.dwIndex = 0;
		m_sScheduleNext.sStartSched = m_vecSchedule[0];
		m_sScheduleNext.CalculateEndTime( 600, m_nPlayTimeM, m_nRewardTimeM );
	}
	else
	{
		CTime ctimecur = CTime::GetCurrentTime();

		//expensive loop
		int nminutes = 0;
		while(1)
		{
			for ( size_t i=0; i<m_vecSchedule.size(); ++i )
			{
				if ( m_vecSchedule[i].wStartDayOfWeek != (WORD)ctimecur.GetDayOfWeek() )	continue;
				if ( m_vecSchedule[i].wStartMonth != (WORD)ctimecur.GetMonth() )			continue;
				if ( m_vecSchedule[i].wStartDay != (WORD)ctimecur.GetDay() )				continue;
				if ( m_vecSchedule[i].wStartHour != (WORD)ctimecur.GetHour() )				continue;
				if ( m_vecSchedule[i].wStartMinute != (WORD)ctimecur.GetMinute() )			continue;
				if ( m_vecSchedule[i].wStartYear != (WORD)ctimecur.GetYear() )			continue;

				m_sScheduleNext.dwIndex = i;
				m_sScheduleNext.sStartSched = m_vecSchedule[i];
				m_sScheduleNext.CalculateEndTime( 600, m_nPlayTimeM, m_nRewardTimeM );
			}

			if ( m_sScheduleNext.dwIndex != UINT_MAX )
				break;

			//week limit
			if ( nminutes >= 11000/*10080*/ )
				break;

			CTimeSpan span( 0, 0, 1, 0 );
			ctimecur += span;
			nminutes ++;
			Sleep(0);
		}

		if ( m_sScheduleNext.dwIndex == UINT_MAX )
		{
			sc::writeLogInfo( "FindScheduleFirst failed! no events will start" );
		}
		else
		{
			sc::writeLogInfo( sc::string::format( "FindScheduleFirst Index:%1% Start(D:%2%/H:%3%/M:%4%) BattleStart(H:%5%/M:%6%) BattleEnd(H:%7%/M:%8%)", m_sScheduleNext.dwIndex, 
				m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
				m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute ) );

			CDebugSet::ToFileWithTime( "CaptureTheFieldMan_FindScheduleFirst.txt", sc::string::format("FindScheduleFirst Index:%1% Start(D:%2%/H:%3%/M:%4%) BattleStart(H:%5%/M:%6%) BattleEnd(H:%7%/M:%8%)", m_sScheduleNext.dwIndex, 
				m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
				m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute ).c_str() );
		}
	}
};

bool CaptureTheFieldMan::FindScheduleNext()
{
	if ( m_sScheduleNext.dwIndex == UINT_MAX )	return false;

	DWORD dwnewidx = m_sScheduleNext.dwIndex + 1;
	if ( dwnewidx >= (DWORD)m_vecSchedule.size() )	dwnewidx = 0;

	CTime ctimecur = CTime::GetCurrentTime();

	if ( m_vecSchedule[dwnewidx].wStartDay != (WORD)ctimecur.GetDayOfWeek() && dwnewidx != 0 )
	{
		/*CTime cTemp(ctimecur.GetYear(), ctimecur.GetMonth(), ctimecur.GetDay(), 
			m_vecSchedule[m_nStartTimeIndexPrev].wStartHour, m_vecSchedule[m_nStartTimeIndexPrev].wStartMinute, 0);

		CTime cTemp2(ctimecur.GetYear(), ctimecur.GetMonth(),  m_vecSchedule[dwnewidx].wStartDay, 
			m_vecSchedule[dwnewidx].wStartHour, m_vecSchedule[dwnewidx].wStartMinute, 0);*/

		CTimeSpan span( 1, 0, 0, 0 );

		CTime tTimeNew = ctimecur + span;

		m_sScheduleNext.dwIndex = dwnewidx;
		m_sScheduleNext.sStartSched = m_vecSchedule[dwnewidx];
		m_sScheduleNext.sStartSched.wStartDay = tTimeNew.GetDay();
		m_sScheduleNext.CalculateEndTime( 600, m_nPlayTimeM, m_nRewardTimeM );

		sc::writeLogInfo( sc::string::format( "FindScheduleNext Index:%1% Start(D:%2%/H:%3%/M:%4%) BattleStart(H:%5%/M:%6%) BattleEnd(H:%7%/M:%8%)", m_sScheduleNext.dwIndex, 
			m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
			m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute ) );

		CDebugSet::ToFileWithTime( "CaptureTheFieldMan_FindScheduleNext.txt", sc::string::format("FindScheduleNext Index:%1% Start(D:%2%/H:%3%/M:%4%) BattleStart(H:%5%/M:%6%) BattleEnd(H:%7%/M:%8%)", m_sScheduleNext.dwIndex, 
			m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
			m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute ).c_str() );

		return true;
	}

	m_sScheduleNext.dwIndex = dwnewidx;
	m_sScheduleNext.sStartSched = m_vecSchedule[dwnewidx];
	m_sScheduleNext.sStartSched.wStartDay = ctimecur.GetDay();
	m_sScheduleNext.CalculateEndTime( 600, m_nPlayTimeM, m_nRewardTimeM );

	sc::writeLogInfo( sc::string::format( "FindScheduleNext Index:%1% Start(D:%2%/H:%3%/M:%4%) BattleStart(H:%5%/M:%6%) BattleEnd(H:%7%/M:%8%)", m_sScheduleNext.dwIndex, 
		m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
		m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute ) );

	CDebugSet::ToFileWithTime( "CaptureTheFieldMan_FindScheduleNext.txt", sc::string::format("FindScheduleNext Index:%1% Start(D:%2%/H:%3%/M:%4%) BattleStart(H:%5%/M:%6%) BattleEnd(H:%7%/M:%8%)", m_sScheduleNext.dwIndex, 
		m_sScheduleNext.sStartSched.wStartDay, m_sScheduleNext.sStartSched.wStartHour, m_sScheduleNext.sStartSched.wStartMinute,
		m_sScheduleNext.wBattleStartHour, m_sScheduleNext.wBattleStartMinute, m_sScheduleNext.wBattleEndHour, m_sScheduleNext.wBattleEndMinute ).c_str() );

	return true;
};
