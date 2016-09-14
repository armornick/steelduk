#include <duktape.h>
#include "duk-types.hpp"

#include <io.h> // _access
#include <stdio.h> // wprintf
#include <windows.h> // GetStdHandle, WriteConsoleW

static duk_ret_t duk_main(duk_context *ctx);
static void setup_env(duk_context *ctx);

// -----------------------------------------------------------------------------

int main(int argc, char const *argv[])
{
	duk_context *ctx = duk_create_heap_default();
	if (!ctx)
	{
		printf("%s: could not allocate Duktape heap\n", argv[0]);
		return -1;
	}

	// setup script host environment
	setup_env(ctx);

	// call the main function
	duk_push_c_function(ctx, duk_main, 2);
	duk_push_int(ctx, argc);
	duk_push_pointer(ctx, static_cast<void*>(argv));
	if (duk_pcall(ctx, 2) != DUK_EXEC_SUCCESS) {
		// printf("%s: %s\n", argv[0], duk_to_string(ctx, -1));
		if (duk_is_error(ctx, -1))
		{
			if (duk_get_prop_string(ctx, -1, "stack"))
			{
				printf("%s: %s\n", argv[0], duk_to_string(ctx, -1));
				duk_pop(ctx);
			}
			else
			{
				duk_pop(ctx);
				printf("%s: %s\n", argv[0], duk_to_string(ctx, -1));
			}
			
		}
		else
		{
			printf("%s: %s\n", argv[0], duk_to_string(ctx, -1));
		}
	}
	duk_pop(ctx);

	// clean up resources and return
	duk_destroy_heap(ctx);
	return 0;
}

// -----------------------------------------------------------------------------

#define DEFAULT_MAIN_FILE "main.js"

static duk_ret_t duk_main(duk_context *ctx)
{
	const char *mainfile = DEFAULT_MAIN_FILE;
	// fetch input arguments
	int argc = duk_require_int(ctx, 0);
	char **argv = static_cast<char **>(duk_require_pointer(ctx, 1));
	// create argument array
	duk_push_array(ctx);
	for (int i = 0; i < argc; i++)
	{
		duk_push_string(ctx, argv[i]);
		duk_put_prop_index(ctx, -2, i);
	}
	duk_put_global_string(ctx, "argv");
	// calculate main file
	if (argc > 1 && (_access(argv[0], 00) == 0))
	{
		mainfile = argv[1];
	}
	// evaluate main file and return
	duk_eval_file_noresult(ctx, mainfile);
	return 0;
}

// -----------------------------------------------------------------------------
duk_ret_t dukopen_encodings(duk_context *ctx);
duk_ret_t dukopen_io_file(duk_context *ctx);


static duk_ret_t duk_wprint(duk_context *ctx)
{
	HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	int count = duk_get_top(ctx);
	for (int i = 0; i < count; i++)
	{
		wchar_t *textW = utf8_to_wstring(ctx, duk_is_string(ctx, i) ? duk_get_string(ctx, i) : duk_to_string(ctx, i));
		WriteConsoleW(StdOut, textW, lstrlenW(textW), NULL, NULL);
	}

	wprintf(L"\n");
	return 0;
}

static void register_global_module(duk_context *ctx, const char *id, duk_c_function dukopen)
{
	duk_push_c_function(ctx, dukopen, 0);
	duk_call(ctx, 0);
	duk_put_global_string(ctx, id);
}

static void setup_env(duk_context *ctx)
{
	// register unicode print/alert function
	duk_push_c_function(ctx, duk_wprint, DUK_VARARGS);
	duk_put_global_string(ctx, "print");
	duk_push_c_function(ctx, duk_wprint, DUK_VARARGS);
	duk_put_global_string(ctx, "alert");

	// register global modules
	register_global_module(ctx, "encodings", dukopen_encodings);
	register_global_module(ctx, "fileio", dukopen_io_file);
}

// -----------------------------------------------------------------------------