# FP_display_for_UNO_V2

An improvement on "Floating point display for UNO", it uses a single IC rather than two, the ATtiny1606.  

The ATtiny 1606 has 20 pins which are used as follows:

Two for power

Eight to drive the digits

Eight to drive the segments

One for programming

One for a simplex firmare UART used to communicate with the UNO


The ATtiny1608 has a UPDI programming interface and programming will be carried out using the "UNO as UPDI programmer" sketch.
