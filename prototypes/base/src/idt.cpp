#include "idt.hpp"
#include "io.hpp"
#include "bsod.hpp"
#include "pic.hpp"

#define ISR(num) extern "C" void isr_##num();
#define ISR_ERR(num) ISR(num)
#include "interrupts.lst"
#undef ISR
#undef ISR_ERR

static InterruptDescriptor invalidDesc;
static Interrupt invalidIntr;

InterruptDescriptor IDT::descriptors[length];
Interrupt IDT::interrupts[length];

InterruptDescriptor & IDT::descriptor(uint32_t idx)
{
	if(idx >= IDT::length) {
		return invalidDesc;
	}
	return IDT::descriptors[idx];
}

Interrupt & IDT::interrupt(uint32_t index)
{
	if(index >= IDT::length) {
		return invalidIntr;
	}
	return IDT::interrupts[index];
}

void IDT::initialize()
{
#define ISR(num) IDT::descriptor(num) = InterruptDescriptor( \
		uint32_t(&isr_##num), \
		0x08, \
		InterruptFlags::Interrupt | InterruptFlags::Use32Bit | InterruptFlags::Ring0 | InterruptFlags::Present);
#define ISR_ERR(num) ISR(num)
#include "interrupts.lst"
#undef ISR
#undef ISR_ERR

	struct {
		uint16_t limit;
		void* pointer;
	} __attribute__((packed)) idtp = {
		.limit = IDT::length * sizeof(InterruptDescriptor) - 1,
		.pointer = IDT::descriptors,
	};
	asm volatile("lidt %0" : : "m" (idtp));
	
	IDT::setupPIC();
}

void IDT::setupPIC()
{
	masterPIC.initialize(0x20, 0x04, 0x01);
	slavePIC.initialize(0x28, 0x02, 0x01);

	masterPIC.maskInterrupts(0x00);
	slavePIC.maskInterrupts(0x00);
}

void IDT::dispatch(CpuState *cpu)
{
	Interrupt &intr = IDT::interrupts[cpu->interrupt];
	if(intr.isEnabled) {
		(*intr.handler)(cpu);
	} else {
		BSOD::die(Error::UnhandledException, "Unhandled Interrupt!", cpu);
	}

	if(cpu->interrupt <= 0x1F) {
		// Exception Handling
		
	} else if (cpu->interrupt >= 0x20 && cpu->interrupt <= 0x2F) {
	
		// IRQ
		// Console::main << "[IRQ " << (cpu->interrupt - 0x20) << "]";
		
		
	
		if(cpu->interrupt >= 0x28) {
			slavePIC.sendEndOfInterrupt();
		}
		masterPIC.sendEndOfInterrupt();
	} else {
		BSOD::die(Error::UnhandledInterrupt, "Ermahgerd, Interrupts!", cpu);
	}
}




Interrupt::Interrupt() :
	isEnabled(false), 
	handler(nullptr)
{

}
	
Interrupt::Interrupt(Handler handler) : 
	isEnabled(true), 
	handler(handler)
{
	
}




InterruptDescriptor::InterruptDescriptor() : 
	offset0(0), selector(0), zero(0), flags(InterruptFlags::None), offset1(0)
{

}
	
InterruptDescriptor::InterruptDescriptor(uint32_t offset, uint32_t selector, InterruptFlags flags) : 
	offset0(0), selector(selector), zero(0), flags(flags), offset1(0)
{
	this->setOffset(offset);
}
	
uint32_t InterruptDescriptor::offset() const
{
	return this->offset0 | (this->offset1 << 16);
}

void InterruptDescriptor::setOffset(uint32_t offset)
{
	this->offset0 = (offset & 0x0000FFFF) >> 0;
	this->offset1 = (offset & 0xFFFF0000) >> 16;
}