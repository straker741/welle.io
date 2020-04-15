/**
 * Thanks to david may for his project rtl-dab
 * which this work is inspired by.
 * 
 * Original Creator:
 * david.may.muc@googlemail.com
 * 
 * Implemented:
 * jakub.svajka98@gmail.com
 */

#ifndef ANALYZER_H
#define ANALYZER_H


#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include "dab-constants.h"
#include "MySQL-handler.h"

extern "C" {
    #include "viterbi2.h"
}

/**
 * The bit error rate (BER) is the number of bit errors per unit time.
 * The bit error ratio (also BER) is the number of bit errors divided
 * by the total number of transferred bits during a studied time interval.
 * Bit error ratio is a unitless performance measure, often expressed as a percentage.
 */
struct BitErrorRate
{
    uint32_t    receivedFIBs;
    uint32_t    faultyFIBs;
    float       faultyFIBs_rate;
    double      BER;
};


class Analyzer
{
    public:
        Analyzer();
        BitErrorRate    ber;
        void            feed_corrected_data(uint8_t *fib);
        void            feed_uncorrected_data(uint8_t *fib, int16_t ficno);
        void            calculateBER();

    private:    
        MySQLhandler    db;

        void            dab_bit_to_byte(uint8_t * in, uint8_t * out, uint32_t len);
        void            puncture(uint8_t *ibuf, uint8_t *obuf);
        uint8_t         FIC[1536*2*3];                  // Uncorrected data
        uint8_t         FIC_dep_dec[768*4];             // Depunctured and convolutionaly decoded data
        uint8_t         FIC_dep_dec_byte[(768*4)/8];    // Depunctured and convolutionaly decoded data in bytes == Corrected data
        uint8_t         FIC_dep_enc[3096*4];            // Convolutionaly encoded corrected data
        uint8_t         FIC_enc_pun[1536*2*3];          // Convolutionaly encoded corrected and punctured data
};


#endif
