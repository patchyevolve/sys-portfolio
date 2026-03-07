#include <stdio.h>
#include <stdbool.h>

#define STACK_SIZE 256
#define PROGRAM_SIZE 256




// INSTRUCTION SET

typedef enum {

    HLT,

    PSH,
    POP,

    ADD,
    SUB,
    MUL,
    DIV,

    SET,
    MOV,

    JMP,
    JZ,

    PRT

} InstructionSet;




// REGISTERS

typedef enum {

    A, B, C, D,
    IP,
    SP,

    NUM_REGS

} Registers;




// PROGRAM

int program[PROGRAM_SIZE] = {

    PSH, 5,
    PSH, 6,
    ADD,
    PRT,
    HLT

};




// STATE

int registers[NUM_REGS];
int stack[STACK_SIZE];

bool running = true;



// STACK HELPERS

void push(int v) {

    if (registers[SP] >= STACK_SIZE - 1) {

        printf("Stack overflow\n");
        running = false;
        return;
    }

    stack[++registers[SP]] = v;
}


int pop() {

    if (registers[SP] < 0) {

        printf("Stack underflow\n");
        running = false;
        return 0;
    }

    return stack[registers[SP]--];
}



// FETCH

int fetch() {

    return program[registers[IP]++];
}




// EXECUTE

void eval(int instr) {

    switch (instr) {

        case HLT:
            running = false;
            printf("HLT\n");
            break;


        case PSH: {

            int val = fetch();

            push(val);

            break;
        }


        case POP: {

            int v = pop();

            printf("POP %d\n", v);

            break;
        }


        case ADD: {

            int a = pop();
            int b = pop();

            push(a + b);

            break;
        }


        case SUB: {

            int a = pop();
            int b = pop();

            push(b - a);

            break;
        }


        case MUL: {

            int a = pop();
            int b = pop();

            push(a * b);

            break;
        }


        case DIV: {

            int a = pop();
            int b = pop();

            push(b / a);

            break;
        }


        case SET: {

            int reg = fetch();
            int val = fetch();

            registers[reg] = val;

            break;
        }


        case MOV: {

            int r1 = fetch();
            int r2 = fetch();

            registers[r1] = registers[r2];

            break;
        }


        case JMP: {

            int addr = fetch();

            registers[IP] = addr;

            break;
        }


        case JZ: {

            int addr = fetch();

            int v = pop();

            if (v == 0)
                registers[IP] = addr;

            break;
        }


        case PRT: {

            int v = pop();

            printf("OUT %d\n", v);

            break;
        }

    }
}



// MAIN LOOP


int main() {

    registers[IP] = 0;
    registers[SP] = -1;

    while (running) {

        int instr = fetch();

        eval(instr);
    }

    return 0;
}