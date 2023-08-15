#include <stdint.h>

#include <util/interrupt.h>
#include <hardware/serial.h>
#include <util/debug.h>

namespace Cobalt::Arch::Interrupts {
	typedef __attribute__((packed)) struct {
		uint16_t offset_low 					:16;
		uint16_t segment_selector 				:16;

		uint8_t interrupt_stack_table_offset 	:3;
		uint8_t reserved_1 						:5;
		uint8_t gate_type 						:4;
		uint8_t reserved_2 						:1;
		uint8_t descriptor_privilege_level 		:2;
		uint8_t present 						:1;
		uint16_t offset_middle 					:16;

		uint32_t offset_high 					:32;

		uint32_t reserved_3 					:32;
	} GateDescriptor64;

	typedef struct { GateDescriptor64 entries[256]; } IDT;

	typedef struct {
		uint16_t length							:16;
		uint64_t base							:64;
	} __attribute__((packed)) IDTDescriptor;

	INTERRUPT void fixme_irq_nop(void) {
		iretq;
	}

	class InterruptDescriptorTable
	{
		private:
			IDT idt;

		public:
			InterruptDescriptorTable();

			inline void set(uint8_t index, void (*handler)()) {
				uint64_t handler_address = (uint64_t)handler | 0xFFFF000000000000;

				GateDescriptor64 entry = {
					.offset_low = (uint16_t)(handler_address & 0xFFFF),

					.segment_selector = 0x28,
					.interrupt_stack_table_offset = 0,
					.reserved_1 = 0,
					.gate_type = 0xF,
					.reserved_2 = 0,
					.descriptor_privilege_level = 0,
					.present = 1,
					.offset_middle = (uint16_t)((handler_address >> 16) & 0xFFFF),
					.offset_high = (uint32_t)((handler_address >> 32) & 0xFFFFFFFF),
					.reserved_3 = 0
				};

				idt.entries[index] = entry;
			}

			inline void load() {
				IDTDescriptor IDTR = {
					.length = sizeof(idt.entries) - 1,
					.base = (uint64_t)&idt
				};

				asm volatile("lidt %0" : : "m"(IDTR));

				debugln("idt", "Loaded Interrupt Descriptor Table successfully!");
			}
	};
	
	InterruptDescriptorTable::InterruptDescriptorTable() {
		
	}
}