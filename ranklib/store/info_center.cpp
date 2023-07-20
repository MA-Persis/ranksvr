#include "store/info_center.h"
#include <gflags/gflags.h>
#include "base/def.h"
namespace qihoo { namespace ad { namespace ranking {

  DEFINE_int32(IC_buffer_size_huge, MAX_STORE_BUFF_SIZE, "huge storage size");
  DEFINE_int32(IC_buffer_size_large, TERM_BUFF_SIZE_LARGE, "large storage size");
  DEFINE_int32(IC_buffer_size_more, TERM_BUFF_SIZE_MORE, "more storage size");
  DEFINE_int32(IC_buffer_size_normal, TERM_BUFF_SIZE_NORMAL, "default storage size");
  DEFINE_int32(IC_buffer_size_less, TERM_BUFF_SIZE_LESS, "less storage size");
  DEFINE_int32(IC_buffer_size_little, TERM_BUFF_SIZE_LITTLE, "little storage size");

} } }

