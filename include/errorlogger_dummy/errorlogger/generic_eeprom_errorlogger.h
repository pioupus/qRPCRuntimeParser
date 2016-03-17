/*
 * generic_config.h
 *
 *  Created on: 05.01.2016
 *      Author: ak
 */

#ifndef MODULES_ERRORLOGGER_INCLUDE_ERRORLOGGER_GENERIC_EEPROM_ERRORLOGGER_H_
#define MODULES_ERRORLOGGER_INCLUDE_ERRORLOGGER_GENERIC_EEPROM_ERRORLOGGER_H_

#include <stdint.h>
#include <assert.h>
#include "errlog_trace.h"


#ifdef __cplusplus
extern "C" {
#endif

#define GEN_ASSERT(EX, ERRNUMBER, ...); {TRACE_ERROR(__VA_ARGS__); assert(EX);}
#define GEN_ASSERT_NO_LOGGING(EX, ERRNUMBER, ...); {TRACE_ERROR(__VA_ARGS__); assert(EX);}
#ifdef __ASSERT_FUNC
#define GEN_WARNING(ERRNUMBER, ...); {TRACE_WARNING(__VA_ARGS__);  printf("\nwarning at file: %s line: %d and function: %s\n",__FILE__, __LINE__,__ASSERT_FUNC);}
#else
#define GEN_WARNING(ERRNUMBER, ...); {TRACE_WARNING(__VA_ARGS__); printf("\nwarning at file: %s line: %d\n",__FILE__, __LINE__);}
#endif


#ifdef __cplusplus
}
#endif

#endif /* MODULES_ERRORLOGGER_INCLUDE_ERRORLOGGER_GENERIC_EEPROM_ERRORLOGGER_H_ */
