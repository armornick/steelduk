#include <stdio.h>
#include <errno.h>

#include <duktape.h>
#include <duk-types.hpp>

/* ----------------------------------------------------------------------------------
Module function implementations
---------------------------------------------------------------------------------- */
static duk_ret_t write_file(duk_context *ctx)
{
	wchar_t *filename; sduk_require(ctx, 0, filename);
	char *buffer = NULL; size_t bufsz = 0;

	if (duk_is_string(ctx, 1))
	{
		buffer = const_cast<char*>(duk_require_lstring(ctx, 1, &bufsz));
	}
	else
	{
		buffer = static_cast<char*>(duk_require_buffer_data(ctx, 1, &bufsz));
	}

	FILE *outputf = _wfopen(filename, L"wb+");
	if (outputf == NULL)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not open file: %s (%d)", strerror(errno), errno);
	}

	fwrite(buffer, sizeof(char), bufsz, outputf);
	fclose(outputf);
	return 0;
}

/* ----------------------------------------------------------------------------------
Module declaration
---------------------------------------------------------------------------------- */
static const duk_function_list_entry _module_functions[] = {
	{ "writeFile", write_file, 2 },
	{ NULL, NULL, 0 }
};

duk_ret_t dukopen_io_file(duk_context *ctx)
{
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, _module_functions);
	return 1;
}