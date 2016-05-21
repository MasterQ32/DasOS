#pragma once

#include <stddef.h>
#include <inttypes.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if !defined(VM_STACKSIZE)
#define VM_STACKSIZE 64
#endif

// Binary Encoding : (enabled, value)
#define VM_EXEC_X 0
#define VM_EXEC_0 2
#define VM_EXEC_1 3

#define VM_INPUT_ZERO 0
#define VM_INPUT_POP 1
#define VM_INPUT_PEEK 2
#define VM_INPUT_ARG 3

#define VM_CMD_COPY     0
#define VM_CMD_STORE    1
#define VM_CMD_LOAD     2  
#define VM_CMD_GET      3
#define VM_CMD_SET      4
#define VM_CMD_BPGET    5
#define VM_CMD_BPSET    6
#define VM_CMD_RSTSTACK 7
#define VM_CMD_MATH     8
#define VM_CMD_SPGET    9
#define VM_CMD_SPSET    10
#define VM_CMD_SYSCALL  11
#define VM_CMD_HWIO     12

#define VM_MATH_ADD 0
#define VM_MATH_SUB 1
#define VM_MATH_MUL 2
#define VM_MATH_DIV 3
#define VM_MATH_MOD 4
#define VM_MATH_AND 5
#define VM_MATH_OR  6
#define VM_MATH_XOR 7
#define VM_MATH_NOT 8
#define VM_MATH_ROL 9
#define VM_MATH_ROR 10
#define VM_MATH_ASL 11
#define VM_MATH_ASR 12
#define VM_MATH_SHL 13
#define VM_MATH_SHR 14

#define VM_FLAG_NO  0
#define VM_FLAG_YES 1

#define VM_OUTPUT_DISCARD 0
#define VM_OUTPUT_PUSH    1
#define VM_OUTPUT_JUMP    2

#define VM_FLAG_Z (1<<0)
#define VM_FLAG_N (1<<1)

typedef struct
{
	unsigned int execZ   : 2;
	unsigned int execN   : 2;
	unsigned int input0  : 2;
	unsigned int input1  : 1;
	unsigned int command : 6;
	unsigned int cmdinfo : 16;
	unsigned int flags   : 1;
	unsigned int output  : 2;
	uint32_t     argument;
} __attribute__ ((packed)) Instruction;

_Static_assert(sizeof(Instruction) == 8, "Instruction must be 8 bytes large.");
_Static_assert(offsetof(Instruction, argument) == 4, "Argument must be  must be 8 bytes large.");

typedef struct
{
	Instruction *code;
	uint32_t length;
} Module;

typedef struct 
{
	Module *module;
	void *tag;
	
	uint32_t codePointer;
	uint32_t stackPointer;
	uint32_t basePointer;
	uint32_t flags;
	
	uint32_t stack[VM_STACKSIZE];
} Process;

typedef struct 
{
	uint32_t input0;
	uint32_t input1;
	uint32_t argument;
	uint32_t additional;
	
	uint32_t output;
} CommandInfo;

/**
 * @brief Steps a given process.
 *
 * Executes a single instruction and processes input and output.
 *
 * @param process The process to be stepped.
 * @returns 1 if the process is still running or 0 if the process is terminated.
 */
int vm_step_process(Process *process);

/**
 * @brief Pushes a value onto the process' stack.
 */
void vm_push(Process *process, uint32_t value);

/**
 * @brief Pops a value from the process' stack.
 */
uint32_t vm_pop(Process *process);

/**
 * @brief Returns the top value of the process' stack.
 */
uint32_t vm_peek(Process *process);


// The following functions need to be host-implemented.

/**
 * An assertion the VM does.
 * @param assertion If zero, the assertion failed.
 * @param msg       The message that should be shown when the assertion fails.
 */
void vm_assert(int assertion, const char *msg);

/**
 * The hosts syscall implementation.
 * @param process The process that calls the syscall.
 * @param info    Additional information for the syscall. Contains arguments and results.
 */ 
void vm_syscall(Process *process, CommandInfo *info);

/**
 * The hosts hardware IO implementation.
 * @param process The process that wants to do IO.
 * @param info    Additional information for the HWIO. Contains arguments and results.
 */ 
void vm_hwio(Process *process, CommandInfo *info);

#if defined(__cplusplus)
}
#endif