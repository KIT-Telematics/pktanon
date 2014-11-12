#ifndef PKTANON_OPTMACROS_H
#define PKTANON_OPTMACROS_H

namespace pktanon {
  
// branch predictions
# define unlikely(expr) __builtin_expect(!!(expr), 0)
# define likely(expr) __builtin_expect(!!(expr), 1)  
  
}

#endif
