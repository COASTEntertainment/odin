#ifndef __ODIN_LOG_H__
#define __ODIN_LOG_H__



#include <stdio.h>
#include <stdlib.h>


/** \brief Logs errors to the iostream and self crashes app. */
#define ODIN_ERROR(filename, description) printf("%s - Line(%d) : %s", filename, __LINE__, description); abort()


#endif /* __ODIN_LOG_H__ */