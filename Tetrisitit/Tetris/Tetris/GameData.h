#ifndef  GAMEDATA_HEADER
#define GAMEDATA_HEADER

/////////////// Konstanta/Viktiga variabler Xd
namespace gameData 
{
	constexpr int SCREEN_HEIGHT{ 40 };
	constexpr int SCREEN_WIDTH{ 40 };

	constexpr int TEXT_HEIGHT{ 20 };
	constexpr int TEXT_WIDTH{ 20 };

	constexpr int BOARD_WIDTH{ 12 };
	constexpr int BOARD_HEIGHT{ 24 };
	
	constexpr int SPAWN_Y{ 9 };

	constexpr int BoardMiddleX()
	{
		return (SCREEN_WIDTH / 2);
	}

	constexpr int BoardMiddleY()
	{
		return (SCREEN_HEIGHT / 2);
	}

	constexpr wchar_t BOX{ L'\u2588' };

	enum Color
	{
		Black,
		DarkBlue,
		DarkGreen,
		DarkAqua,
		DarkRed,
		DarkPurple,
		Gold,
		DarkWhite,
		DarkGray,
		Blue,
		Green,
		Aqua,
		Red,
		Purple,
		Yellow,
		White
	};
}

#endif 

