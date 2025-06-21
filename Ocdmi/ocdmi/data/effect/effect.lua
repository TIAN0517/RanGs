-- m_emEndCondition 	-- 종료조건
END_CONDITION_NONE			= 0	-- 종료조건이 없다.  외부에서 삭제되야만 한다.
END_CONDITION_AUTO 			= 1	-- m_fNextStepTime 타임이 되면 종료가 된다.


-- m_emScreenEffectType
SCREENEFFECT_NONE			= 0 -- 아무작업도 하지 않는다. m_strEgpFile 작업은 가능.
SCREENEFFECT_BASE_TEXTURE	= 1	-- SCREENEFFECT Texture 렌더링.


-- example
-- STEP_DATA
--{
--	m_emEndCondition = END_CONDITION_TIME,				-- 종료 조건								( default : END_CONDITION_TIME )
--	m_bEgpInFrontOfEye = false;							-- egp 파일. 스크린 앞에서 렌더링 되도록 함	( default : false )
--	m_strEgpFile = "effect.egp",						-- 따로 play 될 .epg 파일					( default : "" )
--	m_emScreenEffectType = SCREENEFFECT_BASE_TEXTURE,	-- type 종류.								( default : SCREENEFFECT_NONE )
--	m_strScreenEffectShader = "shader.fx",				-- ScreeenEffect 에서 쓰이는 Shader			( default : "" )
--	m_strScreenEffectTexture = "texture.dds",			-- ScreeenEffect 에서 쓰이는 Texture		( default : "" )
--	m_fScreenEffectCreatingTime = 1,					-- ScreeenEffect 가 생성될때 총걸리는시간	( default : 0 )
--	m_fScreenEffectDestroyingTime = 1,					-- ScreeenEffect 가 종료될때 총걸리는시간	( default : 0 )
--	m_fNextStepTime = 1,								-- 다음 조건으로 이동하는 시간				( default : 0 )
--}


-- 하나의 EFFECT 에 STEP_DATA 이 다중으로 들어가 있다.
-- STEP_DATA 들은 순차적으로 발생을 하고 다음 STEP_DATA 이 불려지려면 m_fNextStepTime 의 시간이 되어야만 한다.
-- 자신의 STEP_DATA 은 독자적으로 살아있고, 
-- 소멸을 위해선 END_CONDITION_TIME 셋팅은 m_fEndTime 으로, END_CONDITION_NONE 셋팅은 외부에서 EFFECT 삭제만이 가능하다.

-- ReleaseD 에서는 \data\effect 폴더에 effect.lua 파일이 있으면 되지만 ( ReleaseD 는 무조건 폴더에 있는것만 읽는다. ),
-- Release 에서는 \data\effect\effect.rcc 압축파일안에 들어있어야함.


EFFECT_LIST =
{
	-- Effect 이름
	EFFECT_BROKEN_GLASS =
	{
		-- 1차
		-- effect 가 날라와서 화면에 부딫힘.
		{
			m_emEndCondition = END_CONDITION_AUTO,
			m_strEgpFile = "example_screen_throwstone.egp",
			m_fNextStepTime = 0.3,
		},

		-- 2차
		{
			m_emEndCondition = END_CONDITION_AUTO,
			m_bEgpInFrontOfEye = true;
			m_strEgpFile = "example_screen_flagmentstone.egp",
			m_fNextStepTime = 0.1,
		},

		-- 3차
		-- 화면에 약간의 금이 간다.
		-- 외부에서 삭제를 주면 0.5초 안으로 알파 빠지면서 사라짐.
		{
			m_emEndCondition = END_CONDITION_NONE,
			m_emScreenEffectType = SCREENEFFECT_BASE_TEXTURE,
			m_strScreenEffectTexture = "pvp_brickFloorBlending01_A.DDS",	-- 단순하게 Texture 만 렌더링.
			m_fScreenEffectDestroyingTime = 0.5,
		},

		-- 4차
		-- shader 를 사용한다.
		-- 깨진 느낌을 주도록 한다.
		-- 외부에서 삭제를 주면 0.5초 안으로 알파 빠지면서 사라짐.
		{
			m_emEndCondition = END_CONDITION_NONE,
			m_emScreenEffectType = SCREENEFFECT_BASE_TEXTURE,
			m_strScreenEffectShader = "ScreenBrokenGrass.fx",				-- shader
			m_strScreenEffectTexture = "weaponph_wand01_s.dds",				-- shader 를 위한 텍스쳐.
			m_fScreenEffectDestroyingTime = 0.5,
		},
	},

	-- Effect 이름
	EFFECT_CAMERA_MOVE = 
	{
		-- 1차
		-- END_CONDITION_NONE 이므로 외부에서 삭제를 해줘야 한다.
		{
			m_emEndCondition = END_CONDITION_NONE,
			m_strEgpFile = "example_camera.egp",
		},
	},


	-- Effect 이름
	EFFECT_WATER = 
	{
		-- 1차
		{
			m_emEndCondition = END_CONDITION_NONE,
			m_emScreenEffectType = SCREENEFFECT_BASE_TEXTURE,
			m_strScreenEffectTexture = "FabricFlag01_A.DDS",
		},
	},

	-- Effect 이름
	EFFECT_DUST = 
	{
		-- 1차
		{
			m_emEndCondition = END_CONDITION_NONE,
			m_emScreenEffectType = SCREENEFFECT_BASE_TEXTURE,
			m_strScreenEffectTexture = "fabricWicker01.DDS",
		},
	},
}