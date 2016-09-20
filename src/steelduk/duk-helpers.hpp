#ifndef _DUK_HELPERS_HPP_
#define _DUK_HELPERS_HPP_

#include <duktape.h>
#include <windows.h>

/* ----------------------------------------------------------------------------------
Duktape string conversion helpers
---------------------------------------------------------------------------------- */
const char *wstring_to_utf8(duk_context *ctx, wchar_t *inputs);
wchar_t *utf8_to_wstring(duk_context *ctx, const char *inputs);
const char *wstring_to_ansi(duk_context *ctx, wchar_t *inputs, UINT codepage = CP_ACP);
wchar_t *ansi_to_wstring(duk_context *ctx, const char *inputs, UINT codepage = CP_ACP);

/* ----------------------------------------------------------------------------------
Duktape type helpers
---------------------------------------------------------------------------------- */
/* get value of type (without error) */
bool sduk_get(duk_context *ctx, duk_idx_t index, bool& v);
bool sduk_get(duk_context *ctx, duk_idx_t index, duk_int_t& v);
bool sduk_get(duk_context *ctx, duk_idx_t index, long& v);
bool sduk_get(duk_context *ctx, duk_idx_t index, duk_double_t& v);
bool sduk_get(duk_context *ctx, duk_idx_t index, const char*& v);
bool sduk_get(duk_context *ctx, duk_idx_t index, wchar_t*& v);
bool sduk_get(duk_context *ctx, duk_idx_t index, duk_uint_t& v);
/* get value of type (with error) */
void sduk_require(duk_context *ctx, duk_idx_t index, bool& v);
void sduk_require(duk_context *ctx, duk_idx_t index, duk_int_t& v);
void sduk_require(duk_context *ctx, duk_idx_t index, long& v);
void sduk_require(duk_context *ctx, duk_idx_t index, duk_double_t& v);
void sduk_require(duk_context *ctx, duk_idx_t index, const char*& v);
void sduk_require(duk_context *ctx, duk_idx_t index, wchar_t*& v);
void sduk_require(duk_context *ctx, duk_idx_t index, duk_uint_t& v);
/* push value to stack */
void sduk_push(duk_context *ctx, bool v);
void sduk_push(duk_context *ctx, duk_int_t v);
void sduk_push(duk_context *ctx, long v);
void sduk_push(duk_context *ctx, duk_double_t v);
void sduk_push(duk_context *ctx, const char* v);
void sduk_push(duk_context *ctx, wchar_t* v);
void sduk_push(duk_context *ctx, duk_uint_t v);

/* ----------------------------------------------------------------------------------
Duktape Object property getter
---------------------------------------------------------------------------------- */
template <typename T>
bool sduk_get_prop(duk_context *ctx, duk_idx_t index, const char *prop, T& value)
{
	bool result = false;

	if (duk_get_prop_string(ctx, index, prop))
	{
		result = sduk_get(ctx, -1, value);
	}
	duk_pop(ctx);

	return result;
}

/* ----------------------------------------------------------------------------------
Duktape Object property setter
---------------------------------------------------------------------------------- */
template <typename T>
void sduk_put_prop(duk_context *ctx, duk_idx_t index, const char *prop, T& value)
{
	index = duk_normalize_index(ctx, index);
	sduk_push(ctx, value);
	duk_put_prop_string(ctx, index, prop);
}


#endif // _DUK_HELPERS_HPP_