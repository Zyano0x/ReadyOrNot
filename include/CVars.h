#pragma once

#define SETTINGS_FILE_NAME "rc_config.ini"
#define STREAM(s) (((std::stringstream&)(std::stringstream() << s)).str())
#define LIMIT_VALUE(Val,Min,Max) \
		if( (Val) > (Max) ) (Val) = (Max); \
		else if( (Val) < (Min) ) (Val) = (Min);
#define VEC4CPY(i,v) (v.x = i.x, v.y = i.y, v.z = i.z, v.w = i.w)

typedef enum
{
	SETTING_BOOL,
	SETTING_INT,
	SETTING_FLOAT,
	SETTING_VEC4,
	SETTING_STRING,
	SETTING_VEC2,
	SETTING_NONE
} SettingType_t;

typedef enum
{
	// AIMBOT	
	AIM_ENABLED,
	AIM_KEY,
	AIM_MODE,
	AIM_SMOOTH,
	AIM_BONE,
	FOV,
	FOV_COLOR,
	FOV_RADIUS,
	VISIBLE_CHECK,
	IGNORE_SURRENDERED,
	BULLET_TP,

	// ESP	
	ESP_ENABLED,
	ESP_CIVILIAN,
	ESP_CIVILIAN_COLOR,
	ESP_ENEMY,
	ESP_ENEMY_COLOR,
	ESP_VISIBLE_COLOR,
	ESP_FRIENDLY,
	ESP_FRIENDLY_COLOR,
	ESP_NAME,
	ESP_SKELETON,
	ESP_DISTANCE,
	ESP_BOX,
	ESP_SNAP_LINES,
	ESP_HEALTH,
	ESP_WEAPON,
	ESP_EVIDENCE,
	ESP_EVIDENCE_COLOR,
	ESP_TRAP,
	ESP_TRAP_COLOR,

	// MISC	
	INFINITE_AMMO,
	NO_RECOIL,
	NO_CLIP,
	FLY,
	FLY_MULTIPLIER,
	FOV_CHANGER,
	FOV_AMOUNT,
	SPEED,
	SPEED_MULTIPLIER,
	BREAK_DOOR,
	DISARM_TRAP,

	//Menu
	WINDOW_BG,
	CHILD_BG,
	TEXT,
	TEXT_HOVERED,
	TEXT_ACTIVE,
	FRAME_BG,
	FRAME_BG_HOVERED,
	FRAME_BG_ACTIVE,

	CONFIGS_

} Configs_t;

typedef struct V
{
	int iValue;
	int	iMin;
	int	iMax;

	float fValue;
	float fMin;
	float fMax;

	bool bValue;

	ImVec4 v4Value;
	ImVec2 v2Value;

	std::string	szValue;

	~V() {}

	V()
	{
		iValue = 0;
		iMin = 0;
		iMax = 0;

		fValue = 0.0f;
		fMin = 0.0f;
		fMax = 0.0f;

		bValue = false;

		v4Value = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
		v2Value = ImVec2(0.0f, 0.0f);
	}
} Value_t;

class Config
{
public:
	SettingType_t eType;
	Value_t Value;

	std::string	pszAppName;
	std::string	pszKeyName;
	std::string	szDefault;

	Config() = default;

	Config(SettingType_t eType, std::string pszAppName, std::string pszKeyName, std::string szDefault, Value_t val)
	{
		this->eType = eType;

		this->pszAppName = pszAppName;
		this->pszKeyName = pszKeyName;
		this->szDefault = szDefault;

		this->Value.iValue = val.iValue;
		this->Value.iMin = val.iMin;
		this->Value.iMax = val.iMax;
		this->Value.fValue = val.fValue;
		this->Value.fMin = val.fMin;
		this->Value.fMax = val.fMax;
		this->Value.bValue = val.bValue;
		this->Value.v4Value = ImVec4(val.v4Value.x, val.v4Value.y, val.v4Value.z, val.v4Value.w);
		this->Value.v2Value = ImVec2(val.v2Value.x, val.v2Value.y);
		this->Value.szValue = val.szValue;
	}
};

//========================================================================

extern Config Settings[CONFIGS_];
extern char	  szSettingsFile[MAX_PATH];
extern char	  szDllDirectory[MAX_PATH];

void BotInitSettings();
bool BotLoadSettings();
bool BotSaveSettings();