#ifndef DWMAC_DESC_RX_REGS_H
#define DWMAC_DESC_RX_REGS_H

#include <stdint.h>

#define DWMAC_DESC_BIT32(bit) \
  ((uint32_t) (((uint32_t) 1) << (bit)))

#define DWMAC_DESC_MSK32(first_bit, last_bit) \
  ((uint32_t) ((DWMAC_DESC_BIT32((last_bit) - (first_bit) + 1) - 1) << (first_bit)))

#define DWMAC_DESC_FLD32(val, first_bit, last_bit) \
  ((uint32_t) \
    ((((uint32_t) (val)) << (first_bit)) & DWMAC_DESC_MSK32(first_bit, last_bit)))

#define DWMAC_DESC_FLD32GET(reg, first_bit, last_bit) \
  ((uint32_t) (((reg) & DWMAC_DESC_MSK32(first_bit, last_bit)) >> (first_bit)))

#define DWMAC_DESC_FLD32SET(reg, val, first_bit, last_bit) \
  ((uint32_t) (((reg) & ~DWMAC_DESC_MSK32(first_bit, last_bit)) \
    | DWMAC_DESC_FLD32(val, first_bit, last_bit)))

typedef struct {
	uint32_t des0;
#define DWMAC_DESC_RX_DES0_OWN_BIT DWMAC_DESC_BIT32(31)
#define DWMAC_DESC_RX_DES0_DEST_ADDR_FILTER_FAIL DWMAC_DESC_BIT32(30)
#define DWMAC_DESC_RX_DES0_FRAME_LENGTH(val) DWMAC_DESC_FLD32(val, 16, 29)
#define DWMAC_DESC_RX_DES0_FRAME_LENGTH_GET(reg) DWMAC_DESC_FLD32GET(reg, 16, 29)
#define DWMAC_DESC_RX_DES0_FRAME_LENGTH_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 16, 29)
#define DWMAC_DESC_RX_DES0_ERROR_SUMMARY DWMAC_DESC_BIT32(15)
#define DWMAC_DESC_RX_DES0_DESCRIPTOR_ERROR DWMAC_DESC_BIT32(14)
#define DWMAC_DESC_RX_DES0_SRC_ADDR_FILTER_FAIL DWMAC_DESC_BIT32(13)
#define DWMAC_DESC_RX_DES0_LENGTH_ERROR DWMAC_DESC_BIT32(12)
#define DWMAC_DESC_RX_DES0_OVERFLOW_ERROR DWMAC_DESC_BIT32(11)
#define DWMAC_DESC_RX_DES0_VLAN_TAG DWMAC_DESC_BIT32(10)
#define DWMAC_DESC_RX_DES0_FIRST_DESCRIPTOR DWMAC_DESC_BIT32(9)
#define DWMAC_DESC_RX_DES0_LAST_DESCRIPTOR DWMAC_DESC_BIT32(8)
#define DWMAC_DESC_RX_DES0_CHECKSUM_ERROR DWMAC_DESC_BIT32(7)
#define DWMAC_DESC_RX_DES0_LATE_COLLISION DWMAC_DESC_BIT32(6)
#define DWMAC_DESC_RX_DES0_FRAME_TYPE DWMAC_DESC_BIT32(5)
#define DWMAC_DESC_RX_DES0_RECEIVE_WATCHDOG_TIMEOUT DWMAC_DESC_BIT32(4)
#define DWMAC_DESC_RX_DES0_RECEIVE_ERROR DWMAC_DESC_BIT32(3)
#define DWMAC_DESC_RX_DES0_DRIBBLE_BIT_ERROR DWMAC_DESC_BIT32(2)
#define DWMAC_DESC_RX_DES0_CRC_ERROR DWMAC_DESC_BIT32(1)
#define DWMAC_DESC_RX_DES0_RX_MAC_ADDR_OR_PAYLOAD_CHECKSUM_ERROR DWMAC_DESC_BIT32(0)
	uint32_t des1;
#define DWMAC_DESC_RX_DES1_DISABLE_IRQ_ON_COMPLETION DWMAC_DESC_BIT32(31)
#define DWMAC_DESC_RX_DES1_RECEIVE_END_OF_RING DWMAC_DESC_BIT32(25)
#define DWMAC_DESC_RX_DES1_SECOND_ADDR_CHAINED DWMAC_DESC_BIT32(24)
#define DWMAC_DESC_RX_DES1_RECEIVE_BUFFER_2_SIZE(val) DWMAC_DESC_FLD32(val, 11, 21)
#define DWMAC_DESC_RX_DES1_RECEIVE_BUFFER_2_SIZE_GET(reg) DWMAC_DESC_FLD32GET(reg, 11, 21)
#define DWMAC_DESC_RX_DES1_RECEIVE_BUFFER_2_SIZE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 11, 21)
#define DWMAC_DESC_RX_DES1_RECIVE_BUFFER_1_SIZE(val) DWMAC_DESC_FLD32(val, 0, 10)
#define DWMAC_DESC_RX_DES1_RECIVE_BUFFER_1_SIZE_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 10)
#define DWMAC_DESC_RX_DES1_RECIVE_BUFFER_1_SIZE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 10)
	uint32_t des2;
#define DWMAC_DESC_RX_DES2_BUFF_1_ADDR_PTR(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_RX_DES2_BUFF_1_ADDR_PTR_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_RX_DES2_BUFF_1_ADDR_PTR_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
#define DWMAC_DESC_RX_DES2_IEEE_RECEIVE_FRAME_TIMESTAMP_LOW(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_RX_DES2_IEEE_RECEIVE_FRAME_TIMESTAMP_LOW_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_RX_DES2_IEEE_RECEIVE_FRAME_TIMESTAMP_LOW_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
	uint32_t des3;
#define DWMAC_DESC_RX_DES3_BUFF_2_ADDR_PTR(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_RX_DES3_BUFF_2_ADDR_PTR_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_RX_DES3_BUFF_2_ADDR_PTR_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
#define DWMAC_DESC_RX_DES3_IEEE_RECEIVE_FRAME_TIMESTAMP_LOW(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_RX_DES3_IEEE_RECEIVE_FRAME_TIMESTAMP_LOW_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_RX_DES3_IEEE_RECEIVE_FRAME_TIMESTAMP_LOW_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
} dwmac_desc_rx;

typedef struct {
	uint32_t des0;
#define DWMAC_DESC_TX_DES0_OWN_BIT DWMAC_DESC_BIT32(31)
#define DWMAC_DESC_TX_DES0_TX_TIMESTAMP_STATUS DWMAC_DESC_BIT32(17)
#define DWMAC_DESC_TX_DES0_IP_HEADER_ERROR DWMAC_DESC_BIT32(16)
#define DWMAC_DESC_TX_DES0_ERROR_SUMMARY DWMAC_DESC_BIT32(15)
#define DWMAC_DESC_TX_DES0_JABBER_TIMEOUT DWMAC_DESC_BIT32(14)
#define DWMAC_DESC_TX_DES0_FRAME_FLUSHED DWMAC_DESC_BIT32(13)
#define DWMAC_DESC_TX_DES0_PAYLOAD_CHECKSUM_ERROR DWMAC_DESC_BIT32(12)
#define DWMAC_DESC_TX_DES0_LOSS_OF_CARRIER DWMAC_DESC_BIT32(11)
#define DWMAC_DESC_TX_DES0_NO_CARRIER DWMAC_DESC_BIT32(10)
#define DWMAC_DESC_TX_DES0_EXCESSIVE_COLLISION DWMAC_DESC_BIT32(8)
#define DWMAC_DESC_TX_DES0_VLAN_FRAME DWMAC_DESC_BIT32(7)
#define DWMAC_DESC_TX_DES0_COLLISION_TIMEOUT(val) DWMAC_DESC_FLD32(val, 3, 6)
#define DWMAC_DESC_TX_DES0_COLLISION_TIMEOUT_GET(reg) DWMAC_DESC_FLD32GET(reg, 3, 6)
#define DWMAC_DESC_TX_DES0_COLLISION_TIMEOUT_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 3, 6)
#define DWMAC_DESC_TX_DES0_EXCESSIVE_DEFERAL DWMAC_DESC_BIT32(2)
#define DWMAC_DESC_TX_DES0_UNDERFLOW_ERROR DWMAC_DESC_BIT32(1)
#define DWMAC_DESC_TX_DES0_DEFERED_BIT DWMAC_DESC_BIT32(0)
	uint32_t des1;
#define DWMAC_DESC_TX_DES1_IRQ_ON_COMPLETION DWMAC_DESC_BIT32(31)
#define DWMAC_DESC_TX_DES1_LAST_SEGMENT DWMAC_DESC_BIT32(30)
#define DWMAC_DESC_TX_DES1_FIRST_SEGMENT DWMAC_DESC_BIT32(29)
#define DWMAC_DESC_TX_DES1_CHECKSUM_INSERTION_CONTROL(val) DWMAC_DESC_FLD32(val, 27, 28)
#define DWMAC_DESC_TX_DES1_CHECKSUM_INSERTION_CONTROL_GET(reg) DWMAC_DESC_FLD32GET(reg, 27, 28)
#define DWMAC_DESC_TX_DES1_CHECKSUM_INSERTION_CONTROL_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 27, 28)
#define DWMAC_DESC_TX_DES1_DISABLE_CRC DWMAC_DESC_BIT32(26)
#define DWMAC_DESC_TX_DES1_TRANSMIT_END_OF_RING DWMAC_DESC_BIT32(25)
#define DWMAC_DESC_TX_DES1_SECOND_ADDRESS_CHAINED DWMAC_DESC_BIT32(24)
#define DWMAC_DESC_TX_DES1_DISABLE_PADDING DWMAC_DESC_BIT32(23)
#define DWMAC_DESC_TX_DES1_TRANSMIT_TIMESTAMP_ENABLE DWMAC_DESC_BIT32(22)
#define DWMAC_DESC_TX_DES1_TRANMIT_BUFFER_2_SIZE(val) DWMAC_DESC_FLD32(val, 11, 21)
#define DWMAC_DESC_TX_DES1_TRANMIT_BUFFER_2_SIZE_GET(reg) DWMAC_DESC_FLD32GET(reg, 11, 21)
#define DWMAC_DESC_TX_DES1_TRANMIT_BUFFER_2_SIZE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 11, 21)
#define DWMAC_DESC_TX_DES1_TRANSMIT_BUFFER_1_SIZE(val) DWMAC_DESC_FLD32(val, 0, 10)
#define DWMAC_DESC_TX_DES1_TRANSMIT_BUFFER_1_SIZE_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 10)
#define DWMAC_DESC_TX_DES1_TRANSMIT_BUFFER_1_SIZE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 10)
	uint32_t des2;
#define DWMAC_DESC_TX_DES2_BUFF_1_ADDR_PTR(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_TX_DES2_BUFF_1_ADDR_PTR_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_TX_DES2_BUFF_1_ADDR_PTR_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
#define DWMAC_DESC_TX_DES2_IEEE_TRANSMIT_FRAME_TIMESTAMP_LOW(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_TX_DES2_IEEE_TRANSMIT_FRAME_TIMESTAMP_LOW_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_TX_DES2_IEEE_TRANSMIT_FRAME_TIMESTAMP_LOW_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
	uint32_t des3;
#define DWMAC_DESC_TX_DES3_BUFF_2_ADDR_PTR(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_TX_DES3_BUFF_2_ADDR_PTR_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_TX_DES3_BUFF_2_ADDR_PTR_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
#define DWMAC_DESC_TX_DES3_IEEE_TRANSMIT_FRAME_TIMESTAMP_LOW(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_TX_DES3_IEEE_TRANSMIT_FRAME_TIMESTAMP_LOW_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_TX_DES3_IEEE_TRANSMIT_FRAME_TIMESTAMP_LOW_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
} dwmac_desc_tx;

typedef struct {
	uint32_t des0;
#define DWMAC_DESC_ERX_DES0_OWN_BIT DWMAC_DESC_BIT32(31)
#define DWMAC_DESC_ERX_DES0_DEST_ADDR_FILTER_FAIL DWMAC_DESC_BIT32(30)
#define DWMAC_DESC_ERX_DES0_FRAME_LENGTH(val) DWMAC_DESC_FLD32(val, 16, 29)
#define DWMAC_DESC_ERX_DES0_FRAME_LENGTH_GET(reg) DWMAC_DESC_FLD32GET(reg, 16, 29)
#define DWMAC_DESC_ERX_DES0_FRAME_LENGTH_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 16, 29)
#define DWMAC_DESC_ERX_DES0_ERROR_SUMMARY DWMAC_DESC_BIT32(15)
#define DWMAC_DESC_ERX_DES0_DESCRIPTOR_ERROR DWMAC_DESC_BIT32(14)
#define DWMAC_DESC_ERX_DES0_SRC_ADDR_FILTER_FAIL DWMAC_DESC_BIT32(13)
#define DWMAC_DESC_ERX_DES0_LENGTH_ERROR DWMAC_DESC_BIT32(12)
#define DWMAC_DESC_ERX_DES0_OVERFLOW_ERROR DWMAC_DESC_BIT32(11)
#define DWMAC_DESC_ERX_DES0_VLAN_TAG DWMAC_DESC_BIT32(10)
#define DWMAC_DESC_ERX_DES0_FIRST_DESCRIPTOR DWMAC_DESC_BIT32(9)
#define DWMAC_DESC_ERX_DES0_LAST_DESCRIPTOR DWMAC_DESC_BIT32(8)
#define DWMAC_DESC_ERX_DES0_TIMESTAMP_AVAIL_OR_CHECKSUM_ERROR_OR_GIANT_FRAME DWMAC_DESC_BIT32(7)
#define DWMAC_DESC_ERX_DES0_LATE_COLLISION DWMAC_DESC_BIT32(6)
#define DWMAC_DESC_ERX_DES0_FREAME_TYPE DWMAC_DESC_BIT32(5)
#define DWMAC_DESC_ERX_DES0_RECEIVE_WATCHDOG_TIMEOUT DWMAC_DESC_BIT32(4)
#define DWMAC_DESC_ERX_DES0_RECEIVE_ERROR DWMAC_DESC_BIT32(3)
#define DWMAC_DESC_ERX_DES0_DRIBBLE_BIT_ERROR DWMAC_DESC_BIT32(2)
#define DWMAC_DESC_ERX_DES0_CRC_ERROR DWMAC_DESC_BIT32(1)
#define DWMAC_DESC_ERX_DES0_EXT_STATUS_AVAIL_OR_RX_MAC_ADDR_STATUS DWMAC_DESC_BIT32(0)
	uint32_t des1;
#define DWMAC_DESC_ERX_DES1_DISABLE_IRQ_ON_COMPLETION DWMAC_DESC_BIT32(31)
#define DWMAC_DESC_ERX_DES1_RECEIVE_BUFF_2_SIZE(val) DWMAC_DESC_FLD32(val, 16, 28)
#define DWMAC_DESC_ERX_DES1_RECEIVE_BUFF_2_SIZE_GET(reg) DWMAC_DESC_FLD32GET(reg, 16, 28)
#define DWMAC_DESC_ERX_DES1_RECEIVE_BUFF_2_SIZE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 16, 28)
#define DWMAC_DESC_ERX_DES1_RECEIVE_END_OF_RING DWMAC_DESC_BIT32(15)
#define DWMAC_DESC_ERX_DES1_SECOND_ADDR_CHAINED DWMAC_DESC_BIT32(14)
#define DWMAC_DESC_ERX_DES1_RECEIVE_BUFF_1_SIZE(val) DWMAC_DESC_FLD32(val, 0, 12)
#define DWMAC_DESC_ERX_DES1_RECEIVE_BUFF_1_SIZE_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 12)
#define DWMAC_DESC_ERX_DES1_RECEIVE_BUFF_1_SIZE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 12)
	uint32_t des2;
#define DWMAC_DESC_ERX_DES2_BUFF_1_ADDR_PTR(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_ERX_DES2_BUFF_1_ADDR_PTR_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_ERX_DES2_BUFF_1_ADDR_PTR_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
	uint32_t des3;
#define DWMAC_DESC_ERX_DES3_BUFF_2_ADDR_PTR(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_ERX_DES3_BUFF_2_ADDR_PTR_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_ERX_DES3_BUFF_2_ADDR_PTR_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
} dwmac_desc_erx;

typedef struct {
	uint32_t des0;
#define DWMAC_DESC_ETX_DES0_OWN_BIT DWMAC_DESC_BIT32(31)
#define DWMAC_DESC_ETX_DES0_IRQ_ON_COMPLETION DWMAC_DESC_BIT32(30)
#define DWMAC_DESC_ETX_DES0_LAST_SEGMENT DWMAC_DESC_BIT32(29)
#define DWMAC_DESC_ETX_DES0_FIRST_SEGMENT DWMAC_DESC_BIT32(28)
#define DWMAC_DESC_ETX_DES0_DISABLE_CRC DWMAC_DESC_BIT32(27)
#define DWMAC_DESC_ETX_DES0_DISABLE_PAD DWMAC_DESC_BIT32(26)
#define DWMAC_DESC_ETX_DES0_TRANSMIT_TIMESTAMP_ENABLE DWMAC_DESC_BIT32(25)
#define DWMAC_DESC_ETX_DES0_CHECKSUM_INSERTION_CONTROL(val) DWMAC_DESC_FLD32(val, 22, 23)
#define DWMAC_DESC_ETX_DES0_CHECKSUM_INSERTION_CONTROL_GET(reg) DWMAC_DESC_FLD32GET(reg, 22, 23)
#define DWMAC_DESC_ETX_DES0_CHECKSUM_INSERTION_CONTROL_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 22, 23)
#define DWMAC_DESC_ETX_DES0_TRANSMIT_END_OF_RING DWMAC_DESC_BIT32(21)
#define DWMAC_DESC_ETX_DES0_SECOND_ADDR_CHAINED DWMAC_DESC_BIT32(20)
#define DWMAC_DESC_ETX_DES0_TRANSMIT_TIMESTAMP_STATUS DWMAC_DESC_BIT32(17)
#define DWMAC_DESC_ETX_DES0_IP_HEADER_ERROR DWMAC_DESC_BIT32(16)
#define DWMAC_DESC_ETX_DES0_ERROR_SUMMARY DWMAC_DESC_BIT32(15)
#define DWMAC_DESC_ETX_DES0_JABBER_TIMEOUT DWMAC_DESC_BIT32(14)
#define DWMAC_DESC_ETX_DES0_FRAME_FLUSHED DWMAC_DESC_BIT32(13)
#define DWMAC_DESC_ETX_DES0_IP_PAYLOAD_ERROR DWMAC_DESC_BIT32(12)
#define DWMAC_DESC_ETX_DES0_LOSS_OF_CARRIER DWMAC_DESC_BIT32(11)
#define DWMAC_DESC_ETX_DES0_NO_CARRIER DWMAC_DESC_BIT32(10)
#define DWMAC_DESC_ETX_DES0_EXCESSIVE_COLLISION DWMAC_DESC_BIT32(8)
#define DWMAC_DESC_ETX_DES0_VLAN_FRAME DWMAC_DESC_BIT32(7)
#define DWMAC_DESC_ETX_DES0_COLLISION_COUNT(val) DWMAC_DESC_FLD32(val, 3, 6)
#define DWMAC_DESC_ETX_DES0_COLLISION_COUNT_GET(reg) DWMAC_DESC_FLD32GET(reg, 3, 6)
#define DWMAC_DESC_ETX_DES0_COLLISION_COUNT_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 3, 6)
#define DWMAC_DESC_ETX_DES0_EXCESSIVE_DEFERAL DWMAC_DESC_BIT32(2)
#define DWMAC_DESC_ETX_DES0_UNDERFLOW_ERROR DWMAC_DESC_BIT32(1)
#define DWMAC_DESC_ETX_DES0_DEFERRED_BIT DWMAC_DESC_BIT32(0)
	uint32_t des1;
#define DWMAC_DESC_ETX_DES1_TRANSMIT_BUFFER_2_SIZE(val) DWMAC_DESC_FLD32(val, 16, 28)
#define DWMAC_DESC_ETX_DES1_TRANSMIT_BUFFER_2_SIZE_GET(reg) DWMAC_DESC_FLD32GET(reg, 16, 28)
#define DWMAC_DESC_ETX_DES1_TRANSMIT_BUFFER_2_SIZE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 16, 28)
#define DWMAC_DESC_ETX_DES1_TRANSMIT_BUFFER_1_SIZE(val) DWMAC_DESC_FLD32(val, 0, 12)
#define DWMAC_DESC_ETX_DES1_TRANSMIT_BUFFER_1_SIZE_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 12)
#define DWMAC_DESC_ETX_DES1_TRANSMIT_BUFFER_1_SIZE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 12)
	uint32_t des2;
#define DWMAC_DESC_ETX_DES2_BUFF_1_ADDR_PTR(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_ETX_DES2_BUFF_1_ADDR_PTR_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_ETX_DES2_BUFF_1_ADDR_PTR_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
	uint32_t des3;
#define DWMAC_DESC_ETX_DES3_BUFF_2_ADDR_PTR(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_ETX_DES3_BUFF_2_ADDR_PTR_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_ETX_DES3_BUFF_2_ADDR_PTR_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
} dwmac_desc_etx;

typedef union {
	dwmac_desc_rx rx;
	dwmac_desc_tx tx;
	dwmac_desc_erx erx;
	dwmac_desc_etx etx;
} dwmac_desc;

typedef struct {
	dwmac_desc_erx des0_3;
	uint32_t des4;
#define DWMAC_DESC_EXT_ERX_DES4_LAYER3_AND_LAYER4_FILTER_MATCHED(val) DWMAC_DESC_FLD32(val, 26, 27)
#define DWMAC_DESC_EXT_ERX_DES4_LAYER3_AND_LAYER4_FILTER_MATCHED_GET(reg) DWMAC_DESC_FLD32GET(reg, 26, 27)
#define DWMAC_DESC_EXT_ERX_DES4_LAYER3_AND_LAYER4_FILTER_MATCHED_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 26, 27)
#define DWMAC_DESC_EXT_ERX_DES4_LAYER4_FILTER_MATCH DWMAC_DESC_BIT32(25)
#define DWMAC_DESC_EXT_ERX_DES4_LAYER3_FILTER_MATCH DWMAC_DESC_BIT32(24)
#define DWMAC_DESC_EXT_ERX_DES4_TIMESTAMP_DROPPED DWMAC_DESC_BIT32(14)
#define DWMAC_DESC_EXT_ERX_DES4_PTP_VERSION DWMAC_DESC_BIT32(13)
#define DWMAC_DESC_EXT_ERX_DES4_PTP_FRAME_TYPE DWMAC_DESC_BIT32(12)
#define DWMAC_DESC_EXT_ERX_DES4_MESSAGE_TYPE(val) DWMAC_DESC_FLD32(val, 8, 11)
#define DWMAC_DESC_EXT_ERX_DES4_MESSAGE_TYPE_GET(reg) DWMAC_DESC_FLD32GET(reg, 8, 11)
#define DWMAC_DESC_EXT_ERX_DES4_MESSAGE_TYPE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 8, 11)
#define DWMAC_DESC_EXT_ERX_DES4_IPV6_PACKET_RECEIVED DWMAC_DESC_BIT32(7)
#define DWMAC_DESC_EXT_ERX_DES4_IPV4_PACKET_RECEIVED DWMAC_DESC_BIT32(6)
#define DWMAC_DESC_EXT_ERX_DES4_IP_CHECKSUM_BYPASSED DWMAC_DESC_BIT32(5)
#define DWMAC_DESC_EXT_ERX_DES4_IP_PAYLOAD_ERROR DWMAC_DESC_BIT32(4)
#define DWMAC_DESC_EXT_ERX_DES4_IP_HEADER_ERROR DWMAC_DESC_BIT32(3)
#define DWMAC_DESC_EXT_ERX_DES4_IP_PAYLOAD_TYPE(val) DWMAC_DESC_FLD32(val, 0, 2)
#define DWMAC_DESC_EXT_ERX_DES4_IP_PAYLOAD_TYPE_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 2)
#define DWMAC_DESC_EXT_ERX_DES4_IP_PAYLOAD_TYPE_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 2)
	uint32_t des5;
	uint32_t des6;
#define DWMAC_DESC_EXT_ERX_DES6_RECEIVE_FRAME_TIMESTAMP_LOW(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_EXT_ERX_DES6_RECEIVE_FRAME_TIMESTAMP_LOW_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_EXT_ERX_DES6_RECEIVE_FRAME_TIMESTAMP_LOW_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
	uint32_t des7;
#define DWMAC_DESC_EXT_ERX_DES7_RECEIVE_FRAME_TIMESTAMP_HIGH(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_EXT_ERX_DES7_RECEIVE_FRAME_TIMESTAMP_HIGH_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_EXT_ERX_DES7_RECEIVE_FRAME_TIMESTAMP_HIGH_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
} dwmac_desc_ext_erx;

typedef struct {
	dwmac_desc_etx des0_3;
	uint32_t des4;
	uint32_t des5;
	uint32_t des6;
#define DWMAC_DESC_EXT_ETX_DES6_TRANSMIT_FRAME_TIMESTAMP_LOW(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_EXT_ETX_DES6_TRANSMIT_FRAME_TIMESTAMP_LOW_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_EXT_ETX_DES6_TRANSMIT_FRAME_TIMESTAMP_LOW_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
	uint32_t des7;
#define DWMAC_DESC_EXT_ETX_DES7_TRANSMIT_FRAME_TIMESTAMP_HIGH(val) DWMAC_DESC_FLD32(val, 0, 31)
#define DWMAC_DESC_EXT_ETX_DES7_TRANSMIT_FRAME_TIMESTAMP_HIGH_GET(reg) DWMAC_DESC_FLD32GET(reg, 0, 31)
#define DWMAC_DESC_EXT_ETX_DES7_TRANSMIT_FRAME_TIMESTAMP_HIGH_SET(reg, val) DWMAC_DESC_FLD32SET(reg, val, 0, 31)
} dwmac_desc_ext_etx;

typedef union {
	dwmac_desc_ext_erx erx;
	dwmac_desc_ext_etx etx;
} dwmac_desc_ext;

#endif /* DWMAC_DESC_RX_REGS_H */
