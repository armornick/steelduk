#include "duk-types.hpp"

/* ----------------------------------------------------------------------------------
Duktape string conversion helpers
---------------------------------------------------------------------------------- */
const char *wstring_to_utf8(duk_context *ctx, wchar_t *inputs)
{
	int needed = WideCharToMultiByte(CP_UTF8, 0, inputs, -1, NULL, 0, NULL, NULL);
	char *buff = static_cast<char*>(duk_push_dynamic_buffer(ctx, sizeof(char) * needed));
	WideCharToMultiByte(CP_UTF8, 0, inputs, -1, buff, needed, NULL, NULL);
	duk_resize_buffer(ctx, -1, lstrlenA(buff));
	duk_to_string(ctx, -1);
	return duk_require_string(ctx, -1);
}

wchar_t *utf8_to_wstring(duk_context *ctx, const char *inputs)
{
	int needed = MultiByteToWideChar(CP_UTF8, 0, inputs, -1, NULL, 0);
	wchar_t *buff = static_cast<wchar_t*>(duk_push_fixed_buffer(ctx, sizeof(wchar_t) * needed));
	MultiByteToWideChar(CP_UTF8, 0, inputs, -1, buff, needed);
	return buff;
}

const char *wstring_to_ansi(duk_context *ctx, wchar_t *inputs, UINT codepage)
{
	int needed = WideCharToMultiByte(codepage, 0, inputs, -1, NULL, 0, NULL, NULL);
	char *buff = static_cast<char*>(duk_push_dynamic_buffer(ctx, sizeof(char) * needed));
	WideCharToMultiByte(codepage, 0, inputs, -1, buff, needed, NULL, NULL);
	duk_resize_buffer(ctx, -1, lstrlenA(buff));
	return buff;
}

wchar_t *ansi_to_wstring(duk_context *ctx, const char *inputs, UINT codepage)
{
	int needed = MultiByteToWideChar(codepage, 0, inputs, -1, NULL, 0);
	wchar_t *buff = static_cast<wchar_t*>(duk_push_fixed_buffer(ctx, sizeof(wchar_t) * needed));
	MultiByteToWideChar(codepage, 0, inputs, -1, buff, needed);
	return buff;
}