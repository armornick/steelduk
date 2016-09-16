#ifndef _DUK_OBJECTS_HPP_
#define _DUK_OBJECTS_HPP_

#include <duktape.h>
#include <duk-helpers.hpp>

/* ----------------------------------------------------------------------------------
Duktape Object properties helpers
---------------------------------------------------------------------------------- */
void sduk_put_getter(duk_context *ctx, duk_idx_t index, const char *prop, duk_c_function getter);
void sduk_put_getter_setter(duk_context *ctx, duk_idx_t index, const char *prop, duk_c_function getter, duk_c_function setter);


template <typename T>
void sduk_put_readonly(duk_context *ctx, duk_idx_t index, const char *prop, T value)
{
	index = duk_normalize_index(ctx, index);
	duk_push_string(ctx, prop);
	sduk_push(ctx, value);
	duk_def_prop(ctx, index, 
		DUK_DEFPROP_HAVE_VALUE |DUK_DEFPROP_CLEAR_WRITABLE);
}



#endif // _DUK_OBJECTS_HPP_