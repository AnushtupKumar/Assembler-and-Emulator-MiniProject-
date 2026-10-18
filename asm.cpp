/*
 * Student Name: Anushtup Kumar
 * User ID: 2501CS32
 * Declaration of Authorship: I declare that this code is my own work.
 */

#include <bits/stdc++.h>
using namespace std;

// instruction set map with opcodes
unordered_map<string, int> instruction_set = {
    {"data", -1}, {"set", -2},    {"ldc", 0},  {"adc", 1},   {"ldl", 2},
    {"stl", 3},   {"ldnl", 4},    {"stnl", 5}, {"add", 6},   {"sub", 7},
    {"shl", 8},   {"shr", 9},     {"adj", 10}, {"a2sp", 11}, {"sp2a", 12},
    {"call", 13}, {"return", 14}, {"brz", 15}, {"brlz", 16}, {"br", 17},
    {"halt", 18}, {"mul", 19},    {"div", 20}};

// instructions requiring an operand
unordered_map<string, bool> requires_operand = {
    {"data", true}, {"set", true},  {"ldc", true},  {"adc", true},
    {"ldl", true},  {"stl", true},  {"ldnl", true}, {"stnl", true},
    {"adj", true},  {"call", true}, {"brz", true},  {"brlz", true},
    {"br", true}};

vector<int> branch_instructions = {13, 15, 16, 17};

unordered_map<string, int> labels;
vector<int> machine_code;
vector<string> source_lines;
vector<pair<int, string>> errors;

int PC = 0; // Program Counter
int PASS = 0;

// function to trim whitespace and comments from a string line
string trim(string s) {
  size_t comment_start = s.find(';'); // strip comments
  if (comment_start != string::npos)
    s.erase(comment_start);

  const string whitepsaces = " \t\n\r\f\v";

  size_t start = s.find_first_not_of(whitepsaces); // ignore whitespace
  if (start == string::npos) // no char found else whitespaces
    return "";
  s.erase(0, start);

  size_t end = s.find_last_not_of(whitepsaces);
  if (end != string::npos)
    s.erase(end + 1);

  return s;
}

// function to store errors string with line number in a vector
void recordError(int line_num, string msg) {
  errors.push_back(
      make_pair(line_num, "Line " + to_string(line_num) + ": " + msg));
}

void parseStatement(string statement, int line_num) {
  string original_statement = statement;

  // trim whitespace and comment form instruction line
  statement = trim(statement);

  if (statement.empty()) // empty statement line
    return;

  string label = "", mnemonic = "", operand_str = "";

  // parse Label
  // we can detect label presence by checking : (colon) presence
  size_t colon_pos = statement.find(':');

  // if colon exist in instruction line
  if (colon_pos != string::npos) {
    label = trim(statement.substr(0, colon_pos));      // extracting label
    statement = trim(statement.substr(colon_pos + 1)); // remaining statement

    if (PASS == 0) {
      if (label.empty() || !isalpha(label[0])) {
        recordError(line_num, "inappropriate label name");
      } else if (labels.count(label) > 0) { // duplcate label founded
        recordError(line_num, "duplicate label definition");
      } else {
        labels[label] = PC;
      }
    }
  }

  if (statement.empty()) // statement do not contains instruction
    return;

  // parse Mnemonic and Operand
  istringstream ss(statement);
  ss >> mnemonic;

  // handle uppercase or lowercase for mnemonics
  for (char &c : mnemonic) {
    c = tolower((unsigned char)c);
  }

  bool has_operand = (ss >> operand_str) ? true : false;

  // statement can contain at most one operand
  string extra;
  if (ss >> extra) {
    if (PASS == 1)
      recordError(line_num, "extra on end of line");
  }

  if (instruction_set.count(mnemonic) == 0) {
    if (PASS == 1)
      recordError(line_num, "unknown mnemonic"); // unknown instructions
    return;
  }

  // SET instruction handling
  // SET : Set the label on this line to the specified value (rather than the
  // PC).
  if (mnemonic == "set") {
    if (PASS == 0 && !label.empty()) {
      char *end;
      int val = strtol(operand_str.c_str(), &end, 0);

      // need to verify val, user cannot provide any value
      // ----IMPLEMENT HERE-------

      labels[label] = val; // set label to specified value
    }

    // proper format :- label : set 10
    if (label.empty() && PASS == 0) {
      recordError(line_num, "SET must follow label");
    }
    return; // do need to increase PC because it is causing updating stack
            // memory
  }

  // we can detect infine loop by checking :- label : br label
  if (has_operand && mnemonic == "br" && label == operand_str) {
    cout << "WARNING: Infinite loop at " << line_num << '\n';
  }

  if (PASS == 0 && ++PC)
    return;

  // Check operand constraints
  if (requires_operand[mnemonic] && !has_operand)
    recordError(line_num, "missing operand");

  else if (!requires_operand[mnemonic] && has_operand)
    recordError(line_num,
                "unexpected operand : do need to need any operand for : " +
                    mnemonic);

  int opcode = instruction_set[mnemonic];
  int operand_val = 0;

  if (has_operand) {
    char *end;
    operand_val = strtol(operand_str.c_str(), &end, 0);

    if (*end != '\0') { // string is not converted to number properly
      if (labels.count(operand_str))
        operand_val = labels[operand_str];

      // label not found in labels column
      else {
        recordError(line_num, "no such label : " + operand_str);
        operand_val = 0;
      }
    }

    // branch offset calculation: PC = PC + offset
    if (find(branch_instructions.begin(), branch_instructions.end(), opcode) !=
        branch_instructions.end()) {
      operand_val = operand_val - (PC + 1);
    }
  }

  int encoded_instruction = 0;
  if (mnemonic == "data") {
    encoded_instruction = operand_val; // reserve memory to initialized to value
  } else {
    // encoding uses bottom 8 bits for opcode, upper 24 for signed operand
    encoded_instruction = ((operand_val & 0xFFFFFF) << 8) | (opcode & 0xFF);
  }

  machine_code.push_back(encoded_instruction);

  // formatting the listing file output for this instruction
  stringstream lst_line;
  lst_line << setfill('0') << setw(4) << hex << PC << " " << setfill('0')
           << setw(8) << hex << (unsigned int)encoded_instruction << " "
           << original_statement;

  // store all instructions machine code and with its code so we can output it
  // later
  source_lines.push_back(lst_line.str());

  PC++;
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <input_file.asm>\n";
  }

  // file path of asm file
  string filename = argv[1];

  // extract base name for output files
  string base_name = filename.substr(0, filename.find_last_of('.'));

  // open asm file in read mode
  ifstream file(filename);

  if (!file.is_open()) {
    cout << "Could not open file\n";
    return 1;
  }

  // PASS 0
  string line;
  int line_num = 1;
  while (getline(file, line))
    parseStatement(line, line_num++);

  // PASS 1
  PASS = 1;
  PC = 0; // reset PC
  // seek file pointer to start of asm file to read file from start
  file.clear();
  file.seekg(0);
  line_num = 1;

  while (getline(file, line))
    parseStatement(line, line_num++);

  file.close();

  // print errors
  if (!errors.empty()) {
    sort(errors.begin(), errors.end(),
         [](const pair<int, string> &a, const pair<int, string> &b) {
           return a.first < b.first;
         });
    ofstream log_file(base_name + ".log");

    cout << "Errors:\n";
    log_file << "Errors:\n";
    for (const pair<int, string> &err : errors) {
      cout << err.second << "\n";
      log_file << err.second << "\n";
    }
    log_file.close();
    // we can return from here if we do not want to log the lines which executed
    // before any error
    return 1; // 1 means execution was not successfull
  }

  // Listing File
  ofstream lst_file(base_name + ".lst");
  for (const string &l : source_lines) {
    lst_file << l << "\n";
  }
  lst_file.close();
  cout << "Listing file created: " << base_name << ".lst\n";

  // Binary File
  ofstream bin_file(base_name + ".o", ios::binary);
  for (int code : machine_code) {
    bin_file.write((const char *)&code, sizeof(int));
  }
  bin_file.close();
  cout << "Object file created: " << base_name << ".o\n";

  cout << "Assembled successful.\n";
  return 0;
}