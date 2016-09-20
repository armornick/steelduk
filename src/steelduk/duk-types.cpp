#include "duk-helpers.hpp"

/* ----------------------------------------------------------------------------------
Duktape type helpers: get value of type (without error)
---------------------------------------------------------------------------------- */
bool sduk_get(duk_context *ctx, duk_idx_t index, bool& v)
{
	bool result = false;

	if (duk_is_boolean(ctx, index))
	{
		result = true;
		v = duk_get_boolean(ctx, index) != 0;
	}

	return result;
}

bool sduk_get(duk_context *ctx, duk_idx_t index, duk_int_t& v)
{
	bool result = false;

	if (duk_is_number(ctx, index))
	{
		result = true;
		v = duk_get_int(ctx, index);
	}

	return result;
}

bool sduk_get(duk_context *ctx, duk_idx_t index, long& v)
{
	bool result = false;

	if (duk_is_number(ctx, index))
	{
		result = true;
		v = static_cast<long>(duk_get_int(ctx, index));
	}

	return result;
}

bool sduk_get(duk_context *ctx, duk_idx_t index, duk_double_t& v)
{
	bool result = false;

	if (duk_is_number(ctx, index))
	{
		result = true;
		v = duk_get_number(ctx, index);
	}

	return result;
}

bool sduk_get(duk_context *ctx, duk_idx_t index, const char*& v)
{
	bool result = false;

	if (duk_is_string(ctx, index))
	{
		result = true;
		v = duk_get_string(ctx, index);
	}

	return result;
}

bool sduk_get(duk_context *ctx, duk_idx_t index, wchar_t*& v)
{
	bool result = false;

	if (duk_is_string(ctx, index))
	{
		result = true;
		const char *tmp = duk_get_string(ctx, index);
		v = utf8_to_wstring(ctx, tmp);
	}

	return result;
}

bool sduk_get(duk_context *ctx, duk_idx_t index, duk_uint_t& v)
{
	bool result = false;

	if (duk_is_number(ctx, index))
	{
		result = true;
		v = duk_get_uint(ctx, index);
	}

	return result;
}


/* ----------------------------------------------------------------------------------
Duktape type helpers: get value of type (with error)
---------------------------------------------------------------------------------- */
void sduk_require(duk_context *ctx, duk_idx_t index, bool& v)
{
	v = duk_require_boolean(ctx, index) != 0;
}

void sduk_require(duk_context *ctx, duk_idx_t index, duk_int_t& v)
{
	v = duk_require_int(ctx, index);
}

void sduk_require(duk_context *ctx, duk_idx_t index, long& v)
{
	v = static_cast<long>(duk_require_int(ctx, index));
}

void sduk_require(duk_context *ctx, duk_idx_t index, duk_double_t& v)
{
	v = duk_require_number(ctx, index);
}

void sduk_require(duk_context *ctx, duk_idx_t index, const char*& v)
{
	v = duk_require_string(ctx, index);
}

void sduk_require(duk_context *ctx, duk_idx_t index, wchar_t*& v)
{
	const char *tmp = duk_require_string(ctx, index);
	v = utf8_to_wstring(ctx, tmp);
}

void sduk_require(duk_context *ctx, duk_idx_t index, duk_uint_t& v)
{
	v = duk_require_uint(ctx, index);
}


/* ----------------------------------------------------------------------------------
Duktape type helpers: push value to stack
---------------------------------------------------------------------------------- */
void sduk_push(duk_context *ctx, bool v)
{
	duk_push_boolean(ctx, v);
}

void sduk_push(duk_context *ctx, duk_int_t v)
{
	duk_push_int(ctx, v);
}

void sduk_push(duk_context *ctx, long v)
{
	duk_push_int(ctx, static_cast<duk_int_t>(v));
}

void sduk_push(duk_context *ctx, duk_double_t v)
{
	duk_push_number(ctx, v);
}

void sduk_push(duk_context *ctx, const char* v)
{
	duk_push_string(ctx, v);
}

void sduk_push(duk_context *ctx, wchar_t* v)
{
	wstring_to_utf8(ctx, v);
}

void sduk_push(duk_context *ctx, duk_uint_t v)
{
	duk_push_uint(ctx, v);
}
