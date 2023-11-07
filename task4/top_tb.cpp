#include "Vtop.h"             
#include "verilated.h"        
#include "verilated_vcd_c.h"  
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env) {
    int i; 
    int clk;
    Verilated::commandArgs(argc, argv);
    
    Vtop* top_module = new Vtop;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top_module->trace(tfp, 99);
    tfp->open("top.vcd");
    
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab1: BCD");

    top_module->clk = 1;     
    top_module->rst = 1;     
    top_module->en = 0;
    top_module->v = vbdValue();       

    for (i = 0; i < 1000; i++) {
        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2 * i + clk);
            top_module->clk = !top_module->clk;
            top_module->eval();
        }

        //++++ send BCD value to VBuddy
        vbdHex(4, (int(top_module->bcd) >> 8) & 0xF);
        vbdHex(3, (int(top_module->bcd) >> 4) & 0xF);
        vbdHex(2, int(top_module->bcd) & 0xF);

        //vbdPlot(int(top_module->bcd), 0, 999);  
        vbdCycle(i+1);

        top_module->rst = (i<2);
        top_module->en = vbdFlag();
        top_module->v = vbdValue()
        if (Verilated::gotFinish())  exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
