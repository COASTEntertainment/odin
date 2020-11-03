#ifndef __ODIN_NOOP_API_H__
#define __ODIN_NOOP_API_H__


#include "odin/odin.h"
#include "odin/apis/noop/o_noop.h"


/** \brief Loads only dummy functions that do not execute any code */
void odin_noop_api_load()
{

    odin_initialize = odin_noop_initialize;
    odin_terminate = odin_noop_terminate;

    

}


#endif /* __ODIN_NOOP_API_H__ */