#ifndef PKTANON_DEBUG_H
#define PKTANON_DEBUG_H

# ifdef TRACE_ENABLED

# include <iostream>
# include <mutex>
# include <assert.h>

namespace pktanon {
extern std::mutex dbg_mutex;

# define TRACE(x) \
  do {std::lock_guard<std::mutex> _(dbg_mutex);  std::cout << std::dec <<  __FILE__ <<  ":" <<   __LINE__ <<  ": " <<  x << std::endl;} while (0);
# define TRACEV(x) \
  do {std::lock_guard<std::mutex> _(dbg_mutex);  std::cout << std::dec <<  __FILE__ <<  ":" <<   __LINE__ <<  ": " <<  #x <<  '=' <<  x << std::endl;} while (0);
# define PRINT(x) \
  do { std::lock_guard<std::mutex> _(dbg_mutex);std::cout << x << std::endl;} while (0);
# define HB() \
  do { std::lock_guard<std::mutex> _(dbg_mutex);std::cout << __FILE__ <<  ":" <<   __LINE__ <<  ": " <<  __func__ << std::endl;} while (0);
}

// inline void  display_hex(unsigned length, char *data)
// {
//   unsigned i;
//   uint8_t* data2 = reinterpret_cast<uint8_t*>(data);
// 
//   for (i = 0; i<length; i++)
//   printf("%02x ", data2[i]);
//   printf("\n");
// }

# else

# define TRACE(X)
# define TRACEV(X)
# define PRINT(X)
# define HB(X)


#endif // TRACE_ENABLED

#endif // PKTANON_DEBUG_H 
