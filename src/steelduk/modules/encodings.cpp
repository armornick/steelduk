#include <windows.h>

#include <duktape.h>
#include <duk-types.hpp>

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

static duk_ret_t decode_utf16(duk_context *ctx)
{
	wchar_t *input = static_cast<wchar_t*>(duk_get_buffer_data(ctx, 0, NULL));
	wstring_to_utf8(ctx, input);
	return 1;
}

static duk_ret_t encode_ansi(duk_context *ctx)
{
	wchar_t *input; sduk_require(ctx, 0, input);
	UINT codepage = !duk_is_null_or_undefined(ctx, 1) ? duk_require_uint(ctx, 1) : CP_ACP;

	wstring_to_ansi(ctx, input, codepage);
	return 1;
}

/* ----------------------------------------------------------------------------------
Module declaration
---------------------------------------------------------------------------------- */
static const duk_function_list_entry _module_functions[] = {
	{ "getActiveCodePage", get_acp, 0 },
	{ "encodeUtf16", encode_utf16, 1 },
	{ "decodeUtf16", decode_utf16, 1 },
	{ "encodeAnsi", encode_ansi, 2 },
	{ NULL, NULL, 0 }
};

duk_ret_t dukopen_encodings(duk_context *ctx)
{
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, _module_functions);
	return 1;
}


/* ----------------------------------------------------------------------------------
Register CodePage identifiers
---------------------------------------------------------------------------------- */
/*
#define CODE_PAGE_LIST_PROP "\xFF" "codePageList"

static const duk_number_list_entry code_page_list[] = {
	{ "IBM037", (double) 037 },
	{ "IBM437", (double) 437 },
	{ "IBM500", (double) 500 },
	{ "ASMO-708", (double) 708 },
	{ "DOS-720", (double) 720 },
	{ "ibm737", (double) 737 },
	{ "ibm775", (double) 775 },
	{ "ibm850", (double) 850 },
	{ "ibm852", (double) 852 },
	{ "IBM855", (double) 855 },
	{ "ibm857", (double) 857 },
	{ "IBM00858", (double) 858 },
	{ "IBM860", (double) 860 },
	{ "ibm861", (double) 861 },
	{ "DOS-862", (double) 862 },
	{ "IBM863", (double) 863 },
	{ "IBM864", (double) 864 },
	{ "IBM865", (double) 865 },
	{ "cp866", (double) 866 },
	{ "ibm869", (double) 869 },
	{ "IBM870", (double) 870 },
	{ "windows-874", (double) 874 },
	{ "cp875", (double) 875 },
	{ "shift_jis", (double) 932 },
	{ "gb2312", (double) 936 },
	{ "ks_c_5601-1987", (double) 949 },
	{ "big5", (double) 950 },
	{ "IBM1026", (double) 1026 },
	{ "IBM01047", (double) 1047 },
	{ "IBM01140", (double) 1140 },
	{ "IBM01141", (double) 1141 },
	{ "IBM01142", (double) 1142 },
	{ "IBM01143", (double) 1143 },
	{ "IBM01144", (double) 1144 },
	{ "IBM01145", (double) 1145 },
	{ "IBM01146", (double) 1146 },
	{ "IBM01147", (double) 1147 },
	{ "IBM01148", (double) 1148 },
	{ "IBM01149", (double) 1149 },
	{ "windows-1250", (double) 1250 },
	{ "windows-1251", (double) 1251 },
	{ "windows-1252", (double) 1252 },
	{ "windows-1253", (double) 1253 },
	{ "windows-1254", (double) 1254 },
	{ "windows-1255", (double) 1255 },
	{ "windows-1256", (double) 1256 },
	{ "windows-1257", (double) 1257 },
	{ "windows-1258", (double) 1258 },
	{ "Johab", (double) 1361 },
	{ NULL, 0.0 }
}

static void register_codepage_table(duk_context *ctx)
{
	duk_push_object(ctx);
	duk_put_number_list(ctx, -1, code_page_list);
	duk_put_global_string(ctx, CODE_PAGE_LIST_PROP);
}
*/