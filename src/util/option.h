#pragma once

#include <util/todo.h>

namespace Cobalt::Utility {
	template <typename T>
	class Option {
		public:
			Option() : _has_value(false) { }
			Option(T value) : _has_value(true), _value(value) { }

			bool has_value() const {
				return this->_has_value;
			}

			T value() const {
				// FIXME: There's probably a better way to handle
				//        this.
				if (!this->_has_value)
					TODO();
				return this->_value;
			}

			static Option<T> none() {
				return Option<T>();
			}

			static Option<T> some(T value) {
				return Option<T>(value);
			}

		private:
			bool _has_value;
			T _value;
	};
}