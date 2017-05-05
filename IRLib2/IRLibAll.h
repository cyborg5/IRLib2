/* IRLibAll.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * In previous versions of IRLib you would #include <IRLib.h> and it would 
 *   include everything from the library into your sketch. Beginning with 
 *   version 2.0+ the design is more modular. You should only include those
 *   header files that you actually need. See documentation for further details.
 *
 * The files IRLib2.h and IRLibAll.h have been provided to make it easy to re-create
 *   the old "everything at once" approach of IRLib.h version 1.xx. In general
 *   WE RECOMMEND YOU DO NOT USE THIS APPROACH. We provide this file for a small 
 *   measure of backwards compatibility. Use of this file will make your program 
 *   potentially much larger than necessary. 
 * The file IRLib2.h will include only the original 7 protocols from IRLib 1.xx.
 * The file IRLibAll.h will include all currently supported protocols.
 */

#ifndef IRLIB_ALL_H
#define IRLIB_ALL_H
#include <IRLibDecodeBase.h>
#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLib_P02_Sony.h>
#include <IRLib_P03_RC5.h>
#include <IRLib_P04_RC6.h>
#include <IRLib_P05_Panasonic_Old.h>
#include <IRLib_P06_JVC.h>
#include <IRLib_P07_NECx.h>
#include <IRLib_P08_Samsung36.h>
#include <IRLib_P09_GICable.h>
#include <IRLib_P10_DirecTV.h>
#include <IRLib_P11_RCMM.h>
#include <IRLib_P12_CYKM.h>
//include additional protocols here
#include <IRLibCombo.h>
#include <IRLibRecv.h>
#include <IRLibRecvLoop.h>
#include <IRLibRecvPCI.h>
#include <IRLibFreq.h>
#endif //IRLIB_ALL_H
