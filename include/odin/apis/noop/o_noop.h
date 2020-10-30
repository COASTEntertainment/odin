#ifndef __ODIN_NOOP__
#define __ODIN_NOOP__


#include "odin/o_data.h"
#include "odin/o_initialize.h"


/* Since there is NOOP on this api nothing will happen so just define the odin functions with no return or output */


/* o_noop_data.h */


/* o_noop_initialize.h */

    /** \brief NOOP and does not do anything! */
    void odin_noop_initialize(odin_data* data) { data = NULL; };

    /** \brief NOOP and does not do anything! */
    void odin_noop_terminate(odin_data data) { };



#endif /* __ODIN_NOOP__ */