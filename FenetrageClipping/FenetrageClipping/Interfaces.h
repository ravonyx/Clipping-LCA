#pragma once
struct s_display_manager {
	bool begin_window = false;
	bool begin_poly = false;
	bool finish_window = false;
	bool finish_poly = false;
	bool finish_fenetrage = false;
	bool move_window = false;
	bool finish_circle = false;
	bool begin_circle = false;
};

// Menu items
enum MENU_TYPE
{
	MENU_DRAW_POLYGON,
	MENU_DRAW_WINDOW,
	MENU_MOVE,
	MENU_COLORS_POLY,
	MENU_COLORS_WINDOW,
	MENU_REMPLISSAGE,
	MENU_DRAW_CIRCLE,
};

enum COLOR_TYPE
{
	COLOR_RED,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_VIOLET,
	COLOR_ORANGE,
};