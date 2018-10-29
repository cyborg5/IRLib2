/* IRLib_P14_RCA.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */

/*
 * This is code implements the TV IR codes for an RCA CRCRN04GR remote control.
 */
#define RCA_HEAD_MARK 3950
#define RCA_HEAD_SPACE 3950
#define RCA_DATA_MARK  470
#define RCA_ZERO      1000
#define RCA_ONE       2020
#define RCA_TOLERANCE 100

#ifndef IRLIB_PROTOCOL_14_H
#define IRLIB_PROTOCOL_14_H
#define IR_SEND_PROTOCOL_14		case 14: IRsendRCA::send(data); break;
#define IR_DECODE_PROTOCOL_14	if(IRdecodeRCA::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_14 ,public virtual IRdecodeRCA
	#define PV_IR_SEND_PROTOCOL_14   ,public virtual IRsendRCA
#else
	#define PV_IR_DECODE_PROTOCOL_14  public virtual IRdecodeRCA
	#define PV_IR_SEND_PROTOCOL_14    public virtual IRsendRCA
#endif

#ifdef IRLIBSENDBASE_H
class IRsendRCA: public virtual IRsendBase {
  public:
    void send(uint32_t data, uint8_t kHz=38) {
        if (data==REPEAT_CODE) {
            sendGeneric(data,24, 0, 0, RCA_DATA_MARK+80, RCA_DATA_MARK+80, RCA_ONE-100, RCA_ZERO-100, kHz, true);
        } else {
            sendGeneric(data,24, RCA_HEAD_MARK, RCA_HEAD_SPACE, RCA_DATA_MARK+80, RCA_DATA_MARK+80, RCA_ONE-100,
                        RCA_ZERO-100, kHz, true);
        }
    };
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodeRCA: public virtual IRdecodeBase {
  public:
    bool decode(void) {
        resetDecoder();//This used to be in the receiver getResults.
        IRLIB_ATTEMPT_MESSAGE(F("RCA"));

        if ( (recvGlobal.decodeLength!=(52)) && (recvGlobal.decodeLength!=(50)) ){
            return RAW_COUNT_ERROR;
        }

        if ( (!decodeGeneric(52, RCA_HEAD_MARK, RCA_HEAD_SPACE, RCA_DATA_MARK , RCA_ONE, RCA_ZERO)) &&
             (!decodeGeneric(50, 0, 0, RCA_DATA_MARK , RCA_ONE, RCA_ZERO)) ) return false;

        protocolNum=RCA;
        return true;
    }
};
#endif //IRLIBDECODEBASE_H
#endif IRLIB_PROTOCOL_14_H
