#include "../Inc/verification_service.h"

SAT_returnState verify_pkt(tc_tm_pkt *pkt, uint8_t step, uint8_t res) {
/*this is TEMP*/
tc_tm_pkt out;

	if(pkt->type != TC ) {
		return SATR_ERROR;
	}

	if(pkt->ack == TC_ACK_NO) {
		return SATR_OK;
	} else if(pkt->ack == TC_ACK_ACC && step == TC_ACK_ACC) {
		verify_crt_pkt(pkt, &out, res, 1);
        return SATR_OK;
	} else if(pkt->ack == TC_ACK_EXE_COMP && step == TC_ACK_EXE_COMP) {
		verify_crt_pkt(pkt, &out, res, 7);
        return SATR_OK;
	} else if(pkt->ack == TC_ACK_ALL || pkt->ack == TC_ACK_EXE_PROG || pkt->ack == TC_ACK_EXE_START) {
		return SATR_ERROR;
	} else {
		return SATR_ERROR;
	}

	return SATR_ERROR;
}

SAT_returnState verify_crt_pkt(tc_tm_pkt *pkt, tc_tm_pkt *out, uint8_t res, uint8_t subtype) {

	union _cnv cnv;

	out->type = TM;
	out->app_id = pkt->dest_id; 
	out->dest_id = pkt->app_id;

	cnv.cnv16[0] = pkt->app_id;
	out->data[0] = ( ECSS_VER_NUMBER << 5 | pkt->type << 4 | ECSS_DATA_FIELD_HDR_FLG << 3 | cnv.cnv8[1]);
	out->data[1] = cnv.cnv8[0];
	
	cnv.cnv16[0] = pkt->seq_count;
	out->data[2] = (pkt->seq_flags << 6 | cnv.cnv8[1]);
	out->data[3] = cnv.cnv8[0];

	out->ser_type = TC_VERIFICATION_SERVICE;
      
	if(res != SATR_OK) {
		out->data[4] = res;
		subtype++;
	}
	out->ser_subtype = subtype;

	return SATR_OK;
}