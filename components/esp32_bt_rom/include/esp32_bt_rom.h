/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Typed declarations for the ESP32 classic (BR/EDR) Bluetooth controller's ROM
 * functions.
 */

#ifndef ESP32_BT_ROM_H
#define ESP32_BT_ROM_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Signature of r_bt_util_buf_init, slot 0 of the IP functions table.
 * It (re)initialises the controller's exchange-memory buffer pools, tearing
 * them down first when reset is non-zero. Recovered from the loaded controller.
 */
typedef void (*r_bt_util_buf_init_fn_t)(bool reset);

/**
 * Signature of r_bt_util_buf_lmp_tx_alloc, slot 1 of the IP functions table.
 * It allocates an LMP TX buffer from exchange memory, returning its EM offset
 * (0 when the pool is exhausted). Recovered from the loaded controller.
 */
typedef uint16_t (*r_bt_util_buf_lmp_tx_alloc_fn_t)(void);

/**
 * Signature of r_bt_util_buf_lmp_tx_free, slot 2 of the IP functions table.
 * It releases the LMP TX buffer at the given exchange-memory offset back to its
 * pool. Recovered from the loaded controller.
 */
typedef void (*r_bt_util_buf_lmp_tx_free_fn_t)(uint16_t buf);

/**
 * Signature of r_bt_util_buf_acl_rx_alloc, slot 3 of the IP functions table.
 * It allocates an ACL RX buffer from exchange memory, returning its EM offset
 * (0 when the pool is exhausted). Recovered from the loaded controller.
 */
typedef uint16_t (*r_bt_util_buf_acl_rx_alloc_fn_t)(void);

/**
 * Signature of r_bt_util_buf_acl_rx_free, slot 4 of the IP functions table.
 * It releases the ACL RX buffer at the given exchange-memory offset back to its
 * pool. Recovered from the loaded controller.
 */
typedef void (*r_bt_util_buf_acl_rx_free_fn_t)(uint16_t buf);

/**
 * Signature of r_bt_util_buf_acl_tx_alloc, slot 5 of the IP functions table.
 * It allocates an ACL TX buffer from exchange memory, returning its EM offset
 * (0 when the pool is exhausted). Recovered from the loaded controller.
 */
typedef uint16_t (*r_bt_util_buf_acl_tx_alloc_fn_t)(void);

/**
 * Signature of r_bt_util_buf_acl_tx_free, slot 6 of the IP functions table.
 * It releases the ACL TX buffer at the given exchange-memory offset back to its
 * pool. Recovered from the loaded controller.
 */
typedef void (*r_bt_util_buf_acl_tx_free_fn_t)(uint16_t buf);

/**
 * Signature of r_bt_util_buf_sync_init, slot 7 of the IP functions table.
 * It sets up the SCO/eSCO (sync) buffer pools for one sync link. The five
 * uint8_t arguments (link index plus per-direction buffer counts and sizes) are
 * inferred from disassembly; their individual roles are not fully pinned.
 * Recovered from the loaded controller.
 */
typedef void (*r_bt_util_buf_sync_init_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * Signature of r_bt_util_buf_sync_clear, slot 8 of the IP functions table.
 * It drops any sync (SCO/eSCO) TX buffers still queued for the given sync link.
 * Recovered from the loaded controller.
 */
typedef void (*r_bt_util_buf_sync_clear_fn_t)(uint8_t sync_id);

/**
 * Signature of r_bt_util_buf_sync_tx_alloc, slot 9 of the IP functions table.
 * It allocates a sync (SCO/eSCO) TX buffer for the given sync link, returning
 * its EM offset (0 when none free). Recovered from the loaded controller.
 */
typedef uint16_t (*r_bt_util_buf_sync_tx_alloc_fn_t)(uint8_t sync_id, uint8_t len);

/**
 * Signature of r_bt_util_buf_sync_tx_free, slot 10 of the IP functions table.
 * It releases the sync (SCO/eSCO) TX buffer at the given exchange-memory offset
 * for the given sync link. Recovered from the loaded controller.
 */
typedef void (*r_bt_util_buf_sync_tx_free_fn_t)(uint8_t sync_id, uint16_t buf);

/**
 * Signature of r_bt_util_buf_sync_rx_alloc, slot 11 of the IP functions table.
 * It allocates a sync (SCO/eSCO) RX buffer for the given sync link, returning
 * its EM offset (0 when none free). Recovered from the loaded controller.
 */
typedef uint16_t (*r_bt_util_buf_sync_rx_alloc_fn_t)(uint8_t sync_id, uint8_t len);

/**
 * Signature of r_bt_util_buf_sync_rx_free, slot 12 of the IP functions table.
 * It releases the sync (SCO/eSCO) RX buffer at the given exchange-memory offset
 * for the given sync link. Recovered from the loaded controller.
 */
typedef void (*r_bt_util_buf_sync_rx_free_fn_t)(uint8_t sync_id, uint16_t buf);

/**
 * r_E1, slot 13 of the IP functions table.
 */
typedef void (*r_E1_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_E21, slot 14 of the IP functions table.
 */
typedef void (*r_E21_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_E22, slot 15 of the IP functions table.
 */
typedef void (*r_E22_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_E3, slot 16 of the IP functions table.
 */
typedef void (*r_E3_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_KPrimC, slot 17 of the IP functions table.
 */
typedef void (*r_KPrimC_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t *);

/**
 * r_XorKey, slot 18 of the IP functions table.
 */
typedef void (*r_XorKey_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_LM_MakeRandVec, slot 19 of the IP functions table.
 */
typedef void (*r_LM_MakeRandVec_fn_t)(int32_t);

/**
 * r_lmp_pack, slot 20 of the IP functions table.
 */
typedef int32_t (*r_lmp_pack_fn_t)(uint8_t *, uint8_t *);

/**
 * r_lmp_unpack, slot 21 of the IP functions table.
 */
typedef uint8_t (*r_lmp_unpack_fn_t)(uint8_t *, uint8_t *, uint8_t *);

/**
 * r_lm_n_is_zero, slot 22 of the IP functions table.
 */
typedef uint32_t (*r_lm_n_is_zero_fn_t)(int32_t, int32_t);

/**
 * r_lm_sp_n_one, slot 23 of the IP functions table.
 */
typedef void (*r_lm_sp_n_one_fn_t)(void *, int32_t);

/**
 * r_lm_sp_sha256_calculate, slot 24 of the IP functions table.
 */
typedef void (*r_lm_sp_sha256_calculate_fn_t)(uint8_t *, uint32_t *, uint16_t);

/**
 * r_lm_sp_n192_convert_wnaf, slot 25 of the IP functions table.
 */
typedef uint32_t (*r_lm_sp_n192_convert_wnaf_fn_t)(int32_t, int32_t, uint32_t *);

/**
 * r_lm_sp_p192_point_to_inf, slot 26 of the IP functions table.
 */
typedef uint32_t (*r_lm_sp_p192_point_to_inf_fn_t)(int32_t);

/**
 * r_lm_sp_p192_point_jacobian_to_affine, slot 27 of the IP functions table.
 */
typedef uint32_t (*r_lm_sp_p192_point_jacobian_to_affine_fn_t)(int32_t);

/**
 * r_lm_sp_p192_points_jacobian_to_affine, slot 28 of the IP functions table.
 */
typedef uint32_t (*r_lm_sp_p192_points_jacobian_to_affine_fn_t)(int32_t);

/**
 * r_lm_sp_pre_compute_points, slot 29 of the IP functions table.
 */
typedef uint32_t (*r_lm_sp_pre_compute_points_fn_t)(int32_t);

/**
 * r_lm_sp_p192_dbl, slot 30 of the IP functions table.
 */
typedef uint32_t (*r_lm_sp_p192_dbl_fn_t)(int32_t, int32_t);

/**
 * r_lm_sp_p192_add, slot 31 of the IP functions table.
 */
typedef uint32_t (*r_lm_sp_p192_add_fn_t)(void *, void *, void *);

/**
 * r_lm_sp_p192_invert, slot 32 of the IP functions table.
 */
typedef uint32_t (*r_lm_sp_p192_invert_fn_t)(int32_t);

/**
 * r_lm_f1, slot 33 of the IP functions table.
 */
typedef void (*r_lm_f1_fn_t)(int32_t, uint8_t *, uint32_t, uint32_t, int32_t);

/**
 * r_lm_f2, slot 34 of the IP functions table.
 */
typedef void (*r_lm_f2_fn_t)(void *, void *, void *, void *, int32_t, uint8_t *);

/**
 * r_lm_oob_f1, slot 35 of the IP functions table.
 */
typedef void (*r_lm_oob_f1_fn_t)(int32_t, uint8_t *, uint32_t, uint32_t, int32_t);

/**
 * r_lm_g, slot 36 of the IP functions table.
 */
typedef void (*r_lm_g_fn_t)(void *, void *, uint8_t, int32_t, int32_t *);

/**
 * r_lm_f3, slot 37 of the IP functions table.
 */
typedef void (*r_lm_f3_fn_t)(void *, void *, void *, uint32_t, void *, void *, int32_t, uint8_t *);

/**
 * r_lm_get_nonce, slot 38 of the IP functions table.
 */
typedef void (*r_lm_get_nonce_fn_t)(void *);

/**
 * r_lm_dhkey_calc_init, slot 39 of the IP functions table.
 */
typedef uint32_t (*r_lm_dhkey_calc_init_fn_t)(void *);

/**
 * r_lm_dhkey_compare, slot 40 of the IP functions table.
 */
typedef uint32_t (*r_lm_dhkey_compare_fn_t)(int32_t);

/**
 * r_F1_256, slot 41 of the IP functions table.
 */
typedef void (*r_F1_256_fn_t)(int32_t, int32_t, int32_t, uint8_t *, int32_t);

/**
 * r_HMAC, slot 42 of the IP functions table.
 */
typedef void (*r_HMAC_fn_t)(int32_t, int32_t, uint32_t, uint32_t);

/**
 * r_G_256, slot 43 of the IP functions table.
 */
typedef void (*r_G_256_fn_t)(int32_t, int32_t, int32_t, int32_t, int32_t *);

/**
 * r_SHA_256, slot 44 of the IP functions table.
 */
typedef void (*r_SHA_256_fn_t)(int32_t, uint32_t, uint8_t *);

/**
 * r_F2_256, slot 45 of the IP functions table.
 */
typedef void (*r_F2_256_fn_t)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);

/**
 * r_F3_256, slot 46 of the IP functions table.
 */
typedef void (*r_F3_256_fn_t)(int32_t, int32_t, int32_t, int32_t, uint8_t *, int32_t, int32_t, int32_t);

/**
 * r_H3, slot 47 of the IP functions table.
 */
typedef int32_t (*r_H3_fn_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

/**
 * r_H4, slot 48 of the IP functions table.
 */
typedef int32_t (*r_H4_fn_t)(int32_t, int32_t, int32_t, int32_t);

/**
 * r_H5, slot 49 of the IP functions table.
 */
typedef int32_t (*r_H5_fn_t)(int32_t, int32_t, int32_t, int32_t);

/**
 * r_hashConcat, slot 50 of the IP functions table.
 */
typedef int32_t (*r_hashConcat_fn_t)(int32_t, uint32_t, int32_t, uint32_t, int32_t);

/**
 * r_ea_elt_cancel, slot 51 of the IP functions table.
 */
typedef void (*r_ea_elt_cancel_fn_t)(uint32_t *);

/**
 * r_ea_time_get_slot_rounded, slot 52 of the IP functions table.
 */
typedef uint32_t (*r_ea_time_get_slot_rounded_fn_t)(void);

/**
 * r_ea_init, slot 53 of the IP functions table.
 */
typedef void (*r_ea_init_fn_t)(void);

/**
 * r_ea_elt_create, slot 54 of the IP functions table.
 */
typedef void *(*r_ea_elt_create_fn_t)(uint32_t);

/**
 * r_ea_elt_insert, slot 55 of the IP functions table.
 */
typedef uint32_t (*r_ea_elt_insert_fn_t)(int32_t);

/**
 * r_ea_time_get_halfslot_rounded, slot 56 of the IP functions table.
 */
typedef uint32_t (*r_ea_time_get_halfslot_rounded_fn_t)(void);

/**
 * r_ea_elt_remove, slot 57 of the IP functions table.
 */
typedef uint32_t (*r_ea_elt_remove_fn_t)(int32_t);

/**
 * r_ea_interval_create, slot 58 of the IP functions table.
 */
typedef void *(*r_ea_interval_create_fn_t)(void);

/**
 * r_ea_interval_insert, slot 59 of the IP functions table.
 */
typedef void (*r_ea_interval_insert_fn_t)(uint32_t);

/**
 * r_ea_interval_remove, slot 60 of the IP functions table.
 */
typedef void (*r_ea_interval_remove_fn_t)(uint32_t);

/**
 * r_ea_interval_delete, slot 61 of the IP functions table.
 */
typedef void (*r_ea_interval_delete_fn_t)(int32_t);

/**
 * r_ea_finetimer_isr, slot 62 of the IP functions table.
 */
typedef void (*r_ea_finetimer_isr_fn_t)(void);

/**
 * r_ea_sw_isr, slot 63 of the IP functions table.
 */
typedef void (*r_ea_sw_isr_fn_t)(void);

/**
 * r_ea_offset_req_hack, slot 64 of the IP functions table.
 */
typedef uint32_t (*r_ea_offset_req_hack_fn_t)(int32_t, uint16_t *);

/**
 * r_ea_sleep_check, slot 65 of the IP functions table.
 */
typedef uint32_t (*r_ea_sleep_check_fn_t)(uint32_t *, uint32_t);

/**
 * r_ea_interval_duration_req, slot 66 of the IP functions table.
 */
typedef void (*r_ea_interval_duration_req_fn_t)(uint16_t *, uint16_t *);

/**
 * r_ea_alarm_set, slot 67 of the IP functions table.
 */
typedef uint32_t (*r_ea_alarm_set_fn_t)(uint32_t *);

/**
 * r_ea_alarm_clear, slot 68 of the IP functions table.
 */
typedef uint32_t (*r_ea_alarm_clear_fn_t)(int32_t);

/**
 * r_em_buf_init, slot 69 of the IP functions table.
 */
typedef void (*r_em_buf_init_fn_t)(void);

/**
 * r_em_buf_rx_free, slot 70 of the IP functions table.
 */
typedef void (*r_em_buf_rx_free_fn_t)(uint32_t);

/**
 * r_em_buf_rx_buff_addr_get, slot 71 of the IP functions table.
 */
typedef uint8_t *(*r_em_buf_rx_buff_addr_get_fn_t)(uint32_t);

/**
 * r_em_buf_tx_buff_addr_get, slot 72 of the IP functions table.
 */
typedef uint8_t *(*r_em_buf_tx_buff_addr_get_fn_t)(int32_t);

/**
 * r_em_buf_tx_free, slot 73 of the IP functions table.
 */
typedef bool (*r_em_buf_tx_free_fn_t)(int32_t);

/**
 * r_hci_fc_init, slot 74 of the IP functions table.
 */
typedef void (*r_hci_fc_init_fn_t)(void);

/**
 * ram_hci_fc_acl_buf_size_set, slot 75 of the IP functions table.
 */
typedef uint32_t (*r_ram_hci_fc_acl_buf_size_set_fn_t)(uint16_t, int16_t);

/**
 * ram_hci_fc_sync_buf_size_set, slot 76 of the IP functions table.
 */
typedef uint32_t (*r_ram_hci_fc_sync_buf_size_set_fn_t)(uint8_t, int16_t);

/**
 * r_hci_fc_acl_en, slot 77 of the IP functions table.
 */
typedef int32_t (*r_hci_fc_acl_en_fn_t)(uint8_t);

/**
 * r_hci_fc_sync_en, slot 78 of the IP functions table.
 */
typedef void (*r_hci_fc_sync_en_fn_t)(uint8_t);

/**
 * r_hci_fc_acl_packet_sent, slot 79 of the IP functions table.
 */
typedef void (*r_hci_fc_acl_packet_sent_fn_t)(void);

/**
 * r_hci_fc_sync_packet_sent, slot 80 of the IP functions table.
 */
typedef void (*r_hci_fc_sync_packet_sent_fn_t)(void);

/**
 * r_hci_fc_host_nb_acl_pkts_complete, slot 81 of the IP functions table.
 */
typedef void (*r_hci_fc_host_nb_acl_pkts_complete_fn_t)(uint16_t);

/**
 * r_hci_fc_host_nb_sync_pkts_complete, slot 82 of the IP functions table.
 */
typedef void (*r_hci_fc_host_nb_sync_pkts_complete_fn_t)(uint16_t);

/**
 * r_hci_fc_check_host_available_nb_acl_packets, slot 83 of the IP functions table.
 */
typedef uint32_t (*r_hci_fc_check_host_available_nb_acl_packets_fn_t)(void);

/**
 * r_hci_fc_check_host_available_nb_sync_packets, slot 84 of the IP functions table.
 */
typedef uint32_t (*r_hci_fc_check_host_available_nb_sync_packets_fn_t)(void);

/**
 * r_hci_look_for_cmd_desc_hack, slot 85 of the IP functions table.
 */
typedef uint16_t *(*r_hci_look_for_cmd_desc_hack_fn_t)(uint16_t);

/**
 * r_hci_look_for_evt_desc, slot 86 of the IP functions table.
 */
typedef uint8_t *(*r_hci_look_for_evt_desc_fn_t)(uint8_t);

/**
 * patch_hci_look_for_dbg_evt_desc, slot 87 of the IP functions table.
 */
typedef uint8_t *(*r_patch_hci_look_for_dbg_evt_desc_fn_t)(uint8_t);

/**
 * r_hci_look_for_le_evt_desc, slot 88 of the IP functions table.
 */
typedef uint8_t *(*r_hci_look_for_le_evt_desc_fn_t)(uint8_t);

/**
 * r_hci_init_hack, slot 89 of the IP functions table.
 */
typedef void (*r_hci_init_hack_fn_t)(void);

/**
 * r_hci_evt_mask_set, slot 90 of the IP functions table.
 */
typedef uint32_t (*r_hci_evt_mask_set_fn_t)(void *, uint8_t);

/**
 * r_hci_reset_hack, slot 91 of the IP functions table.
 */
typedef void (*r_hci_reset_hack_fn_t)(void);

/**
 * r_hci_send_2_host_hack, slot 92 of the IP functions table.
 */
typedef void (*r_hci_send_2_host_hack_fn_t)(void *);

/**
 * r_hci_bt_acl_bdaddr_register_hack, slot 93 of the IP functions table.
 */
typedef void (*r_hci_bt_acl_bdaddr_register_hack_fn_t)(uint32_t, void *);

/**
 * r_hci_bt_acl_conhdl_register, slot 94 of the IP functions table.
 */
typedef void (*r_hci_bt_acl_conhdl_register_fn_t)(uint32_t);

/**
 * r_hci_bt_acl_bdaddr_unregister_hack, slot 95 of the IP functions table.
 */
typedef void (*r_hci_bt_acl_bdaddr_unregister_hack_fn_t)(uint32_t);

/**
 * r_hci_evt_filter_add, slot 96 of the IP functions table.
 */
typedef uint8_t (*r_hci_evt_filter_add_fn_t)(uint8_t *);

/**
 * r_hci_voice_settings_get, slot 97 of the IP functions table.
 */
typedef uint16_t (*r_hci_voice_settings_get_fn_t)(void);

/**
 * r_hci_voice_settings_set, slot 98 of the IP functions table.
 */
typedef uint32_t (*r_hci_voice_settings_set_fn_t)(uint16_t);

/**
 * r_hci_tl_send, slot 99 of the IP functions table.
 */
typedef void (*r_hci_tl_send_fn_t)(int32_t);

/**
 * r_hci_tl_init, slot 100 of the IP functions table.
 */
typedef void (*r_hci_tl_init_fn_t)(uint8_t);

/**
 * r_hci_cmd_get_max_param_size, slot 101 of the IP functions table.
 */
typedef uint8_t (*r_hci_cmd_get_max_param_size_fn_t)(uint16_t);

/**
 * r_hci_cmd_received, slot 102 of the IP functions table.
 */
typedef void (*r_hci_cmd_received_fn_t)(uint16_t, uint8_t, uint16_t *);

/**
 * r_hci_acl_tx_data_alloc, slot 103 of the IP functions table.
 */
typedef uint8_t *(*r_hci_acl_tx_data_alloc_fn_t)(uint32_t, uint16_t);

/**
 * r_hci_acl_tx_data_received, slot 104 of the IP functions table.
 */
typedef void (*r_hci_acl_tx_data_received_fn_t)(uint16_t, uint16_t, uint32_t);

/**
 * r_hci_sync_tx_data_alloc, slot 105 of the IP functions table.
 */
typedef uint8_t *(*r_hci_sync_tx_data_alloc_fn_t)(uint32_t, uint8_t);

/**
 * r_hci_sync_tx_data_received, slot 106 of the IP functions table.
 */
typedef void (*r_hci_sync_tx_data_received_fn_t)(uint32_t, uint32_t, uint32_t);

/**
 * r_hci_util_pack, slot 107 of the IP functions table.
 */
typedef int32_t (*r_hci_util_pack_fn_t)(uint32_t *, uint16_t *, uint8_t *);

/**
 * r_hci_util_unpack, slot 108 of the IP functions table.
 */
typedef int32_t (*r_hci_util_unpack_fn_t)(uint32_t *, uint32_t *, uint16_t *, uint32_t, uint8_t *);

/**
 * r_LM_ExtractMaxEncKeySize, slot 109 of the IP functions table.
 */
typedef uint32_t (*r_LM_ExtractMaxEncKeySize_fn_t)(int16_t);

/**
 * r_lb_init_hack, slot 110 of the IP functions table.
 */
typedef void (*r_lb_init_hack_fn_t)(void);

/**
 * r_lb_mst_stop_act_bcst_enc, slot 111 of the IP functions table.
 */
typedef void (*r_lb_mst_stop_act_bcst_enc_fn_t)(void);

/**
 * r_lb_reset_hack, slot 112 of the IP functions table.
 */
typedef void (*r_lb_reset_hack_fn_t)(void);

/**
 * r_lb_util_get_nb_broadcast, slot 113 of the IP functions table.
 */
typedef uint8_t (*r_lb_util_get_nb_broadcast_fn_t)(void);

/**
 * r_lb_util_set_nb_broadcast, slot 114 of the IP functions table.
 */
typedef void (*r_lb_util_set_nb_broadcast_fn_t)(uint8_t);

/**
 * r_lb_util_get_res_lt_addr, slot 115 of the IP functions table.
 */
typedef uint8_t (*r_lb_util_get_res_lt_addr_fn_t)(void);

/**
 * r_lb_util_get_csb_mode, slot 116 of the IP functions table.
 */
typedef uint8_t (*r_lb_util_get_csb_mode_fn_t)(void);

/**
 * r_lb_send_lmp, slot 117 of the IP functions table.
 */
typedef void (*r_lb_send_lmp_fn_t)(void *, uint32_t, uint8_t);

/**
 * r_lb_clk_adj_activate, slot 118 of the IP functions table.
 */
typedef void (*r_lb_clk_adj_activate_fn_t)(int16_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lb_clk_adj_period_update, slot 119 of the IP functions table.
 */
typedef void (*r_lb_clk_adj_period_update_fn_t)(uint8_t);

/**
 * r_lb_send_pdu_clk_adj, slot 120 of the IP functions table.
 */
typedef void (*r_lb_send_pdu_clk_adj_fn_t)(uint32_t, uint8_t, uint16_t, uint8_t, uint8_t, int32_t, uint8_t);

/**
 * r_lb_clk_adj_id_get, slot 121 of the IP functions table.
 */
typedef uint8_t (*r_lb_clk_adj_id_get_fn_t)(void);

/**
 * r_lb_mst_key_cmp, slot 122 of the IP functions table.
 */
typedef void (*r_lb_mst_key_cmp_fn_t)(uint8_t, uint16_t, uint8_t);

/**
 * r_lb_mst_key, slot 123 of the IP functions table.
 */
typedef void (*r_lb_mst_key_fn_t)(void);

/**
 * r_lb_mst_key_restart_enc, slot 124 of the IP functions table.
 */
typedef void (*r_lb_mst_key_restart_enc_fn_t)(uint16_t);

/**
 * r_lb_mst_start_act_bcst_enc, slot 125 of the IP functions table.
 */
typedef void (*r_lb_mst_start_act_bcst_enc_fn_t)(void);

/**
 * r_LM_GetMasterKey, slot 126 of the IP functions table.
 */
typedef void (*r_LM_GetMasterKey_fn_t)(void *);

/**
 * r_LM_GetMasterKeyRand, slot 127 of the IP functions table.
 */
typedef void (*r_LM_GetMasterKeyRand_fn_t)(void *);

/**
 * r_LM_GetMasterEncRand, slot 128 of the IP functions table.
 */
typedef void (*r_LM_GetMasterEncRand_fn_t)(void *);

/**
 * r_LM_GetMasterEncKeySize, slot 129 of the IP functions table.
 */
typedef uint8_t (*r_LM_GetMasterEncKeySize_fn_t)(void);

/**
 * r_lc_send_lmp, slot 130 of the IP functions table.
 */
typedef void (*r_lc_send_lmp_fn_t)(uint8_t, void *);

/**
 * r_lc_send_pdu_acc, slot 131 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_acc_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_acc_ext4, slot 132 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_acc_ext4_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_not_acc, slot 133 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_not_acc_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_not_acc_ext4, slot 134 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_not_acc_ext4_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_set_afh, slot 135 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_set_afh_fn_t)(uint32_t, uint32_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_au_rand, slot 136 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_au_rand_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_in_rand, slot 137 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_in_rand_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_comb_key, slot 138 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_comb_key_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_unit_key, slot 139 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_unit_key_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_max_slot, slot 140 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_max_slot_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_max_slot_req, slot 141 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_max_slot_req_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_encaps_payl, slot 142 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_encaps_payl_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_encaps_head, slot 143 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_encaps_head_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_clk_adj_ack, slot 144 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_clk_adj_ack_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_clk_adj_req, slot 145 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_clk_adj_req_fn_t)(uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_ptt_req, slot 146 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_ptt_req_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_sp_nb, slot 147 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_sp_nb_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_sp_cfm, slot 148 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_sp_cfm_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_sres, slot 149 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_sres_fn_t)(uint8_t, uint8_t *, uint8_t);

/**
 * r_lc_send_pdu_sco_lk_rem_req, slot 150 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_sco_lk_rem_req_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_esco_lk_rem_req, slot 151 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_esco_lk_rem_req_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_auto_rate, slot 152 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_auto_rate_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_sniff_req, slot 153 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_sniff_req_fn_t)(uint32_t);

/**
 * r_lc_send_pdu_lsto, slot 154 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_lsto_fn_t)(uint8_t, uint16_t, uint8_t);

/**
 * r_lc_send_pdu_enc_key_sz_req, slot 155 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_enc_key_sz_req_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_feats_res, slot 156 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_feats_res_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_tim_acc, slot 157 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_tim_acc_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_feats_ext_req, slot 158 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_feats_ext_req_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_io_cap_res, slot 159 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_io_cap_res_fn_t)(uint32_t);

/**
 * r_lc_send_pdu_num_comp_fail, slot 160 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_num_comp_fail_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_pause_enc_aes_req, slot 161 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_pause_enc_aes_req_fn_t)(uint8_t, uint8_t, void *);

/**
 * r_lc_send_pdu_paus_enc_req, slot 162 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_paus_enc_req_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_resu_enc_req, slot 163 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_resu_enc_req_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_setup_cmp, slot 164 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_setup_cmp_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_qos_req, slot 165 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_qos_req_fn_t)(uint8_t, uint8_t, uint16_t, uint8_t);

/**
 * r_lc_send_pdu_slot_off, slot 166 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_slot_off_fn_t)(uint32_t, uint16_t, uint8_t);

/**
 * r_lc_send_pdu_unsniff_req, slot 167 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_unsniff_req_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_vers_req, slot 168 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_vers_req_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_dhkey_chk, slot 169 of the IP functions table.
 */
typedef void (*r_lc_send_pdu_dhkey_chk_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_cmd_stat_send, slot 170 of the IP functions table.
 */
typedef void (*r_lc_cmd_stat_send_fn_t)(uint16_t, uint8_t);

/**
 * r_lc_init_hack, slot 171 of the IP functions table.
 */
typedef void (*r_lc_init_hack_fn_t)(void);

/**
 * r_lc_reset_hack, slot 172 of the IP functions table.
 */
typedef void (*r_lc_reset_hack_fn_t)(void);

/**
 * r_lc_start_hack, slot 173 of the IP functions table.
 */
typedef void (*r_lc_start_hack_fn_t)(uint32_t, uint8_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t);

/**
 * r_lc_start_lmp_to, slot 174 of the IP functions table.
 */
typedef void (*r_lc_start_lmp_to_fn_t)(uint32_t);

/**
 * r_lc_afh_set, slot 175 of the IP functions table.
 */
typedef void (*r_lc_afh_set_fn_t)(uint32_t, uint8_t);

/**
 * patch_lc_auth_cmp, slot 176 of the IP functions table.
 */
typedef void (*r_patch_lc_auth_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_conn_seq_done, slot 177 of the IP functions table.
 */
typedef uint8_t (*r_lc_conn_seq_done_fn_t)(uint32_t);

/**
 * r_lc_send_enc_chg_evt, slot 178 of the IP functions table.
 */
typedef void (*r_lc_send_enc_chg_evt_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_detach, slot 179 of the IP functions table.
 */
typedef void (*r_lc_detach_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_calc_link_key, slot 180 of the IP functions table.
 */
typedef void (*r_lc_calc_link_key_fn_t)(uint32_t);

/**
 * r_lc_sp_end, slot 181 of the IP functions table.
 */
typedef void (*r_lc_sp_end_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_init_start_mutual_auth, slot 182 of the IP functions table.
 */
typedef void (*r_lc_init_start_mutual_auth_fn_t)(uint32_t);

/**
 * r_lc_chg_pkt_type_cont, slot 183 of the IP functions table.
 */
typedef void (*r_lc_chg_pkt_type_cont_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_chg_pkt_type_cmp, slot 184 of the IP functions table.
 */
typedef void (*r_lc_chg_pkt_type_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_chg_pkt_type_retry, slot 185 of the IP functions table.
 */
typedef uint32_t (*r_lc_chg_pkt_type_retry_fn_t)(uint32_t);

/**
 * r_lc_afh_start, slot 186 of the IP functions table.
 */
typedef void (*r_lc_afh_start_fn_t)(uint32_t);

/**
 * r_lc_chk_to, slot 187 of the IP functions table.
 */
typedef void (*r_lc_chk_to_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_comb_key_svr, slot 188 of the IP functions table.
 */
typedef void (*r_lc_comb_key_svr_fn_t)(uint32_t, void *);

/**
 * r_lc_con_cmp, slot 189 of the IP functions table.
 */
typedef void (*r_lc_con_cmp_fn_t)(uint32_t);

/**
 * r_lc_con_cmp_evt_send, slot 190 of the IP functions table.
 */
typedef void (*r_lc_con_cmp_evt_send_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_dhkey, slot 191 of the IP functions table.
 */
typedef void (*r_lc_dhkey_fn_t)(uint32_t);

/**
 * r_lc_init_calc_f3, slot 192 of the IP functions table.
 */
typedef void (*r_lc_init_calc_f3_fn_t)(uint32_t);

/**
 * r_lc_sp_fail, slot 193 of the IP functions table.
 */
typedef void (*r_lc_sp_fail_fn_t)(uint32_t);

/**
 * r_lc_locepr_rsw, slot 194 of the IP functions table.
 */
typedef void (*r_lc_locepr_rsw_fn_t)(uint32_t);

/**
 * r_lc_locepr_lkref, slot 195 of the IP functions table.
 */
typedef void (*r_lc_locepr_lkref_fn_t)(uint32_t);

/**
 * r_lc_enc_key_refresh, slot 196 of the IP functions table.
 */
typedef void (*r_lc_enc_key_refresh_fn_t)(uint32_t);

/**
 * r_lc_end_chk_colli, slot 197 of the IP functions table.
 */
typedef void (*r_lc_end_chk_colli_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_rem_enc, slot 198 of the IP functions table.
 */
typedef void (*r_lc_rem_enc_fn_t)(uint32_t);

/**
 * r_lc_enc_cmp, slot 199 of the IP functions table.
 */
typedef void (*r_lc_enc_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_restart_enc_cont, slot 200 of the IP functions table.
 */
typedef void (*r_lc_restart_enc_cont_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_end_of_sniff_nego, slot 201 of the IP functions table.
 */
typedef void (*r_lc_end_of_sniff_nego_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_rem_trans_mode, slot 202 of the IP functions table.
 */
typedef void (*r_lc_rem_trans_mode_fn_t)(uint32_t);

/**
 * r_lc_restore_to, slot 203 of the IP functions table.
 */
typedef void (*r_lc_restore_to_fn_t)(uint32_t);

/**
 * r_lc_sniff_slot_unchange, slot 204 of the IP functions table.
 */
typedef void (*r_lc_sniff_slot_unchange_fn_t)(uint32_t, uint32_t);

/**
 * r_lc_sniff_max_slot_chg, slot 205 of the IP functions table.
 */
typedef void (*r_lc_sniff_max_slot_chg_fn_t)(uint32_t);

/**
 * r_lc_epr_cmp, slot 206 of the IP functions table.
 */
typedef void (*r_lc_epr_cmp_fn_t)(uint32_t);

/**
 * r_lc_epr_change_lk, slot 207 of the IP functions table.
 */
typedef void (*r_lc_epr_change_lk_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_rsw_clean_up, slot 208 of the IP functions table.
 */
typedef void (*r_lc_rsw_clean_up_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_rsw_done, slot 209 of the IP functions table.
 */
typedef void (*r_lc_rsw_done_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_mutual_auth_end2, slot 210 of the IP functions table.
 */
typedef void (*r_lc_mutual_auth_end2_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_epr_rsw_cmp, slot 211 of the IP functions table.
 */
typedef void (*r_lc_epr_rsw_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_restore_afh_reporting, slot 212 of the IP functions table.
 */
typedef void (*r_lc_restore_afh_reporting_fn_t)(uint32_t);

/**
 * r_lc_start_enc, slot 213 of the IP functions table.
 */
typedef void (*r_lc_start_enc_fn_t)(uint32_t);

/**
 * r_lc_enter_sniff_mode, slot 214 of the IP functions table.
 */
typedef void (*r_lc_enter_sniff_mode_fn_t)(uint32_t);

/**
 * r_lc_init_passkey_loop, slot 215 of the IP functions table.
 */
typedef void (*r_lc_init_passkey_loop_fn_t)(uint32_t);

/**
 * r_lc_initiator_epr, slot 216 of the IP functions table.
 */
typedef void (*r_lc_initiator_epr_fn_t)(uint32_t);

/**
 * r_lc_epr_resp, slot 217 of the IP functions table.
 */
typedef void (*r_lc_epr_resp_fn_t)(uint32_t);

/**
 * r_lc_stop_enc, slot 218 of the IP functions table.
 */
typedef void (*r_lc_stop_enc_fn_t)(uint32_t);

/**
 * r_lc_key_exch_end, slot 219 of the IP functions table.
 */
typedef void (*r_lc_key_exch_end_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_mutual_auth_end, slot 220 of the IP functions table.
 */
typedef void (*r_lc_mutual_auth_end_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_legacy_pair, slot 221 of the IP functions table.
 */
typedef void (*r_lc_legacy_pair_fn_t)(uint32_t);

/**
 * r_lc_pairing_cont, slot 222 of the IP functions table.
 */
typedef void (*r_lc_pairing_cont_fn_t)(uint32_t);

/**
 * r_lc_local_switch, slot 223 of the IP functions table.
 */
typedef void (*r_lc_local_switch_fn_t)(uint32_t);

/**
 * r_lc_stop_afh_report, slot 224 of the IP functions table.
 */
typedef void (*r_lc_stop_afh_report_fn_t)(uint32_t);

/**
 * r_lc_local_trans_mode, slot 225 of the IP functions table.
 */
typedef void (*r_lc_local_trans_mode_fn_t)(uint32_t);

/**
 * r_lc_local_untrans_mode, slot 226 of the IP functions table.
 */
typedef void (*r_lc_local_untrans_mode_fn_t)(uint32_t);

/**
 * r_lc_max_slot_mgt, slot 227 of the IP functions table.
 */
typedef void (*r_lc_max_slot_mgt_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_mst_key, slot 228 of the IP functions table.
 */
typedef void (*r_lc_mst_key_fn_t)(uint32_t);

/**
 * r_lc_semi_key_cmp, slot 229 of the IP functions table.
 */
typedef void (*r_lc_semi_key_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_mst_send_mst_key, slot 230 of the IP functions table.
 */
typedef void (*r_lc_mst_send_mst_key_fn_t)(uint32_t);

/**
 * r_lc_mst_qos_done, slot 231 of the IP functions table.
 */
typedef void (*r_lc_mst_qos_done_fn_t)(uint32_t);

/**
 * r_lc_passkey_comm, slot 232 of the IP functions table.
 */
typedef void (*r_lc_passkey_comm_fn_t)(uint32_t);

/**
 * r_lc_proc_rcv_dhkey, slot 233 of the IP functions table.
 */
typedef void (*r_lc_proc_rcv_dhkey_fn_t)(uint32_t);

/**
 * r_lc_resp_wait_dhkey_cont, slot 234 of the IP functions table.
 */
typedef void (*r_lc_resp_wait_dhkey_cont_fn_t)(uint32_t);

/**
 * r_lc_ptt, slot 235 of the IP functions table.
 */
typedef void (*r_lc_ptt_fn_t)(uint32_t);

/**
 * r_lc_ptt_cmp, slot 236 of the IP functions table.
 */
typedef void (*r_lc_ptt_cmp_fn_t)(uint32_t);

/**
 * r_lc_qos_setup, slot 237 of the IP functions table.
 */
typedef void (*r_lc_qos_setup_fn_t)(uint32_t);

/**
 * r_lc_rd_rem_name, slot 238 of the IP functions table.
 */
typedef void (*r_lc_rd_rem_name_fn_t)(uint32_t);

/**
 * r_lc_rem_switch, slot 239 of the IP functions table.
 */
typedef void (*r_lc_rem_switch_fn_t)(uint32_t);

/**
 * r_lc_switch_cmp, slot 240 of the IP functions table.
 */
typedef void (*r_lc_switch_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_start_enc_key_size, slot 241 of the IP functions table.
 */
typedef void (*r_lc_start_enc_key_size_fn_t)(uint32_t);

/**
 * r_lc_rem_nego_trans_mode, slot 242 of the IP functions table.
 */
typedef void (*r_lc_rem_nego_trans_mode_fn_t)(uint32_t);

/**
 * r_lc_rem_name_cont, slot 243 of the IP functions table.
 */
typedef void (*r_lc_rem_name_cont_fn_t)(uint32_t);

/**
 * r_lc_rem_untrans_mode, slot 244 of the IP functions table.
 */
typedef void (*r_lc_rem_untrans_mode_fn_t)(uint32_t);

/**
 * r_lc_unsniff_cont, slot 245 of the IP functions table.
 */
typedef void (*r_lc_unsniff_cont_fn_t)(uint32_t);

/**
 * r_lc_sec_auth_compute_sres, slot 246 of the IP functions table.
 */
typedef void (*r_lc_sec_auth_compute_sres_fn_t)(uint32_t);

/**
 * r_lc_resp_sec_auth, slot 247 of the IP functions table.
 */
typedef void (*r_lc_resp_sec_auth_fn_t)(uint32_t);

/**
 * r_lc_resp_auth, slot 248 of the IP functions table.
 */
typedef void (*r_lc_resp_auth_fn_t)(uint32_t);

/**
 * r_lc_resp_oob_wait_nonce, slot 249 of the IP functions table.
 */
typedef void (*r_lc_resp_oob_wait_nonce_fn_t)(uint16_t);

/**
 * r_lc_resp_oob_nonce, slot 250 of the IP functions table.
 */
typedef void (*r_lc_resp_oob_nonce_fn_t)(uint32_t);

/**
 * r_lc_sp_oob_tid_fail, slot 251 of the IP functions table.
 */
typedef void (*r_lc_sp_oob_tid_fail_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_resp_calc_f3, slot 252 of the IP functions table.
 */
typedef void (*r_lc_resp_calc_f3_fn_t)(uint32_t);

/**
 * r_lc_release, slot 253 of the IP functions table.
 */
typedef void (*r_lc_release_fn_t)(uint32_t);

/**
 * r_lc_restart_enc, slot 254 of the IP functions table.
 */
typedef void (*r_lc_restart_enc_fn_t)(uint32_t);

/**
 * r_lc_send_enc_mode, slot 255 of the IP functions table.
 */
typedef void (*r_lc_send_enc_mode_fn_t)(uint32_t);

/**
 * r_lc_ret_sniff_max_slot_chg, slot 256 of the IP functions table.
 */
typedef void (*r_lc_ret_sniff_max_slot_chg_fn_t)(uint32_t);

/**
 * r_lc_upd_to, slot 257 of the IP functions table.
 */
typedef void (*r_lc_upd_to_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_start_key_exch, slot 258 of the IP functions table.
 */
typedef void (*r_lc_start_key_exch_fn_t)(uint32_t);

/**
 * r_lc_start_passkey, slot 259 of the IP functions table.
 */
typedef void (*r_lc_start_passkey_fn_t)(uint32_t);

/**
 * r_lc_start_passkey_loop, slot 260 of the IP functions table.
 */
typedef void (*r_lc_start_passkey_loop_fn_t)(uint32_t);

/**
 * r_lc_start_oob, slot 261 of the IP functions table.
 */
typedef void (*r_lc_start_oob_fn_t)(uint32_t);

/**
 * r_lc_skip_hl_oob_req, slot 262 of the IP functions table.
 */
typedef void (*r_lc_skip_hl_oob_req_fn_t)(uint32_t);

/**
 * r_lc_resp_num_comp, slot 263 of the IP functions table.
 */
typedef void (*r_lc_resp_num_comp_fn_t)(uint32_t);

/**
 * r_lc_sniff_sub_mode, slot 264 of the IP functions table.
 */
typedef void (*r_lc_sniff_sub_mode_fn_t)(uint32_t);

/**
 * r_lc_unit_key_svr, slot 265 of the IP functions table.
 */
typedef void (*r_lc_unit_key_svr_fn_t)(uint32_t, void *);

/**
 * r_lc_rem_unsniff, slot 266 of the IP functions table.
 */
typedef void (*r_lc_rem_unsniff_fn_t)(uint32_t);

/**
 * r_lc_unsniff_cmp, slot 267 of the IP functions table.
 */
typedef void (*r_lc_unsniff_cmp_fn_t)(uint32_t);

/**
 * r_lc_resp_pair, slot 268 of the IP functions table.
 */
typedef void (*r_lc_resp_pair_fn_t)(uint16_t, int32_t);

/**
 * r_lc_feat, slot 269 of the IP functions table.
 */
typedef void (*r_lc_feat_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_hl_connect, slot 270 of the IP functions table.
 */
typedef void (*r_lc_hl_connect_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_version, slot 271 of the IP functions table.
 */
typedef void (*r_lc_version_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_loc_sniff, slot 272 of the IP functions table.
 */
typedef void (*r_lc_loc_sniff_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_rem_sniff_sub_rate, slot 273 of the IP functions table.
 */
typedef void (*r_lc_rem_sniff_sub_rate_fn_t)(uint32_t);

/**
 * r_lc_unsniff, slot 274 of the IP functions table.
 */
typedef void (*r_lc_unsniff_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_rem_sniff, slot 275 of the IP functions table.
 */
typedef void (*r_lc_rem_sniff_fn_t)(uint32_t, uint32_t);

/**
 * r_lc_ext_feat, slot 276 of the IP functions table.
 */
typedef void (*r_lc_ext_feat_fn_t)(uint32_t, uint32_t, uint8_t);

/**
 * r_lc_pair, slot 277 of the IP functions table.
 */
typedef void (*r_lc_pair_fn_t)(uint16_t, int32_t);

/**
 * r_lc_loc_auth, slot 278 of the IP functions table.
 */
typedef void (*r_lc_loc_auth_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_packet_type, slot 279 of the IP functions table.
 */
typedef void (*r_lc_packet_type_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_ssr_nego, slot 280 of the IP functions table.
 */
typedef void (*r_lc_ssr_nego_fn_t)(uint32_t, uint32_t);

/**
 * r_lc_prepare_all_links_for_clk_adj, slot 281 of the IP functions table.
 */
typedef uint32_t (*r_lc_prepare_all_links_for_clk_adj_fn_t)(void);

/**
 * r_lc_sco_init, slot 282 of the IP functions table.
 */
typedef void (*r_lc_sco_init_fn_t)(void);

/**
 * r_lc_sco_reset, slot 283 of the IP functions table.
 */
typedef void (*r_lc_sco_reset_fn_t)(void);

/**
 * r_lc_sco_detach, slot 284 of the IP functions table.
 */
typedef uint32_t (*r_lc_sco_detach_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_sco_release, slot 285 of the IP functions table.
 */
typedef void (*r_lc_sco_release_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_sco_host_request, slot 286 of the IP functions table.
 */
typedef int32_t (*r_lc_sco_host_request_fn_t)(uint32_t, uint8_t, uint32_t, void *);

/**
 * r_lc_sco_host_accept_wrapper, slot 287 of the IP functions table.
 */
typedef int32_t (*r_lc_sco_host_accept_fn_t)(uint32_t, uint8_t, uint8_t, void *);

/**
 * r_lc_sco_host_reject, slot 288 of the IP functions table.
 */
typedef void (*r_lc_sco_host_reject_fn_t)(uint32_t, uint8_t, uint8_t);

/**
 * r_lc_sco_host_request_disc, slot 289 of the IP functions table.
 */
typedef uint32_t (*r_lc_sco_host_request_disc_fn_t)(uint32_t, uint32_t, uint8_t);

/**
 * r_lc_sco_peer_request, slot 290 of the IP functions table.
 */
typedef void (*r_lc_sco_peer_request_fn_t)(uint32_t, uint8_t, uint8_t *);

/**
 * r_lc_sco_peer_accept, slot 291 of the IP functions table.
 */
typedef void (*r_lc_sco_peer_accept_fn_t)(uint32_t);

/**
 * r_lc_sco_peer_reject, slot 292 of the IP functions table.
 */
typedef void (*r_lc_sco_peer_reject_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_sco_peer_request_disc_hack, slot 293 of the IP functions table.
 */
typedef void (*r_lc_sco_peer_request_disc_hack_fn_t)(uint32_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_sco_peer_accept_disc, slot 294 of the IP functions table.
 */
typedef void (*r_lc_sco_peer_accept_disc_fn_t)(uint32_t);

/**
 * r_lc_sco_peer_reject_disc, slot 295 of the IP functions table.
 */
typedef void (*r_lc_sco_peer_reject_disc_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_sco_baseband_ack, slot 296 of the IP functions table.
 */
typedef void (*r_lc_sco_baseband_ack_fn_t)(uint32_t);

/**
 * r_lc_sco_timeout, slot 297 of the IP functions table.
 */
typedef void (*r_lc_sco_timeout_fn_t)(uint32_t);

/**
 * r_lc_sniff_init, slot 298 of the IP functions table.
 */
typedef void (*r_lc_sniff_init_fn_t)(void);

/**
 * r_lc_sniff_reset, slot 299 of the IP functions table.
 */
typedef void (*r_lc_sniff_reset_fn_t)(void);

/**
 * RivieraWaves "IP functions" dispatch table type definition.
 *
 * This is a function pointer table used by RivieraWaves' Bluetooth stack.
 * Because the base RivieraWaves stack is provided in ROM (read-only memory),
 * it cannot be modified directly. For this reason, the stack calls the functions
 * via this pointer table indirectly because it  allows for dynamic replacement
 * or patching of the functions at runtime.
 */
struct r_ip_funcs
{
    r_bt_util_buf_init_fn_t bt_util_buf_init;                                                       /* [0] */
    r_bt_util_buf_lmp_tx_alloc_fn_t bt_util_buf_lmp_tx_alloc;                                       /* [1] */
    r_bt_util_buf_lmp_tx_free_fn_t bt_util_buf_lmp_tx_free;                                         /* [2] */
    r_bt_util_buf_acl_rx_alloc_fn_t bt_util_buf_acl_rx_alloc;                                       /* [3] */
    r_bt_util_buf_acl_rx_free_fn_t bt_util_buf_acl_rx_free;                                         /* [4] */
    r_bt_util_buf_acl_tx_alloc_fn_t bt_util_buf_acl_tx_alloc;                                       /* [5] */
    r_bt_util_buf_acl_tx_free_fn_t bt_util_buf_acl_tx_free;                                         /* [6] */
    r_bt_util_buf_sync_init_fn_t bt_util_buf_sync_init;                                             /* [7] */
    r_bt_util_buf_sync_clear_fn_t bt_util_buf_sync_clear;                                           /* [8] */
    r_bt_util_buf_sync_tx_alloc_fn_t bt_util_buf_sync_tx_alloc;                                     /* [9] */
    r_bt_util_buf_sync_tx_free_fn_t bt_util_buf_sync_tx_free;                                       /* [10] */
    r_bt_util_buf_sync_rx_alloc_fn_t bt_util_buf_sync_rx_alloc;                                     /* [11] */
    r_bt_util_buf_sync_rx_free_fn_t bt_util_buf_sync_rx_free;                                       /* [12] */
    r_E1_fn_t E1;                                                                                   /* [13] */
    r_E21_fn_t E21;                                                                                 /* [14] */
    r_E22_fn_t E22;                                                                                 /* [15] */
    r_E3_fn_t E3;                                                                                   /* [16] */
    r_KPrimC_fn_t KPrimC;                                                                           /* [17] */
    r_XorKey_fn_t XorKey;                                                                           /* [18] */
    r_LM_MakeRandVec_fn_t LM_MakeRandVec;                                                           /* [19] */
    r_lmp_pack_fn_t lmp_pack;                                                                       /* [20] */
    r_lmp_unpack_fn_t lmp_unpack;                                                                   /* [21] */
    r_lm_n_is_zero_fn_t lm_n_is_zero;                                                               /* [22] */
    r_lm_sp_n_one_fn_t lm_sp_n_one;                                                                 /* [23] */
    r_lm_sp_sha256_calculate_fn_t lm_sp_sha256_calculate;                                           /* [24] */
    r_lm_sp_n192_convert_wnaf_fn_t lm_sp_n192_convert_wnaf;                                         /* [25] */
    r_lm_sp_p192_point_to_inf_fn_t lm_sp_p192_point_to_inf;                                         /* [26] */
    r_lm_sp_p192_point_jacobian_to_affine_fn_t lm_sp_p192_point_jacobian_to_affine;                 /* [27] */
    r_lm_sp_p192_points_jacobian_to_affine_fn_t lm_sp_p192_points_jacobian_to_affine;               /* [28] */
    r_lm_sp_pre_compute_points_fn_t lm_sp_pre_compute_points;                                       /* [29] */
    r_lm_sp_p192_dbl_fn_t lm_sp_p192_dbl;                                                           /* [30] */
    r_lm_sp_p192_add_fn_t lm_sp_p192_add;                                                           /* [31] */
    r_lm_sp_p192_invert_fn_t lm_sp_p192_invert;                                                     /* [32] */
    r_lm_f1_fn_t lm_f1;                                                                             /* [33] */
    r_lm_f2_fn_t lm_f2;                                                                             /* [34] */
    r_lm_oob_f1_fn_t lm_oob_f1;                                                                     /* [35] */
    r_lm_g_fn_t lm_g;                                                                               /* [36] */
    r_lm_f3_fn_t lm_f3;                                                                             /* [37] */
    r_lm_get_nonce_fn_t lm_get_nonce;                                                               /* [38] */
    r_lm_dhkey_calc_init_fn_t lm_dhkey_calc_init;                                                   /* [39] */
    r_lm_dhkey_compare_fn_t lm_dhkey_compare;                                                       /* [40] */
    r_F1_256_fn_t F1_256;                                                                           /* [41] */
    r_HMAC_fn_t HMAC;                                                                               /* [42] */
    r_G_256_fn_t G_256;                                                                             /* [43] */
    r_SHA_256_fn_t SHA_256;                                                                         /* [44] */
    r_F2_256_fn_t F2_256;                                                                           /* [45] */
    r_F3_256_fn_t F3_256;                                                                           /* [46] */
    r_H3_fn_t H3;                                                                                   /* [47] */
    r_H4_fn_t H4;                                                                                   /* [48] */
    r_H5_fn_t H5;                                                                                   /* [49] */
    r_hashConcat_fn_t hashConcat;                                                                   /* [50] */
    r_ea_elt_cancel_fn_t ea_elt_cancel;                                                             /* [51] */
    r_ea_time_get_slot_rounded_fn_t ea_time_get_slot_rounded;                                       /* [52] */
    r_ea_init_fn_t ea_init;                                                                         /* [53] */
    r_ea_elt_create_fn_t ea_elt_create;                                                             /* [54] */
    r_ea_elt_insert_fn_t ea_elt_insert;                                                             /* [55] */
    r_ea_time_get_halfslot_rounded_fn_t ea_time_get_halfslot_rounded;                               /* [56] */
    r_ea_elt_remove_fn_t ea_elt_remove;                                                             /* [57] */
    r_ea_interval_create_fn_t ea_interval_create;                                                   /* [58] */
    r_ea_interval_insert_fn_t ea_interval_insert;                                                   /* [59] */
    r_ea_interval_remove_fn_t ea_interval_remove;                                                   /* [60] */
    r_ea_interval_delete_fn_t ea_interval_delete;                                                   /* [61] */
    r_ea_finetimer_isr_fn_t ea_finetimer_isr;                                                       /* [62] */
    r_ea_sw_isr_fn_t ea_sw_isr;                                                                     /* [63] */
    r_ea_offset_req_hack_fn_t ea_offset_req_hack;                                                   /* [64] */
    r_ea_sleep_check_fn_t ea_sleep_check;                                                           /* [65] */
    r_ea_interval_duration_req_fn_t ea_interval_duration_req;                                       /* [66] */
    r_ea_alarm_set_fn_t ea_alarm_set;                                                               /* [67] */
    r_ea_alarm_clear_fn_t ea_alarm_clear;                                                           /* [68] */
    r_em_buf_init_fn_t em_buf_init;                                                                 /* [69] */
    r_em_buf_rx_free_fn_t em_buf_rx_free;                                                           /* [70] */
    r_em_buf_rx_buff_addr_get_fn_t em_buf_rx_buff_addr_get;                                         /* [71] */
    r_em_buf_tx_buff_addr_get_fn_t em_buf_tx_buff_addr_get;                                         /* [72] */
    r_em_buf_tx_free_fn_t em_buf_tx_free;                                                           /* [73] */
    r_hci_fc_init_fn_t hci_fc_init;                                                                 /* [74] */
    r_ram_hci_fc_acl_buf_size_set_fn_t ram_hci_fc_acl_buf_size_set;                                 /* [75] */
    r_ram_hci_fc_sync_buf_size_set_fn_t ram_hci_fc_sync_buf_size_set;                               /* [76] */
    r_hci_fc_acl_en_fn_t hci_fc_acl_en;                                                             /* [77] */
    r_hci_fc_sync_en_fn_t hci_fc_sync_en;                                                           /* [78] */
    r_hci_fc_acl_packet_sent_fn_t hci_fc_acl_packet_sent;                                           /* [79] */
    r_hci_fc_sync_packet_sent_fn_t hci_fc_sync_packet_sent;                                         /* [80] */
    r_hci_fc_host_nb_acl_pkts_complete_fn_t hci_fc_host_nb_acl_pkts_complete;                       /* [81] */
    r_hci_fc_host_nb_sync_pkts_complete_fn_t hci_fc_host_nb_sync_pkts_complete;                     /* [82] */
    r_hci_fc_check_host_available_nb_acl_packets_fn_t hci_fc_check_host_available_nb_acl_packets;   /* [83] */
    r_hci_fc_check_host_available_nb_sync_packets_fn_t hci_fc_check_host_available_nb_sync_packets; /* [84] */
    r_hci_look_for_cmd_desc_hack_fn_t hci_look_for_cmd_desc_hack;                                   /* [85] */
    r_hci_look_for_evt_desc_fn_t hci_look_for_evt_desc;                                             /* [86] */
    r_patch_hci_look_for_dbg_evt_desc_fn_t patch_hci_look_for_dbg_evt_desc;                         /* [87] */
    r_hci_look_for_le_evt_desc_fn_t hci_look_for_le_evt_desc;                                       /* [88] */
    r_hci_init_hack_fn_t hci_init_hack;                                                             /* [89] */
    r_hci_evt_mask_set_fn_t hci_evt_mask_set;                                                       /* [90] */
    r_hci_reset_hack_fn_t hci_reset_hack;                                                           /* [91] */
    r_hci_send_2_host_hack_fn_t hci_send_2_host_hack;                                               /* [92] */
    r_hci_bt_acl_bdaddr_register_hack_fn_t hci_bt_acl_bdaddr_register_hack;                         /* [93] */
    r_hci_bt_acl_conhdl_register_fn_t hci_bt_acl_conhdl_register;                                   /* [94] */
    r_hci_bt_acl_bdaddr_unregister_hack_fn_t hci_bt_acl_bdaddr_unregister_hack;                     /* [95] */
    r_hci_evt_filter_add_fn_t hci_evt_filter_add;                                                   /* [96] */
    r_hci_voice_settings_get_fn_t hci_voice_settings_get;                                           /* [97] */
    r_hci_voice_settings_set_fn_t hci_voice_settings_set;                                           /* [98] */
    r_hci_tl_send_fn_t hci_tl_send;                                                                 /* [99] */
    r_hci_tl_init_fn_t hci_tl_init;                                                                 /* [100] */
    r_hci_cmd_get_max_param_size_fn_t hci_cmd_get_max_param_size;                                   /* [101] */
    r_hci_cmd_received_fn_t hci_cmd_received;                                                       /* [102] */
    r_hci_acl_tx_data_alloc_fn_t hci_acl_tx_data_alloc;                                             /* [103] */
    r_hci_acl_tx_data_received_fn_t hci_acl_tx_data_received;                                       /* [104] */
    r_hci_sync_tx_data_alloc_fn_t hci_sync_tx_data_alloc;                                           /* [105] */
    r_hci_sync_tx_data_received_fn_t hci_sync_tx_data_received;                                     /* [106] */
    r_hci_util_pack_fn_t hci_util_pack;                                                             /* [107] */
    r_hci_util_unpack_fn_t hci_util_unpack;                                                         /* [108] */
    r_LM_ExtractMaxEncKeySize_fn_t LM_ExtractMaxEncKeySize;                                         /* [109] */
    r_lb_init_hack_fn_t lb_init_hack;                                                               /* [110] */
    r_lb_mst_stop_act_bcst_enc_fn_t lb_mst_stop_act_bcst_enc;                                       /* [111] */
    r_lb_reset_hack_fn_t lb_reset_hack;                                                             /* [112] */
    r_lb_util_get_nb_broadcast_fn_t lb_util_get_nb_broadcast;                                       /* [113] */
    r_lb_util_set_nb_broadcast_fn_t lb_util_set_nb_broadcast;                                       /* [114] */
    r_lb_util_get_res_lt_addr_fn_t lb_util_get_res_lt_addr;                                         /* [115] */
    r_lb_util_get_csb_mode_fn_t lb_util_get_csb_mode;                                               /* [116] */
    r_lb_send_lmp_fn_t lb_send_lmp;                                                                 /* [117] */
    r_lb_clk_adj_activate_fn_t lb_clk_adj_activate;                                                 /* [118] */
    r_lb_clk_adj_period_update_fn_t lb_clk_adj_period_update;                                       /* [119] */
    r_lb_send_pdu_clk_adj_fn_t lb_send_pdu_clk_adj;                                                 /* [120] */
    r_lb_clk_adj_id_get_fn_t lb_clk_adj_id_get;                                                     /* [121] */
    r_lb_mst_key_cmp_fn_t lb_mst_key_cmp;                                                           /* [122] */
    r_lb_mst_key_fn_t lb_mst_key;                                                                   /* [123] */
    r_lb_mst_key_restart_enc_fn_t lb_mst_key_restart_enc;                                           /* [124] */
    r_lb_mst_start_act_bcst_enc_fn_t lb_mst_start_act_bcst_enc;                                     /* [125] */
    r_LM_GetMasterKey_fn_t LM_GetMasterKey;                                                         /* [126] */
    r_LM_GetMasterKeyRand_fn_t LM_GetMasterKeyRand;                                                 /* [127] */
    r_LM_GetMasterEncRand_fn_t LM_GetMasterEncRand;                                                 /* [128] */
    r_LM_GetMasterEncKeySize_fn_t LM_GetMasterEncKeySize;                                           /* [129] */
    r_lc_send_lmp_fn_t lc_send_lmp;                                                                 /* [130] */
    r_lc_send_pdu_acc_fn_t lc_send_pdu_acc;                                                         /* [131] */
    r_lc_send_pdu_acc_ext4_fn_t lc_send_pdu_acc_ext4;                                               /* [132] */
    r_lc_send_pdu_not_acc_fn_t lc_send_pdu_not_acc;                                                 /* [133] */
    r_lc_send_pdu_not_acc_ext4_fn_t lc_send_pdu_not_acc_ext4;                                       /* [134] */
    r_lc_send_pdu_set_afh_fn_t lc_send_pdu_set_afh;                                                 /* [135] */
    r_lc_send_pdu_au_rand_fn_t lc_send_pdu_au_rand;                                                 /* [136] */
    r_lc_send_pdu_in_rand_fn_t lc_send_pdu_in_rand;                                                 /* [137] */
    r_lc_send_pdu_comb_key_fn_t lc_send_pdu_comb_key;                                               /* [138] */
    r_lc_send_pdu_unit_key_fn_t lc_send_pdu_unit_key;                                               /* [139] */
    r_lc_send_pdu_max_slot_fn_t lc_send_pdu_max_slot;                                               /* [140] */
    r_lc_send_pdu_max_slot_req_fn_t lc_send_pdu_max_slot_req;                                       /* [141] */
    r_lc_send_pdu_encaps_payl_fn_t lc_send_pdu_encaps_payl;                                         /* [142] */
    r_lc_send_pdu_encaps_head_fn_t lc_send_pdu_encaps_head;                                         /* [143] */
    r_lc_send_pdu_clk_adj_ack_fn_t lc_send_pdu_clk_adj_ack;                                         /* [144] */
    r_lc_send_pdu_clk_adj_req_fn_t lc_send_pdu_clk_adj_req;                                         /* [145] */
    r_lc_send_pdu_ptt_req_fn_t lc_send_pdu_ptt_req;                                                 /* [146] */
    r_lc_send_pdu_sp_nb_fn_t lc_send_pdu_sp_nb;                                                     /* [147] */
    r_lc_send_pdu_sp_cfm_fn_t lc_send_pdu_sp_cfm;                                                   /* [148] */
    r_lc_send_pdu_sres_fn_t lc_send_pdu_sres;                                                       /* [149] */
    r_lc_send_pdu_sco_lk_rem_req_fn_t lc_send_pdu_sco_lk_rem_req;                                   /* [150] */
    r_lc_send_pdu_esco_lk_rem_req_fn_t lc_send_pdu_esco_lk_rem_req;                                 /* [151] */
    r_lc_send_pdu_auto_rate_fn_t lc_send_pdu_auto_rate;                                             /* [152] */
    r_lc_send_pdu_sniff_req_fn_t lc_send_pdu_sniff_req;                                             /* [153] */
    r_lc_send_pdu_lsto_fn_t lc_send_pdu_lsto;                                                       /* [154] */
    r_lc_send_pdu_enc_key_sz_req_fn_t lc_send_pdu_enc_key_sz_req;                                   /* [155] */
    r_lc_send_pdu_feats_res_fn_t lc_send_pdu_feats_res;                                             /* [156] */
    r_lc_send_pdu_tim_acc_fn_t lc_send_pdu_tim_acc;                                                 /* [157] */
    r_lc_send_pdu_feats_ext_req_fn_t lc_send_pdu_feats_ext_req;                                     /* [158] */
    r_lc_send_pdu_io_cap_res_fn_t lc_send_pdu_io_cap_res;                                           /* [159] */
    r_lc_send_pdu_num_comp_fail_fn_t lc_send_pdu_num_comp_fail;                                     /* [160] */
    r_lc_send_pdu_pause_enc_aes_req_fn_t lc_send_pdu_pause_enc_aes_req;                             /* [161] */
    r_lc_send_pdu_paus_enc_req_fn_t lc_send_pdu_paus_enc_req;                                       /* [162] */
    r_lc_send_pdu_resu_enc_req_fn_t lc_send_pdu_resu_enc_req;                                       /* [163] */
    r_lc_send_pdu_setup_cmp_fn_t lc_send_pdu_setup_cmp;                                             /* [164] */
    r_lc_send_pdu_qos_req_fn_t lc_send_pdu_qos_req;                                                 /* [165] */
    r_lc_send_pdu_slot_off_fn_t lc_send_pdu_slot_off;                                               /* [166] */
    r_lc_send_pdu_unsniff_req_fn_t lc_send_pdu_unsniff_req;                                         /* [167] */
    r_lc_send_pdu_vers_req_fn_t lc_send_pdu_vers_req;                                               /* [168] */
    r_lc_send_pdu_dhkey_chk_fn_t lc_send_pdu_dhkey_chk;                                             /* [169] */
    r_lc_cmd_stat_send_fn_t lc_cmd_stat_send;                                                       /* [170] */
    r_lc_init_hack_fn_t lc_init_hack;                                                               /* [171] */
    r_lc_reset_hack_fn_t lc_reset_hack;                                                             /* [172] */
    r_lc_start_hack_fn_t lc_start_hack;                                                             /* [173] */
    r_lc_start_lmp_to_fn_t lc_start_lmp_to;                                                         /* [174] */
    r_lc_afh_set_fn_t lc_afh_set;                                                                   /* [175] */
    r_patch_lc_auth_cmp_fn_t patch_lc_auth_cmp;                                                     /* [176] */
    r_lc_conn_seq_done_fn_t lc_conn_seq_done;                                                       /* [177] */
    r_lc_send_enc_chg_evt_fn_t lc_send_enc_chg_evt;                                                 /* [178] */
    r_lc_detach_fn_t lc_detach;                                                                     /* [179] */
    r_lc_calc_link_key_fn_t lc_calc_link_key;                                                       /* [180] */
    r_lc_sp_end_fn_t lc_sp_end;                                                                     /* [181] */
    r_lc_init_start_mutual_auth_fn_t lc_init_start_mutual_auth;                                     /* [182] */
    r_lc_chg_pkt_type_cont_fn_t lc_chg_pkt_type_cont;                                               /* [183] */
    r_lc_chg_pkt_type_cmp_fn_t lc_chg_pkt_type_cmp;                                                 /* [184] */
    r_lc_chg_pkt_type_retry_fn_t lc_chg_pkt_type_retry;                                             /* [185] */
    r_lc_afh_start_fn_t lc_afh_start;                                                               /* [186] */
    r_lc_chk_to_fn_t lc_chk_to;                                                                     /* [187] */
    r_lc_comb_key_svr_fn_t lc_comb_key_svr;                                                         /* [188] */
    r_lc_con_cmp_fn_t lc_con_cmp;                                                                   /* [189] */
    r_lc_con_cmp_evt_send_fn_t lc_con_cmp_evt_send;                                                 /* [190] */
    r_lc_dhkey_fn_t lc_dhkey;                                                                       /* [191] */
    r_lc_init_calc_f3_fn_t lc_init_calc_f3;                                                         /* [192] */
    r_lc_sp_fail_fn_t lc_sp_fail;                                                                   /* [193] */
    r_lc_locepr_rsw_fn_t lc_locepr_rsw;                                                             /* [194] */
    r_lc_locepr_lkref_fn_t lc_locepr_lkref;                                                         /* [195] */
    r_lc_enc_key_refresh_fn_t lc_enc_key_refresh;                                                   /* [196] */
    r_lc_end_chk_colli_fn_t lc_end_chk_colli;                                                       /* [197] */
    r_lc_rem_enc_fn_t lc_rem_enc;                                                                   /* [198] */
    r_lc_enc_cmp_fn_t lc_enc_cmp;                                                                   /* [199] */
    r_lc_restart_enc_cont_fn_t lc_restart_enc_cont;                                                 /* [200] */
    r_lc_end_of_sniff_nego_fn_t lc_end_of_sniff_nego;                                               /* [201] */
    r_lc_rem_trans_mode_fn_t lc_rem_trans_mode;                                                     /* [202] */
    r_lc_restore_to_fn_t lc_restore_to;                                                             /* [203] */
    r_lc_sniff_slot_unchange_fn_t lc_sniff_slot_unchange;                                           /* [204] */
    r_lc_sniff_max_slot_chg_fn_t lc_sniff_max_slot_chg;                                             /* [205] */
    r_lc_epr_cmp_fn_t lc_epr_cmp;                                                                   /* [206] */
    r_lc_epr_change_lk_fn_t lc_epr_change_lk;                                                       /* [207] */
    r_lc_rsw_clean_up_fn_t lc_rsw_clean_up;                                                         /* [208] */
    r_lc_rsw_done_fn_t lc_rsw_done;                                                                 /* [209] */
    r_lc_mutual_auth_end2_fn_t lc_mutual_auth_end2;                                                 /* [210] */
    r_lc_epr_rsw_cmp_fn_t lc_epr_rsw_cmp;                                                           /* [211] */
    r_lc_restore_afh_reporting_fn_t lc_restore_afh_reporting;                                       /* [212] */
    r_lc_start_enc_fn_t lc_start_enc;                                                               /* [213] */
    r_lc_enter_sniff_mode_fn_t lc_enter_sniff_mode;                                                 /* [214] */
    r_lc_init_passkey_loop_fn_t lc_init_passkey_loop;                                               /* [215] */
    r_lc_initiator_epr_fn_t lc_initiator_epr;                                                       /* [216] */
    r_lc_epr_resp_fn_t lc_epr_resp;                                                                 /* [217] */
    r_lc_stop_enc_fn_t lc_stop_enc;                                                                 /* [218] */
    r_lc_key_exch_end_fn_t lc_key_exch_end;                                                         /* [219] */
    r_lc_mutual_auth_end_fn_t lc_mutual_auth_end;                                                   /* [220] */
    r_lc_legacy_pair_fn_t lc_legacy_pair;                                                           /* [221] */
    r_lc_pairing_cont_fn_t lc_pairing_cont;                                                         /* [222] */
    r_lc_local_switch_fn_t lc_local_switch;                                                         /* [223] */
    r_lc_stop_afh_report_fn_t lc_stop_afh_report;                                                   /* [224] */
    r_lc_local_trans_mode_fn_t lc_local_trans_mode;                                                 /* [225] */
    r_lc_local_untrans_mode_fn_t lc_local_untrans_mode;                                             /* [226] */
    r_lc_max_slot_mgt_fn_t lc_max_slot_mgt;                                                         /* [227] */
    r_lc_mst_key_fn_t lc_mst_key;                                                                   /* [228] */
    r_lc_semi_key_cmp_fn_t lc_semi_key_cmp;                                                         /* [229] */
    r_lc_mst_send_mst_key_fn_t lc_mst_send_mst_key;                                                 /* [230] */
    r_lc_mst_qos_done_fn_t lc_mst_qos_done;                                                         /* [231] */
    r_lc_passkey_comm_fn_t lc_passkey_comm;                                                         /* [232] */
    r_lc_proc_rcv_dhkey_fn_t lc_proc_rcv_dhkey;                                                     /* [233] */
    r_lc_resp_wait_dhkey_cont_fn_t lc_resp_wait_dhkey_cont;                                         /* [234] */
    r_lc_ptt_fn_t lc_ptt;                                                                           /* [235] */
    r_lc_ptt_cmp_fn_t lc_ptt_cmp;                                                                   /* [236] */
    r_lc_qos_setup_fn_t lc_qos_setup;                                                               /* [237] */
    r_lc_rd_rem_name_fn_t lc_rd_rem_name;                                                           /* [238] */
    r_lc_rem_switch_fn_t lc_rem_switch;                                                             /* [239] */
    r_lc_switch_cmp_fn_t lc_switch_cmp;                                                             /* [240] */
    r_lc_start_enc_key_size_fn_t lc_start_enc_key_size;                                             /* [241] */
    r_lc_rem_nego_trans_mode_fn_t lc_rem_nego_trans_mode;                                           /* [242] */
    r_lc_rem_name_cont_fn_t lc_rem_name_cont;                                                       /* [243] */
    r_lc_rem_untrans_mode_fn_t lc_rem_untrans_mode;                                                 /* [244] */
    r_lc_unsniff_cont_fn_t lc_unsniff_cont;                                                         /* [245] */
    r_lc_sec_auth_compute_sres_fn_t lc_sec_auth_compute_sres;                                       /* [246] */
    r_lc_resp_sec_auth_fn_t lc_resp_sec_auth;                                                       /* [247] */
    r_lc_resp_auth_fn_t lc_resp_auth;                                                               /* [248] */
    r_lc_resp_oob_wait_nonce_fn_t lc_resp_oob_wait_nonce;                                           /* [249] */
    r_lc_resp_oob_nonce_fn_t lc_resp_oob_nonce;                                                     /* [250] */
    r_lc_sp_oob_tid_fail_fn_t lc_sp_oob_tid_fail;                                                   /* [251] */
    r_lc_resp_calc_f3_fn_t lc_resp_calc_f3;                                                         /* [252] */
    r_lc_release_fn_t lc_release;                                                                   /* [253] */
    r_lc_restart_enc_fn_t lc_restart_enc;                                                           /* [254] */
    r_lc_send_enc_mode_fn_t lc_send_enc_mode;                                                       /* [255] */
    r_lc_ret_sniff_max_slot_chg_fn_t lc_ret_sniff_max_slot_chg;                                     /* [256] */
    r_lc_upd_to_fn_t lc_upd_to;                                                                     /* [257] */
    r_lc_start_key_exch_fn_t lc_start_key_exch;                                                     /* [258] */
    r_lc_start_passkey_fn_t lc_start_passkey;                                                       /* [259] */
    r_lc_start_passkey_loop_fn_t lc_start_passkey_loop;                                             /* [260] */
    r_lc_start_oob_fn_t lc_start_oob;                                                               /* [261] */
    r_lc_skip_hl_oob_req_fn_t lc_skip_hl_oob_req;                                                   /* [262] */
    r_lc_resp_num_comp_fn_t lc_resp_num_comp;                                                       /* [263] */
    r_lc_sniff_sub_mode_fn_t lc_sniff_sub_mode;                                                     /* [264] */
    r_lc_unit_key_svr_fn_t lc_unit_key_svr;                                                         /* [265] */
    r_lc_rem_unsniff_fn_t lc_rem_unsniff;                                                           /* [266] */
    r_lc_unsniff_cmp_fn_t lc_unsniff_cmp;                                                           /* [267] */
    r_lc_resp_pair_fn_t lc_resp_pair;                                                               /* [268] */
    r_lc_feat_fn_t lc_feat;                                                                         /* [269] */
    r_lc_hl_connect_fn_t lc_hl_connect;                                                             /* [270] */
    r_lc_version_fn_t lc_version;                                                                   /* [271] */
    r_lc_loc_sniff_fn_t lc_loc_sniff;                                                               /* [272] */
    r_lc_rem_sniff_sub_rate_fn_t lc_rem_sniff_sub_rate;                                             /* [273] */
    r_lc_unsniff_fn_t lc_unsniff;                                                                   /* [274] */
    r_lc_rem_sniff_fn_t lc_rem_sniff;                                                               /* [275] */
    r_lc_ext_feat_fn_t lc_ext_feat;                                                                 /* [276] */
    r_lc_pair_fn_t lc_pair;                                                                         /* [277] */
    r_lc_loc_auth_fn_t lc_loc_auth;                                                                 /* [278] */
    r_lc_packet_type_fn_t lc_packet_type;                                                           /* [279] */
    r_lc_ssr_nego_fn_t lc_ssr_nego;                                                                 /* [280] */
    r_lc_prepare_all_links_for_clk_adj_fn_t lc_prepare_all_links_for_clk_adj;                       /* [281] */
    r_lc_sco_init_fn_t lc_sco_init;                                                                 /* [282] */
    r_lc_sco_reset_fn_t lc_sco_reset;                                                               /* [283] */
    r_lc_sco_detach_fn_t lc_sco_detach;                                                             /* [284] */
    r_lc_sco_release_fn_t lc_sco_release;                                                           /* [285] */
    r_lc_sco_host_request_fn_t lc_sco_host_request;                                                 /* [286] */
    r_lc_sco_host_accept_fn_t lc_sco_host_accept;                                                   /* [287] */
    r_lc_sco_host_reject_fn_t lc_sco_host_reject;                                                   /* [288] */
    r_lc_sco_host_request_disc_fn_t lc_sco_host_request_disc;                                       /* [289] */
    r_lc_sco_peer_request_fn_t lc_sco_peer_request;                                                 /* [290] */
    r_lc_sco_peer_accept_fn_t lc_sco_peer_accept;                                                   /* [291] */
    r_lc_sco_peer_reject_fn_t lc_sco_peer_reject;                                                   /* [292] */
    r_lc_sco_peer_request_disc_hack_fn_t lc_sco_peer_request_disc_hack;                             /* [293] */
    r_lc_sco_peer_accept_disc_fn_t lc_sco_peer_accept_disc;                                         /* [294] */
    r_lc_sco_peer_reject_disc_fn_t lc_sco_peer_reject_disc;                                         /* [295] */
    r_lc_sco_baseband_ack_fn_t lc_sco_baseband_ack;                                                 /* [296] */
    r_lc_sco_timeout_fn_t lc_sco_timeout;                                                           /* [297] */
    r_lc_sniff_init_fn_t lc_sniff_init;                                                             /* [298] */
    r_lc_sniff_reset_fn_t lc_sniff_reset;                                                           /* [299] */
    void *fn[];                                                                                     /* [300..] not yet typed */
};

/**
 * Pointer to the RivieraWaves IP functions table.
 *
 * The linker resolves the symbol location against libbtdm_app.a with a fallback
 * on esp32.rom.ld to the internal ROM position.
 */
extern struct r_ip_funcs *r_ip_funcs_p;

/**
 * A self-test function for the ESP32 Bluetooth ROM functions.
 */
bool esp32_bt_rom_selftest(void);

#endif /* ESP32_BT_ROM_H */
