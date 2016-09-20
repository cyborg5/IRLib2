/* IRLibFreq.h 
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This class facilitates detection of frequency of an IR signal. It requires
 * a different kind of receiver specifically a TSMP58000 or equivalent device 
 * connected to the hardware interrupt pin. Create an instance of the object 
 * passing the interrupt number.
 */
#ifndef IRLibRecvFreq_h
#define IRLibRecvFreq_h

//Un-comment only one of the following three lines depending on available RAM
//#define FREQUENCY_BUFFER_TYPE uint8_t
#define FREQUENCY_BUFFER_TYPE uint16_t
//#define FREQUENCY_BUFFER_TYPE uint32_t
class IRfrequency {
  public:
    IRfrequency(uint8_t pin); 
    void enableFreqDetect(void);
    bool haveData(void);      //detect if data is received
    void disableFreqDetect(void);
    void computeFreq(void);	//computes but does not print results
    void dumpResults(bool detail);	//computes and prints result
    double results; //results in kHz
    uint8_t samples; //number of samples used in computation
  private:
    volatile FREQUENCY_BUFFER_TYPE Time_Stamp[256];
    uint8_t intrNum;
    uint16_t i;
    uint32_t sum;
};
#endif // ifdef IRLibRecvFreq_h

