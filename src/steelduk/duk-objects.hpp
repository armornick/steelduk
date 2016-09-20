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

/* ----------------------------------------------------------------------------------
Duktape Bound Object helpers
---------------------------------------------------------------------------------- */
#define SDUK_BOUND_HANDLE_PROP "\xFF""handle"

template <typename T>
struct sduk_object
{
	typedef T* handle;

	static handle get_handle(duk_context *ctx, duk_idx_t index)
	{
		index = duk_normalize_index(ctx, index);
		handle result = NULL;

		if (duk_is_object(ctx, index))
		{
			if (duk_get_prop_string(ctx, index, SDUK_BOUND_HANDLE_PROP))
			{
				result = static_cast<handle>(duk_get_buffer_data(ctx, -1, NULL));
			}
			duk_pop(ctx);
		}

		return result;
	}

	static handle get_handle_from_this(duk_context *ctx)
	{
		duk_push_this(ctx);
		return get_handle(ctx, -1);
	}

	static handle push_handle(duk_context *ctx, const char *prototype)
	{
		duk_push_object(ctx);
		handle result = static_cast<handle>(duk_push_fixed_buffer(ctx, sizeof(T)));
		duk_put_prop_string(ctx, -2, SDUK_BOUND_HANDLE_PROP);
		duk_get_global_string(ctx, prototype);
		duk_set_prototype(ctx, -2);
		return result;
	}
};



#endif // _DUK_OBJECTS_HPP_