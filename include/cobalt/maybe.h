#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "inline.h"

namespace cobalt {
	template<typename T>
	class Maybe {
		public:
			/// @brief Constructs a new Maybe with no value.
			/// @return The constructed Maybe.
			static Maybe<T> none() {
				return Maybe<T>(false);
			}

			/// @brief Constructs a new Maybe with the given value.
			/// @param value The value to place inside the Maybe.
			/// @return The constructed Maybe.
			static Maybe<T> some(T value) {
				return Maybe<T>(value, true);
			}

			/// @brief Grab the value from within the Maybe.
			/// @param default_value The default value to use if the Maybe contains no value.
			/// @return Either the value contained if the Maybe is Some, or the default value if the Maybe is None.
			ALWAYS_INLINE
			T Maybe::value_or(T default_value) {
				return tag ? value : default_value;
			}

			/// @brief Grab the value from within the Maybe.
			/// @param default_value The closure that will be evaluated if the Maybe is None.
			/// @return Either the contained value if the Maybe is Some, or the result of the closure if the Maybe is None.
			ALWAYS_INLINE
			T Maybe::value_or(T (*default_value)()) {
				return tag ? value : default_value();
			}

			/// @brief Extremely unsafe function that will return the value of the option.
			/// @return The value of the Maybe, if the value is set. If not, it's UB.
			ALWAYS_INLINE
			T Maybe::unsafe_unwrap() {
				return value;
			}

			/// @brief Check if the Maybe contains a value.
			/// @return True if the Maybe contains a value, false otherwise.
			ALWAYS_INLINE
			bool Maybe::is_some() {
				return tag;
			}

		private:
			Maybe<T>(T value, bool tag) {
				this->value = value;
				this->tag = tag;
			}
			Maybe<T>(uint8_t tag) {
				this->tag = tag;
			}

			T value;
			bool tag;
	};
} // namespace cobalt
