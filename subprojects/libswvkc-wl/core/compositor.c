#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/log.h>


#include <wayland-server-core.h>
#include <wayland-server-protocol.h>

#include <core/compositor.h>
#include <core/region.h>

static void compositor_create_surface(struct wl_client *client, struct
wl_resource *resource, uint32_t id) {
	struct compositor *compositor = wl_resource_get_user_data(resource);
	struct wl_resource *surface_resource = wl_resource_create(client,
	&wl_surface_interface, 4, id);
	surface_new(surface_resource, compositor->surface_events);
}

static void compositor_create_region(struct wl_client *client, struct
wl_resource *resource, uint32_t id) {
	struct wl_resource *region_resource = wl_resource_create(client,
	&wl_region_interface, 1, id);
	region_new(region_resource);
}

static const struct wl_compositor_interface impl = {
	.create_surface = compositor_create_surface,
	.create_region = compositor_create_region
};

static enum wl_iterator_result set_selection_all(struct wl_resource *resource,
void *user_data) {
	errlog("%d", wl_resource_get_id(resource));
/*	if (!strcmp(wl_resource_get_class(resource), "wl_data_device")) {
		wl_resource_get_client(resource);
	}*/
	return WL_ITERATOR_CONTINUE;
}
void compositor_free(struct wl_resource *resource) {
	struct wl_client *client = wl_resource_get_client(resource);
	wl_client_for_each_resource(client, set_selection_all, resource);

	struct compositor *compositor = wl_resource_get_user_data(resource);
	free(compositor);
}

struct compositor *compositor_new(struct wl_resource *resource, struct
surface_events surface_events) {
	struct compositor *compositor = calloc(1, sizeof(struct compositor));
	compositor->surface_events = surface_events;
	wl_resource_set_implementation(resource, &impl, compositor,
	compositor_free);
	return compositor;
}
