#pragma once

#include "../../InternalCommonLib/dxstdafx.h"
#include "./UITextUtil.h"

//	NOTE
//		�Ʒ��� �̸��� 'color_chart.html'�� �����ϴ�.
//		/=document/color_chart.html

//	CAUTION
//		���İ��� �ٲ��� ���ʽÿ�.

namespace NS_UITEXTCOLOR
{
	//	�⺻�� ����
	//
	const DWORD WHITESMOKE		= D3DCOLOR_ARGB(0xFF,0xF5,0xF5,0xF5);
	const DWORD WHITE			= D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0xFF);
	const DWORD DARKGRAY		= D3DCOLOR_ARGB(0xFF,0xA9,0xA9,0xA9);
	const DWORD	LIGHTSKYBLUE	= D3DCOLOR_ARGB(0xFF,0x87,0xCE,0xFA);
	const DWORD	PALEGREEN		= D3DCOLOR_ARGB(0xFF,0x98,0xFB,0x98);
	const DWORD RED				= D3DCOLOR_ARGB(0xFF,0xFF,0x00,0x00);
	const DWORD GREEN			= D3DCOLOR_ARGB(0xFF,0x00,0xFF,0x00);
	const DWORD BLUE			= D3DCOLOR_ARGB(0xFF,0x00,0x00,0xFF);
	const DWORD GOLD			= D3DCOLOR_ARGB(0xFF,0xFF,0xD7,0x00);
	const DWORD PALEGOLDENROD	= D3DCOLOR_ARGB(0xFF,0xEE,0xE8,0xAA);	
	const DWORD DODGERBLUE		= D3DCOLOR_ARGB(0xFF,0x1E,0x90,0xFF);
	const DWORD DARKTURQUOISE	= D3DCOLOR_ARGB(0xFF,0x00,0xCE,0xD1);
	const DWORD TOMATO			= D3DCOLOR_ARGB(0xFF,0xFF,0x63,0x47);
	const DWORD AQUA			= D3DCOLOR_ARGB(0xFF,0x00,0xFF,0xFF);
	const DWORD ORANGE			= D3DCOLOR_ARGB(0xFF,0xFF,0xA5,0x00);
	const DWORD PALETURQUOISE	= D3DCOLOR_ARGB(0xFF,0xAF,0xEE,0xEE);
	const DWORD SILVER			= D3DCOLOR_ARGB(0xFF,0xC0,0xC0,0xC0);
    const DWORD LIGHTGREY		= D3DCOLOR_ARGB(0xFF,0xD3,0xD3,0xD3);	
    const DWORD DIMGREY		    = D3DCOLOR_ARGB(0xFF,0x69,0x69,0x69);	
	const DWORD GREENYELLOW		= D3DCOLOR_ARGB(0xFF,0xAD,0xFF,0x2F);
	const DWORD ORNAGERED		= D3DCOLOR_ARGB(0xFF,0xFF,0x45,0x00);
	const DWORD HONEYDEW		= D3DCOLOR_ARGB(0xFF,0xF0,0xFF,0xF0);
	const DWORD PERU			= D3DCOLOR_ARGB(0xFF,0xCD,0x85,0x3F);
	const DWORD BURLYWOOD		= D3DCOLOR_ARGB(0xFF,0xDE,0xB8,0x87);
	const DWORD GOLDENROD		= D3DCOLOR_ARGB(0xFF,0xDA,0xA5,0x20);
	const DWORD DARKORANGE		= D3DCOLOR_ARGB(0xFF,0xFF,0x8C,0x00);	
	const DWORD LIGHTCYAN		= D3DCOLOR_ARGB(0xFF,0xE0,0xFF,0xFF);
	const DWORD IVORY			= D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0xF0);
	const DWORD YELLOW			= D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0x00);
	const DWORD CHARTREUSE		= D3DCOLOR_ARGB(0xFF,0x7F,0xFF,0x00);
	const DWORD THISTLE			= D3DCOLOR_ARGB(0xFF,0xD8,0xBF,0xD8);	
	const DWORD LIGHTSALMON		= D3DCOLOR_ARGB(0xFF,0xFF,0xA0,0x7A);
	const DWORD TURQUOISE		= D3DCOLOR_ARGB(0xFF,0x40,0xE0,0xD0);
	const DWORD VIOLET			= D3DCOLOR_ARGB(0xFF,0xEE,0x82,0xEE);
	const DWORD AQUAMARINE		= D3DCOLOR_ARGB(0xFF,0x7F,0xFF,0xD4);
	const DWORD FORESTGREEN		= D3DCOLOR_ARGB(0xFF,0x22,0x8B,0x22);
	const DWORD HOTPINK			= D3DCOLOR_ARGB(0xFF,0xFF,0x69,0xB4);
	const DWORD MAROON			= D3DCOLOR_ARGB(0xFF,0x80,0x00,0x00);
	const DWORD NAVY			= D3DCOLOR_ARGB(0xFF,0x00,0x00,0x80); // £����������������
	const DWORD BLACK			= D3DCOLOR_ARGB(0xFF,0x00,0x00,0x00);
	const DWORD OLIVE			= D3DCOLOR_ARGB(0xFF,0xD3,0xEB,0x2E); // �ø���
	const DWORD WARMPINK		= D3DCOLOR_ARGB(0xFF,0xF9,0xB6,0xB0); // �ణ ��ο� ��ũ
	
	// clubBattle ����ҷ��� �߰� ( ���� ���ݾ� Ʋ�� ) 
	const DWORD AQUABLUE		= D3DCOLOR_ARGB(0xFF,0x00,0x26,0xFF);
	const DWORD DARKVIOLET		= D3DCOLOR_ARGB(0xFF,0xB2,0x00,0xFF);
	const DWORD FUCHSIA			= D3DCOLOR_ARGB(0xFF,0xFF,0x00,0xDC);
	const DWORD PURPLE			= D3DCOLOR_ARGB(0xFF,0x7F,0x00,0x6E);
	const DWORD LIGHTYELLOW		= D3DCOLOR_ARGB(0xFF,0xFF,0xE9,0x7F);

	// 4D Tech Style Colors - 科技感顏色系統
	const DWORD TECH_CYAN_GLOW		= D3DCOLOR_ARGB(0xFF,0x00,0xFF,0xFF); // 青色光暈
	const DWORD TECH_BLUE_DEEP		= D3DCOLOR_ARGB(0xFF,0x00,0x7F,0xFF); // 深藍科技色
	const DWORD TECH_BLUE_BRIGHT	= D3DCOLOR_ARGB(0xFF,0x40,0xC0,0xFF); // 亮藍科技色
	const DWORD TECH_GOLD_VICTORY	= D3DCOLOR_ARGB(0xFF,0xFF,0xD7,0x00); // 勝利金色
	const DWORD TECH_RED_WARNING	= D3DCOLOR_ARGB(0xFF,0xFF,0x30,0x30); // 警告紅色
	const DWORD TECH_GLASS_ALPHA	= D3DCOLOR_ARGB(0x80,0xE0,0xFF,0xFF); // 玻璃質感半透明
	const DWORD TECH_HOLOGRAM		= D3DCOLOR_ARGB(0xCC,0x00,0xFF,0xCC); // 全息投影色


	//	���ó
	const DWORD ENABLE			= LIGHTGREY;	
	const DWORD DISABLE			= RED;

	const DWORD DEFAULT			= ENABLE;
	const DWORD DIALOGUE_MOUSEOVER = GOLD;
	const DWORD ITEMMONEY		= GOLD;
	const DWORD SELF			= PALEGOLDENROD;	
	const DWORD SYS_ERROR		= RED;
	const DWORD PARTY			= DARKTURQUOISE;
	const DWORD GUILD			= TOMATO;
	const DWORD PRIVATE			= GOLD;	
	const DWORD	PLUSOPTION		= PALEGOLDENROD;
	const DWORD PARTYNAME		= PLUSOPTION;
	const DWORD IDCOLOR			= LIGHTSKYBLUE;
	const DWORD ADMIN_COLOR		= DISABLE;	
	const DWORD NEGATIVE		= RED;
	const DWORD FRIEND_OFF		= MAROON;
	const DWORD ACTIVITY		= WARMPINK;

	const DWORD MAX_CLUB_BATTLE_COLOR = 8;
	const DWORD CLUB_BATTLE_COLOR[MAX_CLUB_BATTLE_COLOR] = { AQUA, DODGERBLUE, AQUABLUE, DARKVIOLET,
		FUCHSIA, PURPLE, LIGHTYELLOW, GOLD };


	//	ä�� Ÿ�� ���� ����
	const DWORD CHAT_NORMAL = ENABLE;
	const DWORD CHAT_PRIVATE = PRIVATE;
	const DWORD CHAT_PARTY = AQUAMARINE;
	const DWORD CHAT_MASTER = OLIVE;
	const DWORD CHAT_GUILD = VIOLET;
	const DWORD CHAT_ALLIANCE = HOTPINK;
	const DWORD CHAT_AREA = D3DCOLOR_ARGB( 0xFF, 0x71, 0x78, 0xFF );
	const DWORD CHAT_PARTY_RECRUIT = D3DCOLOR_ARGB( 0xFF, 0x00, 0xC4, 0xA1 );
	const DWORD CHAT_TOALL = ORANGE;
	const DWORD CHAT_SYSTEM = LIGHTSKYBLUE;
	const DWORD CHAT_FACTION = CHAT_NORMAL;
    const DWORD CHAT_CHAR_NAME = D3DCOLOR_ARGB( 0xFF, 0x00, 0xA8, 0xFF );
	const DWORD	CHAT_GM = YELLOW;

	const DWORD	GM_CHAR_NAME = DODGERBLUE;
 
	const DWORD CHAT_LINK_PARTY_RECRUIT = D3DCOLOR_ARGB( 0xFF, 0x54, 0x8D, 0xD4 );

    const DWORD ACTIVITY_TITLE_LOW = ENABLE;
    const DWORD ACTIVITY_TITLE_MEDIUM = D3DCOLOR_ARGB( 0xFF, 0x22, 0xB1, 0x4C );
    const DWORD ACTIVITY_TITLE_HIGH = D3DCOLOR_ARGB( 0xFF, 0x35, 0x71, 0xFD );

    const DWORD ACTIVITY_TITLE_COLOR_SIZE = 3;
    const DWORD ACTIVITY_TITLE_COLOR[ACTIVITY_TITLE_COLOR_SIZE] = { ACTIVITY_TITLE_LOW, ACTIVITY_TITLE_MEDIUM, ACTIVITY_TITLE_HIGH };

    const DWORD ATTENDANCE_TASK_AWARD_ReceiveD   = D3DCOLOR_ARGB(0xFF,0x62,0xCB,0x62);
    const DWORD ATTENDANCE_TASK_PROGRESS_DISABLE = D3DCOLOR_ARGB(0xFF,0xFF,0x82,0x0A);
    const DWORD ATTENDANCE_DAYLIST_DEFAULT       = D3DCOLOR_ARGB(0xFF,0x40,0x54,0x5E);
    const DWORD ATTENDANCE_DAYLIST_COMPLETE      = D3DCOLOR_ARGB(0xFF,0x8A,0x90,0xC0);
    const DWORD ATTENDANCE_DAYLIST_TODAY         = D3DCOLOR_ARGB(0xFF,0xEA,0xED,0xFC);

	// ��ų����;
	const DWORD LINK_SKILL_TOOLTIP = D3DCOLOR_ARGB(0xFF,0xFF,0xC9,0x0E);
}

namespace	NS_UITEXTCONTROL
{
	using namespace NS_UITEXTUTIL;

	DWORD	GetEvaluateColor ( BOOL bValue );
    DWORD   GetActivityColor ( int nClass );
}

/*
778899 lightslategray ����Ǫ����ȸ�� 
696969 dimgray ĢĢ��ȸ�� 
808080 gray ȸ�� 
a9a9a9 darkgray ��ο�ȸ�� 
c0c0c0 silver ���� 
d3d3d3 lightgrey ����ȸ�� 
dcdcdc gainsboro 
ffffff white ��� 
fff5ee seashell �ٴ������� 
fffafa snow ���� 
f8f8ff ghostwhite ������� 
fffaf0 floralwhite ���ǹ�� 
f5f5f5 whitesmoke ������� 
f0f8ff aliceblue ��������������Ǫ���� 
f0ffff azure �ϴû� 
fdf5e6 oldlace ������ 
f5fffa mintcream ����ũ�� 
ffefd5 papayawhip ����Ƹ޸�ī��������� 
ffdab9 peachpuff �����ƺ��ұ� 
faf0e6 linen ���� 
eee8aa palegoldenrod ������ȭ���Ǵٳ��� 
ffe4e1 mistyrose £����̺� 
ffe4b5 moccasin �罿�����Ǳ��� 
ffdead navajowhite ����ȣ����� 
d2b48c tan ����ź���� 
f5deb3 wheat �� 
fafad2 lightgoldenrodyellow ������ȭ���ٳ��ʳ�� 
ffffe0 lightyellow ������� 
fff8dc cornsilk ���������ֽ� 
faebd7 antiquewhite �����ǹ�� 
f5f5dc beige �������� 
fffacd lemonchiffon ������������������� 
fffff0 ivory ��Ѻ� 
f0e68c khaki �������������ٻ��̼��κ��� 
e6e6fa lavender �������ֻ� 
fff0f5 lavenderblush �������ֺ��ӱ� 
ffe4c4 bisque ��ȫ���̵��°��������������� 
ffebcd blanchedalmond ���������Ȳ���� 
deb887 burlywood ưư�Ѹ��� 
cd853f peru ���̼����ǰ�ȭ�� 
00ced1 darkturquoise ��ο�Ǫ������� 
00bfff deepskyblue £���ϴ��Ķ� 
7fffd4 aquamarine ����Ǫ������� 
1e90ff dodgerblue���������Ķ� 
00ffff cyan Ǫ������ 
f0fff0 honeydew �Ĺ����ٿ������´ܹ� 
87cefa lightskyblue �����ϴ��Ķ� 
afeeee paleturquoise ����Ǫ������� 
e0ffff lightcyan ����Ǫ������ 
add8e6 lightblue �����Ķ� 
b0c4de lightsteelblue ����ö�����Ķ� 
40e0d0 turquoise Ǫ������� 
48d1cc mediumturquoise �߰���Ǫ������� 
00ffff aqua ����Ǫ������� 
7b68ee mediumslateblue ��Ǫ����ȸ���Ķ� 
191970 midnightblue �������Ķ� 
6495ed cornflowerblue ���������Ķ� 
0000cd mediumblue �߰����Ķ� 
6a5acd slateblue Ǫ����ȸ���Ķ� 
4682b4 steelblue ö�����Ķ� 
0000ff blue �Ķ� 
483d8b darkslateblue ��ο�Ǫ����ȸ���Ķ� 
5f9ea0 cadetblue �������Ķ� 
87ceeb skyblue �ϴ��Ķ� 
4169e1 royalblue Ȳ�����Ķ� 
b0e0e6 powderblue �����Ķ� 
000080 navy £���������������� 
00008b darkblue ��ο��Ķ� 
8a2be2 blueviolet �Ķ�����ɻ� 
8b008b darkmagenta ��ο�£����ȫ�� 
9932cc darkorchid ��ο����� 
9400d3 darkviolet ��ο�����ɻ� 
ff00ff magenta £����ȫ�� 
ff00ff fuchsia ���������̳������޺� 
c71585 mediumvioletred ������ɻ��� 
ba55d3 mediumorchid �߰��ǿ������ 
9370db mediumpurple �߰������޺� 
dc143c crimson £�Ժ������� 
ff1493 deeppink £������ȫ�� 
ffb6c1 lightpink ��������ȫ�� 
ff69b4 hotpink �����ѿ���ȫ�� 
ffc0cb pink ����ȫ�� 
dda0dd plum £�� ����� 
800080 purple ���޺� 
ee82ee violet ����ɻ� 
d8bfd8 thistle ������ 
da70d6 orchid ������� 
4b0082 indigo ���� 
a52a2a brown ���� 
e9967a darksalmon ��ο���Ȳ�� 
f08080 lightcoral ������ȣ�� 
cd5c5c indianred �ε�𻡰� 
ffa07a lightsalmon ������Ȳ�� 
db7093 palevioletred ��������ɻ��� 
f4a460 sandybrown �������� 
fa8072 salmon ��Ȳ�� 
ff6347 tomato �丶��� 
ff4500 ornagered 
ff0000 red ���� 
800000 maroon �������������� 
8b0000 darkred ��ο�� 
b22222 firebrick ��ȭ 
d2691e chocolate ���� 
8b4513 saddlebrown ���尥�� 
a0522d sienna �������������� 
bc8f8f rosybrown ��̺����� 
ff7f50 coral ��ȣ�� 
ff8c00 darkorange ��ο���������������� 
ffa500 orange ���������������� 
b8860b darkgoldenrod ��пȭ���Ǵٳ��� 
ffd700 gold �ݺ� 
ffff00 yellow ��� 
7fff00 chartreuse ���λ� 
7cfc00 lawngreen �ܵ��� 
00ff00 lime ������Ƿ������Ѱ��� 
32cd32 limegreen ���ӳ�� 
00ff7f springgreen ����� 
3cb371 mediumseagreen �߰��ǹٴٳ�� 
adff2f greenyellow ����ǳ�� 
8fbc8f darkseagreen ��ο�ٴٳ�� 
90ee90 lightgreen ������� 
98fb98 palegreen ������� 
9acd32 yello: wgree: n 
2e8b57 seagreen �ٴٳ�� 
00fa9a mediumspringgreen �ߺ���� 
20b2aa lightseagreen �����ٴٳ�� 
66cdaa mediumaquamarine �߿���û�ϻ� 
228b22 forestgreen ����� 
008b8b darkcyan ��ο�Ǫ������ 
008080 teal �Ϸϻ�����û�� 
006400 darkgreen ��ο��� 
556b2f darkolivegreen ��ο�ø����� 
008000 green ��� 
808000 olive ��Ǫ���������ǻ�ϱ��� 
6b8e23 olivedrab �ø��꿯������ 
bdb76b darkkhaki ��ο�����������ٻ� 
daa520 goldenrod ��ȭ���Ǵٳ��� 
*/







