#pragma once

class Game
{
public:
	explicit Game();
	~Game() noexcept;

	void Initilize();

	static void Setup();
	static void Visual();
	static void Misc();

	float CalculateHeadCircleRadius(float Distance);
	std::string GetTrapType(ETrapType Type);

private:
	static inline int m_ScreenWidth = 0;
	static inline int m_ScreenHeight = 0;
	static inline ImVec4 m_Color{};
	static inline APlayerController* LocalPlayerController = nullptr;
	static inline APlayerCharacter* LocalCharacter = nullptr;
	static inline APlayerCameraManager* LocalPlayerCamera = nullptr;
	static inline UKismetSystemLibrary* UKSystemLib = nullptr;
	static inline UKismetMathLibrary* UKMathLib = nullptr;

private:
	typedef void(*tGetViewPoint)(ULocalPlayer*, FMinimalViewInfo*);
	tGetViewPoint GetViewPoint = nullptr;

	static void GetViewPointHook(ULocalPlayer* LocalPlayer, FMinimalViewInfo* OutViewInfo);
};
inline std::unique_ptr<Game> g_Game;