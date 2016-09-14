#include <windows.h>

#include <duktape.h>
#include "duk-types.hpp"

/* ----------------------------------------------------------------------------------
Module function implementations
---------------------------------------------------------------------------------- */
static duk_ret_t get_acp(duk_context *ctx)
{
	UINT acp = GetACP();
	sduk_push(ctx, acp);
	return 1;
}

static duk_ret_t encode_utf16(duk_context *ctx)
{
	const char *input = duk_require_string(ctx, 0);
	utf8_to_wstring(ctx, input);
	return 1;
}

/* ----------------------------------------------------------------------------------
Module declaration
---------------------------------------------------------------------------------- */
static const duk_function_list_entry _module_functions[] = {
	{ "getActiveCodePage", get_acp, 0 },
	{ "encodeUtf16", encode_utf16, 1 },
	{ NULL, NULL, 0 }
};

duk_ret_t dukopen_encodings(duk_context *ctx)
{
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, _module_functions);
	return 1;
}