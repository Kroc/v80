#ifndef SEEN_POLYFILL_ASSERT_H
#define SEEN_POLYFILL_ASSERT_H

#ifndef NDEBUG
#   include <assert.h>
#else
#   define assert(_exp) (void*)NULL
#endif

#endif
