#include <stdio.h>
#include <errno.h>

#include <duktape.h>
#include <duk-helpers.hpp>

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

static duk_ret_t append_file(duk_context *ctx)
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

	FILE *outputf = _wfopen(filename, L"ab+");
	if (outputf == NULL)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not open file: %s (%d)", strerror(errno), errno);
	}

	fwrite(buffer, sizeof(char), bufsz, outputf);
	fclose(outputf);
	return 0;
}

static duk_ret_t read_file(duk_context *ctx)
{
	const char *errmsg = NULL;
	wchar_t *filename; sduk_require(ctx, 0, filename);

	// open file for reading
	FILE *inputf = _wfopen(filename, L"rb");
	if (inputf == NULL)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not open file: %s (%d)", strerror(errno), errno);
	}

	// move file pointer to end of file
	if (fseek(inputf, 0, SEEK_END))
	{
		errmsg = "could not seek to end of file";
		goto error;
	}

	// get number of bytes in file
	long bytelen = ftell(inputf);
	if (bytelen == EOF)
	{
		errmsg = "could not get number of bytes in file";
		goto error;
	}

	// move file pointer back to beginning
	rewind(inputf);

	// read the file into a buffer
	void *buffer = duk_push_fixed_buffer(ctx, bytelen);
	fread(buffer, 1, bytelen, inputf);

	// close the file and return
	fclose(inputf);
	return 1;

error:
	if (inputf != NULL) { fclose(inputf); }
	duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "%s: %s (%d)", errmsg, strerror(errno), errno);
	return 0;
}

/* ----------------------------------------------------------------------------------
Module declaration
---------------------------------------------------------------------------------- */
static const duk_function_list_entry _module_functions[] = {
	{ "writeFile", write_file, 2 },
	{ "appendFile", append_file, 2 },
	{ "readFile", read_file, 1 },
	{ NULL, NULL, 0 }
};

duk_ret_t dukopen_io_file(duk_context *ctx)
{
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, _module_functions);
	return 1;
}