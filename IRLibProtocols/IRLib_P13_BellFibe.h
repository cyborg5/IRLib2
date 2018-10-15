/* IRLib_P13_BellFibe.h
 * Part of IRLib2 Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * This file implements the protocol used by Bell Fibe slim remote controls
 * manufactured by Ruwido.
 *
 * Note that it is up to the user to implement the 0x00008000 toggle bit
 */
#define RUWIDO_HEAD_MARK 334
#define RUWIDO_DATA_MARK  86                                                                                                                      
#define RUWIDO_ZERO      360                                                                                                                        
#define RUWIDO_ONE       526                                                                                                                        
#define RUWIDO_TWO       700                                                                                                                        
#define RUWIDO_THREE     865 

#ifndef IRLIB_P13_H
#define IRLIB_PROTOCOL_13_H
#define IR_SEND_PROTOCOL_13		case 13: IRsendBellFibe::send(data); break;
#define IR_DECODE_PROTOCOL_13	if(IRdecodeBellFibe::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_13 ,public virtual IRdecodeBellFibe
	#define PV_IR_SEND_PROTOCOL_13   ,public virtual IRsendBellFibe
#else
	#define PV_IR_DECODE_PROTOCOL_13  public virtual IRdecodeBellFibe
	#define PV_IR_SEND_PROTOCOL_13    public virtual IRsendBellFibe
#endif

#ifdef IRLIBSENDBASE_H
class IRsendBellFibe: public virtual IRsendBase {
  public:
    void send(uint32_t data, uint8_t nBits= 32) {
      if (nBits==0) nBits=32;
      extent=0;
      data <<= (32 - nBits);
      nBits=nBits/2;
      enableIROut(36);
      mark(RUWIDO_HEAD_MARK); space(RUWIDO_ZERO);//Send header
      for (uint8_t i = 0; i < nBits; i++) {
        mark(RUWIDO_DATA_MARK);
        switch (data & 0xC0000000UL) {//use the leftmost two bits
          case 0x00000000UL: space(RUWIDO_ZERO); break;
          case 0x40000000UL: space(RUWIDO_ONE); break;
          case 0x80000000UL: space(RUWIDO_TWO); break;
          case 0xC0000000UL: space(RUWIDO_THREE); break;
        }
        data <<= 2;
      };
      mark(RUWIDO_DATA_MARK);  
      space(27778-extent);
    };
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
#define RUWIDO_TOLERANCE 80 
class IRdecodeBellFibe: public virtual IRdecodeBase {
  public:
    bool decode(void) {
      resetDecoder();//This used to be in the receiver getResults.
      IRLIB_ATTEMPT_MESSAGE(F("BellFibe"));
      if ( (recvGlobal.decodeLength!=(12+4)) && (recvGlobal.decodeLength!=(24+4)) && (recvGlobal.decodeLength!=(32+4)) ) return RAW_COUNT_ERROR;
      if (!ignoreHeader) if (!MATCH(recvGlobal.decodeBuffer[1],RUWIDO_HEAD_MARK)) return HEADER_MARK_ERROR(RUWIDO_HEAD_MARK);
      if (!MATCH(recvGlobal.decodeBuffer[2],RUWIDO_ZERO)) return HEADER_SPACE_ERROR(RUWIDO_ZERO);
      offset=3; uint32_t data=0;
      while (offset < (recvGlobal.decodeLength-1)) {
        if (!ABS_MATCH(recvGlobal.decodeBuffer[offset],RUWIDO_DATA_MARK,RUWIDO_TOLERANCE)) return DATA_MARK_ERROR(RUWIDO_DATA_MARK);
        offset++;
        if (ABS_MATCH(recvGlobal.decodeBuffer[offset],RUWIDO_ZERO, RUWIDO_TOLERANCE) ) { //Logical "0"
          data <<= 2;
        } 
        else if (ABS_MATCH(recvGlobal.decodeBuffer[offset],RUWIDO_ONE, RUWIDO_TOLERANCE) ) { //Logical "1"
          data = (data<<2) + 1;
        } 
        else if (ABS_MATCH(recvGlobal.decodeBuffer[offset],RUWIDO_TWO, RUWIDO_TOLERANCE) ) { //Logical "2"
          data = (data<<2) + 2;
        } 
        else if (ABS_MATCH(recvGlobal.decodeBuffer[offset],RUWIDO_THREE, RUWIDO_TOLERANCE) ) { //Logical "3"
          data = (data<<2) + 3;
        } 
        else return DATA_SPACE_ERROR(RUWIDO_ZERO);
        offset++;
      }
      if (!MATCH(recvGlobal.decodeBuffer[offset],RUWIDO_DATA_MARK))  return DATA_MARK_ERROR(RUWIDO_DATA_MARK);
      bits = recvGlobal.decodeLength-4;//set bit length
      value = data;//put remaining bits in value
      protocolNum=BELLFIBE;
      return true;
    }
};
#endif //IRLIBDECODEBASE_H

#define IRLIB_HAVE_COMBO

#endif //IRLIB_PROTOCOL_13_H
