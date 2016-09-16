#include <windows.h>

#include <duktape.h>

/* ----------------------------------------------------------------------------------
Define Module Constants
---------------------------------------------------------------------------------- */
static const duk_number_list_entry _module_consts[] = {
	// foreground colors
	{ "FOREGROUND_BLUE", (double) FOREGROUND_BLUE },
	{ "FOREGROUND_GREEN", (double) FOREGROUND_GREEN },
	{ "FOREGROUND_RED", (double) FOREGROUND_RED },
	{ "FOREGROUND_INTENSITY", (double) FOREGROUND_INTENSITY },
	// foreground color shorthands
	{ "FG_BLACK", (double) 0 },
	{ "FG_BLUE", (double) FOREGROUND_BLUE },
	{ "FG_LIGHTBLUE", (double) (FOREGROUND_INTENSITY | FOREGROUND_BLUE) },
	{ "FG_MAGENTA", (double) (FOREGROUND_BLUE | FOREGROUND_RED) },
	{ "FG_LIGHTMAGENTA", (double) (FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED) },
	{ "FG_CYAN", (double) (FOREGROUND_BLUE | FOREGROUND_GREEN) },
	{ "FG_LIGHTCYAN", (double) (FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN) },
	{ "FG_GREEN", (double) FOREGROUND_GREEN },
	{ "FG_LIGHTGREEN", (double) (FOREGROUND_INTENSITY | FOREGROUND_GREEN) },
	{ "FG_BROWN", (double) (FOREGROUND_GREEN | FOREGROUND_RED) },
	{ "FG_YELLOW", (double) (FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED) },
	{ "FG_RED", (double) FOREGROUND_RED },
	{ "FG_LIGHTRED", (double) (FOREGROUND_INTENSITY | FOREGROUND_RED) },
	{ "FG_GRAY", (double) FOREGROUND_INTENSITY },
	{ "FG_LIGHTGRAY", (double) (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED) },
	{ "FG_WHITE", (double) (FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED) },
	// background colors
	{ "BACKGROUND_BLUE", (double) BACKGROUND_BLUE },
	{ "BACKGROUND_GREEN", (double) BACKGROUND_GREEN },
	{ "BACKGROUND_RED", (double) BACKGROUND_RED },
	{ "BACKGROUND_INTENSITY", (double) BACKGROUND_INTENSITY },
	// background color shorthands
	{ "BG_BLACK", (double) 0 },
	{ "BG_BLUE", (double) BACKGROUND_BLUE },
	{ "BG_LIGHTBLUE", (double) (BACKGROUND_INTENSITY | BACKGROUND_BLUE) },
	{ "BG_MAGENTA", (double) (BACKGROUND_BLUE | BACKGROUND_RED) },
	{ "BG_LIGHTMAGENTA", (double) (BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED) },
	{ "BG_CYAN", (double) (BACKGROUND_BLUE | BACKGROUND_GREEN) },
	{ "BG_LIGHTCYAN", (double) (BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN) },
	{ "BG_GREEN", (double) (BACKGROUND_GREEN) },
	{ "BG_LIGHTGREEN", (double) (BACKGROUND_INTENSITY | BACKGROUND_GREEN) },
	{ "BG_BROWN", (double) (BACKGROUND_GREEN | BACKGROUND_RED) },
	{ "BG_YELLOW", (double) (BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_RED) },
	{ "BG_RED", (double) BACKGROUND_RED },
	{ "BG_LIGHTRED", (double) (BACKGROUND_INTENSITY | BACKGROUND_RED) },
	{ "BG_GRAY", (double) BACKGROUND_INTENSITY },
	{ "BG_LIGHTGRAY", (double) (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED) },
	{ "BG_WHITE", (double) (BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED) },
	{ NULL, 0.0 }
};

/* ----------------------------------------------------------------------------------
Module function implementations
---------------------------------------------------------------------------------- */
static duk_ret_t consolecolor_string(duk_context *ctx)
{
	WORD color = duk_require_uint(ctx, 0);

	unsigned i = 0;
	duk_push_array(ctx);

	if (color & FOREGROUND_BLUE)
	{
		duk_push_string(ctx, "FOREGROUND_BLUE");
		duk_put_prop_index(ctx, -2, i++);
	}
	if (color & FOREGROUND_GREEN)
	{
		duk_push_string(ctx, "FOREGROUND_GREEN");
		duk_put_prop_index(ctx, -2, i++);
	}
	if (color & FOREGROUND_RED)
	{
		duk_push_string(ctx, "FOREGROUND_RED");
		duk_put_prop_index(ctx, -2, i++);
	}
	if (color & FOREGROUND_INTENSITY)
	{
		duk_push_string(ctx, "FOREGROUND_INTENSITY");
		duk_put_prop_index(ctx, -2, i++);
	}
	if (color & BACKGROUND_BLUE)
	{
		duk_push_string(ctx, "BACKGROUND_BLUE");
		duk_put_prop_index(ctx, -2, i++);
	}
	if (color & BACKGROUND_GREEN)
	{
		duk_push_string(ctx, "BACKGROUND_GREEN");
		duk_put_prop_index(ctx, -2, i++);
	}
	if (color & BACKGROUND_RED)
	{
		duk_push_string(ctx, "BACKGROUND_RED");
		duk_put_prop_index(ctx, -2, i++);
	}
	if (color & BACKGROUND_INTENSITY)
	{
		duk_push_string(ctx, "BACKGROUND_INTENSITY");
		duk_put_prop_index(ctx, -2, i++);
	}

	return 1;
}

/* ----------------------------------------------------------------------------------
Module declaration
---------------------------------------------------------------------------------- */
static const duk_function_list_entry _module_functions[] = {
	{ "getString", consolecolor_string, 1 },
	{ NULL, NULL, 0 }
};

duk_ret_t dukopen_console_color(duk_context *ctx)
{
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, _module_consts);
	duk_put_function_list(ctx, -1, _module_functions);
	return 1;
}
