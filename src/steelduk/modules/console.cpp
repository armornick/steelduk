#include <windows.h>

#include <duktape.h>
#include <duk-helpers.hpp>
#include <duk-objects.hpp>

#define SDUK_DEFAULT_READ_BUFFER 1024

/* ----------------------------------------------------------------------------------
Module function implementations
---------------------------------------------------------------------------------- */
static duk_ret_t console_write(duk_context *ctx)
{
	wchar_t *output = utf8_to_wstring(ctx, duk_is_string(ctx, 0) ? duk_get_string(ctx, 0) : duk_to_string(ctx, 0));
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	
	BOOL ret = WriteConsoleW(console, output, lstrlenW(output), NULL, NULL);
	if (ret == 0)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not read from console (%d)", GetLastError());
	}

	return 0;
}

static duk_ret_t console_read(duk_context *ctx)
{
	size_t bufsz = duk_is_null_or_undefined(ctx, 0) ? SDUK_DEFAULT_READ_BUFFER : duk_require_uint(ctx, 0);

	HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
	wchar_t *buffer = static_cast<wchar_t*>(duk_push_fixed_buffer(ctx, sizeof(wchar_t) * bufsz));

	DWORD read = 0;
	BOOL ret = ReadConsoleW(console, buffer, bufsz, &read, NULL);
	if (ret == 0)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not read from console (%d)", GetLastError());
	}

	wstring_to_utf8(ctx, buffer);
	return 1;
}

static duk_ret_t console_set_cursor_position(duk_context *ctx)
{
	COORD pos = {0};
	pos.X = static_cast<SHORT>(duk_require_int(ctx, 0));
	pos.Y = static_cast<SHORT>(duk_require_int(ctx, 1));

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(console, pos);
	return 0;
}

/* ----------------------------------------------------------------------------------
Console Object properties
---------------------------------------------------------------------------------- */
static duk_ret_t console_get_foreground_color(duk_context *ctx)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info = {0};

	BOOL ret = GetConsoleScreenBufferInfo(console, &info);
	if (ret == 0)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not get console attributes (%d)", GetLastError());
	}

	sduk_push(ctx, info.wAttributes & (FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY));
	return 1;
}

static duk_ret_t console_set_foreground_color(duk_context *ctx)
{
	WORD color = static_cast<WORD>(duk_require_uint(ctx, 0));

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info = {0};

	BOOL ret = GetConsoleScreenBufferInfo(console, &info);
	if (ret == 0)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not get console attributes (%d)", GetLastError());
	}

	WORD attrs = info.wAttributes & ~(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
	attrs |= color;

	SetConsoleTextAttribute(console, attrs);
	return 0;
}

static duk_ret_t console_get_background_color(duk_context *ctx)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info = {0};

	BOOL ret = GetConsoleScreenBufferInfo(console, &info);
	if (ret == 0)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not get console attributes (%d)", GetLastError());
	}

	sduk_push(ctx, info.wAttributes & (BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY));
	return 1;
}

static duk_ret_t console_set_background_color(duk_context *ctx)
{
	WORD color = static_cast<WORD>(duk_require_uint(ctx, 0));

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info = {0};

	BOOL ret = GetConsoleScreenBufferInfo(console, &info);
	if (ret == 0)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not get console attributes (%d)", GetLastError());
	}

	WORD attrs = info.wAttributes & ~(BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED|BACKGROUND_INTENSITY);
	attrs |= color;

	SetConsoleTextAttribute(console, attrs);
	return 0;
}

/* ----------------------------------------------------------------------------------
Module declaration
---------------------------------------------------------------------------------- */
static const duk_function_list_entry _module_functions[] = {
	{ "write", console_write, 1 },
	{ "read", console_read, 1 },
	{ "setCursorPosition", console_set_cursor_position, 2 },
	{ NULL, NULL, 0 }
};

duk_ret_t dukopen_console(duk_context *ctx)
{
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, _module_functions);
	sduk_put_getter_setter(ctx, -1, "foregroundColor", console_get_foreground_color, console_set_foreground_color);
	sduk_put_getter_setter(ctx, -1, "backgroundColor", console_get_background_color, console_set_background_color);
	return 1;
}
