#include "analyzer.h"

// Constructor
Analyzer::Analyzer() : db() {
    ber.receivedFIBs = 0;
    ber.faultyFIBs = 0;
    ber.faultyFIBs_rate = 0;
    ber.BER = 0;
    ber.meanBER = 0;
}

/**
 * Converts bits to bytes
 * Taken from rtl-dab
 * https://github.com/maydavid/rtl-dab
 */
void Analyzer::dab_bit_to_byte(uint8_t * in, uint8_t * out,uint32_t len)
{
    uint32_t i;
    uint32_t bc=0;
    for (i=0;i<len;i+=8) {
        out[bc] = (in[i+0]<<7) + (in[i+1]<<6) + (in[i+2]<<5) + (in[i+3]<<4) +   
            (in[i+4]<<3) + (in[i+5]<<2) + (in[i+6]<<1) +(in[i+7]<<0);
        bc++;
    }
}

void Analyzer::feed_corrected_data(uint8_t *fib)
{
    memcpy(FIC_dep_dec + 256 * (ber.receivedFIBs % 12) * sizeof(uint8_t), fib, 256 * 3 * sizeof(uint8_t));
}

void Analyzer::feed_uncorrected_data(uint8_t *fib, int16_t ficno)
{
    memcpy(FIC + 2304 * ficno * sizeof(uint8_t), fib, 2304 * sizeof(uint8_t));
}

/**
 * Taken from openDAB
 * https://sourceforge.net/projects/opendab/
 */
void Analyzer::puncture(uint8_t *ibuf, uint8_t *obuf)
{
    int i, j, k;
    // 21 blocks, using puncture 1110 1110 1110 1110 1110 1110 1110 1110
    //  3 blocks, using puncture 1110 1110 1110 1110 1110 1110 1110 1100
    // 24 bits,   using puncture 1100 1100 1100 1100 1100 1100
    k = 0;
    for (i=0; i<21*128; i+=32)
    {
        for (j=0; j<8; j++)
        {
            obuf[k+0] = ibuf[i+j*4+0];
            obuf[k+1] = ibuf[i+j*4+1];
            obuf[k+2] = ibuf[i+j*4+2];
            k+=3;
        }
    }
    for (i=21*128; i<24*128; i+=32)
    {
        for (j=0; j<7; j++)
        {
            obuf[k+0] = ibuf[i+j*4+0];
            obuf[k+1] = ibuf[i+j*4+1];
            obuf[k+2] = ibuf[i+j*4+2];
            k+=3;
        }
        obuf[k+0] = ibuf[i+j*4+0];
        obuf[k+1] = ibuf[i+j*4+1];
        k+=2;
    }
    for (j=0; j<6; j++)
    {
        obuf[k+0] = ibuf[i+j*4+0];
        obuf[k+1] = ibuf[i+j*4+1];
        k+=2;
    }
}

void Analyzer::calculateBER()
{
    int i, temp, faultyBits = 0;

    dab_bit_to_byte(&FIC_dep_dec[0], &FIC_dep_dec_byte[0], (768*4));

    encode(&FIC_dep_enc[0*3096], &FIC_dep_dec_byte[0*768/8], 768/8, 0, 0);
    encode(&FIC_dep_enc[1*3096], &FIC_dep_dec_byte[1*768/8], 768/8, 0, 0);
    encode(&FIC_dep_enc[2*3096], &FIC_dep_dec_byte[2*768/8], 768/8, 0, 0);
    encode(&FIC_dep_enc[3*3096], &FIC_dep_dec_byte[3*768/8], 768/8, 0, 0);

    puncture(&FIC_dep_enc[0*3096], &FIC_enc_pun[0*2304]);
    puncture(&FIC_dep_enc[1*3096], &FIC_enc_pun[1*2304]);
    puncture(&FIC_dep_enc[2*3096], &FIC_enc_pun[2*2304]);
    puncture(&FIC_dep_enc[3*3096], &FIC_enc_pun[3*2304]);

    //Debugging
    if (0) {
        for (i=0; i<768; i++) {
            fprintf(stderr, "Re: FIC: %d | FIC_enc_pun: %d\n", FIC[i], FIC_enc_pun[i]);
            fprintf(stderr, "Im: FIC: %d | FIC_enc_pun: %d\n", FIC[i+1536], FIC_enc_pun[i+1536]);
        }
    }

    // By comparing  FIC  and  FIC_enc_pun  we are able to estimate BER
    for (i=0; i<1536*2*3; i++) {
        temp = (FIC[i] - FIC_enc_pun[i]);
        if (temp!=0) {
            faultyBits++;
        }
    }

    // (pseudo) channel BER
    ber.BER = (float)faultyBits/(float)(1536*2*3);
    ber.meanBER = ber.BER*ALPHA + (1 - ALPHA)*ber.meanBER;
    ber.faultyFIBs_rate = ber.faultyFIBs/(float)ber.receivedFIBs;

    if (ber.receivedFIBs > 119) {
        fprintf(stderr, "meanBER: %f\n", ber.meanBER);
        db.executeInsert(ber.meanBER, "BitErrorRatio");
        ber.receivedFIBs = 0;
        ber.faultyFIBs = 0;
    }
}
