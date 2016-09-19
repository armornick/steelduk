#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <duktape.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <io.h>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

// -----------------------------------------------------------------------------

#define STEELDUK_PRELOAD_TABLE "\xFF""preload"

static duk_ret_t try_preload_module(duk_context *ctx)
{
	const char *module = duk_require_string(ctx, 0);
	if (!duk_get_global_string(ctx, STEELDUK_PRELOAD_TABLE))
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "preload object is not available: not found");
	}
	if (!duk_is_object(ctx, -1))
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "preload object is not available: not an object");
	}
	if (duk_get_prop_string(ctx, -1, module) && duk_is_callable(ctx, -1))
	{
		duk_call(ctx, 0);
		duk_remove(ctx, -2); // pop preload table
		duk_remove(ctx, -2); // pop package object
		return 1;
	}
	else
	{
		duk_pop_3(ctx);
		duk_push_false(ctx);
		return 1;
	}
}

// -----------------------------------------------------------------------------

static void file_to_string(duk_context *ctx, const char *filename)
{
	FILE *inputf = fopen(filename, "rb");
	if (inputf == NULL)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not open module file '%s'", filename);
	}

	fseek(inputf, 0, SEEK_END);
	long fsize = ftell(inputf);
	fseek(inputf, 0, SEEK_SET);

	char *buff = static_cast<char*>(duk_push_fixed_buffer(ctx, sizeof(char) * fsize));
	fread(buff, fsize, sizeof(char), inputf);
	fclose(inputf);
	duk_to_string(ctx, -1);
}

static duk_ret_t try_load_file(duk_context *ctx)
{
	const char *module = duk_require_string(ctx, 0);
	// first step: try loading raw module name as file
	errno_t ret = _access_s(module, 04);
	if (ret == 0)
	{
		file_to_string(ctx, module);
		return 1;
	}
	else if (ret != ENOENT)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not check module file '%s': %s (%d)", module, strerror(ret), ret);
	}
	// second step: try adding '.js' extension
	char filepath[MAX_PATH];
	ret = _makepath_s(filepath, MAX_PATH, NULL, NULL, module, ".js");
	if (ret != 0)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not build possible file path for module '%s': %s (%d)", module, strerror(ret), ret);
	}
	ret = _access_s(filepath, 04);
	if (ret == 0)
	{
		file_to_string(ctx, filepath);
		return 1;
	}
	else if (ret != ENOENT)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not check module file '%s': %s (%d)", filepath, strerror(ret), ret);
	}
	duk_push_null(ctx);
	return 1;
}

// -----------------------------------------------------------------------------

// Duktape.modSearch = function (id, require, exports, module)
static duk_ret_t sduk_modsearch(duk_context *ctx)
{
	// failsafe: exit on empty string
	const char *module = duk_require_string(ctx, 0);
	if (module == NULL || *module == '\0')
	{
		duk_error(ctx, DUK_ERR_ERROR, "invalid module name");
		return 0;
	}
	// first step: try package.preload
	duk_push_c_function(ctx, try_preload_module, 2);
	duk_dup(ctx, 0);
	duk_dup(ctx, 2);
	duk_call(ctx, 2);
	if (!duk_is_boolean(ctx, -1))
	{
		duk_put_prop_string(ctx, 3, "exports");
		duk_push_undefined(ctx);
		return 1;
	}
	// next step: try loading file
	duk_push_c_function(ctx, try_load_file, 1);
	duk_dup(ctx, 0);
	duk_call(ctx, 1);
	if (duk_is_string(ctx, -1))
	{
		return 1;
	}
	// final step: throw error if not found
	duk_error(ctx, DUK_ERR_ERROR, "module '%s' not found", duk_require_string(ctx, 0));
	return 0;
}

void sduk_setup_modsearch(duk_context *ctx)
{
	// setup preload table
	duk_push_object(ctx);
	duk_put_global_string(ctx, STEELDUK_PRELOAD_TABLE);
	// define Duktape.modSearch
	duk_get_global_string(ctx, "Duktape");
	duk_push_c_function(ctx, sduk_modsearch, 4);
	duk_put_prop_string(ctx, -2, "modSearch");
	duk_pop(ctx);
}