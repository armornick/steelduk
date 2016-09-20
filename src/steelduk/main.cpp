#include <duktape.h>
#include "duk-helpers.hpp"

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
// Unicode-enabled print/alert function

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

// -----------------------------------------------------------------------------
duk_ret_t dukopen_encodings(duk_context *ctx);
duk_ret_t dukopen_io_file(duk_context *ctx);
duk_ret_t dukopen_io_fstream(duk_context *ctx);
duk_ret_t dukopen_console(duk_context *ctx);
duk_ret_t dukopen_console_color(duk_context *ctx);

void sduk_setup_modsearch(duk_context *ctx);


static const duk_function_list_entry _modules[] = {
	{ "encodings", dukopen_encodings, 0 },
	{ "io/file", dukopen_io_file, 0 },
	{ "io/fstream", dukopen_io_fstream, 0 },
	{ "console", dukopen_console, 0 },
	{ "console/color", dukopen_console_color, 0 },
	{ NULL, NULL, 0 }
};

#define STEELDUK_PRELOAD_TABLE "\xFF""preload"

static void setup_env(duk_context *ctx)
{
	// register unicode print/alert function
	duk_push_c_function(ctx, duk_wprint, DUK_VARARGS);
	duk_put_global_string(ctx, "print");
	duk_push_c_function(ctx, duk_wprint, DUK_VARARGS);
	duk_put_global_string(ctx, "alert");

	// setup require function
	sduk_setup_modsearch(ctx);

	// register preloaded modules
	duk_get_global_string(ctx, STEELDUK_PRELOAD_TABLE);
	duk_put_function_list(ctx, -1, _modules);
	duk_pop(ctx);
}

// -----------------------------------------------------------------------------