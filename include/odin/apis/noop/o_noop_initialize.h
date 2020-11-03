#ifndef __ODIN_NOOP_INITIALIZE_H__
#define __ODIN_NOOP_INITIALIZE_H__


#include "odin/o_initialize.h"


/** \brief NOOP and does not do anything! */
void odin_noop_initialize(odin_render_device* render_device, odin_initialize_info* initialize_info) { render_device = NULL; };

/** \brief NOOP and does not do anything! */
void odin_noop_terminate(odin_render_device render_device) { };



#endif /* __ODIN_NOOP_INITIALIZE_H__ */