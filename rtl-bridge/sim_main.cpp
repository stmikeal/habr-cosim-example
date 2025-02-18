#include "Vtestbench.h"
#include "svdpi.h"
#include "verilated.h"

void sk_connect();
void sk_сlose();

int main(int argc, char** argv) {
  VerilatedContext* contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  Vtestbench* top = new Vtestbench{contextp};
  sk_connect();
  while (!contextp->gotFinish()) {
    top->eval();
    contextp->time(top->nextTimeSlot());
  }
  void sk_close();
  delete top;
  delete contextp;
  return 0;
}
