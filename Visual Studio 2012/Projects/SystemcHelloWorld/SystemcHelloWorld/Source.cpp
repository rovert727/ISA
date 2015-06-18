//-----------------------------------------------------
// This is my second Systemc Example
// Design Name : first_counter
// File Name : first_counter.cpp
// Function : This is a 4 bit up-counter with
// Synchronous active high reset and
// with active high enable signal
//-----------------------------------------------------
#include "systemc.h"

typedef union format{
    unsigned char v8[4];
    unsigned short v16[2];
    unsigned long v32;
}ieee_single;

SC_MODULE (IEEE_SingleP) {
  sc_in_clk     clock ;      // Clock input of the design
  sc_in<bool>   reset ;      // active high, synchronous Reset input
  sc_in<bool>   enable;      // Active high enable signal for counter
  sc_in<sc_uint<32> >   num1; 
  sc_in<sc_uint<32> >   num2; 
  sc_in<sc_uint<8> >   operand; 
  sc_out<sc_uint<4> > counter_out; // 4 bit vector output of the counter
  sc_out<sc_uint<32> > result_out; 

  //------------Local Variables Here---------------------
  sc_uint<4>	count;
  sc_uint<8> sign1,sign2,signa;
  ieee_single a,b,u,v,ansx,ans,m,n;


  //------------Code Starts Here-------------------------

  
  void op(){
	if (reset.read() == 1) {

		ans.v32 =  0;
		result_out.write(ans.v32);

	} else if (enable.read() == 1) {
    u.v32=num1.read();
    v.v32=num2.read();
  swap();
  
  switch(operand.read()){
     case 0://suma
         if(sign1 == sign2){
             sum();
         }
         else{
             rest();
         }
     break;
     case 1://resta
         if(sign1 == sign2){
             sign2 = sign2^1;
             rest();
         }
         else{
             sign2 = sign2^1;
             sum();
         }
     break;
     case 2://multiplicacion
         if((a.v32 == 0x00800000) || (b.v32 == 0x00800000))
            ansx.v32 = 0;
         else
             mult();
     break;
     case 3://divicion
         div();
     break;
  }
  ieee();
   result_out.write(ans.v32);
 cout<<"@" << sc_time_stamp() <<" :: resultado "
        <<result_out.read()<<endl;
	}
}


  void swap(){
  sign1 = u.v8[3]>>7;
  sign2 = v.v8[3]>>7;
  a.v8[3] = (u.v8[3]<<1) + (u.v8[2]>>7);
  b.v8[3] = (v.v8[3]<<1) + (v.v8[2]>>7);
  a.v8[2] = u.v8[2]|128;
  b.v8[2] = v.v8[2]|128;
  a.v16[0] = u.v16[0];
  b.v16[0] = v.v16[0];
}


  void sum(){
  uint8_t exp;
  signa = sign1;
  greater();
  exp = a.v8[3]-b.v8[3];
  if(exp<24){
     ans.v8[3] = a.v8[3];
     b.v8[3] = 0;
     a.v8[3] = 0;
     b.v32 = b.v32>>exp;
     a.v32 = a.v32 + b.v32;
     if(a.v8[3] == 1){
         ansx.v32 = a.v32>>1;
         ansx.v8[3] = ans.v8[3]+1;
     }
     else{
         ansx.v32 = a.v32;
         ansx.v8[3] = ans.v8[3];
     }
  }
  else{
     ansx.v32 = a.v32;
  }
}

  void rest(){
  uint8_t exp;
  greater();
  signa = sign1;
  exp = a.v8[3]-b.v8[3];
  if(exp<24){
     ans.v8[3] = a.v8[3];
     b.v8[3] = 0;
     a.v8[3] = 0;
     b.v32 = (b.v32>>exp);
     a.v32 = a.v32 - b.v32;
     if(a.v32 > 0){
         while(a.v8[2] < 128 && a.v8[3] > 0){
             ans.v8[3]--;
             a.v32 = a.v32<<1;
         }
         ansx.v32 = a.v32;
         ansx.v8[3] = ans.v8[3];
     }
     else{
         ansx.v32 = 0;
     }
  }
  else{
     ansx.v32 = a.v32;
  }
}

  void mult(){
    uint8_t i=0;
    signa = sign1 ^ sign2;
    ansx.v32 =(uint32_t) a.v16[0]*b.v16[0];
    ansx.v16[0]=ansx.v16[1];
    ansx.v16[1]=0;
    ans.v32 = (uint32_t) a.v8[2]*b.v16[0];
    m.v32 = (uint32_t) a.v16[0]*b.v8[2];
    ans.v32 = m.v32 + ans.v32;
    ansx.v32 = ansx.v32 + ans.v32;
    ans.v32 = (uint32_t) a.v8[2]*b.v8[2];
    ans.v8[3] = ans.v8[1];
    ans.v8[2] = ans.v8[0];
    ans.v8[0] = 0;
    ans.v8[1] = 0;
    ansx.v32 = ansx.v32 + ans.v32;
    if(ansx.v8[3] >127)
       i = 1;
       ansx.v32=ansx.v32>>(7+i);
       ansx.v32 = ansx.v32;
       uint16_t barry=(uint16_t) a.v8[3]+b.v8[3];
    if((barry > 0x007E) && (barry < 0x017F) ){
       if(a.v8[3] >127)
           ansx.v8[3]= (a.v8[3]-127+i)+b.v8[3];
       else if(b.v8[3] >127)
           ansx.v8[3]= (b.v8[3]-127+i)+a.v8[3];
       else

           ansx.v8[3]= a.v8[3]+b.v8[3]-127+i;
   }
    else{
       ansx.v32 = 0xFFFFFFFF;
    }
}

  void div(){
    signa = sign1 ^ sign2;
    int32_t expa,expb;
    m.v32 = a.v32<<8;
    n.v32=0;
    uint32_t ddd= (b.v32<<8) >>8;
    uint8_t msbit,i;
    for(i=0;i<47;i++){
       if(m.v32<0x80000000){
          msbit=0;
       }else{
          msbit=1;
       }
       m.v32=m.v32<<1;
       n.v32=(n.v32<<1)+ msbit;
       uint32_t A = n.v32;
       n.v32=n.v32-ddd;
       int32_t aux=n.v32;   
       if(aux<0x80000000 | aux==0){
          m.v32=((m.v32>>9<<1)|1)<<8;
       }
       else{
          m.v32=m.v32>>9; m.v32=m.v32<<9;
          n.v32=A;
       }
    }
    int8_t u=0;
    ansx.v32=m.v32;
    expa=a.v8[3]; expb=b.v8[3];
    if(ansx.v8[3]>127){
       ansx.v32=ansx.v32>>8;
    }
    else{
       ansx.v32=ansx.v32>>7;
       u--;
    }
    ansx.v8[3]=expa-expb+127+u; 
}

void greater(){
  if(a.v32 < b.v32){
     ansx.v32 = a.v32;
     a.v32 = b.v32;
     b.v32 = ansx.v32;
     ansx.v8[0] = sign2;
     sign2 = sign1;
     sign1 = ansx.v8[0];
  }
}


  void ieee(){
	ans.v8[3] = (signa<<7)+(ansx.v8[3]>>1);
	ans.v8[2] = (ansx.v8[3]<<7)+(ansx.v8[2]&127);
	ans.v16[0] = ansx.v16[0];
 }
 
  // Constructor for the counter
  // Since this counter is a positive edge trigged one,
  // We trigger the below block with respect to positive
  // edge of the clock and also when ever reset changes state
  SC_CTOR(IEEE_SingleP) {
    cout<<"Executing new"<<endl;
	SC_METHOD(op);
    sensitive << reset;
    sensitive << clock.pos();
  } // End of Constructor

}; // End of Module counter