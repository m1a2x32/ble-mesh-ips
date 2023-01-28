#include <persistant_store_manager.h>
#include <string.h>

static int position_settings_set(const char *name, size_t len, settings_read_cb read_cb, void *cb_arg)
{
	const char *next;
	size_t name_len;
    int rc;

	name_len = settings_name_next(name, &next);
	if (!next){
		return -ENOENT;
	}
	
	if (!strncmp(name, "local", name_len)) {
		name_len = settings_name_next(name, &next);

		if (!strncmp(next, "x", name_len)) {
			rc = read_cb(cb_arg, &local.east, sizeof(local.east));
			return 0;
		}
		if (!strncmp(next, "y", name_len)) {
			rc = read_cb(cb_arg, &local.north, sizeof(local.north));
			return 0;
		}
		return -ENOENT;
	}

    return -ENOENT;
}

static int position_setting_export(int (*storage_func)(const char *name, const void *value, size_t val_len))
{
	storage_func("pos/local/x", &local.east, sizeof(local.east));
	storage_func("pos/local/y", &local.north, sizeof(local.north));
	
	return 0;
}

struct settings_handler pos_conf = {
	.name = "pos",
	.h_set = position_settings_set,
	.h_export = position_setting_export
};