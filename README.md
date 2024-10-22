[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/3MmVbb7f)
# Debugging 24/25 Exercise 1

Detailed instructions are in the exercise sheet. Following are your checkpoints:

- [ ✓ ] Implement parser and evaluator
- [ ✓ ] Implement a fuzzer
- [ ✓ ] Generate *lots* of random instances with your fuzzer. Your evaluator and z3 must return the same result on generated instances
- [ ✓ ] Provide detailed build instructions for your code so that we can evaluate it

# Build Instruction:
## For Windows 
1. **Install MinGW or MSYS2**
   - Download and install [MinGW](https://www.mingw-w64.org/)
2. **Create a Source File**
   - Save your C++ code to a file named `ex1.cpp`.
   - Navigate to the directory where `ex1.cpp` is located.
4. **Compile and run the Program**
   - Run the following command in powershell:
     ```bash
     g++ ex1.cpp
     ```
     ```bash
     .\ex1.exe
     ```
5. **Compare with z3 output**
   - Run the following command in powershell:
     ```bash
     z3 z3_input.smt2 > res1; if ($?) { ./ex1 z3_input.smt2 > res2; if ($?) { Compare-Object (Get-Content res1) (Get-Content res2) | Out-Null; if ($?) { "Outputs are identical." } else { "Outputs differ." } } }
     ```

## For Linux
1. **Install g++**
   ```bash
   sudo apt update
   sudo apt install g++
   ```
2. **Create a Source File**
   - Save your C++ code to a file named ex1.cpp.
3. **Compile and Run the Program**
   ```bash
   g++ ex1.cpp -o ex1
   ./ex1
   ```
4. **Compare with Z3 Output**
  ```bash
   z3 z3_input.smt2 > res1
  ./ex1 z3_input.smt2 > res2
  diff res1 res2 && echo "Outputs are identical." || echo "Outputs differ."
  ```
