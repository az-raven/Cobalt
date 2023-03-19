#include <stdlib.h>
#include "cobalt/maybe.h"
#include "cobalt/verify.h"

namespace cobalt {
	/// @brief Used for managing allocations internally.
	typedef struct _Allocation {
		size_t 	size;
		bool 	free;

		struct _Allocation *next;
	} Allocation;

	class Heap {
		public:
			static Maybe<Heap> create(void *start, size_t size) {
				// Verify that the start address is valid.
				if(!verify_kernel_data_address(start))
					return Maybe<Heap>::none();
				// Verify that the end address is valid.
				if(!verify_kernel_data_address(start + size))
					return Maybe<Heap>::none();

				return Maybe<Heap>::some(Heap(start, size));
			}

		private:
			Heap(void *start, size_t size) {
				base = (Allocation *)start;
				base->size = size - sizeof(Allocation);
				base->free = true;
				base->next = NULL;
			}

			Allocation *base;
	};
}