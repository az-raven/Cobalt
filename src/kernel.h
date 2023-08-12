#pragma once

#include <util/option.h>
#include <hardware/framebuffer.h>

using Cobalt::Utility::Option;
using Cobalt::Graphics::Framebuffer;

namespace Cobalt::Kernel {
	[[noreturn]]
	void main(
		Option<Framebuffer> primary_framebuffer
	);
}