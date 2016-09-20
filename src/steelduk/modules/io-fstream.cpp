#include <stdio.h>
#include <errno.h>

#include <duktape.h>
#include <duk-helpers.hpp>
#include <duk-objects.hpp>

#define SDUK_FILESTREAM_PROTOTYPE "\xFF""steelduk::FileStream"

/* ----------------------------------------------------------------------------------
FileStream method implementations
---------------------------------------------------------------------------------- */
// helper function to clean the file handle if it is valid
static void clean_filestream(FILE **handle)
{
	FILE *file = *handle;

	if (file != NULL)
	{
#ifdef SDUK_VERBOSE
		printf("finalizing FILE %p\n", file);
#endif
		fclose(file);
		*handle = NULL;
	}
}

static FILE *check_valid_file(duk_context *ctx)
{
	FILE **handle = sduk_object<FILE*>::get_handle_from_this(ctx);
	if (handle == NULL || *handle == NULL)
	{
		duk_error(ctx, DUK_ERR_TYPE_ERROR, "invalid file handle");
	}
	return *handle;
}

// finalizer for garbage collector
static duk_ret_t filestream_finalizer(duk_context *ctx)
{
	FILE **handle = sduk_object<FILE*>::get_handle(ctx, 0);

	if (handle != NULL)
	{
		clean_filestream(handle);
	}

	return 0;
}

// close method for fine-grained lifetime management
static duk_ret_t filestream_close(duk_context *ctx)
{
	FILE **handle = sduk_object<FILE*>::get_handle_from_this(ctx);

	if (handle != NULL)
	{
		clean_filestream(handle);
	}

	return 0;
}

// overload for the toString method
static duk_ret_t filestream_to_string(duk_context *ctx)
{
	const char *path = NULL;
	
	duk_push_this(ctx);
	sduk_get_prop(ctx, -1, "path", path);
	duk_pop(ctx);

	duk_push_sprintf(ctx, "FileStream<%s>", path != NULL ? path : "???");
	return 1;
}

static duk_ret_t filestream_read(duk_context *ctx)
{
	FILE *file = check_valid_file(ctx);
	void *buffer = NULL; size_t bufsz;

	if (duk_is_number(ctx, 0))
	{
		bufsz = duk_require_uint(ctx, 0);
		buffer = duk_push_dynamic_buffer(ctx, bufsz);
	}
	else
	{
		buffer = duk_require_buffer_data(ctx, 0, &bufsz);
	}

	size_t read = fread(buffer, 1, bufsz, file);
	
	if (ferror(file))
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "error while reading from file");
	}

	if (read < bufsz && duk_is_dynamic_buffer(ctx, -1))
	{
		duk_resize_buffer(ctx, -1, read);
	}

	return 1;
}

static duk_ret_t filestream_write(duk_context *ctx)
{
	FILE *file = check_valid_file(ctx);
	void *buffer = NULL; size_t bufsz;

	if (duk_is_string(ctx, 0))
	{
		buffer = static_cast<void*>(const_cast<char*>(duk_require_lstring(ctx, 0, &bufsz))); // use C-style cast here?
	}
	else
	{
		buffer = duk_require_buffer_data(ctx, 0, &bufsz);
	}

	size_t written = fwrite(buffer, 1, bufsz, file);

	if (ferror(file))
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "error while reading from file");
	}

	duk_push_uint(ctx, written);
	return 1;
}

static duk_ret_t filestream_flush(duk_context *ctx)
{
	FILE *file = check_valid_file(ctx);
	int ret = fflush(file);
	sduk_push(ctx, ret == 0);
	return 1;
}

static duk_ret_t filestream_rewind(duk_context *ctx)
{
	FILE *file = check_valid_file(ctx);
	rewind(file);
	return 0;
}

/* ----------------------------------------------------------------------------------
FileStream prototype declaration
---------------------------------------------------------------------------------- */
static const duk_function_list_entry _prototype[] = {
	{ "close", filestream_close, 0 },
	{ "toString", filestream_to_string, 0 },
	{ "read", filestream_read, 1 },
	{ "write", filestream_write, 1 },
	{ "flush", filestream_flush, 0 },
	{ "rewind", filestream_rewind, 0 },
	{ NULL, NULL, 0 }
};

static void register_filestream_prototype(duk_context *ctx)
{
	duk_push_object(ctx);
	duk_put_function_list(ctx, -1, _prototype);
	duk_push_c_function(ctx, filestream_finalizer, 1);
	duk_set_finalizer(ctx, -2);
	duk_put_global_string(ctx, SDUK_FILESTREAM_PROTOTYPE);
}

/* ----------------------------------------------------------------------------------
FileStream object constructor
---------------------------------------------------------------------------------- */
static duk_ret_t filestream_constructor(duk_context *ctx)
{
	wchar_t *filename; sduk_require(ctx, 0, filename);
	wchar_t *mode; sduk_require(ctx, 1, mode);

	FILE *file = _wfopen(filename, mode);
	if (file == NULL)
	{
		duk_error(ctx, DUK_ERR_INTERNAL_ERROR, "could not open file: %s (%d)", strerror(errno), errno);
	}

	FILE **handle = sduk_object<FILE*>::push_handle(ctx, SDUK_FILESTREAM_PROTOTYPE);
	*handle = file;

	duk_dup(ctx, 0);
	duk_put_prop_string(ctx, -2, "path");

	return 1;
}

/* ----------------------------------------------------------------------------------
Module declaration
---------------------------------------------------------------------------------- */
duk_ret_t dukopen_io_fstream(duk_context *ctx)
{
	// register the FileStream prototype
	register_filestream_prototype(ctx);
	// return the constructor
	duk_push_c_function(ctx, filestream_constructor, 2);
	return 1;
}