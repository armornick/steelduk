#include "duk-objects.hpp"


void sduk_put_getter(duk_context *ctx, duk_idx_t index, const char *prop, duk_c_function getter)
{
	index = duk_normalize_index(ctx, index);
	duk_push_string(ctx, prop);
	duk_push_c_function(ctx, getter, 0);
	duk_def_prop(ctx, -3, 
		DUK_DEFPROP_HAVE_GETTER);
}

void sduk_put_getter_setter(duk_context *ctx, duk_idx_t index, const char *prop, duk_c_function getter, duk_c_function setter)
{
	index = duk_normalize_index(ctx, index);
	duk_push_string(ctx, prop);
	duk_push_c_function(ctx, getter, 0);
	duk_push_c_function(ctx, setter, 1);
	duk_def_prop(ctx, -4, 
		DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
}