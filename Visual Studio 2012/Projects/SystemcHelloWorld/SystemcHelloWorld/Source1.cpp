#include "systemc.h"
#include "Source.cpp"

int sc_main (int argc, char* argv[]) {
  sc_signal<bool>   clock;
  sc_signal<bool>   reset;
  sc_signal<bool>   enable;
  sc_signal<sc_uint<4> > counter_out;
  sc_signal<sc_uint<32>> num1;
  sc_signal<sc_uint<32>> num2;
  sc_signal<sc_uint<8>> op;
  sc_signal<sc_uint<32>> result_out;

  // Connect the DUT
  IEEE_SingleP counter("COUNTER");
    counter.clock(clock);
    counter.reset(reset);
    counter.enable(enable);
    counter.counter_out(counter_out);
	counter.num1(num1);
	counter.num2(num2);
	counter.operand(op);
	counter.result_out(result_out);

  sc_start();

  // Open VCD file
  sc_trace_file *wf = sc_create_vcd_trace_file("counter");
  // Dump the desired signals
  sc_trace(wf, clock, "clock");
  sc_trace(wf, reset, "reset");
  sc_trace(wf, enable, "enable");
  sc_trace(wf, counter_out, "count");
  sc_trace(wf, num1, "num1");
  sc_trace(wf, num2, "num2");
  sc_trace(wf, op, "op");
  sc_trace(wf, result_out, "result_out");

  // Initialize all variables
  reset = 0;       // initial value of reset
  enable = 1;      // initial value of enable
  num1 = (sc_uint<32>) 0x40333333;
  num2 = (sc_uint<32>) 0x3fb33333;
  op=2;
  clock = 0; 
  sc_start();
  clock = 1; 
  sc_start();

  clock = 0; 
  sc_start();
  clock = 1; 
  sc_start();

  cout << "@" << sc_time_stamp() <<" Terminating simulation\n" << endl;
  sc_close_vcd_trace_file(wf);

  system("pause");
  return 0;// Terminate simulation

 }