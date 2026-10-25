/*
 * Student Name: Anushtup Kumar
 * User ID: 2501CS32
 * Declaration of Authorship: I declare that this code is my own work.
 */

#include <bits/stdc++.h>
using namespace std;

ofstream ofile;
int reg_A = 0;
int reg_B = 0;
int reg_PC = 0;
int reg_SP = 0;

// this variable will be use for HALT instruction
bool running = true;
int instructions_count = 0;

// allocates enough memory : 2 ^ 16
vector<int> memory(1 << 16, 0);

unordered_map<int, function<void(int)>> instruction_map = {
    {0,
     [](int operand) {
       reg_B = reg_A;
       reg_A = operand;
     }},                                               // ldc
    {1, [](int operand) { reg_A = reg_A + operand; }}, // adc
    {2,
     [](int operand) {
       reg_B = reg_A;
       reg_A = memory[reg_SP + operand];
     }}, // ldl
    {3,
     [](int operand) {
       memory[reg_SP + operand] = reg_A;
       reg_A = reg_B;
     }},                                                       // stl
    {4, [](int operand) { reg_A = memory[reg_A + operand]; }}, // ldnl
    {5, [](int operand) { memory[reg_A + operand] = reg_B; }}, // stnl
    {6, [](int operand) { reg_A = reg_B + reg_A; }},           // add
    {7, [](int operand) { reg_A = reg_B - reg_A; }},           // sub
    {8, [](int operand) { reg_A = reg_B << reg_A; }},          // shl
    {9, [](int operand) { reg_A = reg_B >> reg_A; }},          // shr
    {10, [](int operand) { reg_SP = reg_SP + operand; }},      // adj
    {11,
     [](int operand) {
       reg_SP = reg_A;
       reg_A = reg_B;
     }}, // a2sp
    {12,
     [](int operand) {
       reg_B = reg_A;
       reg_A = reg_SP;
     }}, // sp2a
    {13,
     [](int operand) {
       int old_reg_PC = reg_PC;
       reg_B = reg_A;
       reg_A = reg_PC;
       reg_PC = reg_PC + operand;

       if (old_reg_PC == reg_PC) {
         // there is infinite loop
       }
     }}, // call
    {14,
     [](int operand) {
       reg_PC = reg_A;
       reg_A = reg_B;
       if (reg_PC == reg_A) {
         // there is infinite loop
       }
     }}, // return
    {15,
     [](int operand) {
       if (reg_A == 0)
         reg_PC = reg_PC + operand;
     }}, // brz
    {16,
     [](int operand) {
       if (reg_A < 0)
         reg_PC = reg_PC + operand;
     }}, // brlz
    {17,
     [](int operand) {
       int old_reg_PC = reg_PC;
       reg_PC = reg_PC + operand;
       if (old_reg_PC == reg_PC) {
         // there is infinite loop
       }
     }},                                              // br
    {18, [](int operand) { running = false; }},       // HALT
    {19, [](int operand) { reg_A = reg_B * reg_A; }}, // mul
    {20, [](int operand) {                            // div
       if (reg_A == 0) {
         ofile << "Error: Division by zero at PC " << reg_PC - 1 << "\n";
         running = false;
       } else {
         reg_A = reg_B / reg_A;
       }
     }}};

void execute() {
  running = true;
  int count_executed = 0;
  long long max_instruction = 1e7; // 10 million will be enough

  while (running) {

    if (count_executed >= max_instruction) {
      ofile << "Termanating... timeout warning: There can be Infine loop. "
               "Check your code!!\n";
      running = false;
      break;
    }

    if (reg_PC < 0 || reg_PC >= (int)memory.size()) {
      ofile << "Warning: PC out of bounds (" << reg_PC
            << "). Halting execution.\n";
      running = false;
      break;
    }

    // read instruction from stack memory
    int instruction = memory[reg_PC];

    // Increment PC before executing instruction
    reg_PC++;
    count_executed++;

    // Decode
    int opcode = instruction & 0xFF; // Bottom 8 bits
    int operand = instruction >> 8;  // Upper 24 bits

    // Sign extend the 24-bit operand to 32 bits
    if (operand & 0x800000) {
      operand |= 0xFF000000;
    }

    if (instruction_map.count(opcode)) {
      instruction_map[opcode](operand);
    } else {
      ofile << "Error: Unknown opcode " << opcode << " at PC " << reg_PC - 1
            << "\n";
      running = false;
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    cout << "usage: " << argv[0] << "<input_file.o>\n";
  }

  const string file_name = argv[1];
  string file_basename = file_name.substr(0, file_name.find_last_of('.'));

  // Load binary object file
  ifstream file(file_name, ios::binary);
  if (!file.is_open()) {
    cout << "Error: Could not open file\n";
    return 1;
  }

  file.seekg(0, ios::end);
  streamsize size = file.tellg();
  instructions_count = size / sizeof(int);

  file.seekg(0, ios::beg);

  if (instructions_count > 0) {
    file.read((char *)memory.data(), size);
  }
  file.close();

  // from assembler
  reg_SP = instructions_count;

  ofile.open(file_basename + ".log");

  ofile << "instruction_count : " << instructions_count << '\n';
  ofile << "Loaded instructions. Starting execution...\n";

  execute();

  ofile << "Final Register State:\n";
  ofile << "A:  0x" << setfill('0') << setw(8) << hex << reg_A << "\n";
  ofile << "B:  0x" << setfill('0') << setw(8) << hex << reg_B << "\n";
  ofile << "PC: 0x" << setfill('0') << setw(8) << hex << reg_PC << "\n";
  ofile << "SP: 0x" << setfill('0') << setw(8) << hex << reg_SP << "\n";

  ofile << "\nMemory dump:\n";
  // first 1024 memory address
  for (int i = 0; i < 1024; i++) {
    ofile << "0x" << setfill('0') << setw(8) << hex << i << " 0x" << setw(8)
          << hex << memory[i] << "(" << dec << memory[i] << ")" << '\n';
  }

  cout << "Output is saved to : '" + file_basename + ".log' file\n";

  return 0;
}