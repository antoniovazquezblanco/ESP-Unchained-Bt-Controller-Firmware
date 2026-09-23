/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Typed declarations for the RivieraWaves "IP functions" dispatch table
 * (r_ip_funcs) of the ESP32 classic (BR/EDR) Bluetooth controller ROM.
 */

#ifndef R_IP_FUNCS_H
#define R_IP_FUNCS_H

#include <stdbool.h>
#include <stdint.h>

/**
 * Signature of r_bt_util_buf_init, slot 0 of the IP functions table.
 * It (re)initialises the controller's exchange-memory buffer pools, tearing
 * them down first when reset is non-zero.
 */
typedef void (*r_bt_util_buf_init_fn_t)(bool reset);

/**
 * Signature of r_bt_util_buf_lmp_tx_alloc, slot 1 of the IP functions table.
 * It allocates an LMP TX buffer from exchange memory, returning its EM offset
 * (0 when the pool is exhausted).
 */
typedef uint16_t (*r_bt_util_buf_lmp_tx_alloc_fn_t)(void);

/**
 * Signature of r_bt_util_buf_lmp_tx_free, slot 2 of the IP functions table.
 * It releases the LMP TX buffer at the given exchange-memory offset back to its
 * pool.
 */
typedef void (*r_bt_util_buf_lmp_tx_free_fn_t)(uint16_t buf);

/**
 * Signature of r_bt_util_buf_acl_rx_alloc, slot 3 of the IP functions table.
 * It allocates an ACL RX buffer from exchange memory, returning its EM offset
 * (0 when the pool is exhausted).
 */
typedef uint16_t (*r_bt_util_buf_acl_rx_alloc_fn_t)(void);

/**
 * Signature of r_bt_util_buf_acl_rx_free, slot 4 of the IP functions table.
 * It releases the ACL RX buffer at the given exchange-memory offset back to its
 * pool.
 */
typedef void (*r_bt_util_buf_acl_rx_free_fn_t)(uint16_t buf);

/**
 * Signature of r_bt_util_buf_acl_tx_alloc, slot 5 of the IP functions table.
 * It allocates an ACL TX buffer from exchange memory, returning its EM offset
 * (0 when the pool is exhausted).
 */
typedef uint16_t (*r_bt_util_buf_acl_tx_alloc_fn_t)(void);

/**
 * Signature of r_bt_util_buf_acl_tx_free, slot 6 of the IP functions table.
 * It releases the ACL TX buffer at the given exchange-memory offset back to its
 * pool.
 */
typedef void (*r_bt_util_buf_acl_tx_free_fn_t)(uint16_t buf);

/**
 * Signature of r_bt_util_buf_sync_init, slot 7 of the IP functions table.
 * It sets up the SCO/eSCO (sync) buffer pools for one sync link. The five
 * uint8_t arguments (link index plus per-direction buffer counts and sizes) are
 * inferred from disassembly; their individual roles are not fully pinned.
 */
typedef void (*r_bt_util_buf_sync_init_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * Signature of r_bt_util_buf_sync_clear, slot 8 of the IP functions table.
 * It drops any sync (SCO/eSCO) TX buffers still queued for the given sync link.
 */
typedef void (*r_bt_util_buf_sync_clear_fn_t)(uint8_t sync_id);

/**
 * Signature of r_bt_util_buf_sync_tx_alloc, slot 9 of the IP functions table.
 * It allocates a sync (SCO/eSCO) TX buffer for the given sync link, returning
 * its EM offset (0 when none free).
 */
typedef uint16_t (*r_bt_util_buf_sync_tx_alloc_fn_t)(uint8_t sync_id, uint8_t len);

/**
 * Signature of r_bt_util_buf_sync_tx_free, slot 10 of the IP functions table.
 * It releases the sync (SCO/eSCO) TX buffer at the given exchange-memory offset
 * for the given sync link.
 */
typedef void (*r_bt_util_buf_sync_tx_free_fn_t)(uint8_t sync_id, uint16_t buf);

/**
 * Signature of r_bt_util_buf_sync_rx_alloc, slot 11 of the IP functions table.
 * It allocates a sync (SCO/eSCO) RX buffer for the given sync link, returning
 * its EM offset (0 when none free).
 */
typedef uint16_t (*r_bt_util_buf_sync_rx_alloc_fn_t)(uint8_t sync_id, uint8_t len);

/**
 * Signature of r_bt_util_buf_sync_rx_free, slot 12 of the IP functions table.
 * It releases the sync (SCO/eSCO) RX buffer at the given exchange-memory offset
 * for the given sync link.
 */
typedef void (*r_bt_util_buf_sync_rx_free_fn_t)(uint8_t sync_id, uint16_t buf);

/**
 * r_E1, slot 13 of the IP functions table.
 * BR/EDR authentication function E1 (SRES/ACO).
 */
typedef void (*r_E1_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_E21, slot 14 of the IP functions table.
 * BR/EDR unit-key generation E21.
 */
typedef void (*r_E21_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t *);

/**
 * r_E22, slot 15 of the IP functions table.
 * BR/EDR init/PIN-key generation E22.
 */
typedef void (*r_E22_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_E3, slot 16 of the IP functions table.
 * BR/EDR ciphering-key generation E3.
 */
typedef void (*r_E3_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_KPrimC, slot 17 of the IP functions table.
 * Constrained ciphering key (Kc') derivation.
 */
typedef void (*r_KPrimC_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t *);

/**
 * r_XorKey, slot 18 of the IP functions table.
 * XOR two key/byte buffers.
 */
typedef void (*r_XorKey_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_LM_MakeRandVec, slot 19 of the IP functions table.
 * Link Manager: MakeRandVec.
 */
typedef void (*r_LM_MakeRandVec_fn_t)(int32_t);

/**
 * r_lmp_pack, slot 20 of the IP functions table.
 * LMP: pack.
 */
typedef int32_t (*r_lmp_pack_fn_t)(uint8_t *, uint8_t *);

/**
 * r_lmp_unpack, slot 21 of the IP functions table.
 * LMP: unpack.
 */
typedef uint8_t (*r_lmp_unpack_fn_t)(uint8_t *, uint8_t *, uint8_t *);

/**
 * r_lm_n_is_zero, slot 22 of the IP functions table.
 * Link Manager: number is zero.
 */
typedef uint32_t (*r_lm_n_is_zero_fn_t)(int32_t, int32_t);

/**
 * r_lm_sp_n_one, slot 23 of the IP functions table.
 * Link Manager: secure simple pairing number one.
 */
typedef void (*r_lm_sp_n_one_fn_t)(void *, int32_t);

/**
 * r_lm_sp_sha256_calculate, slot 24 of the IP functions table.
 * Link Manager: secure simple pairing SHA-256 calculate.
 */
typedef void (*r_lm_sp_sha256_calculate_fn_t)(uint8_t *, uint32_t *, uint16_t);

/**
 * r_lm_sp_n192_convert_wnaf, slot 25 of the IP functions table.
 * Link Manager: secure simple pairing 192-bit number convert wnaf.
 */
typedef uint32_t (*r_lm_sp_n192_convert_wnaf_fn_t)(int32_t, int32_t, uint32_t *);

/**
 * r_lm_sp_p192_point_to_inf, slot 26 of the IP functions table.
 * Link Manager: secure simple pairing P-192 EC point timeout inf.
 */
typedef uint32_t (*r_lm_sp_p192_point_to_inf_fn_t)(int32_t);

/**
 * r_lm_sp_p192_point_jacobian_to_affine, slot 27 of the IP functions table.
 * Link Manager: secure simple pairing P-192 EC point jacobian timeout affine.
 */
typedef uint32_t (*r_lm_sp_p192_point_jacobian_to_affine_fn_t)(int32_t *);

/**
 * r_lm_sp_p192_points_jacobian_to_affine, slot 28 of the IP functions table.
 * Link Manager: secure simple pairing P-192 EC points jacobian timeout affine.
 */
typedef uint32_t (*r_lm_sp_p192_points_jacobian_to_affine_fn_t)(int32_t);

/**
 * r_lm_sp_pre_compute_points, slot 29 of the IP functions table.
 * Link Manager: secure simple pairing pre compute points.
 */
typedef uint32_t (*r_lm_sp_pre_compute_points_fn_t)(int32_t);

/**
 * r_lm_sp_p192_dbl, slot 30 of the IP functions table.
 * Link Manager: secure simple pairing P-192 EC dbl.
 */
typedef uint32_t (*r_lm_sp_p192_dbl_fn_t)(int32_t *, int32_t *);

/**
 * r_lm_sp_p192_add, slot 31 of the IP functions table.
 * Link Manager: secure simple pairing P-192 EC add.
 */
typedef uint32_t (*r_lm_sp_p192_add_fn_t)(uint32_t *, uint32_t *, uint32_t *);

/**
 * r_lm_sp_p192_invert, slot 32 of the IP functions table.
 * Link Manager: secure simple pairing P-192 EC invert.
 */
typedef bool (*r_lm_sp_p192_invert_fn_t)(int32_t);

/**
 * r_lm_f1, slot 33 of the IP functions table.
 * Link Manager: SSP crypto f1.
 */
typedef void (*r_lm_f1_fn_t)(int32_t, uint8_t *, uint32_t, uint32_t, int32_t);

/**
 * r_lm_f2, slot 34 of the IP functions table.
 * Link Manager: SSP crypto f2.
 */
typedef void (*r_lm_f2_fn_t)(void *, void *, void *, void *, int32_t, uint8_t *);

/**
 * r_lm_oob_f1, slot 35 of the IP functions table.
 * Link Manager: SSP crypto oob_f1.
 */
typedef void (*r_lm_oob_f1_fn_t)(int32_t, uint8_t *, uint32_t, uint32_t, int32_t);

/**
 * r_lm_g, slot 36 of the IP functions table.
 * Link Manager: SSP crypto g.
 */
typedef void (*r_lm_g_fn_t)(void *, void *, uint8_t, int32_t, uint32_t *);

/**
 * r_lm_f3, slot 37 of the IP functions table.
 * Link Manager: SSP crypto f3.
 */
typedef void (*r_lm_f3_fn_t)(void *, void *, void *, uint32_t, void *, void *, int32_t, uint8_t *);

/**
 * r_lm_get_nonce, slot 38 of the IP functions table.
 * Link Manager: get nonce.
 */
typedef void (*r_lm_get_nonce_fn_t)(void *);

/**
 * r_lm_dhkey_calc_init, slot 39 of the IP functions table.
 * Link Manager: DH key calc init.
 */
typedef uint32_t (*r_lm_dhkey_calc_init_fn_t)(void *);

/**
 * r_lm_dhkey_compare, slot 40 of the IP functions table.
 * Link Manager: DH key compare.
 */
typedef uint32_t (*r_lm_dhkey_compare_fn_t)(int32_t);

/**
 * r_F1_256, slot 41 of the IP functions table.
 * Secure Connections f1 (SHA-256) function.
 */
typedef void (*r_F1_256_fn_t)(int32_t, int32_t, int32_t, uint8_t *, int32_t);

/**
 * r_HMAC, slot 42 of the IP functions table.
 * HMAC-SHA-256.
 */
typedef void (*r_HMAC_fn_t)(int32_t, int32_t, uint32_t, uint32_t);

/**
 * r_G_256, slot 43 of the IP functions table.
 * Secure Connections g (SHA-256) numeric-compare value.
 */
typedef void (*r_G_256_fn_t)(int32_t, int32_t, int32_t, int32_t, uint32_t *);

/**
 * r_SHA_256, slot 44 of the IP functions table.
 * SHA-256 hash.
 */
typedef void (*r_SHA_256_fn_t)(int32_t, uint32_t, uint8_t *);

/**
 * r_F2_256, slot 45 of the IP functions table.
 * Secure Connections f2 (SHA-256) key derivation.
 */
typedef void (*r_F2_256_fn_t)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);

/**
 * r_F3_256, slot 46 of the IP functions table.
 * Secure Connections f3 (SHA-256) check value.
 */
typedef void (*r_F3_256_fn_t)(int32_t, int32_t, int32_t, int32_t, uint8_t *, int32_t, int32_t, int32_t);

/**
 * r_H3, slot 47 of the IP functions table.
 * Secure Connections h3 key derivation.
 */
typedef int32_t (*r_H3_fn_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

/**
 * r_H4, slot 48 of the IP functions table.
 * Secure Connections h4 key derivation.
 */
typedef int32_t (*r_H4_fn_t)(int32_t, int32_t, int32_t, int32_t);

/**
 * r_H5, slot 49 of the IP functions table.
 * Secure Connections h5 key derivation.
 */
typedef int32_t (*r_H5_fn_t)(int32_t, int32_t, int32_t, int32_t);

/**
 * r_hashConcat, slot 50 of the IP functions table.
 * concatenate and hash inputs.
 */
typedef int32_t (*r_hashConcat_fn_t)(int32_t, uint32_t, int32_t, uint32_t, int32_t);

/**
 * r_ea_elt_cancel, slot 51 of the IP functions table.
 * Event scheduler: elt cancel.
 */
typedef void (*r_ea_elt_cancel_fn_t)(uint32_t *);

/**
 * r_ea_time_get_slot_rounded, slot 52 of the IP functions table.
 * Event scheduler: time get slot rounded.
 */
typedef uint32_t (*r_ea_time_get_slot_rounded_fn_t)(void);

/**
 * r_ea_init, slot 53 of the IP functions table.
 * Event scheduler: init.
 */
typedef void (*r_ea_init_fn_t)(void);

/**
 * r_ea_elt_create, slot 54 of the IP functions table.
 * Event scheduler: elt create.
 */
typedef void *(*r_ea_elt_create_fn_t)(uint32_t);

/**
 * r_ea_elt_insert, slot 55 of the IP functions table.
 * Event scheduler: elt insert.
 */
typedef uint32_t (*r_ea_elt_insert_fn_t)(int32_t);

/**
 * r_ea_time_get_halfslot_rounded, slot 56 of the IP functions table.
 * Event scheduler: time get halfslot rounded.
 */
typedef uint32_t (*r_ea_time_get_halfslot_rounded_fn_t)(void);

/**
 * r_ea_elt_remove, slot 57 of the IP functions table.
 * Event scheduler: elt remove.
 */
typedef uint32_t (*r_ea_elt_remove_fn_t)(int32_t);

/**
 * r_ea_interval_create, slot 58 of the IP functions table.
 * Event scheduler: interval create.
 */
typedef void *(*r_ea_interval_create_fn_t)(void);

/**
 * r_ea_interval_insert, slot 59 of the IP functions table.
 * Event scheduler: interval insert.
 */
typedef void (*r_ea_interval_insert_fn_t)(uint32_t);

/**
 * r_ea_interval_remove, slot 60 of the IP functions table.
 * Event scheduler: interval remove.
 */
typedef void (*r_ea_interval_remove_fn_t)(uint32_t);

/**
 * r_ea_interval_delete, slot 61 of the IP functions table.
 * Event scheduler: interval delete.
 */
typedef void (*r_ea_interval_delete_fn_t)(int32_t);

/**
 * r_ea_finetimer_isr, slot 62 of the IP functions table.
 * Event scheduler: finetimer isr.
 */
typedef void (*r_ea_finetimer_isr_fn_t)(void);

/**
 * r_ea_sw_isr, slot 63 of the IP functions table.
 * Event scheduler: switch isr.
 */
typedef void (*r_ea_sw_isr_fn_t)(void);

/**
 * r_ea_offset_req_hack, slot 64 of the IP functions table.
 * Event scheduler: offset request (Espressif override).
 */
typedef uint32_t (*r_ea_offset_req_hack_fn_t)(int32_t, uint16_t *);

/**
 * r_ea_sleep_check, slot 65 of the IP functions table.
 * Event scheduler: sleep check.
 */
typedef uint32_t (*r_ea_sleep_check_fn_t)(uint32_t *, uint32_t);

/**
 * r_ea_interval_duration_req, slot 66 of the IP functions table.
 * Event scheduler: interval duration request.
 */
typedef void (*r_ea_interval_duration_req_fn_t)(uint16_t *, uint16_t *);

/**
 * r_ea_alarm_set, slot 67 of the IP functions table.
 * Event scheduler: alarm set.
 */
typedef uint32_t (*r_ea_alarm_set_fn_t)(uint32_t *);

/**
 * r_ea_alarm_clear, slot 68 of the IP functions table.
 * Event scheduler: alarm clear.
 */
typedef uint32_t (*r_ea_alarm_clear_fn_t)(int32_t);

/**
 * r_em_buf_init, slot 69 of the IP functions table.
 * Exchange memory: buffer init.
 */
typedef void (*r_em_buf_init_fn_t)(void);

/**
 * r_em_buf_rx_free, slot 70 of the IP functions table.
 * Exchange memory: buffer RX free.
 */
typedef void (*r_em_buf_rx_free_fn_t)(uint32_t);

/**
 * r_em_buf_rx_buff_addr_get, slot 71 of the IP functions table.
 * Exchange memory: buffer RX buff address get.
 */
typedef int32_t (*r_em_buf_rx_buff_addr_get_fn_t)(uint32_t);

/**
 * r_em_buf_tx_buff_addr_get, slot 72 of the IP functions table.
 * Exchange memory: buffer TX buff address get.
 */
typedef int32_t (*r_em_buf_tx_buff_addr_get_fn_t)(int32_t);

/**
 * r_em_buf_tx_free, slot 73 of the IP functions table.
 * Exchange memory: buffer TX free.
 */
typedef bool (*r_em_buf_tx_free_fn_t)(int32_t);

/**
 * r_hci_fc_init, slot 74 of the IP functions table.
 * HCI: flow control init.
 */
typedef void (*r_hci_fc_init_fn_t)(void);

/**
 * ram_hci_fc_acl_buf_size_set, slot 75 of the IP functions table.
 * HCI: flow control ACL buffer size set (Espressif override).
 */
typedef uint32_t (*r_ram_hci_fc_acl_buf_size_set_fn_t)(uint16_t, int16_t);

/**
 * ram_hci_fc_sync_buf_size_set, slot 76 of the IP functions table.
 * HCI: flow control SCO/sync buffer size set (Espressif override).
 */
typedef uint32_t (*r_ram_hci_fc_sync_buf_size_set_fn_t)(uint8_t, int16_t);

/**
 * r_hci_fc_acl_en, slot 77 of the IP functions table.
 * HCI: flow control ACL enable.
 */
typedef int32_t (*r_hci_fc_acl_en_fn_t)(uint8_t);

/**
 * r_hci_fc_sync_en, slot 78 of the IP functions table.
 * HCI: flow control SCO/sync enable.
 */
typedef void (*r_hci_fc_sync_en_fn_t)(uint8_t);

/**
 * r_hci_fc_acl_packet_sent, slot 79 of the IP functions table.
 * HCI: flow control ACL packet sent.
 */
typedef void (*r_hci_fc_acl_packet_sent_fn_t)(void);

/**
 * r_hci_fc_sync_packet_sent, slot 80 of the IP functions table.
 * HCI: flow control SCO/sync packet sent.
 */
typedef void (*r_hci_fc_sync_packet_sent_fn_t)(void);

/**
 * r_hci_fc_host_nb_acl_pkts_complete, slot 81 of the IP functions table.
 * HCI: flow control host number of ACL pkts complete.
 */
typedef void (*r_hci_fc_host_nb_acl_pkts_complete_fn_t)(uint16_t);

/**
 * r_hci_fc_host_nb_sync_pkts_complete, slot 82 of the IP functions table.
 * HCI: flow control host number of SCO/sync pkts complete.
 */
typedef void (*r_hci_fc_host_nb_sync_pkts_complete_fn_t)(uint16_t);

/**
 * r_hci_fc_check_host_available_nb_acl_packets, slot 83 of the IP functions table.
 * HCI: flow control check host available number of ACL packets.
 */
typedef int16_t (*r_hci_fc_check_host_available_nb_acl_packets_fn_t)(void);

/**
 * r_hci_fc_check_host_available_nb_sync_packets, slot 84 of the IP functions table.
 * HCI: flow control check host available number of SCO/sync packets.
 */
typedef int16_t (*r_hci_fc_check_host_available_nb_sync_packets_fn_t)(void);

/**
 * r_hci_look_for_cmd_desc_hack, slot 85 of the IP functions table.
 * HCI: look for command desc (Espressif override).
 */
typedef uint16_t *(*r_hci_look_for_cmd_desc_hack_fn_t)(uint16_t);

/**
 * r_hci_look_for_evt_desc, slot 86 of the IP functions table.
 * HCI: look for event desc.
 */
typedef uint8_t *(*r_hci_look_for_evt_desc_fn_t)(uint8_t);

/**
 * patch_hci_look_for_dbg_evt_desc, slot 87 of the IP functions table.
 * HCI: look for debug event desc (Espressif override).
 */
typedef uint8_t *(*r_patch_hci_look_for_dbg_evt_desc_fn_t)(uint8_t);

/**
 * r_hci_look_for_le_evt_desc, slot 88 of the IP functions table.
 * HCI: look for LE event desc.
 */
typedef uint8_t *(*r_hci_look_for_le_evt_desc_fn_t)(uint8_t);

/**
 * r_hci_init_hack, slot 89 of the IP functions table.
 * HCI: init (Espressif override).
 */
typedef void (*r_hci_init_hack_fn_t)(void);

/**
 * r_hci_evt_mask_set, slot 90 of the IP functions table.
 * HCI: event mask set.
 */
typedef uint32_t (*r_hci_evt_mask_set_fn_t)(void *, uint8_t);

/**
 * r_hci_reset_hack, slot 91 of the IP functions table.
 * HCI: reset (Espressif override).
 */
typedef void (*r_hci_reset_hack_fn_t)(void);

/**
 * r_hci_send_2_host_hack, slot 92 of the IP functions table.
 * Delivers a completed HCI message to the host. Takes the parameter-area pointer
 * from ke_msg_alloc() and reads the message id/kind through negative offsets
 * (param[-8] = id). Applies the HCI event mask and the event/advertising filter
 * tables in hci_env, then either forwards the message to the transport
 * (hci_tl_send) or, when masked or filtered out, drops it; either way the
 * underlying ke_msg is released before returning. The ROM default target is
 * r_hci_send_2_host; this slot is an Espressif override point (hence "_hack").
 *
 * @param param parameter-area pointer from ke_msg_alloc (ke_msg header + 12 bytes).
 */
typedef void (*r_hci_send_2_host_hack_fn_t)(void *param);

/**
 * r_hci_bt_acl_bdaddr_register_hack, slot 93 of the IP functions table.
 * HCI: bt ACL bdaddr register (Espressif override).
 */
typedef void (*r_hci_bt_acl_bdaddr_register_hack_fn_t)(uint32_t, void *);

/**
 * r_hci_bt_acl_conhdl_register, slot 94 of the IP functions table.
 * HCI: bt ACL conhdl register.
 */
typedef void (*r_hci_bt_acl_conhdl_register_fn_t)(uint32_t);

/**
 * r_hci_bt_acl_bdaddr_unregister_hack, slot 95 of the IP functions table.
 * HCI: bt ACL bdaddr unregister (Espressif override).
 */
typedef void (*r_hci_bt_acl_bdaddr_unregister_hack_fn_t)(uint32_t);

/**
 * r_hci_evt_filter_add, slot 96 of the IP functions table.
 * HCI: event filter add.
 */
typedef uint8_t (*r_hci_evt_filter_add_fn_t)(uint8_t *);

/**
 * r_hci_voice_settings_get, slot 97 of the IP functions table.
 * HCI: voice settings get.
 */
typedef uint16_t (*r_hci_voice_settings_get_fn_t)(void);

/**
 * r_hci_voice_settings_set, slot 98 of the IP functions table.
 * HCI: voice settings set.
 */
typedef uint32_t (*r_hci_voice_settings_set_fn_t)(uint16_t);

/**
 * r_hci_tl_send, slot 99 of the IP functions table.
 * HCI: tl send.
 */
typedef void (*r_hci_tl_send_fn_t)(int32_t);

/**
 * r_hci_tl_init, slot 100 of the IP functions table.
 * HCI: tl init.
 */
typedef void (*r_hci_tl_init_fn_t)(uint8_t);

/**
 * r_hci_cmd_get_max_param_size, slot 101 of the IP functions table.
 * HCI: command get max param size.
 */
typedef uint8_t (*r_hci_cmd_get_max_param_size_fn_t)(uint16_t);

/**
 * r_hci_cmd_received, slot 102 of the IP functions table.
 * The controller's entry point for a host HCI command. It looks up the command
 * descriptor (via hci_look_for_cmd_desc_hack), unpacks the parameters into a
 * kernel message and routes it to the owning task, which runs the actual handler;
 * an unregistered opcode is answered with "Unknown HCI Command".
 *
 * @param opcode  16-bit HCI opcode (OGF << 10 | OCF).
 * @param length  number of parameter bytes at payload.
 * @param payload the command's raw parameter bytes (length bytes).
 */
typedef void (*r_hci_cmd_received_fn_t)(uint16_t opcode, uint8_t length, uint8_t *payload);

/**
 * r_hci_acl_tx_data_alloc, slot 103 of the IP functions table.
 * HCI: ACL TX data alloc.
 */
typedef int32_t (*r_hci_acl_tx_data_alloc_fn_t)(uint32_t, uint16_t);

/**
 * r_hci_acl_tx_data_received, slot 104 of the IP functions table.
 * HCI: ACL TX data received.
 */
typedef void (*r_hci_acl_tx_data_received_fn_t)(uint16_t, uint16_t, uint32_t);

/**
 * r_hci_sync_tx_data_alloc, slot 105 of the IP functions table.
 * HCI: SCO/sync TX data alloc.
 */
typedef int32_t (*r_hci_sync_tx_data_alloc_fn_t)(uint32_t, uint8_t);

/**
 * r_hci_sync_tx_data_received, slot 106 of the IP functions table.
 * HCI: SCO/sync TX data received.
 */
typedef void (*r_hci_sync_tx_data_received_fn_t)(uint16_t, uint32_t, uint32_t);

/**
 * r_hci_util_pack, slot 107 of the IP functions table.
 * HCI: util pack.
 */
typedef int32_t (*r_hci_util_pack_fn_t)(uint32_t *, uint16_t *, uint8_t *);

/**
 * r_hci_util_unpack, slot 108 of the IP functions table.
 * HCI: util unpack.
 */
typedef int32_t (*r_hci_util_unpack_fn_t)(uint32_t *, uint32_t *, uint16_t *, uint32_t, uint8_t *);

/**
 * r_LM_ExtractMaxEncKeySize, slot 109 of the IP functions table.
 * Link Manager: ExtractMaxEncKeySize.
 */
typedef uint32_t (*r_LM_ExtractMaxEncKeySize_fn_t)(int16_t);

/**
 * r_lb_init_hack, slot 110 of the IP functions table.
 * Broadcast: init (Espressif override).
 */
typedef void (*r_lb_init_hack_fn_t)(void);

/**
 * r_lb_mst_stop_act_bcst_enc, slot 111 of the IP functions table.
 * Broadcast: mst stop act broadcast encryption.
 */
typedef void (*r_lb_mst_stop_act_bcst_enc_fn_t)(void);

/**
 * r_lb_reset_hack, slot 112 of the IP functions table.
 * Broadcast: reset (Espressif override).
 */
typedef void (*r_lb_reset_hack_fn_t)(void);

/**
 * r_lb_util_get_nb_broadcast, slot 113 of the IP functions table.
 * Broadcast: util get number of broadcast.
 */
typedef uint8_t (*r_lb_util_get_nb_broadcast_fn_t)(void);

/**
 * r_lb_util_set_nb_broadcast, slot 114 of the IP functions table.
 * Broadcast: util set number of broadcast.
 */
typedef void (*r_lb_util_set_nb_broadcast_fn_t)(uint8_t);

/**
 * r_lb_util_get_res_lt_addr, slot 115 of the IP functions table.
 * Broadcast: util get res logical transport address.
 */
typedef uint8_t (*r_lb_util_get_res_lt_addr_fn_t)(void);

/**
 * r_lb_util_get_csb_mode, slot 116 of the IP functions table.
 * Broadcast: util get connectionless slave broadcast mode.
 */
typedef uint8_t (*r_lb_util_get_csb_mode_fn_t)(void);

/**
 * r_lb_send_lmp, slot 117 of the IP functions table.
 * Broadcast: send lmp.
 */
typedef void (*r_lb_send_lmp_fn_t)(void *, uint32_t, uint8_t);

/**
 * r_lb_clk_adj_activate, slot 118 of the IP functions table.
 * Broadcast: clock adjust activate.
 */
typedef void (*r_lb_clk_adj_activate_fn_t)(int16_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lb_clk_adj_period_update, slot 119 of the IP functions table.
 * Broadcast: clock adjust period update.
 */
typedef void (*r_lb_clk_adj_period_update_fn_t)(uint8_t);

/**
 * r_lb_send_pdu_clk_adj, slot 120 of the IP functions table.
 * Broadcast: send PDU clock adjust.
 */
typedef void (*r_lb_send_pdu_clk_adj_fn_t)(uint32_t, uint8_t, uint16_t, uint8_t, uint8_t, int32_t, uint8_t);

/**
 * r_lb_clk_adj_id_get, slot 121 of the IP functions table.
 * Broadcast: clock adjust identifier get.
 */
typedef uint8_t (*r_lb_clk_adj_id_get_fn_t)(void);

/**
 * r_lb_mst_key_cmp, slot 122 of the IP functions table.
 * Broadcast: mst key cmp.
 */
typedef void (*r_lb_mst_key_cmp_fn_t)(uint8_t, uint16_t, uint8_t);

/**
 * r_lb_mst_key, slot 123 of the IP functions table.
 * Broadcast: mst key.
 */
typedef void (*r_lb_mst_key_fn_t)(void);

/**
 * r_lb_mst_key_restart_enc, slot 124 of the IP functions table.
 * Broadcast: mst key restart encryption.
 */
typedef void (*r_lb_mst_key_restart_enc_fn_t)(uint16_t);

/**
 * r_lb_mst_start_act_bcst_enc, slot 125 of the IP functions table.
 * Broadcast: mst start act broadcast encryption.
 */
typedef void (*r_lb_mst_start_act_bcst_enc_fn_t)(void);

/**
 * r_LM_GetMasterKey, slot 126 of the IP functions table.
 * Link Manager: GetMasterKey.
 */
typedef void (*r_LM_GetMasterKey_fn_t)(void *);

/**
 * r_LM_GetMasterKeyRand, slot 127 of the IP functions table.
 * Link Manager: GetMasterKeyRand.
 */
typedef void (*r_LM_GetMasterKeyRand_fn_t)(void *);

/**
 * r_LM_GetMasterEncRand, slot 128 of the IP functions table.
 * Link Manager: GetMasterEncRand.
 */
typedef void (*r_LM_GetMasterEncRand_fn_t)(void *);

/**
 * r_LM_GetMasterEncKeySize, slot 129 of the IP functions table.
 * Link Manager: GetMasterEncKeySize.
 */
typedef uint8_t (*r_LM_GetMasterEncKeySize_fn_t)(void);

/**
 * r_lc_send_lmp, slot 130 of the IP functions table.
 * Link Controller: send lmp.
 */
typedef void (*r_lc_send_lmp_fn_t)(uint8_t, void *);

/**
 * r_lc_send_pdu_acc, slot 131 of the IP functions table.
 * Link Controller: send PDU acc.
 */
typedef void (*r_lc_send_pdu_acc_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_acc_ext4, slot 132 of the IP functions table.
 * Link Controller: send PDU acc ext4.
 */
typedef void (*r_lc_send_pdu_acc_ext4_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_not_acc, slot 133 of the IP functions table.
 * Link Controller: send PDU not acc.
 */
typedef void (*r_lc_send_pdu_not_acc_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_not_acc_ext4, slot 134 of the IP functions table.
 * Link Controller: send PDU not acc ext4.
 */
typedef void (*r_lc_send_pdu_not_acc_ext4_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_set_afh, slot 135 of the IP functions table.
 * Link Controller: send PDU set AFH.
 */
typedef void (*r_lc_send_pdu_set_afh_fn_t)(uint32_t, uint32_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_au_rand, slot 136 of the IP functions table.
 * Link Controller: send PDU au random.
 */
typedef void (*r_lc_send_pdu_au_rand_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_in_rand, slot 137 of the IP functions table.
 * Link Controller: send PDU in random.
 */
typedef void (*r_lc_send_pdu_in_rand_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_comb_key, slot 138 of the IP functions table.
 * Link Controller: send PDU comb key.
 */
typedef void (*r_lc_send_pdu_comb_key_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_unit_key, slot 139 of the IP functions table.
 * Link Controller: send PDU unit key.
 */
typedef void (*r_lc_send_pdu_unit_key_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_max_slot, slot 140 of the IP functions table.
 * Link Controller: send PDU max slot.
 */
typedef void (*r_lc_send_pdu_max_slot_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_max_slot_req, slot 141 of the IP functions table.
 * Link Controller: send PDU max slot request.
 */
typedef void (*r_lc_send_pdu_max_slot_req_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_encaps_payl, slot 142 of the IP functions table.
 * Link Controller: send PDU encaps payl.
 */
typedef void (*r_lc_send_pdu_encaps_payl_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_encaps_head, slot 143 of the IP functions table.
 * Link Controller: send PDU encaps head.
 */
typedef void (*r_lc_send_pdu_encaps_head_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_clk_adj_ack, slot 144 of the IP functions table.
 * Link Controller: send PDU clock adjust ack.
 */
typedef void (*r_lc_send_pdu_clk_adj_ack_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_clk_adj_req, slot 145 of the IP functions table.
 * Link Controller: send PDU clock adjust request.
 */
typedef void (*r_lc_send_pdu_clk_adj_req_fn_t)(uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_ptt_req, slot 146 of the IP functions table.
 * Link Controller: send PDU packet-type table request.
 */
typedef void (*r_lc_send_pdu_ptt_req_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_sp_nb, slot 147 of the IP functions table.
 * Link Controller: send PDU secure simple pairing number of.
 */
typedef void (*r_lc_send_pdu_sp_nb_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_sp_cfm, slot 148 of the IP functions table.
 * Link Controller: send PDU secure simple pairing confirm.
 */
typedef void (*r_lc_send_pdu_sp_cfm_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_send_pdu_sres, slot 149 of the IP functions table.
 * Link Controller: send PDU sres.
 */
typedef void (*r_lc_send_pdu_sres_fn_t)(uint8_t, uint8_t *, uint8_t);

/**
 * r_lc_send_pdu_sco_lk_rem_req, slot 150 of the IP functions table.
 * Link Controller: send PDU SCO lk remote request.
 */
typedef void (*r_lc_send_pdu_sco_lk_rem_req_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_esco_lk_rem_req, slot 151 of the IP functions table.
 * Link Controller: send PDU eSCO lk remote request.
 */
typedef void (*r_lc_send_pdu_esco_lk_rem_req_fn_t)(uint8_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_auto_rate, slot 152 of the IP functions table.
 * Link Controller: send PDU auto rate.
 */
typedef void (*r_lc_send_pdu_auto_rate_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_sniff_req, slot 153 of the IP functions table.
 * Link Controller: send PDU sniff mode request.
 */
typedef void (*r_lc_send_pdu_sniff_req_fn_t)(uint32_t);

/**
 * r_lc_send_pdu_lsto, slot 154 of the IP functions table.
 * Link Controller: send PDU link supervision timeout.
 */
typedef void (*r_lc_send_pdu_lsto_fn_t)(uint8_t, uint16_t, uint8_t);

/**
 * r_lc_send_pdu_enc_key_sz_req, slot 155 of the IP functions table.
 * Link Controller: send PDU encryption key sz request.
 */
typedef void (*r_lc_send_pdu_enc_key_sz_req_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_feats_res, slot 156 of the IP functions table.
 * Link Controller: send PDU feats res.
 */
typedef void (*r_lc_send_pdu_feats_res_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_tim_acc, slot 157 of the IP functions table.
 * Link Controller: send PDU tim acc.
 */
typedef void (*r_lc_send_pdu_tim_acc_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_feats_ext_req, slot 158 of the IP functions table.
 * Link Controller: send PDU feats ext request.
 */
typedef void (*r_lc_send_pdu_feats_ext_req_fn_t)(uint8_t, uint8_t, uint8_t);

/**
 * r_lc_send_pdu_io_cap_res, slot 159 of the IP functions table.
 * Link Controller: send PDU io cap res.
 */
typedef void (*r_lc_send_pdu_io_cap_res_fn_t)(uint32_t);

/**
 * r_lc_send_pdu_num_comp_fail, slot 160 of the IP functions table.
 * Link Controller: send PDU num comp fail.
 */
typedef void (*r_lc_send_pdu_num_comp_fail_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_pause_enc_aes_req, slot 161 of the IP functions table.
 * Link Controller: send PDU pause encryption aes request.
 */
typedef void (*r_lc_send_pdu_pause_enc_aes_req_fn_t)(uint8_t, uint8_t, void *);

/**
 * r_lc_send_pdu_paus_enc_req, slot 162 of the IP functions table.
 * Link Controller: send PDU paus encryption request.
 */
typedef void (*r_lc_send_pdu_paus_enc_req_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_resu_enc_req, slot 163 of the IP functions table.
 * Link Controller: send PDU resu encryption request.
 */
typedef void (*r_lc_send_pdu_resu_enc_req_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_setup_cmp, slot 164 of the IP functions table.
 * Link Controller: send PDU setup cmp.
 */
typedef void (*r_lc_send_pdu_setup_cmp_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_qos_req, slot 165 of the IP functions table.
 * Link Controller: send PDU QoS request.
 */
typedef void (*r_lc_send_pdu_qos_req_fn_t)(uint8_t, uint8_t, uint16_t, uint8_t);

/**
 * r_lc_send_pdu_slot_off, slot 166 of the IP functions table.
 * Link Controller: send PDU slot off.
 */
typedef void (*r_lc_send_pdu_slot_off_fn_t)(uint32_t, uint16_t, uint8_t);

/**
 * r_lc_send_pdu_unsniff_req, slot 167 of the IP functions table.
 * Link Controller: send PDU unsniff request.
 */
typedef void (*r_lc_send_pdu_unsniff_req_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_vers_req, slot 168 of the IP functions table.
 * Link Controller: send PDU vers request.
 */
typedef void (*r_lc_send_pdu_vers_req_fn_t)(uint8_t, uint8_t);

/**
 * r_lc_send_pdu_dhkey_chk, slot 169 of the IP functions table.
 * Link Controller: send PDU DH key chk.
 */
typedef void (*r_lc_send_pdu_dhkey_chk_fn_t)(uint8_t, void *, uint8_t);

/**
 * r_lc_cmd_stat_send, slot 170 of the IP functions table.
 * Link Controller: command stat send.
 */
typedef void (*r_lc_cmd_stat_send_fn_t)(uint16_t, uint8_t);

/**
 * r_lc_init_hack, slot 171 of the IP functions table.
 * Link Controller: init (Espressif override).
 */
typedef void (*r_lc_init_hack_fn_t)(void);

/**
 * r_lc_reset_hack, slot 172 of the IP functions table.
 * Link Controller: reset (Espressif override).
 */
typedef void (*r_lc_reset_hack_fn_t)(void);

/**
 * r_lc_start_hack, slot 173 of the IP functions table.
 * Link Controller: start (Espressif override).
 */
typedef void (*r_lc_start_hack_fn_t)(uint32_t, uint8_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint8_t);

/**
 * r_lc_start_lmp_to, slot 174 of the IP functions table.
 * Link Controller: start lmp timeout.
 */
typedef void (*r_lc_start_lmp_to_fn_t)(uint32_t);

/**
 * r_lc_afh_set, slot 175 of the IP functions table.
 * Link Controller: AFH set.
 */
typedef void (*r_lc_afh_set_fn_t)(uint32_t, uint8_t);

/**
 * patch_lc_auth_cmp, slot 176 of the IP functions table.
 * Link Controller: authentication cmp (Espressif override).
 */
typedef void (*r_patch_lc_auth_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_conn_seq_done, slot 177 of the IP functions table.
 * Link Controller: connection seq done.
 */
typedef uint8_t (*r_lc_conn_seq_done_fn_t)(uint32_t);

/**
 * r_lc_send_enc_chg_evt, slot 178 of the IP functions table.
 * Link Controller: send encryption chg event.
 */
typedef void (*r_lc_send_enc_chg_evt_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_detach, slot 179 of the IP functions table.
 * Link Controller: detach.
 */
typedef void (*r_lc_detach_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_calc_link_key, slot 180 of the IP functions table.
 * Link Controller: calc link key.
 */
typedef void (*r_lc_calc_link_key_fn_t)(uint32_t);

/**
 * r_lc_sp_end, slot 181 of the IP functions table.
 * Link Controller: secure simple pairing end.
 */
typedef void (*r_lc_sp_end_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_init_start_mutual_auth, slot 182 of the IP functions table.
 * Link Controller: init start mutual authentication.
 */
typedef void (*r_lc_init_start_mutual_auth_fn_t)(uint32_t);

/**
 * r_lc_chg_pkt_type_cont, slot 183 of the IP functions table.
 * Link Controller: chg pkt type cont.
 */
typedef void (*r_lc_chg_pkt_type_cont_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_chg_pkt_type_cmp, slot 184 of the IP functions table.
 * Link Controller: chg pkt type cmp.
 */
typedef void (*r_lc_chg_pkt_type_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_chg_pkt_type_retry, slot 185 of the IP functions table.
 * Link Controller: chg pkt type retry.
 */
typedef uint32_t (*r_lc_chg_pkt_type_retry_fn_t)(uint32_t);

/**
 * r_lc_afh_start, slot 186 of the IP functions table.
 * Link Controller: AFH start.
 */
typedef void (*r_lc_afh_start_fn_t)(uint32_t);

/**
 * r_lc_chk_to, slot 187 of the IP functions table.
 * Link Controller: chk timeout.
 */
typedef void (*r_lc_chk_to_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_comb_key_svr, slot 188 of the IP functions table.
 * Link Controller: comb key svr.
 */
typedef void (*r_lc_comb_key_svr_fn_t)(uint32_t, void *);

/**
 * r_lc_con_cmp, slot 189 of the IP functions table.
 * Link Controller: connection cmp.
 */
typedef void (*r_lc_con_cmp_fn_t)(uint32_t);

/**
 * r_lc_con_cmp_evt_send, slot 190 of the IP functions table.
 * Link Controller: connection cmp event send.
 */
typedef void (*r_lc_con_cmp_evt_send_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_dhkey, slot 191 of the IP functions table.
 * Link Controller: DH key.
 */
typedef void (*r_lc_dhkey_fn_t)(uint32_t);

/**
 * r_lc_init_calc_f3, slot 192 of the IP functions table.
 * Link Controller: init calc f3.
 */
typedef void (*r_lc_init_calc_f3_fn_t)(uint32_t);

/**
 * r_lc_sp_fail, slot 193 of the IP functions table.
 * Link Controller: secure simple pairing fail.
 */
typedef void (*r_lc_sp_fail_fn_t)(uint32_t);

/**
 * r_lc_locepr_rsw, slot 194 of the IP functions table.
 * Link Controller: locepr rsw.
 */
typedef void (*r_lc_locepr_rsw_fn_t)(uint32_t);

/**
 * r_lc_locepr_lkref, slot 195 of the IP functions table.
 * Link Controller: locepr lkref.
 */
typedef void (*r_lc_locepr_lkref_fn_t)(uint32_t);

/**
 * r_lc_enc_key_refresh, slot 196 of the IP functions table.
 * Link Controller: encryption key refresh.
 */
typedef void (*r_lc_enc_key_refresh_fn_t)(uint32_t);

/**
 * r_lc_end_chk_colli, slot 197 of the IP functions table.
 * Link Controller: end chk colli.
 */
typedef void (*r_lc_end_chk_colli_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_rem_enc, slot 198 of the IP functions table.
 * Link Controller: remote encryption.
 */
typedef void (*r_lc_rem_enc_fn_t)(uint32_t);

/**
 * r_lc_enc_cmp, slot 199 of the IP functions table.
 * Link Controller: encryption cmp.
 */
typedef void (*r_lc_enc_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_restart_enc_cont, slot 200 of the IP functions table.
 * Link Controller: restart encryption cont.
 */
typedef void (*r_lc_restart_enc_cont_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_end_of_sniff_nego, slot 201 of the IP functions table.
 * Link Controller: end of sniff mode negotiation.
 */
typedef void (*r_lc_end_of_sniff_nego_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_rem_trans_mode, slot 202 of the IP functions table.
 * Link Controller: remote trans mode.
 */
typedef void (*r_lc_rem_trans_mode_fn_t)(uint32_t);

/**
 * r_lc_restore_to, slot 203 of the IP functions table.
 * Link Controller: restore timeout.
 */
typedef void (*r_lc_restore_to_fn_t)(uint32_t);

/**
 * r_lc_sniff_slot_unchange, slot 204 of the IP functions table.
 * Link Controller: sniff mode slot unchange.
 */
typedef void (*r_lc_sniff_slot_unchange_fn_t)(uint32_t, uint32_t);

/**
 * r_lc_sniff_max_slot_chg, slot 205 of the IP functions table.
 * Link Controller: sniff mode max slot chg.
 */
typedef void (*r_lc_sniff_max_slot_chg_fn_t)(uint32_t);

/**
 * r_lc_epr_cmp, slot 206 of the IP functions table.
 * Link Controller: epr cmp.
 */
typedef void (*r_lc_epr_cmp_fn_t)(uint32_t);

/**
 * r_lc_epr_change_lk, slot 207 of the IP functions table.
 * Link Controller: epr change lk.
 */
typedef void (*r_lc_epr_change_lk_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_rsw_clean_up, slot 208 of the IP functions table.
 * Link Controller: rsw clean up.
 */
typedef void (*r_lc_rsw_clean_up_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_rsw_done, slot 209 of the IP functions table.
 * Link Controller: rsw done.
 */
typedef void (*r_lc_rsw_done_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_mutual_auth_end2, slot 210 of the IP functions table.
 * Link Controller: mutual authentication end2.
 */
typedef void (*r_lc_mutual_auth_end2_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_epr_rsw_cmp, slot 211 of the IP functions table.
 * Link Controller: epr rsw cmp.
 */
typedef void (*r_lc_epr_rsw_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_restore_afh_reporting, slot 212 of the IP functions table.
 * Link Controller: restore AFH reporting.
 */
typedef void (*r_lc_restore_afh_reporting_fn_t)(uint32_t);

/**
 * r_lc_start_enc, slot 213 of the IP functions table.
 * Link Controller: start encryption.
 */
typedef void (*r_lc_start_enc_fn_t)(uint32_t);

/**
 * r_lc_enter_sniff_mode, slot 214 of the IP functions table.
 * Link Controller: enter sniff mode mode.
 */
typedef void (*r_lc_enter_sniff_mode_fn_t)(uint32_t);

/**
 * r_lc_init_passkey_loop, slot 215 of the IP functions table.
 * Link Controller: init passkey loop.
 */
typedef void (*r_lc_init_passkey_loop_fn_t)(uint32_t);

/**
 * r_lc_initiator_epr, slot 216 of the IP functions table.
 * Link Controller: initiator epr.
 */
typedef void (*r_lc_initiator_epr_fn_t)(uint32_t);

/**
 * r_lc_epr_resp, slot 217 of the IP functions table.
 * Link Controller: epr resp.
 */
typedef void (*r_lc_epr_resp_fn_t)(uint32_t);

/**
 * r_lc_stop_enc, slot 218 of the IP functions table.
 * Link Controller: stop encryption.
 */
typedef void (*r_lc_stop_enc_fn_t)(uint32_t);

/**
 * r_lc_key_exch_end, slot 219 of the IP functions table.
 * Link Controller: key exch end.
 */
typedef void (*r_lc_key_exch_end_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_mutual_auth_end, slot 220 of the IP functions table.
 * Link Controller: mutual authentication end.
 */
typedef void (*r_lc_mutual_auth_end_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_legacy_pair, slot 221 of the IP functions table.
 * Link Controller: legacy pair.
 */
typedef void (*r_lc_legacy_pair_fn_t)(uint32_t);

/**
 * r_lc_pairing_cont, slot 222 of the IP functions table.
 * Link Controller: pairing cont.
 */
typedef void (*r_lc_pairing_cont_fn_t)(uint32_t);

/**
 * r_lc_local_switch, slot 223 of the IP functions table.
 * Link Controller: local switch.
 */
typedef void (*r_lc_local_switch_fn_t)(uint32_t);

/**
 * r_lc_stop_afh_report, slot 224 of the IP functions table.
 * Link Controller: stop AFH report.
 */
typedef void (*r_lc_stop_afh_report_fn_t)(uint32_t);

/**
 * r_lc_local_trans_mode, slot 225 of the IP functions table.
 * Link Controller: local trans mode.
 */
typedef void (*r_lc_local_trans_mode_fn_t)(uint32_t);

/**
 * r_lc_local_untrans_mode, slot 226 of the IP functions table.
 * Link Controller: local untrans mode.
 */
typedef void (*r_lc_local_untrans_mode_fn_t)(uint32_t);

/**
 * r_lc_max_slot_mgt, slot 227 of the IP functions table.
 * Link Controller: max slot mgt.
 */
typedef void (*r_lc_max_slot_mgt_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_mst_key, slot 228 of the IP functions table.
 * Link Controller: mst key.
 */
typedef void (*r_lc_mst_key_fn_t)(uint32_t);

/**
 * r_lc_semi_key_cmp, slot 229 of the IP functions table.
 * Link Controller: semi key cmp.
 */
typedef void (*r_lc_semi_key_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_mst_send_mst_key, slot 230 of the IP functions table.
 * Link Controller: mst send mst key.
 */
typedef void (*r_lc_mst_send_mst_key_fn_t)(uint32_t);

/**
 * r_lc_mst_qos_done, slot 231 of the IP functions table.
 * Link Controller: mst QoS done.
 */
typedef void (*r_lc_mst_qos_done_fn_t)(uint32_t);

/**
 * r_lc_passkey_comm, slot 232 of the IP functions table.
 * Link Controller: passkey comm.
 */
typedef void (*r_lc_passkey_comm_fn_t)(uint32_t);

/**
 * r_lc_proc_rcv_dhkey, slot 233 of the IP functions table.
 * Link Controller: proc rcv DH key.
 */
typedef void (*r_lc_proc_rcv_dhkey_fn_t)(uint32_t);

/**
 * r_lc_resp_wait_dhkey_cont, slot 234 of the IP functions table.
 * Link Controller: resp wait DH key cont.
 */
typedef void (*r_lc_resp_wait_dhkey_cont_fn_t)(uint32_t);

/**
 * r_lc_ptt, slot 235 of the IP functions table.
 * Link Controller: packet-type table.
 */
typedef void (*r_lc_ptt_fn_t)(uint32_t);

/**
 * r_lc_ptt_cmp, slot 236 of the IP functions table.
 * Link Controller: packet-type table cmp.
 */
typedef void (*r_lc_ptt_cmp_fn_t)(uint32_t);

/**
 * r_lc_qos_setup, slot 237 of the IP functions table.
 * Link Controller: QoS setup.
 */
typedef void (*r_lc_qos_setup_fn_t)(uint32_t);

/**
 * r_lc_rd_rem_name, slot 238 of the IP functions table.
 * Link Controller: rd remote name.
 */
typedef void (*r_lc_rd_rem_name_fn_t)(uint32_t);

/**
 * r_lc_rem_switch, slot 239 of the IP functions table.
 * Link Controller: remote switch.
 */
typedef void (*r_lc_rem_switch_fn_t)(uint32_t);

/**
 * r_lc_switch_cmp, slot 240 of the IP functions table.
 * Link Controller: switch cmp.
 */
typedef void (*r_lc_switch_cmp_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_start_enc_key_size, slot 241 of the IP functions table.
 * Link Controller: start encryption key size.
 */
typedef void (*r_lc_start_enc_key_size_fn_t)(uint32_t);

/**
 * r_lc_rem_nego_trans_mode, slot 242 of the IP functions table.
 * Link Controller: remote negotiation trans mode.
 */
typedef void (*r_lc_rem_nego_trans_mode_fn_t)(uint32_t);

/**
 * r_lc_rem_name_cont, slot 243 of the IP functions table.
 * Link Controller: remote name cont.
 */
typedef void (*r_lc_rem_name_cont_fn_t)(uint32_t);

/**
 * r_lc_rem_untrans_mode, slot 244 of the IP functions table.
 * Link Controller: remote untrans mode.
 */
typedef void (*r_lc_rem_untrans_mode_fn_t)(uint32_t);

/**
 * r_lc_unsniff_cont, slot 245 of the IP functions table.
 * Link Controller: unsniff cont.
 */
typedef void (*r_lc_unsniff_cont_fn_t)(uint32_t);

/**
 * r_lc_sec_auth_compute_sres, slot 246 of the IP functions table.
 * Link Controller: sec authentication compute sres.
 */
typedef void (*r_lc_sec_auth_compute_sres_fn_t)(uint32_t);

/**
 * r_lc_resp_sec_auth, slot 247 of the IP functions table.
 * Link Controller: resp sec authentication.
 */
typedef void (*r_lc_resp_sec_auth_fn_t)(uint32_t);

/**
 * r_lc_resp_auth, slot 248 of the IP functions table.
 * Link Controller: resp authentication.
 */
typedef void (*r_lc_resp_auth_fn_t)(uint32_t);

/**
 * r_lc_resp_oob_wait_nonce, slot 249 of the IP functions table.
 * Link Controller: resp oob wait nonce.
 */
typedef void (*r_lc_resp_oob_wait_nonce_fn_t)(uint16_t);

/**
 * r_lc_resp_oob_nonce, slot 250 of the IP functions table.
 * Link Controller: resp oob nonce.
 */
typedef void (*r_lc_resp_oob_nonce_fn_t)(uint32_t);

/**
 * r_lc_sp_oob_tid_fail, slot 251 of the IP functions table.
 * Link Controller: secure simple pairing oob tid fail.
 */
typedef void (*r_lc_sp_oob_tid_fail_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_resp_calc_f3, slot 252 of the IP functions table.
 * Link Controller: resp calc f3.
 */
typedef void (*r_lc_resp_calc_f3_fn_t)(uint32_t);

/**
 * r_lc_release, slot 253 of the IP functions table.
 * Link Controller: release.
 */
typedef void (*r_lc_release_fn_t)(uint32_t);

/**
 * r_lc_restart_enc, slot 254 of the IP functions table.
 * Link Controller: restart encryption.
 */
typedef void (*r_lc_restart_enc_fn_t)(uint32_t);

/**
 * r_lc_send_enc_mode, slot 255 of the IP functions table.
 * Link Controller: send encryption mode.
 */
typedef void (*r_lc_send_enc_mode_fn_t)(uint32_t);

/**
 * r_lc_ret_sniff_max_slot_chg, slot 256 of the IP functions table.
 * Link Controller: ret sniff mode max slot chg.
 */
typedef void (*r_lc_ret_sniff_max_slot_chg_fn_t)(uint32_t);

/**
 * r_lc_upd_to, slot 257 of the IP functions table.
 * Link Controller: update timeout.
 */
typedef void (*r_lc_upd_to_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_start_key_exch, slot 258 of the IP functions table.
 * Link Controller: start key exch.
 */
typedef void (*r_lc_start_key_exch_fn_t)(uint32_t);

/**
 * r_lc_start_passkey, slot 259 of the IP functions table.
 * Link Controller: start passkey.
 */
typedef void (*r_lc_start_passkey_fn_t)(uint32_t);

/**
 * r_lc_start_passkey_loop, slot 260 of the IP functions table.
 * Link Controller: start passkey loop.
 */
typedef void (*r_lc_start_passkey_loop_fn_t)(uint32_t);

/**
 * r_lc_start_oob, slot 261 of the IP functions table.
 * Link Controller: start oob.
 */
typedef void (*r_lc_start_oob_fn_t)(uint32_t);

/**
 * r_lc_skip_hl_oob_req, slot 262 of the IP functions table.
 * Link Controller: skip higher-layer oob request.
 */
typedef void (*r_lc_skip_hl_oob_req_fn_t)(uint32_t);

/**
 * r_lc_resp_num_comp, slot 263 of the IP functions table.
 * Link Controller: resp num comp.
 */
typedef void (*r_lc_resp_num_comp_fn_t)(uint32_t);

/**
 * r_lc_sniff_sub_mode, slot 264 of the IP functions table.
 * Link Controller: sniff mode sub mode.
 */
typedef void (*r_lc_sniff_sub_mode_fn_t)(uint32_t);

/**
 * r_lc_unit_key_svr, slot 265 of the IP functions table.
 * Link Controller: unit key svr.
 */
typedef void (*r_lc_unit_key_svr_fn_t)(uint32_t, void *);

/**
 * r_lc_rem_unsniff, slot 266 of the IP functions table.
 * Link Controller: remote unsniff.
 */
typedef void (*r_lc_rem_unsniff_fn_t)(uint32_t);

/**
 * r_lc_unsniff_cmp, slot 267 of the IP functions table.
 * Link Controller: unsniff cmp.
 */
typedef void (*r_lc_unsniff_cmp_fn_t)(uint32_t);

/**
 * r_lc_resp_pair, slot 268 of the IP functions table.
 * Link Controller: resp pair.
 */
typedef void (*r_lc_resp_pair_fn_t)(uint16_t, int32_t);

/**
 * r_lc_feat, slot 269 of the IP functions table.
 * Link Controller: features.
 */
typedef void (*r_lc_feat_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_hl_connect, slot 270 of the IP functions table.
 * Link Controller: higher-layer connect.
 */
typedef void (*r_lc_hl_connect_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_version, slot 271 of the IP functions table.
 * Link Controller: version.
 */
typedef void (*r_lc_version_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_loc_sniff, slot 272 of the IP functions table.
 * Link Controller: local sniff mode.
 */
typedef void (*r_lc_loc_sniff_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_rem_sniff_sub_rate, slot 273 of the IP functions table.
 * Link Controller: remote sniff mode sub rate.
 */
typedef void (*r_lc_rem_sniff_sub_rate_fn_t)(uint32_t);

/**
 * r_lc_unsniff, slot 274 of the IP functions table.
 * Link Controller: unsniff.
 */
typedef void (*r_lc_unsniff_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_rem_sniff, slot 275 of the IP functions table.
 * Link Controller: remote sniff mode.
 */
typedef void (*r_lc_rem_sniff_fn_t)(uint32_t, uint32_t);

/**
 * r_lc_ext_feat, slot 276 of the IP functions table.
 * Link Controller: ext features.
 */
typedef void (*r_lc_ext_feat_fn_t)(uint32_t, uint32_t, uint8_t);

/**
 * r_lc_pair, slot 277 of the IP functions table.
 * Link Controller: pair.
 */
typedef void (*r_lc_pair_fn_t)(uint16_t, int32_t);

/**
 * r_lc_loc_auth, slot 278 of the IP functions table.
 * Link Controller: local authentication.
 */
typedef void (*r_lc_loc_auth_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_packet_type, slot 279 of the IP functions table.
 * Link Controller: packet type.
 */
typedef void (*r_lc_packet_type_fn_t)(uint16_t, uint32_t);

/**
 * r_lc_ssr_nego, slot 280 of the IP functions table.
 * Link Controller: sniff subrating negotiation.
 */
typedef void (*r_lc_ssr_nego_fn_t)(uint32_t, uint32_t);

/**
 * r_lc_prepare_all_links_for_clk_adj, slot 281 of the IP functions table.
 * Link Controller: prepare all links for clock adjust.
 */
typedef uint32_t (*r_lc_prepare_all_links_for_clk_adj_fn_t)(void);

/**
 * r_lc_sco_init, slot 282 of the IP functions table.
 * Link Controller: SCO init.
 */
typedef void (*r_lc_sco_init_fn_t)(void);

/**
 * r_lc_sco_reset, slot 283 of the IP functions table.
 * Link Controller: SCO reset.
 */
typedef void (*r_lc_sco_reset_fn_t)(void);

/**
 * r_lc_sco_detach, slot 284 of the IP functions table.
 * Link Controller: SCO detach.
 */
typedef uint32_t (*r_lc_sco_detach_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_sco_release, slot 285 of the IP functions table.
 * Link Controller: SCO release.
 */
typedef void (*r_lc_sco_release_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_sco_host_request, slot 286 of the IP functions table.
 * Link Controller: SCO host request.
 */
typedef int32_t (*r_lc_sco_host_request_fn_t)(uint32_t, uint8_t, uint32_t, void *);

/**
 * r_lc_sco_host_accept_wrapper, slot 287 of the IP functions table.
 * Link Controller: SCO host accept (libbtdm wrapper).
 */
typedef int32_t (*r_lc_sco_host_accept_fn_t)(uint32_t, uint8_t, uint8_t, void *);

/**
 * r_lc_sco_host_reject, slot 288 of the IP functions table.
 * Link Controller: SCO host reject.
 */
typedef void (*r_lc_sco_host_reject_fn_t)(uint32_t, uint8_t, uint8_t);

/**
 * r_lc_sco_host_request_disc, slot 289 of the IP functions table.
 * Link Controller: SCO host request disc.
 */
typedef uint32_t (*r_lc_sco_host_request_disc_fn_t)(uint32_t, uint32_t, uint32_t);

/**
 * r_lc_sco_peer_request, slot 290 of the IP functions table.
 * Link Controller: SCO peer request.
 */
typedef void (*r_lc_sco_peer_request_fn_t)(uint32_t, uint8_t, uint8_t *);

/**
 * r_lc_sco_peer_accept, slot 291 of the IP functions table.
 * Link Controller: SCO peer accept.
 */
typedef void (*r_lc_sco_peer_accept_fn_t)(uint32_t);

/**
 * r_lc_sco_peer_reject, slot 292 of the IP functions table.
 * Link Controller: SCO peer reject.
 */
typedef void (*r_lc_sco_peer_reject_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_sco_peer_request_disc_hack, slot 293 of the IP functions table.
 * Link Controller: SCO peer request disc (Espressif override).
 */
typedef void (*r_lc_sco_peer_request_disc_hack_fn_t)(uint32_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_sco_peer_accept_disc, slot 294 of the IP functions table.
 * Link Controller: SCO peer accept disc.
 */
typedef void (*r_lc_sco_peer_accept_disc_fn_t)(uint32_t);

/**
 * r_lc_sco_peer_reject_disc, slot 295 of the IP functions table.
 * Link Controller: SCO peer reject disc.
 */
typedef void (*r_lc_sco_peer_reject_disc_fn_t)(uint32_t, uint8_t);

/**
 * r_lc_sco_baseband_ack, slot 296 of the IP functions table.
 * Link Controller: SCO baseband ack.
 */
typedef void (*r_lc_sco_baseband_ack_fn_t)(uint32_t);

/**
 * r_lc_sco_timeout, slot 297 of the IP functions table.
 * Link Controller: SCO timeout.
 */
typedef void (*r_lc_sco_timeout_fn_t)(uint32_t);

/**
 * r_lc_sniff_init, slot 298 of the IP functions table.
 * Link Controller: sniff mode init.
 */
typedef void (*r_lc_sniff_init_fn_t)(void);

/**
 * r_lc_sniff_reset, slot 299 of the IP functions table.
 * Link Controller: sniff mode reset.
 */
typedef void (*r_lc_sniff_reset_fn_t)(void);

/**
 * r_LM_AddSniff, slot 300 of the IP functions table.
 * Link Manager: AddSniff.
 */
typedef uint32_t (*r_LM_AddSniff_fn_t)(uint32_t, uint8_t, uint8_t, uint32_t, uint32_t, uint16_t *, uint16_t *, uint16_t *, uint16_t *, uint8_t *);

/**
 * r_LM_RemoveSniff, slot 301 of the IP functions table.
 * Link Manager: RemoveSniff.
 */
typedef void (*r_LM_RemoveSniff_fn_t)(uint32_t);

/**
 * r_LM_SniffSubratingHlReq, slot 302 of the IP functions table.
 * Link Manager: SniffSubratingHlReq.
 */
typedef uint32_t (*r_LM_SniffSubratingHlReq_fn_t)(uint32_t, int16_t, int16_t, uint8_t *);

/**
 * r_LM_SniffSubratingPeerReq, slot 303 of the IP functions table.
 * Link Manager: SniffSubratingPeerReq.
 */
typedef uint32_t (*r_LM_SniffSubratingPeerReq_fn_t)(uint32_t, uint8_t, uint8_t, uint32_t);

/**
 * r_LM_GetSniffSubratingParam, slot 304 of the IP functions table.
 * Link Manager: GetSniffSubratingParam.
 */
typedef uint32_t (*r_LM_GetSniffSubratingParam_fn_t)(uint32_t, uint8_t, uint32_t, uint8_t *, uint16_t *, uint32_t *);

/**
 * r_LM_SniffSubrateNegoRequired, slot 305 of the IP functions table.
 * Link Manager: SniffSubrateNegoRequired.
 */
typedef bool (*r_LM_SniffSubrateNegoRequired_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_LM_ComputeSniffSubRate, slot 306 of the IP functions table.
 * Link Manager: ComputeSniffSubRate.
 */
typedef uint32_t (*r_LM_ComputeSniffSubRate_fn_t)(uint32_t, uint32_t);

/**
 * r_LM_RemoveSniffSubrating, slot 307 of the IP functions table.
 * Link Manager: RemoveSniffSubrating.
 */
typedef void (*r_LM_RemoveSniffSubrating_fn_t)(uint32_t);

/**
 * r_LM_GetLinkTimeout, slot 308 of the IP functions table.
 * Link Manager: GetLinkTimeout.
 */
typedef uint32_t (*r_LM_GetLinkTimeout_fn_t)(uint32_t, uint32_t, uint32_t, uint32_t);

/**
 * r_lc_util_get_max_packet_size, slot 309 of the IP functions table.
 * Link Controller: util get max packet size.
 */
typedef int32_t (*r_lc_util_get_max_packet_size_fn_t)(uint32_t);

/**
 * r_lc_util_set_loc_trans_coll, slot 310 of the IP functions table.
 * Link Controller: util set local trans coll.
 */
typedef void (*r_lc_util_set_loc_trans_coll_fn_t)(uint32_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lc_util_get_offset_clkn, slot 311 of the IP functions table.
 * Link Controller: util get offset native clock.
 */
typedef uint32_t (*r_lc_util_get_offset_clkn_fn_t)(uint32_t, uint32_t, uint32_t);

/**
 * r_lc_util_get_offset_clke, slot 312 of the IP functions table.
 * Link Controller: util get offset estimated clock.
 */
typedef uint32_t (*r_lc_util_get_offset_clke_fn_t)(uint32_t, uint32_t, uint32_t);

/**
 * r_LM_ComputePacketType, slot 313 of the IP functions table.
 * Link Manager: ComputePacketType.
 */
typedef uint16_t (*r_LM_ComputePacketType_fn_t)(uint16_t, uint16_t, uint8_t);

/**
 * r_LM_UpdateAclPacketType, slot 314 of the IP functions table.
 * Link Manager: UpdateAclPacketType.
 */
typedef uint16_t (*r_LM_UpdateAclPacketType_fn_t)(uint16_t, uint32_t);

/**
 * r_LM_UpdateAclEdrPacketType, slot 315 of the IP functions table.
 * Link Manager: UpdateAclEdrPacketType.
 */
typedef uint32_t (*r_LM_UpdateAclEdrPacketType_fn_t)(uint32_t, uint32_t, uint8_t, uint32_t);

/**
 * r_LM_SuppressAclPacket, slot 316 of the IP functions table.
 * Link Manager: SuppressAclPacket.
 */
typedef void (*r_LM_SuppressAclPacket_fn_t)(uint16_t *, uint8_t);

/**
 * r_LM_MaxSlot, slot 317 of the IP functions table.
 * Link Manager: MaxSlot.
 */
typedef uint32_t (*r_LM_MaxSlot_fn_t)(uint32_t);

/**
 * r_LM_GetQoSParam, slot 318 of the IP functions table.
 * Link Manager: GetQoSParam.
 */
typedef uint32_t (*r_LM_GetQoSParam_fn_t)(uint8_t, uint8_t *, uint32_t *, uint32_t *, uint32_t *, uint32_t *, uint16_t *, uint16_t, uint8_t);

/**
 * r_LM_GetSwitchInstant, slot 319 of the IP functions table.
 * Link Manager: GetSwitchInstant.
 */
typedef uint32_t (*r_LM_GetSwitchInstant_fn_t)(uint8_t);

/**
 * r_LM_MakeCof, slot 320 of the IP functions table.
 * Link Manager: MakeCof.
 */
typedef void (*r_LM_MakeCof_fn_t)(uint32_t, uint32_t, uint8_t *);

/**
 * r_LM_GetAFHSwitchInstant, slot 321 of the IP functions table.
 * Link Manager: GetAFHSwitchInstant.
 */
typedef uint32_t (*r_LM_GetAFHSwitchInstant_fn_t)(uint8_t);

/**
 * r_LM_CheckSwitchInstant, slot 322 of the IP functions table.
 * Link Manager: CheckSwitchInstant.
 */
typedef uint32_t (*r_LM_CheckSwitchInstant_fn_t)(uint8_t, uint32_t);

/**
 * r_LM_CheckEdrFeatureRequest, slot 323 of the IP functions table.
 * Link Manager: CheckEdrFeatureRequest.
 */
typedef bool (*r_LM_CheckEdrFeatureRequest_fn_t)(uint32_t);

/**
 * r_LM_GetFeature, slot 324 of the IP functions table.
 * Link Manager: GetFeature.
 */
typedef bool (*r_LM_GetFeature_fn_t)(int32_t, uint32_t);

/**
 * r_lm_look_for_stored_link_key, slot 325 of the IP functions table.
 * Link Manager: look for stored link key.
 */
typedef uint32_t (*r_lm_look_for_stored_link_key_fn_t)(void *, void *);

/**
 * r_lc_util_convert_pref_rate_to_packet_type, slot 326 of the IP functions table.
 * Link Controller: util convert pref rate timeout packet type.
 */
typedef uint32_t (*r_lc_util_convert_pref_rate_to_packet_type_fn_t)(uint32_t, uint32_t, uint8_t);

/**
 * r_lm_get_common_pkt_types, slot 327 of the IP functions table.
 * Link Manager: get common pkt types.
 */
typedef uint16_t (*r_lm_get_common_pkt_types_fn_t)(uint16_t, int32_t);

/**
 * r_ld_acl_init, slot 328 of the IP functions table.
 * Link Driver (baseband): ACL init.
 */
typedef void (*r_ld_acl_init_fn_t)(void);

/**
 * r_ld_acl_reset, slot 329 of the IP functions table.
 * Link Driver (baseband): ACL reset.
 */
typedef void (*r_ld_acl_reset_fn_t)(void);

/**
 * r_ld_acl_start, slot 330 of the IP functions table.
 * Link Driver (baseband): ACL start.
 */
typedef uint32_t (*r_ld_acl_start_fn_t)(uint8_t, uint8_t);

/**
 * r_ld_acl_stop, slot 331 of the IP functions table.
 * Link Driver (baseband): ACL stop.
 */
typedef uint32_t (*r_ld_acl_stop_fn_t)(uint32_t);

/**
 * r_ld_acl_flow_off, slot 332 of the IP functions table.
 * Link Driver (baseband): ACL flow off.
 */
typedef uint32_t (*r_ld_acl_flow_off_fn_t)(uint32_t);

/**
 * r_ld_acl_flow_on, slot 333 of the IP functions table.
 * Link Driver (baseband): ACL flow on.
 */
typedef uint32_t (*r_ld_acl_flow_on_fn_t)(uint32_t);

/**
 * r_ld_acl_data_tx, slot 334 of the IP functions table.
 * Link Driver (baseband): ACL data TX.
 */
typedef uint8_t (*r_ld_acl_data_tx_fn_t)(uint32_t, int32_t);

/**
 * r_ld_acl_data_flush, slot 335 of the IP functions table.
 * Link Driver (baseband): ACL data flush.
 */
typedef uint32_t (*r_ld_acl_data_flush_fn_t)(uint32_t, uint8_t *, uint8_t);

/**
 * r_ld_acl_lmp_tx, slot 336 of the IP functions table.
 * Link Driver (baseband): ACL lmp TX.
 */
typedef uint32_t (*r_ld_acl_lmp_tx_fn_t)(uint32_t, int32_t);

/**
 * r_ld_acl_lmp_flush, slot 337 of the IP functions table.
 * Link Driver (baseband): ACL lmp flush.
 */
typedef uint32_t (*r_ld_acl_lmp_flush_fn_t)(uint32_t);

/**
 * r_ld_acl_rsw_req, slot 338 of the IP functions table.
 * Link Driver (baseband): ACL rsw request.
 */
typedef uint32_t (*r_ld_acl_rsw_req_fn_t)(uint32_t, uint8_t, uint16_t, int32_t);

/**
 * r_ld_acl_flush_timeout_get, slot 339 of the IP functions table.
 * Link Driver (baseband): ACL flush timeout get.
 */
typedef uint16_t (*r_ld_acl_flush_timeout_get_fn_t)(uint32_t);

/**
 * r_ld_acl_flush_timeout_set, slot 340 of the IP functions table.
 * Link Driver (baseband): ACL flush timeout set.
 */
typedef void (*r_ld_acl_flush_timeout_set_fn_t)(uint32_t, uint16_t);

/**
 * r_ld_acl_t_poll_get, slot 341 of the IP functions table.
 * Link Driver (baseband): ACL t poll get.
 */
typedef uint16_t (*r_ld_acl_t_poll_get_fn_t)(uint32_t);

/**
 * r_ld_acl_t_poll_set, slot 342 of the IP functions table.
 * Link Driver (baseband): ACL t poll set.
 */
typedef void (*r_ld_acl_t_poll_set_fn_t)(uint32_t, uint16_t);

/**
 * r_ld_acl_sniff_trans_hack, slot 343 of the IP functions table.
 * Link Driver (baseband): ACL sniff mode trans (Espressif override).
 */
typedef uint32_t (*r_ld_acl_sniff_trans_hack_fn_t)(uint32_t, uint16_t, uint16_t, uint16_t, uint16_t);

/**
 * r_ld_acl_sniff_wrapper, slot 344 of the IP functions table.
 * Link Driver (baseband): ACL sniff mode (libbtdm wrapper).
 */
typedef uint32_t (*r_ld_acl_sniff_fn_t)(uint32_t, uint16_t, uint16_t, uint16_t, uint16_t);

/**
 * r_ld_acl_unsniff, slot 345 of the IP functions table.
 * Link Driver (baseband): ACL unsniff.
 */
typedef uint32_t (*r_ld_acl_unsniff_fn_t)(uint32_t);

/**
 * r_ld_acl_ssr_set, slot 346 of the IP functions table.
 * Link Driver (baseband): ACL sniff subrating set.
 */
typedef uint32_t (*r_ld_acl_ssr_set_fn_t)(uint32_t, uint8_t, uint16_t, uint32_t);

/**
 * r_ld_acl_tx_enc, slot 347 of the IP functions table.
 * Link Driver (baseband): ACL TX encryption.
 */
typedef void (*r_ld_acl_tx_enc_fn_t)(uint32_t, uint16_t);

/**
 * r_ld_acl_rx_enc, slot 348 of the IP functions table.
 * Link Driver (baseband): ACL RX encryption.
 */
typedef void (*r_ld_acl_rx_enc_fn_t)(uint32_t, int16_t);

/**
 * r_ld_acl_bcst_rx_dec, slot 349 of the IP functions table.
 * Link Driver (baseband): ACL broadcast RX dec.
 */
typedef void (*r_ld_acl_bcst_rx_dec_fn_t)(uint32_t, uint32_t);

/**
 * r_ld_acl_enc_key_load, slot 350 of the IP functions table.
 * Link Driver (baseband): ACL encryption key load.
 */
typedef void (*r_ld_acl_enc_key_load_fn_t)(uint32_t, int32_t, int32_t);

/**
 * r_ld_acl_clock_offset_get, slot 351 of the IP functions table.
 * Link Driver (baseband): ACL clock offset get.
 */
typedef int32_t (*r_ld_acl_clock_offset_get_fn_t)(uint32_t);

/**
 * r_ld_acl_slot_offset_get, slot 352 of the IP functions table.
 * Link Driver (baseband): ACL slot offset get.
 */
typedef uint32_t (*r_ld_acl_slot_offset_get_fn_t)(uint32_t);

/**
 * r_ld_acl_slot_offset_set, slot 353 of the IP functions table.
 * Link Driver (baseband): ACL slot offset set.
 */
typedef void (*r_ld_acl_slot_offset_set_fn_t)(uint32_t, uint16_t);

/**
 * r_ld_acl_lsto_get, slot 354 of the IP functions table.
 * Link Driver (baseband): ACL link supervision timeout get.
 */
typedef uint16_t (*r_ld_acl_lsto_get_fn_t)(uint32_t);

/**
 * r_ld_acl_lsto_set, slot 355 of the IP functions table.
 * Link Driver (baseband): ACL link supervision timeout set.
 */
typedef void (*r_ld_acl_lsto_set_fn_t)(uint32_t, uint16_t);

/**
 * r_ld_acl_timing_accuracy_set, slot 356 of the IP functions table.
 * Link Driver (baseband): ACL timing accuracy set.
 */
typedef void (*r_ld_acl_timing_accuracy_set_fn_t)(uint32_t, uint8_t, uint8_t);

/**
 * r_ld_acl_edr_set, slot 357 of the IP functions table.
 * Link Driver (baseband): ACL EDR set.
 */
typedef void (*r_ld_acl_edr_set_fn_t)(uint32_t, uint8_t);

/**
 * r_ld_acl_allowed_tx_packet_types_set, slot 358 of the IP functions table.
 * Link Driver (baseband): ACL allowed TX packet types set.
 */
typedef void (*r_ld_acl_allowed_tx_packet_types_set_fn_t)(uint32_t, uint16_t);

/**
 * r_ld_acl_current_tx_power_get, slot 359 of the IP functions table.
 * Link Driver (baseband): ACL current TX power get.
 */
typedef uint32_t (*r_ld_acl_current_tx_power_get_fn_t)(uint32_t, uint8_t);

/**
 * r_ld_acl_clk_set, slot 360 of the IP functions table.
 * Link Driver (baseband): ACL clock set.
 */
typedef uint32_t (*r_ld_acl_clk_set_fn_t)(uint32_t, uint32_t, int16_t);

/**
 * r_ld_acl_clk_adj_set, slot 361 of the IP functions table.
 * Link Driver (baseband): ACL clock adjust set.
 */
typedef uint32_t (*r_ld_acl_clk_adj_set_fn_t)(uint32_t, uint32_t, int16_t, int32_t);

/**
 * r_ld_acl_clk_off_get, slot 362 of the IP functions table.
 * Link Driver (baseband): ACL clock off get.
 */
typedef uint32_t (*r_ld_acl_clk_off_get_fn_t)(uint32_t);

/**
 * r_ld_acl_bit_off_get, slot 363 of the IP functions table.
 * Link Driver (baseband): ACL bit off get.
 */
typedef int32_t (*r_ld_acl_bit_off_get_fn_t)(uint32_t);

/**
 * r_ld_acl_role_get, slot 364 of the IP functions table.
 * Link Driver (baseband): ACL role get.
 */
typedef uint8_t (*r_ld_acl_role_get_fn_t)(uint32_t);

/**
 * r_ld_acl_afh_set, slot 365 of the IP functions table.
 * Link Driver (baseband): ACL AFH set.
 */
typedef uint32_t (*r_ld_acl_afh_set_fn_t)(uint32_t, int32_t, uint8_t, uint16_t *);

/**
 * r_ld_acl_afh_prepare, slot 366 of the IP functions table.
 * Link Driver (baseband): ACL AFH prepare.
 */
typedef uint32_t (*r_ld_acl_afh_prepare_fn_t)(uint32_t, int32_t, uint8_t, void *);

/**
 * r_ld_acl_afh_confirm, slot 367 of the IP functions table.
 * Link Driver (baseband): ACL AFH confirm.
 */
typedef uint32_t (*r_ld_acl_afh_confirm_fn_t)(uint32_t);

/**
 * r_ld_acl_active_hop_types_get, slot 368 of the IP functions table.
 * Link Driver (baseband): ACL active hop types get.
 */
typedef void (*r_ld_acl_active_hop_types_get_fn_t)(void);

/**
 * r_ld_acl_rx_max_slot_get, slot 369 of the IP functions table.
 * Link Driver (baseband): ACL RX max slot get.
 */
typedef uint8_t (*r_ld_acl_rx_max_slot_get_fn_t)(uint32_t);

/**
 * r_ld_acl_rx_max_slot_set, slot 370 of the IP functions table.
 * Link Driver (baseband): ACL RX max slot set.
 */
typedef void (*r_ld_acl_rx_max_slot_set_fn_t)(uint32_t, uint8_t);

/**
 * r_ld_acl_test_mode_set, slot 371 of the IP functions table.
 * Link Driver (baseband): ACL test mode set.
 */
typedef void (*r_ld_acl_test_mode_set_fn_t)(uint32_t, void *);

/**
 * r_ld_acl_rssi_delta_get, slot 372 of the IP functions table.
 * Link Driver (baseband): ACL rssi delta get.
 */
typedef uint8_t (*r_ld_acl_rssi_delta_get_fn_t)(uint32_t);

/**
 * r_ld_sco_start_wrapper, slot 373 of the IP functions table.
 * Link Driver (baseband): SCO start (libbtdm wrapper).
 */
typedef int32_t (*r_ld_sco_start_fn_t)(uint8_t, uint8_t, uint8_t *, uint8_t);

/**
 * r_ld_sco_update, slot 374 of the IP functions table.
 * Link Driver (baseband): SCO update.
 */
typedef uint32_t (*r_ld_sco_update_fn_t)(uint8_t, int32_t);

/**
 * r_ld_sco_stop, slot 375 of the IP functions table.
 * Link Driver (baseband): SCO stop.
 */
typedef uint32_t (*r_ld_sco_stop_fn_t)(uint32_t);

/**
 * r_ld_sco_audio_isr, slot 376 of the IP functions table.
 * Link Driver (baseband): SCO audio isr.
 */
typedef void (*r_ld_sco_audio_isr_fn_t)(uint8_t);

/**
 * r_ld_sco_data_tx_hack, slot 377 of the IP functions table.
 * Link Driver (baseband): SCO data TX (Espressif override).
 */
typedef uint32_t (*r_ld_sco_data_tx_hack_fn_t)(uint32_t, uint32_t);

/**
 * r_ld_bcst_acl_start, slot 378 of the IP functions table.
 * Link Driver (baseband): broadcast ACL start.
 */
typedef void (*r_ld_bcst_acl_start_fn_t)(uint16_t *);

/**
 * r_ld_bcst_acl_init, slot 379 of the IP functions table.
 * Link Driver (baseband): broadcast ACL init.
 */
typedef void (*r_ld_bcst_acl_init_fn_t)(void);

/**
 * r_ld_bcst_acl_reset, slot 380 of the IP functions table.
 * Link Driver (baseband): broadcast ACL reset.
 */
typedef void (*r_ld_bcst_acl_reset_fn_t)(void);

/**
 * r_ld_bcst_lmp_tx, slot 381 of the IP functions table.
 * Link Driver (baseband): broadcast lmp TX.
 */
typedef int32_t (*r_ld_bcst_lmp_tx_fn_t)(int32_t, uint32_t, int32_t, uint8_t);

/**
 * r_ld_bcst_acl_data_tx, slot 382 of the IP functions table.
 * Link Driver (baseband): broadcast ACL data TX.
 */
typedef uint32_t (*r_ld_bcst_acl_data_tx_fn_t)(uint32_t, int32_t, uint32_t, uint8_t);

/**
 * r_ld_bcst_afh_update, slot 383 of the IP functions table.
 * Link Driver (baseband): broadcast AFH update.
 */
typedef uint32_t (*r_ld_bcst_afh_update_fn_t)(uint16_t *);

/**
 * r_ld_bcst_tx_enc, slot 384 of the IP functions table.
 * Link Driver (baseband): broadcast TX encryption.
 */
typedef void (*r_ld_bcst_tx_enc_fn_t)(uint16_t);

/**
 * r_ld_bcst_enc_key_load, slot 385 of the IP functions table.
 * Link Driver (baseband): broadcast encryption key load.
 */
typedef void (*r_ld_bcst_enc_key_load_fn_t)(int32_t);

/**
 * r_ld_csb_rx_init, slot 386 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast RX init.
 */
typedef void (*r_ld_csb_rx_init_fn_t)(void);

/**
 * r_ld_csb_rx_reset, slot 387 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast RX reset.
 */
typedef void (*r_ld_csb_rx_reset_fn_t)(void);

/**
 * r_ld_csb_rx_start, slot 388 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast RX start.
 */
typedef uint32_t (*r_ld_csb_rx_start_fn_t)(uint16_t *);

/**
 * r_ld_csb_rx_afh_update, slot 389 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast RX AFH update.
 */
typedef uint32_t (*r_ld_csb_rx_afh_update_fn_t)(uint16_t *);

/**
 * r_ld_csb_rx_stop, slot 390 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast RX stop.
 */
typedef uint8_t (*r_ld_csb_rx_stop_fn_t)(void);

/**
 * r_ld_csb_tx_init, slot 391 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast TX init.
 */
typedef void (*r_ld_csb_tx_init_fn_t)(void);

/**
 * r_ld_csb_tx_reset, slot 392 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast TX reset.
 */
typedef void (*r_ld_csb_tx_reset_fn_t)(void);

/**
 * r_ld_csb_tx_en_wrapper, slot 393 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast TX enable (libbtdm wrapper).
 */
typedef uint32_t (*r_ld_csb_tx_en_fn_t)(uint8_t *);

/**
 * r_ld_csb_tx_dis, slot 394 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast TX disable.
 */
typedef void (*r_ld_csb_tx_dis_fn_t)(void);

/**
 * r_ld_csb_tx_afh_update, slot 395 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast TX AFH update.
 */
typedef uint32_t (*r_ld_csb_tx_afh_update_fn_t)(uint16_t *);

/**
 * r_ld_csb_tx_set_data, slot 396 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast TX set data.
 */
typedef void (*r_ld_csb_tx_set_data_fn_t)(int32_t);

/**
 * r_ld_csb_tx_clr_data, slot 397 of the IP functions table.
 * Link Driver (baseband): connectionless slave broadcast TX clr data.
 */
typedef void (*r_ld_csb_tx_clr_data_fn_t)(void);

/**
 * r_ld_fm_init, slot 398 of the IP functions table.
 * Link Driver (baseband): fm init.
 */
typedef void (*r_ld_fm_init_fn_t)(void);

/**
 * r_ld_fm_reset, slot 399 of the IP functions table.
 * Link Driver (baseband): fm reset.
 */
typedef void (*r_ld_fm_reset_fn_t)(void);

/**
 * r_ld_fm_clk_isr, slot 400 of the IP functions table.
 * Link Driver (baseband): fm clock isr.
 */
typedef void (*r_ld_fm_clk_isr_fn_t)(void);

/**
 * r_ld_fm_rx_isr, slot 401 of the IP functions table.
 * Link Driver (baseband): fm RX isr.
 */
typedef void (*r_ld_fm_rx_isr_fn_t)(void);

/**
 * r_ld_fm_frame_isr, slot 402 of the IP functions table.
 * Link Driver (baseband): fm frame isr.
 */
typedef void (*r_ld_fm_frame_isr_fn_t)(void);

/**
 * r_ld_fm_sket_isr, slot 403 of the IP functions table.
 * Link Driver (baseband): fm sket isr.
 */
typedef void (*r_ld_fm_sket_isr_fn_t)(void);

/**
 * r_ld_fm_prog_enable, slot 404 of the IP functions table.
 * Link Driver (baseband): fm program enable.
 */
typedef void (*r_ld_fm_prog_enable_fn_t)(uint32_t, uint8_t);

/**
 * r_ld_fm_prog_disable, slot 405 of the IP functions table.
 * Link Driver (baseband): fm program disable.
 */
typedef void (*r_ld_fm_prog_disable_fn_t)(uint8_t);

/**
 * r_ld_fm_prog_push, slot 406 of the IP functions table.
 * Link Driver (baseband): fm program push.
 */
typedef void (*r_ld_fm_prog_push_fn_t)(uint32_t, uint8_t, uint8_t, uint8_t, uint32_t);

/**
 * r_ld_fm_prog_check, slot 407 of the IP functions table.
 * Link Driver (baseband): fm program check.
 */
typedef bool (*r_ld_fm_prog_check_fn_t)(void);

/**
 * r_ld_inq_init, slot 408 of the IP functions table.
 * Link Driver (baseband): inquiry init.
 */
typedef void (*r_ld_inq_init_fn_t)(void);

/**
 * r_ld_inq_reset, slot 409 of the IP functions table.
 * Link Driver (baseband): inquiry reset.
 */
typedef void (*r_ld_inq_reset_fn_t)(void);

/**
 * r_ld_inq_start, slot 410 of the IP functions table.
 * Link Driver (baseband): inquiry start.
 */
typedef uint32_t (*r_ld_inq_start_fn_t)(uint16_t *);

/**
 * r_ld_inq_stop, slot 411 of the IP functions table.
 * Link Driver (baseband): inquiry stop.
 */
typedef uint8_t (*r_ld_inq_stop_fn_t)(void);

/**
 * r_ld_iscan_init, slot 412 of the IP functions table.
 * Link Driver (baseband): inquiry scan init.
 */
typedef void (*r_ld_iscan_init_fn_t)(void);

/**
 * r_ld_iscan_reset, slot 413 of the IP functions table.
 * Link Driver (baseband): inquiry scan reset.
 */
typedef void (*r_ld_iscan_reset_fn_t)(void);

/**
 * r_ld_iscan_restart, slot 414 of the IP functions table.
 * Link Driver (baseband): inquiry scan restart.
 */
typedef void (*r_ld_iscan_restart_fn_t)(void);

/**
 * r_ld_iscan_start, slot 415 of the IP functions table.
 * Link Driver (baseband): inquiry scan start.
 */
typedef uint32_t (*r_ld_iscan_start_fn_t)(uint16_t *);

/**
 * r_ld_iscan_stop, slot 416 of the IP functions table.
 * Link Driver (baseband): inquiry scan stop.
 */
typedef uint32_t (*r_ld_iscan_stop_fn_t)(void);

/**
 * r_ld_iscan_eir_set, slot 417 of the IP functions table.
 * Link Driver (baseband): inquiry scan eir set.
 */
typedef uint32_t (*r_ld_iscan_eir_set_fn_t)(uint8_t, void *);

/**
 * r_ld_iscan_eir_get, slot 418 of the IP functions table.
 * Link Driver (baseband): inquiry scan eir get.
 */
typedef uint32_t (*r_ld_iscan_eir_get_fn_t)(uint8_t *, void *);

/**
 * r_ld_iscan_tx_pwr_get, slot 419 of the IP functions table.
 * Link Driver (baseband): inquiry scan TX power get.
 */
typedef uint32_t (*r_ld_iscan_tx_pwr_get_fn_t)(void);

/**
 * r_ld_wlcoex_set, slot 420 of the IP functions table.
 * Link Driver (baseband): wlcoex set.
 */
typedef void (*r_ld_wlcoex_set_fn_t)(uint8_t);

/**
 * r_ld_channel_assess, slot 421 of the IP functions table.
 * Link Driver (baseband): channel assess.
 */
typedef void (*r_ld_channel_assess_fn_t)(uint32_t, uint8_t, uint32_t);

/**
 * r_ld_init_hark, slot 422 of the IP functions table.
 * Link Driver (baseband): init (Espressif override).
 */
typedef void (*r_ld_init_hark_fn_t)(void);

/**
 * r_ld_reset_hark, slot 423 of the IP functions table.
 * Link Driver (baseband): reset (Espressif override).
 */
typedef void (*r_ld_reset_hark_fn_t)(void);

/**
 * r_ld_read_clock, slot 424 of the IP functions table.
 * Link Driver (baseband): read clock.
 */
typedef uint32_t (*r_ld_read_clock_fn_t)(void);

/**
 * r_ld_bd_addr_get, slot 425 of the IP functions table.
 * Link Driver (baseband): Bluetooth device address get.
 */
typedef void (*r_ld_bd_addr_get_fn_t)(void *);

/**
 * r_ld_class_of_dev_get, slot 426 of the IP functions table.
 * Link Driver (baseband): class of dev get.
 */
typedef void (*r_ld_class_of_dev_get_fn_t)(uint8_t *);

/**
 * r_ld_class_of_dev_set, slot 427 of the IP functions table.
 * Link Driver (baseband): class of dev set.
 */
typedef void (*r_ld_class_of_dev_set_fn_t)(uint8_t *);

/**
 * r_ld_version_get, slot 428 of the IP functions table.
 * Link Driver (baseband): version get.
 */
typedef void (*r_ld_version_get_fn_t)(uint8_t *);

/**
 * r_ld_timing_accuracy_get, slot 429 of the IP functions table.
 * Link Driver (baseband): timing accuracy get.
 */
typedef void (*r_ld_timing_accuracy_get_fn_t)(uint8_t *, uint8_t *);

/**
 * r_ld_active_check, slot 430 of the IP functions table.
 * Link Driver (baseband): active check.
 */
typedef uint32_t (*r_ld_active_check_fn_t)(void);

/**
 * r_ld_afh_ch_assess_data_get, slot 431 of the IP functions table.
 * Link Driver (baseband): AFH channel assess data get.
 */
typedef uint8_t *(*r_ld_afh_ch_assess_data_get_fn_t)(void);

/**
 * r_ld_page_init, slot 432 of the IP functions table.
 * Link Driver (baseband): page init.
 */
typedef void (*r_ld_page_init_fn_t)(void);

/**
 * r_ld_page_reset, slot 433 of the IP functions table.
 * Link Driver (baseband): page reset.
 */
typedef void (*r_ld_page_reset_fn_t)(void);

/**
 * r_ld_page_start_wrapper, slot 434 of the IP functions table.
 * Link Driver (baseband): page start (libbtdm wrapper).
 */
typedef uint32_t (*r_ld_page_start_fn_t)(void *);

/**
 * r_ld_page_stop, slot 435 of the IP functions table.
 * Link Driver (baseband): page stop.
 */
typedef uint8_t (*r_ld_page_stop_fn_t)(void);

/**
 * r_ld_pca_reporting_enable, slot 436 of the IP functions table.
 * Link Driver (baseband): pca reporting enable.
 */
typedef void (*r_ld_pca_reporting_enable_fn_t)(uint8_t);

/**
 * r_ld_pca_init, slot 437 of the IP functions table.
 * Link Driver (baseband): pca init.
 */
typedef void (*r_ld_pca_init_fn_t)(void);

/**
 * r_ld_pca_reset, slot 438 of the IP functions table.
 * Link Driver (baseband): pca reset.
 */
typedef void (*r_ld_pca_reset_fn_t)(void);

/**
 * r_ld_pca_local_config, slot 439 of the IP functions table.
 * Link Driver (baseband): pca local config.
 */
typedef void (*r_ld_pca_local_config_fn_t)(uint16_t, uint32_t, uint32_t, uint8_t);

/**
 * r_ld_pca_update_target_offset, slot 440 of the IP functions table.
 * Link Driver (baseband): pca update target offset.
 */
typedef void (*r_ld_pca_update_target_offset_fn_t)(uint8_t);

/**
 * r_ld_pca_mws_frame_sync, slot 441 of the IP functions table.
 * Link Driver (baseband): pca mws frame SCO/sync.
 */
typedef void (*r_ld_pca_mws_frame_sync_fn_t)(void);

/**
 * r_ld_pca_coarse_clock_adjust, slot 442 of the IP functions table.
 * Link Driver (baseband): pca coarse clock adjust.
 */
typedef uint32_t (*r_ld_pca_coarse_clock_adjust_fn_t)(int16_t, uint16_t, int32_t);

/**
 * r_ld_pca_mws_moment_offset_gt, slot 443 of the IP functions table.
 * Link Driver (baseband): pca mws moment offset gt.
 */
typedef void (*r_ld_pca_mws_moment_offset_gt_fn_t)(void);

/**
 * r_ld_pca_mws_moment_offset_lt, slot 444 of the IP functions table.
 * Link Driver (baseband): pca mws moment offset logical transport.
 */
typedef void (*r_ld_pca_mws_moment_offset_lt_fn_t)(void);

/**
 * r_ld_pca_initiate_clock_dragging, slot 445 of the IP functions table.
 * Link Driver (baseband): pca initiate clock dragging.
 */
typedef uint32_t (*r_ld_pca_initiate_clock_dragging_fn_t)(int16_t);

/**
 * r_ld_pscan_init, slot 446 of the IP functions table.
 * Link Driver (baseband): page scan init.
 */
typedef void (*r_ld_pscan_init_fn_t)(void);

/**
 * r_ld_pscan_reset, slot 447 of the IP functions table.
 * Link Driver (baseband): page scan reset.
 */
typedef void (*r_ld_pscan_reset_fn_t)(void);

/**
 * r_ld_pscan_restart, slot 448 of the IP functions table.
 * Link Driver (baseband): page scan restart.
 */
typedef void (*r_ld_pscan_restart_fn_t)(void);

/**
 * r_ld_pscan_start, slot 449 of the IP functions table.
 * Link Driver (baseband): page scan start.
 */
typedef uint32_t (*r_ld_pscan_start_fn_t)(uint16_t *);

/**
 * r_ld_pscan_stop, slot 450 of the IP functions table.
 * Link Driver (baseband): page scan stop.
 */
typedef uint32_t (*r_ld_pscan_stop_fn_t)(void);

/**
 * r_ld_pscan_evt_handler, slot 451 of the IP functions table.
 * Link Driver (baseband): page scan event handler.
 */
typedef void (*r_ld_pscan_evt_handler_fn_t)(void);

/**
 * r_ld_sched_compute, slot 452 of the IP functions table.
 * Link Driver (baseband): sched compute.
 */
typedef void (*r_ld_sched_compute_fn_t)(void);

/**
 * r_ld_sched_init, slot 453 of the IP functions table.
 * Link Driver (baseband): sched init.
 */
typedef void (*r_ld_sched_init_fn_t)(void);

/**
 * r_ld_sched_reset, slot 454 of the IP functions table.
 * Link Driver (baseband): sched reset.
 */
typedef void (*r_ld_sched_reset_fn_t)(void);

/**
 * r_ld_sched_iscan_add, slot 455 of the IP functions table.
 * Link Driver (baseband): sched inquiry scan add.
 */
typedef void (*r_ld_sched_iscan_add_fn_t)(void);

/**
 * r_ld_sched_iscan_remove, slot 456 of the IP functions table.
 * Link Driver (baseband): sched inquiry scan remove.
 */
typedef void (*r_ld_sched_iscan_remove_fn_t)(void);

/**
 * r_ld_sched_pscan_add, slot 457 of the IP functions table.
 * Link Driver (baseband): sched page scan add.
 */
typedef void (*r_ld_sched_pscan_add_fn_t)(void);

/**
 * r_ld_sched_pscan_remove, slot 458 of the IP functions table.
 * Link Driver (baseband): sched page scan remove.
 */
typedef void (*r_ld_sched_pscan_remove_fn_t)(void);

/**
 * r_ld_sched_sscan_add, slot 459 of the IP functions table.
 * Link Driver (baseband): sched sscan add.
 */
typedef void (*r_ld_sched_sscan_add_fn_t)(void);

/**
 * r_ld_sched_sscan_remove, slot 460 of the IP functions table.
 * Link Driver (baseband): sched sscan remove.
 */
typedef void (*r_ld_sched_sscan_remove_fn_t)(void);

/**
 * r_ld_sched_inq_add, slot 461 of the IP functions table.
 * Link Driver (baseband): sched inquiry add.
 */
typedef void (*r_ld_sched_inq_add_fn_t)(uint32_t);

/**
 * r_ld_sched_inq_remove, slot 462 of the IP functions table.
 * Link Driver (baseband): sched inquiry remove.
 */
typedef void (*r_ld_sched_inq_remove_fn_t)(void);

/**
 * r_ld_sched_page_add, slot 463 of the IP functions table.
 * Link Driver (baseband): sched page add.
 */
typedef void (*r_ld_sched_page_add_fn_t)(uint32_t);

/**
 * r_ld_sched_page_remove, slot 464 of the IP functions table.
 * Link Driver (baseband): sched page remove.
 */
typedef void (*r_ld_sched_page_remove_fn_t)(void);

/**
 * r_ld_sched_acl_add, slot 465 of the IP functions table.
 * Link Driver (baseband): sched ACL add.
 */
typedef void (*r_ld_sched_acl_add_fn_t)(uint32_t);

/**
 * r_ld_sched_acl_remove, slot 466 of the IP functions table.
 * Link Driver (baseband): sched ACL remove.
 */
typedef void (*r_ld_sched_acl_remove_fn_t)(uint32_t);

/**
 * r_ld_sched_sniff_add_wrapper, slot 467 of the IP functions table.
 * Link Driver (baseband): sched sniff mode add (libbtdm wrapper).
 */
typedef void (*r_ld_sched_sniff_add_fn_t)(uint32_t, uint8_t, uint8_t);

/**
 * r_ld_sched_sniff_remove, slot 468 of the IP functions table.
 * Link Driver (baseband): sched sniff mode remove.
 */
typedef void (*r_ld_sched_sniff_remove_fn_t)(uint32_t);

/**
 * r_ld_sched_sco_add, slot 469 of the IP functions table.
 * Link Driver (baseband): sched SCO add.
 */
typedef void (*r_ld_sched_sco_add_fn_t)(uint32_t, uint8_t, uint8_t, uint8_t);

/**
 * r_ld_sched_sco_remove, slot 470 of the IP functions table.
 * Link Driver (baseband): sched SCO remove.
 */
typedef void (*r_ld_sched_sco_remove_fn_t)(uint32_t);

/**
 * r_ld_sscan_init, slot 471 of the IP functions table.
 * Link Driver (baseband): sscan init.
 */
typedef void (*r_ld_sscan_init_fn_t)(void);

/**
 * r_ld_sscan_reset, slot 472 of the IP functions table.
 * Link Driver (baseband): sscan reset.
 */
typedef void (*r_ld_sscan_reset_fn_t)(void);

/**
 * r_ld_sscan_activated, slot 473 of the IP functions table.
 * Link Driver (baseband): sscan activated.
 */
typedef bool (*r_ld_sscan_activated_fn_t)(void);

/**
 * r_ld_sscan_start_hark, slot 474 of the IP functions table.
 * Link Driver (baseband): sscan start (Espressif override).
 */
typedef uint32_t (*r_ld_sscan_start_hark_fn_t)(uint16_t *, int32_t, uint16_t, uint16_t, uint32_t);

/**
 * r_ld_strain_init, slot 475 of the IP functions table.
 * Link Driver (baseband): strain init.
 */
typedef void (*r_ld_strain_init_fn_t)(void);

/**
 * r_ld_strain_reset, slot 476 of the IP functions table.
 * Link Driver (baseband): strain reset.
 */
typedef void (*r_ld_strain_reset_fn_t)(void);

/**
 * r_ld_strain_start, slot 477 of the IP functions table.
 * Link Driver (baseband): strain start.
 */
typedef uint32_t (*r_ld_strain_start_fn_t)(int32_t, uint16_t, uint8_t, uint32_t, uint16_t, uint16_t, uint8_t, uint8_t);

/**
 * r_ld_strain_stop, slot 478 of the IP functions table.
 * Link Driver (baseband): strain stop.
 */
typedef uint8_t (*r_ld_strain_stop_fn_t)(void);

/**
 * r_ld_util_fhs_unpk, slot 479 of the IP functions table.
 * Link Driver (baseband): util FHS unpk.
 */
typedef void (*r_ld_util_fhs_unpk_fn_t)(uint32_t, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint32_t *, uint8_t *);

/**
 * r_ld_util_bch_create, slot 480 of the IP functions table.
 * Link Driver (baseband): util bch create.
 */
typedef void (*r_ld_util_bch_create_fn_t)(uint8_t *, int32_t);

/**
 * r_ld_util_fhs_pk, slot 481 of the IP functions table.
 * Link Driver (baseband): util FHS pk.
 */
typedef void (*r_ld_util_fhs_pk_fn_t)(uint32_t, uint8_t *, uint32_t, uint32_t, uint8_t *, uint8_t *, uint8_t, uint8_t);

/**
 * r_ld_util_active_master_afh_map_set, slot 482 of the IP functions table.
 * Link Driver (baseband): util active master AFH map set.
 */
typedef void (*r_ld_util_active_master_afh_map_set_fn_t)(void *);

/**
 * r_ld_util_active_master_afh_map_get, slot 483 of the IP functions table.
 * Link Driver (baseband): util active master AFH map get.
 */
typedef uint8_t *(*r_ld_util_active_master_afh_map_get_fn_t)(void);

/**
 * r_ld_util_stp_unpk, slot 484 of the IP functions table.
 * Link Driver (baseband): util stp unpk.
 */
typedef void (*r_ld_util_stp_unpk_fn_t)(uint32_t, uint32_t *, uint32_t *, void *, void *, uint16_t *, uint8_t *, uint8_t *);

/**
 * r_ld_util_stp_pk, slot 485 of the IP functions table.
 * Link Driver (baseband): util stp pk.
 */
typedef void (*r_ld_util_stp_pk_fn_t)(uint32_t, uint32_t, uint32_t, void *, void *, uint32_t, uint8_t, uint8_t);

/**
 * r_llc_ch_assess_local, slot 486 of the IP functions table.
 * Logical Link Controller (LE): channel assess local.
 */
typedef void (*r_llc_ch_assess_local_fn_t)(uint32_t, uint32_t, uint8_t, uint32_t);

/**
 * r_llc_ch_assess_get_local_ch_map, slot 487 of the IP functions table.
 * Logical Link Controller (LE): channel assess get local channel map.
 */
typedef void (*r_llc_ch_assess_get_local_ch_map_fn_t)(uint32_t, void *);

/**
 * r_llc_ch_assess_get_current_ch_map, slot 488 of the IP functions table.
 * Logical Link Controller (LE): channel assess get current channel map.
 */
typedef int32_t (*r_llc_ch_assess_get_current_ch_map_fn_t)(uint32_t);

/**
 * r_llc_ch_assess_merge_ch, slot 489 of the IP functions table.
 * Logical Link Controller (LE): channel assess merge channel.
 */
typedef uint8_t (*r_llc_ch_assess_merge_ch_fn_t)(int32_t, int32_t, int32_t);

/**
 * r_llc_ch_assess_reass_ch, slot 490 of the IP functions table.
 * Logical Link Controller (LE): channel assess reass channel.
 */
typedef void (*r_llc_ch_assess_reass_ch_fn_t)(void *, uint32_t);

/**
 * r_llc_llcp_version_ind_pdu_send, slot 491 of the IP functions table.
 * Logical Link Controller (LE): LLCP version indication PDU send.
 */
typedef void (*r_llc_llcp_version_ind_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_ch_map_update_pdu_send, slot 492 of the IP functions table.
 * Logical Link Controller (LE): LLCP channel map update PDU send.
 */
typedef void (*r_llc_llcp_ch_map_update_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_pause_enc_req_pdu_send, slot 493 of the IP functions table.
 * Logical Link Controller (LE): LLCP pause encryption request PDU send.
 */
typedef void (*r_llc_llcp_pause_enc_req_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_pause_enc_rsp_pdu_send, slot 494 of the IP functions table.
 * Logical Link Controller (LE): LLCP pause encryption response PDU send.
 */
typedef void (*r_llc_llcp_pause_enc_rsp_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_enc_req_pdu_send, slot 495 of the IP functions table.
 * Logical Link Controller (LE): LLCP encryption request PDU send.
 */
typedef void (*r_llc_llcp_enc_req_pdu_send_fn_t)(uint32_t, int32_t);

/**
 * r_llc_llcp_enc_rsp_pdu_send, slot 496 of the IP functions table.
 * Logical Link Controller (LE): LLCP encryption response PDU send.
 */
typedef void (*r_llc_llcp_enc_rsp_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_start_enc_rsp_pdu_send, slot 497 of the IP functions table.
 * Logical Link Controller (LE): LLCP start encryption response PDU send.
 */
typedef void (*r_llc_llcp_start_enc_rsp_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_reject_ind_pdu_send, slot 498 of the IP functions table.
 * Logical Link Controller (LE): LLCP reject indication PDU send.
 */
typedef void (*r_llc_llcp_reject_ind_pdu_send_fn_t)(uint32_t, uint8_t, uint8_t);

/**
 * r_llc_llcp_con_update_pdu_send, slot 499 of the IP functions table.
 * Logical Link Controller (LE): LLCP connection update PDU send.
 */
typedef void (*r_llc_llcp_con_update_pdu_send_fn_t)(uint32_t, int32_t);

/**
 * r_llc_llcp_con_param_req_pdu_send, slot 500 of the IP functions table.
 * Logical Link Controller (LE): LLCP connection param request PDU send.
 */
typedef void (*r_llc_llcp_con_param_req_pdu_send_fn_t)(uint32_t, int32_t);

/**
 * r_llc_llcp_con_param_rsp_pdu_send, slot 501 of the IP functions table.
 * Logical Link Controller (LE): LLCP connection param response PDU send.
 */
typedef void (*r_llc_llcp_con_param_rsp_pdu_send_fn_t)(uint32_t, int32_t);

/**
 * r_llc_llcp_feats_req_pdu_send, slot 502 of the IP functions table.
 * Logical Link Controller (LE): LLCP feats request PDU send.
 */
typedef void (*r_llc_llcp_feats_req_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_feats_rsp_pdu_send, slot 503 of the IP functions table.
 * Logical Link Controller (LE): LLCP feats response PDU send.
 */
typedef void (*r_llc_llcp_feats_rsp_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_start_enc_req_pdu_send, slot 504 of the IP functions table.
 * Logical Link Controller (LE): LLCP start encryption request PDU send.
 */
typedef void (*r_llc_llcp_start_enc_req_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_terminate_ind_pdu_send, slot 505 of the IP functions table.
 * Logical Link Controller (LE): LLCP terminate indication PDU send.
 */
typedef void (*r_llc_llcp_terminate_ind_pdu_send_fn_t)(uint32_t, uint8_t);

/**
 * r_llc_llcp_unknown_rsp_send_pdu, slot 506 of the IP functions table.
 * Logical Link Controller (LE): LLCP unknown response send PDU.
 */
typedef void (*r_llc_llcp_unknown_rsp_send_pdu_fn_t)(uint32_t, uint8_t);

/**
 * r_llc_llcp_ping_req_pdu_send, slot 507 of the IP functions table.
 * Logical Link Controller (LE): LLCP ping request PDU send.
 */
typedef void (*r_llc_llcp_ping_req_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_ping_rsp_pdu_send, slot 508 of the IP functions table.
 * Logical Link Controller (LE): LLCP ping response PDU send.
 */
typedef void (*r_llc_llcp_ping_rsp_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_length_req_pdu_send, slot 509 of the IP functions table.
 * Logical Link Controller (LE): LLCP length request PDU send.
 */
typedef void (*r_llc_llcp_length_req_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_length_rsp_pdu_send, slot 510 of the IP functions table.
 * Logical Link Controller (LE): LLCP length response PDU send.
 */
typedef void (*r_llc_llcp_length_rsp_pdu_send_fn_t)(uint32_t);

/**
 * r_llc_llcp_tester_send, slot 511 of the IP functions table.
 * Logical Link Controller (LE): LLCP tester send.
 */
typedef void (*r_llc_llcp_tester_send_fn_t)(uint32_t, uint32_t, void *);

/**
 * r_llc_llcp_recv_handler_hack, slot 512 of the IP functions table.
 * Logical Link Controller (LE): LLCP recv handler (Espressif override).
 */
typedef uint32_t (*r_llc_llcp_recv_handler_hack_fn_t)(uint32_t, uint8_t, uint8_t *, uint8_t);

/**
 * r_llc_llcp_get_autorize, slot 513 of the IP functions table.
 * Logical Link Controller (LE): LLCP get autorize.
 */
typedef uint8_t (*r_llc_llcp_get_autorize_fn_t)(uint32_t);

/**
 * r_llc_init_hack, slot 514 of the IP functions table.
 * Logical Link Controller (LE): init (Espressif override).
 */
typedef void (*r_llc_init_hack_fn_t)(void);

/**
 * r_llc_reset_hack, slot 515 of the IP functions table.
 * Logical Link Controller (LE): reset (Espressif override).
 */
typedef void (*r_llc_reset_hack_fn_t)(void);

/**
 * r_llc_stop_hack, slot 516 of the IP functions table.
 * Logical Link Controller (LE): stop (Espressif override).
 */
typedef void (*r_llc_stop_hack_fn_t)(uint32_t);

/**
 * r_llc_start, slot 517 of the IP functions table.
 * Logical Link Controller (LE): start.
 */
typedef void (*r_llc_start_fn_t)(uint16_t *, int32_t);

/**
 * r_llc_le_con_cmp_evt_send, slot 518 of the IP functions table.
 * Logical Link Controller (LE): LE connection cmp event send.
 */
typedef void (*r_llc_le_con_cmp_evt_send_fn_t)(uint8_t, uint16_t, uint16_t *);

/**
 * r_llc_discon_event_complete_send, slot 519 of the IP functions table.
 * Logical Link Controller (LE): discon event complete send.
 */
typedef void (*r_llc_discon_event_complete_send_fn_t)(uint32_t, uint8_t, uint16_t, uint8_t);

/**
 * r_llc_con_update_complete_send, slot 520 of the IP functions table.
 * Logical Link Controller (LE): connection update complete send.
 */
typedef void (*r_llc_con_update_complete_send_fn_t)(uint8_t, uint16_t, int32_t);

/**
 * r_llc_ltk_req_send, slot 521 of the IP functions table.
 * Logical Link Controller (LE): ltk request send.
 */
typedef void (*r_llc_ltk_req_send_fn_t)(uint16_t, int32_t);

/**
 * r_llc_feats_rd_event_send, slot 522 of the IP functions table.
 * Logical Link Controller (LE): feats rd event send.
 */
typedef void (*r_llc_feats_rd_event_send_fn_t)(uint8_t, uint16_t, void *);

/**
 * r_llc_version_rd_event_send, slot 523 of the IP functions table.
 * Logical Link Controller (LE): version rd event send.
 */
typedef void (*r_llc_version_rd_event_send_fn_t)(uint8_t, uint16_t);

/**
 * r_llc_common_cmd_complete_send, slot 524 of the IP functions table.
 * Logical Link Controller (LE): common command complete send.
 */
typedef void (*r_llc_common_cmd_complete_send_fn_t)(uint16_t, uint8_t, uint16_t);

/**
 * r_llc_common_cmd_status_send_hack, slot 525 of the IP functions table.
 * Logical Link Controller (LE): common command status send (Espressif override).
 */
typedef void (*r_llc_common_cmd_status_send_hack_fn_t)(uint16_t, uint8_t, uint16_t);

/**
 * r_llc_common_flush_occurred_send, slot 526 of the IP functions table.
 * Logical Link Controller (LE): common flush occurred send.
 */
typedef void (*r_llc_common_flush_occurred_send_fn_t)(uint16_t);

/**
 * r_llc_common_enc_key_ref_comp_evt_send, slot 527 of the IP functions table.
 * Logical Link Controller (LE): common encryption key ref comp event send.
 */
typedef void (*r_llc_common_enc_key_ref_comp_evt_send_fn_t)(uint16_t, uint8_t);

/**
 * r_llc_common_enc_change_evt_send, slot 528 of the IP functions table.
 * Logical Link Controller (LE): common encryption change event send.
 */
typedef void (*r_llc_common_enc_change_evt_send_fn_t)(uint32_t, uint8_t, uint8_t);

/**
 * r_llc_common_nb_of_pkt_comp_evt_send, slot 529 of the IP functions table.
 * Logical Link Controller (LE): common number of of pkt comp event send.
 */
typedef void (*r_llc_common_nb_of_pkt_comp_evt_send_fn_t)(uint16_t, uint16_t);

/**
 * r_llc_con_update_ind, slot 530 of the IP functions table.
 * Logical Link Controller (LE): connection update indication.
 */
typedef void (*r_llc_con_update_ind_fn_t)(uint32_t, int32_t);

/**
 * r_llc_lsto_con_update, slot 531 of the IP functions table.
 * Logical Link Controller (LE): link supervision timeout connection update.
 */
typedef void (*r_llc_lsto_con_update_fn_t)(uint32_t);

/**
 * r_llc_map_update_ind, slot 532 of the IP functions table.
 * Logical Link Controller (LE): map update indication.
 */
typedef void (*r_llc_map_update_ind_fn_t)(uint32_t);

/**
 * r_llc_con_update_finished, slot 533 of the IP functions table.
 * Logical Link Controller (LE): connection update finished.
 */
typedef void (*r_llc_con_update_finished_fn_t)(uint32_t);

/**
 * r_llc_map_update_finished, slot 534 of the IP functions table.
 * Logical Link Controller (LE): map update finished.
 */
typedef void (*r_llc_map_update_finished_fn_t)(uint32_t);

/**
 * r_llc_util_get_free_conhdl, slot 535 of the IP functions table.
 * Logical Link Controller (LE): util get free conhdl.
 */
typedef uint32_t (*r_llc_util_get_free_conhdl_fn_t)(uint16_t *);

/**
 * r_llc_util_get_nb_active_link, slot 536 of the IP functions table.
 * Logical Link Controller (LE): util get number of active link.
 */
typedef uint8_t (*r_llc_util_get_nb_active_link_fn_t)(void);

/**
 * r_llc_util_dicon_procedure, slot 537 of the IP functions table.
 * Logical Link Controller (LE): util dicon procedure.
 */
typedef void (*r_llc_util_dicon_procedure_fn_t)(uint32_t, uint8_t);

/**
 * r_llc_util_clear_operation_ptr, slot 538 of the IP functions table.
 * Logical Link Controller (LE): util clear operation ptr.
 */
typedef void (*r_llc_util_clear_operation_ptr_fn_t)(uint32_t, uint32_t);

/**
 * r_llc_util_update_channel_map, slot 539 of the IP functions table.
 * Logical Link Controller (LE): util update channel map.
 */
typedef void (*r_llc_util_update_channel_map_fn_t)(uint32_t, void *);

/**
 * r_llc_util_set_llcp_discard_enable, slot 540 of the IP functions table.
 * Logical Link Controller (LE): util set LLCP discard enable.
 */
typedef void (*r_llc_util_set_llcp_discard_enable_fn_t)(uint32_t, uint8_t);

/**
 * r_llc_util_set_auth_payl_to_margin, slot 541 of the IP functions table.
 * Logical Link Controller (LE): util set authentication payl timeout margin.
 */
typedef void (*r_llc_util_set_auth_payl_to_margin_fn_t)(int32_t);

/**
 * r_llc_util_bw_mgt, slot 542 of the IP functions table.
 * Logical Link Controller (LE): util bw mgt.
 */
typedef void (*r_llc_util_bw_mgt_fn_t)(uint32_t);

/**
 * r_llc_end_evt_defer, slot 543 of the IP functions table.
 * Logical Link Controller (LE): end event defer.
 */
typedef void (*r_llc_end_evt_defer_fn_t)(uint32_t);

/**
 * r_llc_pdu_llcp_tx_ack_defer, slot 544 of the IP functions table.
 * Logical Link Controller (LE): PDU LLCP TX ack defer.
 */
typedef void (*r_llc_pdu_llcp_tx_ack_defer_fn_t)(uint32_t, uint8_t);

/**
 * r_llc_pdu_acl_tx_ack_defer, slot 545 of the IP functions table.
 * Logical Link Controller (LE): PDU ACL TX ack defer.
 */
typedef void (*r_llc_pdu_acl_tx_ack_defer_fn_t)(uint32_t, uint8_t);

/**
 * r_llc_pdu_defer, slot 546 of the IP functions table.
 * Logical Link Controller (LE): PDU defer.
 */
typedef void (*r_llc_pdu_defer_fn_t)(uint32_t, uint16_t, uint8_t, uint8_t, uint8_t);

/**
 * r_lld_evt_elt_delete, slot 547 of the IP functions table.
 * Lower Link Driver (LE baseband): event elt delete.
 */
typedef int32_t (*r_lld_evt_elt_delete_fn_t)(int32_t, uint8_t, uint8_t);

/**
 * r_lld_evt_drift_compute, slot 548 of the IP functions table.
 * Lower Link Driver (LE baseband): event drift compute.
 */
typedef uint32_t (*r_lld_evt_drift_compute_fn_t)(uint32_t, uint32_t);

/**
 * r_lld_evt_delete_elt_push, slot 549 of the IP functions table.
 * Lower Link Driver (LE baseband): event delete elt push.
 */
typedef void (*r_lld_evt_delete_elt_push_fn_t)(int32_t, uint8_t, uint8_t);

/**
 * r_lld_evt_channel_next, slot 550 of the IP functions table.
 * Lower Link Driver (LE baseband): event channel next.
 */
typedef void (*r_lld_evt_channel_next_fn_t)(uint32_t, int16_t);

/**
 * r_lld_evt_deffered_elt_handler, slot 551 of the IP functions table.
 * Lower Link Driver (LE baseband): event deffered elt handler.
 */
typedef void (*r_lld_evt_deffered_elt_handler_fn_t)(void);

/**
 * r_lld_evt_delete_elt_handler, slot 552 of the IP functions table.
 * Lower Link Driver (LE baseband): event delete elt handler.
 */
typedef void (*r_lld_evt_delete_elt_handler_fn_t)(void);

/**
 * r_lld_evt_init_wrapper, slot 553 of the IP functions table.
 * Lower Link Driver (LE baseband): event init (libbtdm wrapper).
 */
typedef void (*r_lld_evt_init_fn_t)(uint8_t);

/**
 * r_lld_evt_init_evt, slot 554 of the IP functions table.
 * Lower Link Driver (LE baseband): event init event.
 */
typedef void (*r_lld_evt_init_evt_fn_t)(int32_t);

/**
 * r_lld_evt_restart, slot 555 of the IP functions table.
 * Lower Link Driver (LE baseband): event restart.
 */
typedef uint8_t (*r_lld_evt_restart_fn_t)(int32_t, uint32_t);

/**
 * r_lld_evt_elt_insert, slot 556 of the IP functions table.
 * Lower Link Driver (LE baseband): event elt insert.
 */
typedef void (*r_lld_evt_elt_insert_fn_t)(int32_t, uint8_t);

/**
 * r_lld_evt_schedule_next_instant_hack, slot 557 of the IP functions table.
 * Lower Link Driver (LE baseband): event schedule next instant (Espressif override).
 */
typedef void (*r_lld_evt_schedule_next_instant_hack_fn_t)(int32_t);

/**
 * r_lld_evt_schedule_next, slot 558 of the IP functions table.
 * Lower Link Driver (LE baseband): event schedule next.
 */
typedef void (*r_lld_evt_schedule_next_fn_t)(int32_t);

/**
 * r_lld_evt_schedule, slot 559 of the IP functions table.
 * Lower Link Driver (LE baseband): event schedule.
 */
typedef void (*r_lld_evt_schedule_fn_t)(int32_t);

/**
 * r_lld_evt_prevent_stop, slot 560 of the IP functions table.
 * Lower Link Driver (LE baseband): event prevent stop.
 */
typedef void (*r_lld_evt_prevent_stop_fn_t)(void);

/**
 * r_lld_evt_canceled, slot 561 of the IP functions table.
 * Lower Link Driver (LE baseband): event canceled.
 */
typedef void (*r_lld_evt_canceled_fn_t)(int32_t);

/**
 * r_lld_evt_scan_create, slot 562 of the IP functions table.
 * Lower Link Driver (LE baseband): event scan create.
 */
typedef void (*r_lld_evt_scan_create_fn_t)(int16_t, int16_t);

/**
 * r_lld_evt_move_to_master, slot 563 of the IP functions table.
 * Lower Link Driver (LE baseband): event move timeout master.
 */
typedef void (*r_lld_evt_move_to_master_fn_t)(int32_t, uint16_t, uint32_t, uint32_t);

/**
 * r_lld_evt_update_create, slot 564 of the IP functions table.
 * Lower Link Driver (LE baseband): event update create.
 */
typedef void *(*r_lld_evt_update_create_fn_t)(void *, uint16_t, uint16_t, uint16_t, int16_t, uint16_t *);

/**
 * r_lld_evt_move_to_slave, slot 565 of the IP functions table.
 * Lower Link Driver (LE baseband): event move timeout slave.
 */
typedef int32_t (*r_lld_evt_move_to_slave_fn_t)(uint16_t *, int32_t, int32_t, uint32_t);

/**
 * r_lld_evt_slave_update, slot 566 of the IP functions table.
 * Lower Link Driver (LE baseband): event slave update.
 */
typedef void (*r_lld_evt_slave_update_fn_t)(int32_t, void *);

/**
 * r_lld_evt_adv_create, slot 567 of the IP functions table.
 * Lower Link Driver (LE baseband): event advertising create.
 */
typedef void (*r_lld_evt_adv_create_fn_t)(int16_t, uint32_t, uint16_t, uint8_t);

/**
 * r_lld_evt_end, slot 568 of the IP functions table.
 * Lower Link Driver (LE baseband): event end.
 */
typedef void (*r_lld_evt_end_fn_t)(int32_t);

/**
 * r_lld_evt_rx, slot 569 of the IP functions table.
 * Lower Link Driver (LE baseband): event RX.
 */
typedef void (*r_lld_evt_rx_fn_t)(int32_t);

/**
 * r_lld_evt_end_isr, slot 570 of the IP functions table.
 * Lower Link Driver (LE baseband): event end isr.
 */
typedef void (*r_lld_evt_end_isr_fn_t)(uint8_t);

/**
 * r_lld_evt_rx_isr, slot 571 of the IP functions table.
 * Lower Link Driver (LE baseband): event RX isr.
 */
typedef void (*r_lld_evt_rx_isr_fn_t)(void);

/**
 * r_lld_init, slot 572 of the IP functions table.
 * Lower Link Driver (LE baseband): init.
 */
typedef void (*r_lld_init_fn_t)(uint8_t);

/**
 * r_lld_core_reset, slot 573 of the IP functions table.
 * Lower Link Driver (LE baseband): core reset.
 */
typedef void (*r_lld_core_reset_fn_t)(void);

/**
 * r_lld_adv_start, slot 574 of the IP functions table.
 * Lower Link Driver (LE baseband): advertising start.
 */
typedef int32_t (*r_lld_adv_start_fn_t)(int32_t, uint32_t, int32_t, uint8_t);

/**
 * r_lld_adv_stop_hack, slot 575 of the IP functions table.
 * Lower Link Driver (LE baseband): advertising stop (Espressif override).
 */
typedef void (*r_lld_adv_stop_hack_fn_t)(int32_t);

/**
 * r_lld_scan_start_hack, slot 576 of the IP functions table.
 * Lower Link Driver (LE baseband): scan start (Espressif override).
 */
typedef int32_t (*r_lld_scan_start_hack_fn_t)(int32_t, int32_t);

/**
 * r_lld_scan_stop_hack, slot 577 of the IP functions table.
 * Lower Link Driver (LE baseband): scan stop (Espressif override).
 */
typedef void (*r_lld_scan_stop_hack_fn_t)(int32_t);

/**
 * r_lld_con_start, slot 578 of the IP functions table.
 * Lower Link Driver (LE baseband): connection start.
 */
typedef uint32_t (*r_lld_con_start_fn_t)(uint16_t *, int32_t, uint32_t);

/**
 * r_lld_move_to_master_hack, slot 579 of the IP functions table.
 * Lower Link Driver (LE baseband): move timeout master (Espressif override).
 */
typedef void (*r_lld_move_to_master_hack_fn_t)(uint32_t, uint32_t, uint32_t, uint8_t);

/**
 * r_lld_con_update_req, slot 580 of the IP functions table.
 * Lower Link Driver (LE baseband): connection update request.
 */
typedef void (*r_lld_con_update_req_fn_t)(uint32_t, int32_t, int32_t);

/**
 * r_lld_con_update_after_param_req, slot 581 of the IP functions table.
 * Lower Link Driver (LE baseband): connection update after param request.
 */
typedef void (*r_lld_con_update_after_param_req_fn_t)(uint32_t, void *, int32_t, int32_t, uint8_t);

/**
 * r_lld_con_param_rsp, slot 582 of the IP functions table.
 * Lower Link Driver (LE baseband): connection param response.
 */
typedef uint32_t (*r_lld_con_param_rsp_fn_t)(int16_t, int32_t, int32_t);

/**
 * r_lld_con_param_req, slot 583 of the IP functions table.
 * Lower Link Driver (LE baseband): connection param request.
 */
typedef void (*r_lld_con_param_req_fn_t)(int16_t, int32_t, int32_t);

/**
 * r_lld_con_stop, slot 584 of the IP functions table.
 * Lower Link Driver (LE baseband): connection stop.
 */
typedef void (*r_lld_con_stop_fn_t)(uint32_t);

/**
 * r_lld_get_mode, slot 585 of the IP functions table.
 * Lower Link Driver (LE baseband): get mode.
 */
typedef uint8_t (*r_lld_get_mode_fn_t)(uint32_t);

/**
 * r_lld_move_to_slave_hack, slot 586 of the IP functions table.
 * Lower Link Driver (LE baseband): move timeout slave (Espressif override).
 */
typedef uint32_t (*r_lld_move_to_slave_hack_fn_t)(uint32_t, int32_t, uint32_t, uint16_t);

/**
 * r_lld_ch_map_ind, slot 587 of the IP functions table.
 * Lower Link Driver (LE baseband): channel map indication.
 */
typedef void (*r_lld_ch_map_ind_fn_t)(int32_t, uint16_t);

/**
 * r_lld_con_update_ind, slot 588 of the IP functions table.
 * Lower Link Driver (LE baseband): connection update indication.
 */
typedef void (*r_lld_con_update_ind_fn_t)(uint32_t, uint32_t);

/**
 * r_lld_crypt_isr, slot 589 of the IP functions table.
 * Lower Link Driver (LE baseband): crypt isr.
 */
typedef void (*r_lld_crypt_isr_fn_t)(void);

/**
 * r_lld_test_mode_tx, slot 590 of the IP functions table.
 * Lower Link Driver (LE baseband): test mode TX.
 */
typedef void (*r_lld_test_mode_tx_fn_t)(uint32_t, uint8_t);

/**
 * r_lld_test_mode_rx, slot 591 of the IP functions table.
 * Lower Link Driver (LE baseband): test mode RX.
 */
typedef void (*r_lld_test_mode_rx_fn_t)(uint8_t);

/**
 * r_lld_test_stop, slot 592 of the IP functions table.
 * Lower Link Driver (LE baseband): test stop.
 */
typedef void (*r_lld_test_stop_fn_t)(void);

/**
 * r_lld_ral_renew_req, slot 593 of the IP functions table.
 * Lower Link Driver (LE baseband): ral renew request.
 */
typedef void (*r_lld_ral_renew_req_fn_t)(void);

/**
 * r_lld_pdu_check, slot 594 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU check.
 */
typedef uint32_t (*r_lld_pdu_check_fn_t)(int32_t);

/**
 * r_lld_pdu_tx_loop, slot 595 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU TX loop.
 */
typedef void (*r_lld_pdu_tx_loop_fn_t)(int32_t);

/**
 * r_lld_pdu_data_tx_push, slot 596 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU data TX push.
 */
typedef void (*r_lld_pdu_data_tx_push_fn_t)(int32_t, int32_t, uint8_t);

/**
 * r_lld_pdu_data_send, slot 597 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU data send.
 */
typedef bool (*r_lld_pdu_data_send_fn_t)(uint16_t *);

/**
 * r_lld_pdu_tx_push, slot 598 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU TX push.
 */
typedef void (*r_lld_pdu_tx_push_fn_t)(int32_t, int32_t);

/**
 * r_lld_pdu_tx_prog, slot 599 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU TX program.
 */
typedef void (*r_lld_pdu_tx_prog_fn_t)(int32_t);

/**
 * r_lld_pdu_send_packet, slot 600 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU send packet.
 */
typedef uint32_t (*r_lld_pdu_send_packet_fn_t)(int32_t, int32_t, uint8_t *);

/**
 * ram_lld_pdu_tx_flush, slot 601 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU TX flush (Espressif override).
 */
typedef void (*r_ram_lld_pdu_tx_flush_fn_t)(int32_t);

/**
 * r_lld_pdu_adv_pack, slot 602 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU advertising pack.
 */
typedef uint32_t (*r_lld_pdu_adv_pack_fn_t)(uint32_t, uint32_t *, uint8_t *);

/**
 * r_lld_pdu_rx_handler, slot 603 of the IP functions table.
 * Lower Link Driver (LE baseband): PDU RX handler.
 */
typedef void (*r_lld_pdu_rx_handler_fn_t)(int32_t, uint8_t);

/**
 * r_lld_util_instant_get, slot 604 of the IP functions table.
 * Lower Link Driver (LE baseband): util instant get.
 */
typedef void (*r_lld_util_instant_get_fn_t)(int32_t, uint8_t);

/**
 * r_lld_util_get_bd_address, slot 605 of the IP functions table.
 * Lower Link Driver (LE baseband): util get Bluetooth device address.
 */
typedef void (*r_lld_util_get_bd_address_fn_t)(uint8_t *);

/**
 * r_lld_util_set_bd_address, slot 606 of the IP functions table.
 * Lower Link Driver (LE baseband): util set Bluetooth device address.
 */
typedef void (*r_lld_util_set_bd_address_fn_t)(uint32_t *, uint32_t);

/**
 * r_lld_util_ral_force_rpa_renew, slot 607 of the IP functions table.
 * Lower Link Driver (LE baseband): util ral force rpa renew.
 */
typedef void (*r_lld_util_ral_force_rpa_renew_fn_t)(void);

/**
 * r_lld_util_freq2chnl, slot 608 of the IP functions table.
 * Lower Link Driver (LE baseband): util freq2chnl.
 */
typedef uint8_t (*r_lld_util_freq2chnl_fn_t)(uint8_t);

/**
 * r_lld_util_get_local_offset, slot 609 of the IP functions table.
 * Lower Link Driver (LE baseband): util get local offset.
 */
typedef uint32_t (*r_lld_util_get_local_offset_fn_t)(uint32_t, uint32_t, uint32_t);

/**
 * r_lld_util_get_peer_offset, slot 610 of the IP functions table.
 * Lower Link Driver (LE baseband): util get peer offset.
 */
typedef uint32_t (*r_lld_util_get_peer_offset_fn_t)(uint32_t, uint32_t, uint32_t);

/**
 * r_lld_util_connection_param_set, slot 611 of the IP functions table.
 * Lower Link Driver (LE baseband): util connection param set.
 */
typedef void (*r_lld_util_connection_param_set_fn_t)(int32_t, uint16_t *);

/**
 * r_lld_util_dle_set_cs_fields, slot 612 of the IP functions table.
 * Lower Link Driver (LE baseband): util dle set cs fields.
 */
typedef void (*r_lld_util_dle_set_cs_fields_fn_t)(uint32_t);

/**
 * r_lld_util_anchor_point_move, slot 613 of the IP functions table.
 * Lower Link Driver (LE baseband): util anchor point move.
 */
typedef void (*r_lld_util_anchor_point_move_fn_t)(int32_t);

/**
 * r_lld_util_flush_list, slot 614 of the IP functions table.
 * Lower Link Driver (LE baseband): util flush list.
 */
typedef void (*r_lld_util_flush_list_fn_t)(uint32_t);

/**
 * r_lld_util_instant_ongoing, slot 615 of the IP functions table.
 * Lower Link Driver (LE baseband): util instant ongoing.
 */
typedef bool (*r_lld_util_instant_ongoing_fn_t)(int32_t);

/**
 * r_lld_util_compute_ce_max, slot 616 of the IP functions table.
 * Lower Link Driver (LE baseband): util compute ce max.
 */
typedef void (*r_lld_util_compute_ce_max_fn_t)(int32_t, uint32_t);

/**
 * r_lld_util_elt_programmed, slot 617 of the IP functions table.
 * Lower Link Driver (LE baseband): util elt programmed.
 */
typedef bool (*r_lld_util_elt_programmed_fn_t)(int32_t);

/**
 * r_lld_util_priority_set, slot 618 of the IP functions table.
 * Lower Link Driver (LE baseband): util priority set.
 */
typedef void (*r_lld_util_priority_set_fn_t)(int32_t, uint8_t);

/**
 * r_lld_util_priority_update, slot 619 of the IP functions table.
 * Lower Link Driver (LE baseband): util priority update.
 */
typedef void (*r_lld_util_priority_update_fn_t)(int32_t, uint8_t);

/**
 * r_lld_util_get_tx_pkt_cnt, slot 620 of the IP functions table.
 * Lower Link Driver (LE baseband): util get TX pkt cnt.
 */
typedef uint8_t (*r_lld_util_get_tx_pkt_cnt_fn_t)(int32_t);

/**
 * r_lld_util_eff_tx_time_set_hack, slot 621 of the IP functions table.
 * Lower Link Driver (LE baseband): util eff TX time set (Espressif override).
 */
typedef void (*r_lld_util_eff_tx_time_set_hack_fn_t)(int32_t, uint16_t, uint16_t);

/**
 * r_lld_wlcoex_set, slot 622 of the IP functions table.
 * Lower Link Driver (LE baseband): wlcoex set.
 */
typedef void (*r_lld_wlcoex_set_fn_t)(uint8_t);

/**
 * r_llm_encryption_done, slot 623 of the IP functions table.
 * Lower Link Manager (LE): encryption done.
 */
typedef void (*r_llm_encryption_done_fn_t)(void);

/**
 * r_llm_init_hack, slot 624 of the IP functions table.
 * Lower Link Manager (LE): init (Espressif override).
 */
typedef void (*r_llm_init_hack_fn_t)(uint8_t);

/**
 * r_llm_wl_clr, slot 625 of the IP functions table.
 * Lower Link Manager (LE): whitelist clr.
 */
typedef void (*r_llm_wl_clr_fn_t)(void);

/**
 * r_llm_ral_clear, slot 626 of the IP functions table.
 * Lower Link Manager (LE): ral clear.
 */
typedef void (*r_llm_ral_clear_fn_t)(void);

/**
 * r_llm_ble_ready, slot 627 of the IP functions table.
 * Lower Link Manager (LE): ble ready.
 */
typedef void (*r_llm_ble_ready_fn_t)(void);

/**
 * r_llm_common_cmd_complete_send, slot 628 of the IP functions table.
 * Lower Link Manager (LE): common command complete send.
 */
typedef void (*r_llm_common_cmd_complete_send_fn_t)(uint16_t, uint8_t);

/**
 * r_llm_con_req_ind, slot 629 of the IP functions table.
 * Lower Link Manager (LE): connection request indication.
 */
typedef void (*r_llm_con_req_ind_fn_t)(uint32_t);

/**
 * r_llm_le_adv_report_ind, slot 630 of the IP functions table.
 * Lower Link Manager (LE): LE advertising report indication.
 */
typedef void (*r_llm_le_adv_report_ind_fn_t)(uint32_t);

/**
 * r_llm_con_req_tx_cfm, slot 631 of the IP functions table.
 * Lower Link Manager (LE): connection request TX confirm.
 */
typedef void (*r_llm_con_req_tx_cfm_fn_t)(uint32_t);

/**
 * r_llm_common_cmd_status_send, slot 632 of the IP functions table.
 * Lower Link Manager (LE): common command status send.
 */
typedef void (*r_llm_common_cmd_status_send_fn_t)(uint16_t, uint8_t);

/**
 * r_llm_test_mode_start_tx, slot 633 of the IP functions table.
 * Lower Link Manager (LE): test mode start TX.
 */
typedef uint32_t (*r_llm_test_mode_start_tx_fn_t)(uint8_t *);

/**
 * r_llm_test_mode_start_rx, slot 634 of the IP functions table.
 * Lower Link Manager (LE): test mode start RX.
 */
typedef uint32_t (*r_llm_test_mode_start_rx_fn_t)(uint8_t *);

/**
 * r_llm_set_adv_param, slot 635 of the IP functions table.
 * Lower Link Manager (LE): set advertising param.
 */
typedef uint32_t (*r_llm_set_adv_param_fn_t)(uint16_t *);

/**
 * r_llm_set_adv_en, slot 636 of the IP functions table.
 * Lower Link Manager (LE): set advertising enable.
 */
typedef uint32_t (*r_llm_set_adv_en_fn_t)(uint8_t *);

/**
 * r_llm_ral_update, slot 637 of the IP functions table.
 * Lower Link Manager (LE): ral update.
 */
typedef void (*r_llm_ral_update_fn_t)(void);

/**
 * r_llm_set_adv_data, slot 638 of the IP functions table.
 * Lower Link Manager (LE): set advertising data.
 */
typedef uint32_t (*r_llm_set_adv_data_fn_t)(uint8_t *);

/**
 * r_llm_set_scan_rsp_data, slot 639 of the IP functions table.
 * Lower Link Manager (LE): set scan response data.
 */
typedef uint32_t (*r_llm_set_scan_rsp_data_fn_t)(uint8_t *);

/**
 * r_llm_set_scan_param, slot 640 of the IP functions table.
 * Lower Link Manager (LE): set scan param.
 */
typedef uint32_t (*r_llm_set_scan_param_fn_t)(uint8_t *);

/**
 * r_llm_set_scan_en, slot 641 of the IP functions table.
 * Lower Link Manager (LE): set scan enable.
 */
typedef uint32_t (*r_llm_set_scan_en_fn_t)(uint8_t *);

/**
 * r_llm_wl_dev_add, slot 642 of the IP functions table.
 * Lower Link Manager (LE): whitelist dev add.
 */
typedef void (*r_llm_wl_dev_add_fn_t)(int32_t, uint8_t);

/**
 * r_llm_wl_dev_rem, slot 643 of the IP functions table.
 * Lower Link Manager (LE): whitelist dev remote.
 */
typedef void (*r_llm_wl_dev_rem_fn_t)(uint32_t, uint8_t);

/**
 * r_llm_wl_dev_add_hdl, slot 644 of the IP functions table.
 * Lower Link Manager (LE): whitelist dev add handle.
 */
typedef uint32_t (*r_llm_wl_dev_add_hdl_fn_t)(uint32_t, uint8_t);

/**
 * r_llm_wl_dev_rem_hdl, slot 645 of the IP functions table.
 * Lower Link Manager (LE): whitelist dev remote handle.
 */
typedef uint32_t (*r_llm_wl_dev_rem_hdl_fn_t)(uint32_t, uint8_t);

/**
 * r_llm_create_con, slot 646 of the IP functions table.
 * Lower Link Manager (LE): create connection.
 */
typedef int32_t (*r_llm_create_con_fn_t)(uint16_t *);

/**
 * r_llm_encryption_start, slot 647 of the IP functions table.
 * Lower Link Manager (LE): encryption start.
 */
typedef void (*r_llm_encryption_start_fn_t)(uint32_t *);

/**
 * r_llm_ral_dev_add, slot 648 of the IP functions table.
 * Lower Link Manager (LE): ral dev add.
 */
typedef uint32_t (*r_llm_ral_dev_add_fn_t)(uint8_t *);

/**
 * r_llm_ral_dev_rm, slot 649 of the IP functions table.
 * Lower Link Manager (LE): ral dev rm.
 */
typedef uint32_t (*r_llm_ral_dev_rm_fn_t)(uint8_t *);

/**
 * r_llm_ral_get_rpa, slot 650 of the IP functions table.
 * Lower Link Manager (LE): ral get rpa.
 */
typedef uint32_t (*r_llm_ral_get_rpa_fn_t)(uint8_t *, void *, uint8_t);

/**
 * r_llm_ral_set_timeout, slot 651 of the IP functions table.
 * Lower Link Manager (LE): ral set timeout.
 */
typedef uint32_t (*r_llm_ral_set_timeout_fn_t)(int16_t *);

/**
 * r_llm_util_bd_addr_wl_position, slot 652 of the IP functions table.
 * Lower Link Manager (LE): util Bluetooth device address whitelist position.
 */
typedef int16_t (*r_llm_util_bd_addr_wl_position_fn_t)(uint32_t, uint32_t);

/**
 * r_llm_util_bd_addr_in_wl, slot 653 of the IP functions table.
 * Lower Link Manager (LE): util Bluetooth device address in whitelist.
 */
typedef uint8_t (*r_llm_util_bd_addr_in_wl_fn_t)(uint32_t, uint8_t, uint8_t *);

/**
 * r_llm_util_bl_check, slot 654 of the IP functions table.
 * Lower Link Manager (LE): util bl check.
 */
typedef uint32_t (*r_llm_util_bl_check_fn_t)(int32_t, uint8_t, uint16_t *, uint8_t, uint8_t *);

/**
 * r_llm_util_check_address_validity, slot 655 of the IP functions table.
 * Lower Link Manager (LE): util check address validity.
 */
typedef uint32_t (*r_llm_util_check_address_validity_fn_t)(uint32_t);

/**
 * r_llm_util_check_map_validity, slot 656 of the IP functions table.
 * Lower Link Manager (LE): util check map validity.
 */
typedef uint32_t (*r_llm_util_check_map_validity_fn_t)(uint8_t *, uint32_t);

/**
 * r_llm_util_apply_bd_addr, slot 657 of the IP functions table.
 * Programs one of the controller's stored device addresses into the link-layer
 * hardware (via lld_util_set_bd_address). The ROM calls it during init; call it
 * again after overwriting a stored address for the change to take effect on air.
 *
 * @param addr_type which stored address to apply: 0 = public (llm_local_pub_addr),
 *                  1 or 3 = random.
 */
typedef void (*r_llm_util_apply_bd_addr_fn_t)(uint8_t addr_type);

/**
 * r_llm_util_set_public_addr, slot 658 of the IP functions table.
 * Copies a 6-byte BD address into the controller's stored public address
 * (llm_local_pub_addr). It only updates the stored value -- it does NOT reprogram
 * the radio; call llm_util_apply_bd_addr(0) afterwards for the new address to take
 * effect on air. This is the mechanism the ROM's dbg "set BD address" uses.
 *
 * @param bd_addr pointer to the 6 address bytes (LSB first).
 */
typedef void (*r_llm_util_set_public_addr_fn_t)(const uint8_t *bd_addr);

/**
 * r_llm_util_check_evt_mask, slot 659 of the IP functions table.
 * Lower Link Manager (LE): util check event mask.
 */
typedef uint32_t (*r_llm_util_check_evt_mask_fn_t)(uint32_t);

/**
 * r_llm_util_get_channel_map, slot 660 of the IP functions table.
 * Lower Link Manager (LE): util get channel map.
 */
typedef void (*r_llm_util_get_channel_map_fn_t)(void *);

/**
 * r_llm_util_get_supp_features, slot 661 of the IP functions table.
 * Lower Link Manager (LE): util get supp features.
 */
typedef void (*r_llm_util_get_supp_features_fn_t)(void *);

/**
 * r_llm_util_adv_data_update, slot 662 of the IP functions table.
 * Lower Link Manager (LE): util advertising data update.
 */
typedef void (*r_llm_util_adv_data_update_fn_t)(void);

/**
 * r_llm_util_bl_add, slot 663 of the IP functions table.
 * Lower Link Manager (LE): util bl add.
 */
typedef uint32_t (*r_llm_util_bl_add_fn_t)(void *, uint32_t, uint16_t);

/**
 * r_llm_util_bd_addr_in_ral, slot 664 of the IP functions table.
 * Lower Link Manager (LE): util Bluetooth device address in ral.
 */
typedef bool (*r_llm_util_bd_addr_in_ral_fn_t)(uint32_t, uint16_t, uint8_t *);

/**
 * r_llm_util_bl_rem, slot 665 of the IP functions table.
 * Lower Link Manager (LE): util bl remote.
 */
typedef uint32_t (*r_llm_util_bl_rem_fn_t)(int16_t);

/**
 * r_llm_end_evt_defer, slot 666 of the IP functions table.
 * Lower Link Manager (LE): end event defer.
 */
typedef void (*r_llm_end_evt_defer_fn_t)(void);

/**
 * r_llm_pdu_defer, slot 667 of the IP functions table.
 * Lower Link Manager (LE): PDU defer.
 */
typedef uint32_t (*r_llm_pdu_defer_fn_t)(uint32_t, uint32_t, uint8_t);

/**
 * r_lm_init_wrap, slot 668 of the IP functions table.
 * Link Manager: init (libbtdm wrapper).
 */
typedef void (*r_lm_init_wrap_fn_t)(uint8_t);

/**
 * r_lm_lt_addr_alloc, slot 669 of the IP functions table.
 * Link Manager: logical transport address alloc.
 */
typedef uint32_t (*r_lm_lt_addr_alloc_fn_t)(void);

/**
 * r_lm_lt_addr_reserve, slot 670 of the IP functions table.
 * Link Manager: logical transport address reserve.
 */
typedef uint32_t (*r_lm_lt_addr_reserve_fn_t)(uint32_t);

/**
 * r_lm_lt_addr_free, slot 671 of the IP functions table.
 * Link Manager: logical transport address free.
 */
typedef void (*r_lm_lt_addr_free_fn_t)(uint32_t);

/**
 * r_lm_get_nb_acl, slot 672 of the IP functions table.
 * Link Manager: get number of ACL.
 */
typedef uint8_t (*r_lm_get_nb_acl_fn_t)(uint32_t);

/**
 * r_lm_role_switch_start, slot 673 of the IP functions table.
 * Link Manager: role switch start.
 */
typedef uint32_t (*r_lm_role_switch_start_fn_t)(uint32_t, uint8_t *);

/**
 * r_lm_role_switch_finished, slot 674 of the IP functions table.
 * Link Manager: role switch finished.
 */
typedef void (*r_lm_role_switch_finished_fn_t)(uint32_t, uint8_t);

/**
 * r_lm_read_features, slot 675 of the IP functions table.
 * Link Manager: read features.
 */
typedef void (*r_lm_read_features_fn_t)(uint32_t, uint8_t *, uint8_t *);

/**
 * r_lm_acl_disc, slot 676 of the IP functions table.
 * Link Manager: ACL disc.
 */
typedef void (*r_lm_acl_disc_fn_t)(uint8_t);

/**
 * r_lm_get_auth_en, slot 677 of the IP functions table.
 * Link Manager: get authentication enable.
 */
typedef bool (*r_lm_get_auth_en_fn_t)(void);

/**
 * r_lm_get_sp_en, slot 678 of the IP functions table.
 * Link Manager: get secure simple pairing enable.
 */
typedef bool (*r_lm_get_sp_en_fn_t)(void);

/**
 * r_lm_get_sec_con_host_supp, slot 679 of the IP functions table.
 * Link Manager: get sec connection host supp.
 */
typedef bool (*r_lm_get_sec_con_host_supp_fn_t)(void);

/**
 * r_LM_GetPINType, slot 680 of the IP functions table.
 * Link Manager: GetPINType.
 */
typedef uint8_t (*r_LM_GetPINType_fn_t)(void);

/**
 * r_LM_GetConnectionAcceptTimeout, slot 681 of the IP functions table.
 * Link Manager: GetConnectionAcceptTimeout.
 */
typedef uint16_t (*r_LM_GetConnectionAcceptTimeout_fn_t)(void);

/**
 * r_LM_GetLocalNameSeg, slot 682 of the IP functions table.
 * Link Manager: GetLocalNameSeg.
 */
typedef void (*r_LM_GetLocalNameSeg_fn_t)(void *, uint32_t, uint8_t *);

/**
 * r_lm_get_loopback_mode, slot 683 of the IP functions table.
 * Link Manager: get loopback mode.
 */
typedef uint8_t (*r_lm_get_loopback_mode_fn_t)(void);

/**
 * r_lm_get_pub_key_192, slot 684 of the IP functions table.
 * Link Manager: get pub key 192.
 */
typedef void (*r_lm_get_pub_key_192_fn_t)(void *);

/**
 * r_lm_get_priv_key_192, slot 685 of the IP functions table.
 * Link Manager: get priv key 192.
 */
typedef void (*r_lm_get_priv_key_192_fn_t)(void *);

/**
 * r_lm_get_pub_key_256, slot 686 of the IP functions table.
 * Link Manager: get pub key 256.
 */
typedef void (*r_lm_get_pub_key_256_fn_t)(void *);

/**
 * r_lm_get_priv_key_256, slot 687 of the IP functions table.
 * Link Manager: get priv key 256.
 */
typedef void (*r_lm_get_priv_key_256_fn_t)(void *);

/**
 * r_lm_get_oob_local_data_192, slot 688 of the IP functions table.
 * Link Manager: get oob local data 192.
 */
typedef void (*r_lm_get_oob_local_data_192_fn_t)(void *, void *);

/**
 * r_lm_get_oob_local_data_256, slot 689 of the IP functions table.
 * Link Manager: get oob local data 256.
 */
typedef void (*r_lm_get_oob_local_data_256_fn_t)(void *, void *);

/**
 * r_lm_get_oob_local_commit, slot 690 of the IP functions table.
 * Link Manager: get oob local commit.
 */
typedef void (*r_lm_get_oob_local_commit_fn_t)(void *, uint32_t);

/**
 * r_lm_sp_debug_mode_get, slot 691 of the IP functions table.
 * Link Manager: secure simple pairing debug mode get.
 */
typedef uint8_t (*r_lm_sp_debug_mode_get_fn_t)(void);

/**
 * r_lm_debug_key_compare_192, slot 692 of the IP functions table.
 * Link Manager: debug key compare 192.
 */
typedef bool (*r_lm_debug_key_compare_192_fn_t)(void *, void *);

/**
 * r_lm_debug_key_compare_256, slot 693 of the IP functions table.
 * Link Manager: debug key compare 256.
 */
typedef bool (*r_lm_debug_key_compare_256_fn_t)(void *);

/**
 * r_lm_dut_mode_en_get, slot 694 of the IP functions table.
 * Link Manager: dut mode enable get.
 */
typedef uint8_t (*r_lm_dut_mode_en_get_fn_t)(void);

/**
 * r_lm_afh_ch_ass_en_get, slot 695 of the IP functions table.
 * Link Manager: AFH channel ass enable get.
 */
typedef uint8_t (*r_lm_afh_ch_ass_en_get_fn_t)(void);

/**
 * r_lm_sync_flow_ctrl_en_get, slot 696 of the IP functions table.
 * Link Manager: SCO/sync flow ctrl enable get.
 */
typedef uint8_t (*r_lm_sync_flow_ctrl_en_get_fn_t)(void);

/**
 * r_lm_afh_host_ch_class_get, slot 697 of the IP functions table.
 * Link Manager: AFH host channel class get.
 */
typedef uint8_t *(*r_lm_afh_host_ch_class_get_fn_t)(void);

/**
 * r_lm_afh_peer_ch_class_set, slot 698 of the IP functions table.
 * Link Manager: AFH peer channel class set.
 */
typedef void (*r_lm_afh_peer_ch_class_set_fn_t)(uint32_t, void *);

/**
 * r_lm_afh_master_ch_map_get, slot 699 of the IP functions table.
 * Link Manager: AFH master channel map get.
 */
typedef uint8_t *(*r_lm_afh_master_ch_map_get_fn_t)(void);

/**
 * r_lm_afh_activate_timer, slot 700 of the IP functions table.
 * Link Manager: AFH activate timer.
 */
typedef void (*r_lm_afh_activate_timer_fn_t)(void);

/**
 * r_lm_is_acl_con, slot 701 of the IP functions table.
 * Link Manager: is ACL connection.
 */
typedef bool (*r_lm_is_acl_con_fn_t)(uint32_t);

/**
 * r_lm_is_acl_con_role, slot 702 of the IP functions table.
 * Link Manager: is ACL connection role.
 */
typedef uint32_t (*r_lm_is_acl_con_role_fn_t)(uint32_t, uint8_t);

/**
 * r_lm_is_clk_adj_instant_pending, slot 703 of the IP functions table.
 * Link Manager: is clock adjust instant pending.
 */
typedef uint8_t (*r_lm_is_clk_adj_instant_pending_fn_t)(void);

/**
 * r_lm_clk_adj_instant_pending_set, slot 704 of the IP functions table.
 * Link Manager: clock adjust instant pending set.
 */
typedef void (*r_lm_clk_adj_instant_pending_set_fn_t)(uint8_t);

/**
 * r_lm_is_clk_adj_ack_pending, slot 705 of the IP functions table.
 * Link Manager: is clock adjust ack pending.
 */
typedef bool (*r_lm_is_clk_adj_ack_pending_fn_t)(void);

/**
 * r_lm_num_clk_adj_ack_pending_set, slot 706 of the IP functions table.
 * Link Manager: num clock adjust ack pending set.
 */
typedef void (*r_lm_num_clk_adj_ack_pending_set_fn_t)(uint8_t);

/**
 * r_lm_clk_adj_ack_pending_clear, slot 707 of the IP functions table.
 * Link Manager: clock adjust ack pending clear.
 */
typedef void (*r_lm_clk_adj_ack_pending_clear_fn_t)(uint32_t);

/**
 * r_lm_local_ext_fr_configured, slot 708 of the IP functions table.
 * Link Manager: local ext fr configured.
 */
typedef uint8_t (*r_lm_local_ext_fr_configured_fn_t)(void);

/**
 * r_lm_pca_sscan_link_set, slot 709 of the IP functions table.
 * Link Manager: pca sscan link set.
 */
typedef void (*r_lm_pca_sscan_link_set_fn_t)(uint8_t);

/**
 * r_lm_pca_sscan_link_get, slot 710 of the IP functions table.
 * Link Manager: pca sscan link get.
 */
typedef uint8_t (*r_lm_pca_sscan_link_get_fn_t)(void);

/**
 * r_lm_get_sync_param, slot 711 of the IP functions table.
 * Link Manager: get SCO/sync param.
 */
typedef uint32_t (*r_lm_get_sync_param_fn_t)(uint32_t, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint16_t *, uint16_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t);

/**
 * r_lm_init_sync, slot 712 of the IP functions table.
 * Link Manager: init SCO/sync.
 */
typedef void (*r_lm_init_sync_fn_t)(void);

/**
 * r_lm_reset_sync, slot 713 of the IP functions table.
 * Link Manager: reset SCO/sync.
 */
typedef void (*r_lm_reset_sync_fn_t)(void);

/**
 * r_lm_check_active_sync, slot 714 of the IP functions table.
 * Link Manager: check active SCO/sync.
 */
typedef bool (*r_lm_check_active_sync_fn_t)(void);

/**
 * r_lm_add_sync, slot 715 of the IP functions table.
 * Link Manager: add SCO/sync.
 */
typedef uint32_t (*r_lm_add_sync_fn_t)(uint8_t, int32_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t *, uint32_t, uint32_t, uint16_t, uint16_t, uint8_t, uint8_t, uint16_t, uint8_t);

/**
 * r_lm_modif_sync, slot 716 of the IP functions table.
 * Link Manager: modif SCO/sync.
 */
typedef uint32_t (*r_lm_modif_sync_fn_t)(uint32_t, uint8_t, int32_t, int32_t, uint16_t, uint16_t, uint8_t, uint16_t, uint8_t);

/**
 * r_lm_check_sync_hl_rsp, slot 717 of the IP functions table.
 * Link Manager: check SCO/sync higher-layer response.
 */
typedef uint32_t (*r_lm_check_sync_hl_rsp_fn_t)(uint32_t, int32_t, int32_t, uint16_t, uint32_t, uint8_t, uint16_t);

/**
 * r_lm_get_synchdl, slot 718 of the IP functions table.
 * Link Manager: get synchdl.
 */
typedef uint8_t (*r_lm_get_synchdl_fn_t)(uint32_t);

/**
 * r_lm_look_for_sync, slot 719 of the IP functions table.
 * Link Manager: look for SCO/sync.
 */
typedef uint32_t (*r_lm_look_for_sync_fn_t)(uint8_t);

/**
 * r_lm_get_nb_sync_link, slot 720 of the IP functions table.
 * Link Manager: get number of SCO/sync link.
 */
typedef uint8_t (*r_lm_get_nb_sync_link_fn_t)(void);

/**
 * r_lm_get_min_sync_intv, slot 721 of the IP functions table.
 * Link Manager: get min SCO/sync intv.
 */
typedef uint32_t (*r_lm_get_min_sync_intv_fn_t)(void);

/**
 * r_lm_remove_sync, slot 722 of the IP functions table.
 * Link Manager: remove SCO/sync.
 */
typedef void (*r_lm_remove_sync_fn_t)(uint32_t);

/**
 * r_lm_sco_nego_end, slot 723 of the IP functions table.
 * Link Manager: SCO negotiation end.
 */
typedef void (*r_lm_sco_nego_end_fn_t)(void);

/**
 * r_lm_master_clk_adj_req_handler, slot 724 of the IP functions table.
 * Link Manager: master clock adjust request handler.
 */
typedef int32_t (*r_lm_master_clk_adj_req_handler_fn_t)(int16_t, uint8_t, uint8_t, uint8_t);

/**
 * r_rwble_init, slot 725 of the IP functions table.
 * RW BLE core: init.
 */
typedef void (*r_rwble_init_fn_t)(void);

/**
 * r_rwble_reset, slot 726 of the IP functions table.
 * RW BLE core: reset.
 */
typedef void (*r_rwble_reset_fn_t)(void);

/**
 * r_rwble_sleep_check, slot 727 of the IP functions table.
 * RW BLE core: sleep check.
 */
typedef bool (*r_rwble_sleep_check_fn_t)(void);

/**
 * r_rwble_activity_ongoing_check, slot 728 of the IP functions table.
 * RW BLE core: activity ongoing check.
 */
typedef bool (*r_rwble_activity_ongoing_check_fn_t)(void);

/**
 * r_rwble_version, slot 729 of the IP functions table.
 * RW BLE core: version.
 */
typedef void (*r_rwble_version_fn_t)(uint8_t *, uint8_t *);

/**
 * r_rwble_isr, slot 730 of the IP functions table.
 * RW BLE core: isr.
 */
typedef void (*r_rwble_isr_fn_t)(void);

/**
 * r_rwbt_init, slot 731 of the IP functions table.
 * RW BT core: init.
 */
typedef void (*r_rwbt_init_fn_t)(void);

/**
 * r_rwbt_reset, slot 732 of the IP functions table.
 * RW BT core: reset.
 */
typedef void (*r_rwbt_reset_fn_t)(void);

/**
 * r_rwbt_version, slot 733 of the IP functions table.
 * RW BT core: version.
 */
typedef void (*r_rwbt_version_fn_t)(uint8_t *, uint32_t);

/**
 * r_rwbt_isr, slot 734 of the IP functions table.
 * RW BT core: isr.
 */
typedef void (*r_rwbt_isr_fn_t)(void);

/**
 * r_rwbt_sleep_check, slot 735 of the IP functions table.
 * RW BT core: sleep check.
 */
typedef uint8_t (*r_rwbt_sleep_check_fn_t)(void);

/**
 * r_rwbt_sleep_enter, slot 736 of the IP functions table.
 * RW BT core: sleep enter.
 */
typedef void (*r_rwbt_sleep_enter_fn_t)(uint32_t, uint8_t);

/**
 * r_rwbt_sleep_wakeup, slot 737 of the IP functions table.
 * RW BT core: sleep wakeup.
 */
typedef void (*r_rwbt_sleep_wakeup_fn_t)(void);

/**
 * r_rwbt_sleep_wakeup_end, slot 738 of the IP functions table.
 * RW BT core: sleep wakeup end.
 */
typedef void (*r_rwbt_sleep_wakeup_end_fn_t)(void);

/**
 * btdm_lpcycles_2_us, slot 739 of the IP functions table.
 * BTDM: lpcycles 2 us.
 * Parameters not recovered (no ROM decompilation target).
 */
typedef void (*r_btdm_lpcycles_2_us_fn_t)(void);

/**
 * btdm_us_2_lpcycles, slot 740 of the IP functions table.
 * BTDM: us 2 lpcycles.
 * Parameters not recovered (no ROM decompilation target).
 */
typedef void (*r_btdm_us_2_lpcycles_fn_t)(void);

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
    r_LM_AddSniff_fn_t LM_AddSniff;                                                                 /* [300] */
    r_LM_RemoveSniff_fn_t LM_RemoveSniff;                                                           /* [301] */
    r_LM_SniffSubratingHlReq_fn_t LM_SniffSubratingHlReq;                                           /* [302] */
    r_LM_SniffSubratingPeerReq_fn_t LM_SniffSubratingPeerReq;                                       /* [303] */
    r_LM_GetSniffSubratingParam_fn_t LM_GetSniffSubratingParam;                                     /* [304] */
    r_LM_SniffSubrateNegoRequired_fn_t LM_SniffSubrateNegoRequired;                                 /* [305] */
    r_LM_ComputeSniffSubRate_fn_t LM_ComputeSniffSubRate;                                           /* [306] */
    r_LM_RemoveSniffSubrating_fn_t LM_RemoveSniffSubrating;                                         /* [307] */
    r_LM_GetLinkTimeout_fn_t LM_GetLinkTimeout;                                                     /* [308] */
    r_lc_util_get_max_packet_size_fn_t lc_util_get_max_packet_size;                                 /* [309] */
    r_lc_util_set_loc_trans_coll_fn_t lc_util_set_loc_trans_coll;                                   /* [310] */
    r_lc_util_get_offset_clkn_fn_t lc_util_get_offset_clkn;                                         /* [311] */
    r_lc_util_get_offset_clke_fn_t lc_util_get_offset_clke;                                         /* [312] */
    r_LM_ComputePacketType_fn_t LM_ComputePacketType;                                               /* [313] */
    r_LM_UpdateAclPacketType_fn_t LM_UpdateAclPacketType;                                           /* [314] */
    r_LM_UpdateAclEdrPacketType_fn_t LM_UpdateAclEdrPacketType;                                     /* [315] */
    r_LM_SuppressAclPacket_fn_t LM_SuppressAclPacket;                                               /* [316] */
    r_LM_MaxSlot_fn_t LM_MaxSlot;                                                                   /* [317] */
    r_LM_GetQoSParam_fn_t LM_GetQoSParam;                                                           /* [318] */
    r_LM_GetSwitchInstant_fn_t LM_GetSwitchInstant;                                                 /* [319] */
    r_LM_MakeCof_fn_t LM_MakeCof;                                                                   /* [320] */
    r_LM_GetAFHSwitchInstant_fn_t LM_GetAFHSwitchInstant;                                           /* [321] */
    r_LM_CheckSwitchInstant_fn_t LM_CheckSwitchInstant;                                             /* [322] */
    r_LM_CheckEdrFeatureRequest_fn_t LM_CheckEdrFeatureRequest;                                     /* [323] */
    r_LM_GetFeature_fn_t LM_GetFeature;                                                             /* [324] */
    r_lm_look_for_stored_link_key_fn_t lm_look_for_stored_link_key;                                 /* [325] */
    r_lc_util_convert_pref_rate_to_packet_type_fn_t lc_util_convert_pref_rate_to_packet_type;       /* [326] */
    r_lm_get_common_pkt_types_fn_t lm_get_common_pkt_types;                                         /* [327] */
    r_ld_acl_init_fn_t ld_acl_init;                                                                 /* [328] */
    r_ld_acl_reset_fn_t ld_acl_reset;                                                               /* [329] */
    r_ld_acl_start_fn_t ld_acl_start;                                                               /* [330] */
    r_ld_acl_stop_fn_t ld_acl_stop;                                                                 /* [331] */
    r_ld_acl_flow_off_fn_t ld_acl_flow_off;                                                         /* [332] */
    r_ld_acl_flow_on_fn_t ld_acl_flow_on;                                                           /* [333] */
    r_ld_acl_data_tx_fn_t ld_acl_data_tx;                                                           /* [334] */
    r_ld_acl_data_flush_fn_t ld_acl_data_flush;                                                     /* [335] */
    r_ld_acl_lmp_tx_fn_t ld_acl_lmp_tx;                                                             /* [336] */
    r_ld_acl_lmp_flush_fn_t ld_acl_lmp_flush;                                                       /* [337] */
    r_ld_acl_rsw_req_fn_t ld_acl_rsw_req;                                                           /* [338] */
    r_ld_acl_flush_timeout_get_fn_t ld_acl_flush_timeout_get;                                       /* [339] */
    r_ld_acl_flush_timeout_set_fn_t ld_acl_flush_timeout_set;                                       /* [340] */
    r_ld_acl_t_poll_get_fn_t ld_acl_t_poll_get;                                                     /* [341] */
    r_ld_acl_t_poll_set_fn_t ld_acl_t_poll_set;                                                     /* [342] */
    r_ld_acl_sniff_trans_hack_fn_t ld_acl_sniff_trans_hack;                                         /* [343] */
    r_ld_acl_sniff_fn_t ld_acl_sniff;                                                               /* [344] */
    r_ld_acl_unsniff_fn_t ld_acl_unsniff;                                                           /* [345] */
    r_ld_acl_ssr_set_fn_t ld_acl_ssr_set;                                                           /* [346] */
    r_ld_acl_tx_enc_fn_t ld_acl_tx_enc;                                                             /* [347] */
    r_ld_acl_rx_enc_fn_t ld_acl_rx_enc;                                                             /* [348] */
    r_ld_acl_bcst_rx_dec_fn_t ld_acl_bcst_rx_dec;                                                   /* [349] */
    r_ld_acl_enc_key_load_fn_t ld_acl_enc_key_load;                                                 /* [350] */
    r_ld_acl_clock_offset_get_fn_t ld_acl_clock_offset_get;                                         /* [351] */
    r_ld_acl_slot_offset_get_fn_t ld_acl_slot_offset_get;                                           /* [352] */
    r_ld_acl_slot_offset_set_fn_t ld_acl_slot_offset_set;                                           /* [353] */
    r_ld_acl_lsto_get_fn_t ld_acl_lsto_get;                                                         /* [354] */
    r_ld_acl_lsto_set_fn_t ld_acl_lsto_set;                                                         /* [355] */
    r_ld_acl_timing_accuracy_set_fn_t ld_acl_timing_accuracy_set;                                   /* [356] */
    r_ld_acl_edr_set_fn_t ld_acl_edr_set;                                                           /* [357] */
    r_ld_acl_allowed_tx_packet_types_set_fn_t ld_acl_allowed_tx_packet_types_set;                   /* [358] */
    r_ld_acl_current_tx_power_get_fn_t ld_acl_current_tx_power_get;                                 /* [359] */
    r_ld_acl_clk_set_fn_t ld_acl_clk_set;                                                           /* [360] */
    r_ld_acl_clk_adj_set_fn_t ld_acl_clk_adj_set;                                                   /* [361] */
    r_ld_acl_clk_off_get_fn_t ld_acl_clk_off_get;                                                   /* [362] */
    r_ld_acl_bit_off_get_fn_t ld_acl_bit_off_get;                                                   /* [363] */
    r_ld_acl_role_get_fn_t ld_acl_role_get;                                                         /* [364] */
    r_ld_acl_afh_set_fn_t ld_acl_afh_set;                                                           /* [365] */
    r_ld_acl_afh_prepare_fn_t ld_acl_afh_prepare;                                                   /* [366] */
    r_ld_acl_afh_confirm_fn_t ld_acl_afh_confirm;                                                   /* [367] */
    r_ld_acl_active_hop_types_get_fn_t ld_acl_active_hop_types_get;                                 /* [368] */
    r_ld_acl_rx_max_slot_get_fn_t ld_acl_rx_max_slot_get;                                           /* [369] */
    r_ld_acl_rx_max_slot_set_fn_t ld_acl_rx_max_slot_set;                                           /* [370] */
    r_ld_acl_test_mode_set_fn_t ld_acl_test_mode_set;                                               /* [371] */
    r_ld_acl_rssi_delta_get_fn_t ld_acl_rssi_delta_get;                                             /* [372] */
    r_ld_sco_start_fn_t ld_sco_start;                                                               /* [373] */
    r_ld_sco_update_fn_t ld_sco_update;                                                             /* [374] */
    r_ld_sco_stop_fn_t ld_sco_stop;                                                                 /* [375] */
    r_ld_sco_audio_isr_fn_t ld_sco_audio_isr;                                                       /* [376] */
    r_ld_sco_data_tx_hack_fn_t ld_sco_data_tx_hack;                                                 /* [377] */
    r_ld_bcst_acl_start_fn_t ld_bcst_acl_start;                                                     /* [378] */
    r_ld_bcst_acl_init_fn_t ld_bcst_acl_init;                                                       /* [379] */
    r_ld_bcst_acl_reset_fn_t ld_bcst_acl_reset;                                                     /* [380] */
    r_ld_bcst_lmp_tx_fn_t ld_bcst_lmp_tx;                                                           /* [381] */
    r_ld_bcst_acl_data_tx_fn_t ld_bcst_acl_data_tx;                                                 /* [382] */
    r_ld_bcst_afh_update_fn_t ld_bcst_afh_update;                                                   /* [383] */
    r_ld_bcst_tx_enc_fn_t ld_bcst_tx_enc;                                                           /* [384] */
    r_ld_bcst_enc_key_load_fn_t ld_bcst_enc_key_load;                                               /* [385] */
    r_ld_csb_rx_init_fn_t ld_csb_rx_init;                                                           /* [386] */
    r_ld_csb_rx_reset_fn_t ld_csb_rx_reset;                                                         /* [387] */
    r_ld_csb_rx_start_fn_t ld_csb_rx_start;                                                         /* [388] */
    r_ld_csb_rx_afh_update_fn_t ld_csb_rx_afh_update;                                               /* [389] */
    r_ld_csb_rx_stop_fn_t ld_csb_rx_stop;                                                           /* [390] */
    r_ld_csb_tx_init_fn_t ld_csb_tx_init;                                                           /* [391] */
    r_ld_csb_tx_reset_fn_t ld_csb_tx_reset;                                                         /* [392] */
    r_ld_csb_tx_en_fn_t ld_csb_tx_en;                                                               /* [393] */
    r_ld_csb_tx_dis_fn_t ld_csb_tx_dis;                                                             /* [394] */
    r_ld_csb_tx_afh_update_fn_t ld_csb_tx_afh_update;                                               /* [395] */
    r_ld_csb_tx_set_data_fn_t ld_csb_tx_set_data;                                                   /* [396] */
    r_ld_csb_tx_clr_data_fn_t ld_csb_tx_clr_data;                                                   /* [397] */
    r_ld_fm_init_fn_t ld_fm_init;                                                                   /* [398] */
    r_ld_fm_reset_fn_t ld_fm_reset;                                                                 /* [399] */
    r_ld_fm_clk_isr_fn_t ld_fm_clk_isr;                                                             /* [400] */
    r_ld_fm_rx_isr_fn_t ld_fm_rx_isr;                                                               /* [401] */
    r_ld_fm_frame_isr_fn_t ld_fm_frame_isr;                                                         /* [402] */
    r_ld_fm_sket_isr_fn_t ld_fm_sket_isr;                                                           /* [403] */
    r_ld_fm_prog_enable_fn_t ld_fm_prog_enable;                                                     /* [404] */
    r_ld_fm_prog_disable_fn_t ld_fm_prog_disable;                                                   /* [405] */
    r_ld_fm_prog_push_fn_t ld_fm_prog_push;                                                         /* [406] */
    r_ld_fm_prog_check_fn_t ld_fm_prog_check;                                                       /* [407] */
    r_ld_inq_init_fn_t ld_inq_init;                                                                 /* [408] */
    r_ld_inq_reset_fn_t ld_inq_reset;                                                               /* [409] */
    r_ld_inq_start_fn_t ld_inq_start;                                                               /* [410] */
    r_ld_inq_stop_fn_t ld_inq_stop;                                                                 /* [411] */
    r_ld_iscan_init_fn_t ld_iscan_init;                                                             /* [412] */
    r_ld_iscan_reset_fn_t ld_iscan_reset;                                                           /* [413] */
    r_ld_iscan_restart_fn_t ld_iscan_restart;                                                       /* [414] */
    r_ld_iscan_start_fn_t ld_iscan_start;                                                           /* [415] */
    r_ld_iscan_stop_fn_t ld_iscan_stop;                                                             /* [416] */
    r_ld_iscan_eir_set_fn_t ld_iscan_eir_set;                                                       /* [417] */
    r_ld_iscan_eir_get_fn_t ld_iscan_eir_get;                                                       /* [418] */
    r_ld_iscan_tx_pwr_get_fn_t ld_iscan_tx_pwr_get;                                                 /* [419] */
    r_ld_wlcoex_set_fn_t ld_wlcoex_set;                                                             /* [420] */
    r_ld_channel_assess_fn_t ld_channel_assess;                                                     /* [421] */
    r_ld_init_hark_fn_t ld_init_hark;                                                               /* [422] */
    r_ld_reset_hark_fn_t ld_reset_hark;                                                             /* [423] */
    r_ld_read_clock_fn_t ld_read_clock;                                                             /* [424] */
    r_ld_bd_addr_get_fn_t ld_bd_addr_get;                                                           /* [425] */
    r_ld_class_of_dev_get_fn_t ld_class_of_dev_get;                                                 /* [426] */
    r_ld_class_of_dev_set_fn_t ld_class_of_dev_set;                                                 /* [427] */
    r_ld_version_get_fn_t ld_version_get;                                                           /* [428] */
    r_ld_timing_accuracy_get_fn_t ld_timing_accuracy_get;                                           /* [429] */
    r_ld_active_check_fn_t ld_active_check;                                                         /* [430] */
    r_ld_afh_ch_assess_data_get_fn_t ld_afh_ch_assess_data_get;                                     /* [431] */
    r_ld_page_init_fn_t ld_page_init;                                                               /* [432] */
    r_ld_page_reset_fn_t ld_page_reset;                                                             /* [433] */
    r_ld_page_start_fn_t ld_page_start;                                                             /* [434] */
    r_ld_page_stop_fn_t ld_page_stop;                                                               /* [435] */
    r_ld_pca_reporting_enable_fn_t ld_pca_reporting_enable;                                         /* [436] */
    r_ld_pca_init_fn_t ld_pca_init;                                                                 /* [437] */
    r_ld_pca_reset_fn_t ld_pca_reset;                                                               /* [438] */
    r_ld_pca_local_config_fn_t ld_pca_local_config;                                                 /* [439] */
    r_ld_pca_update_target_offset_fn_t ld_pca_update_target_offset;                                 /* [440] */
    r_ld_pca_mws_frame_sync_fn_t ld_pca_mws_frame_sync;                                             /* [441] */
    r_ld_pca_coarse_clock_adjust_fn_t ld_pca_coarse_clock_adjust;                                   /* [442] */
    r_ld_pca_mws_moment_offset_gt_fn_t ld_pca_mws_moment_offset_gt;                                 /* [443] */
    r_ld_pca_mws_moment_offset_lt_fn_t ld_pca_mws_moment_offset_lt;                                 /* [444] */
    r_ld_pca_initiate_clock_dragging_fn_t ld_pca_initiate_clock_dragging;                           /* [445] */
    r_ld_pscan_init_fn_t ld_pscan_init;                                                             /* [446] */
    r_ld_pscan_reset_fn_t ld_pscan_reset;                                                           /* [447] */
    r_ld_pscan_restart_fn_t ld_pscan_restart;                                                       /* [448] */
    r_ld_pscan_start_fn_t ld_pscan_start;                                                           /* [449] */
    r_ld_pscan_stop_fn_t ld_pscan_stop;                                                             /* [450] */
    r_ld_pscan_evt_handler_fn_t ld_pscan_evt_handler;                                               /* [451] */
    r_ld_sched_compute_fn_t ld_sched_compute;                                                       /* [452] */
    r_ld_sched_init_fn_t ld_sched_init;                                                             /* [453] */
    r_ld_sched_reset_fn_t ld_sched_reset;                                                           /* [454] */
    r_ld_sched_iscan_add_fn_t ld_sched_iscan_add;                                                   /* [455] */
    r_ld_sched_iscan_remove_fn_t ld_sched_iscan_remove;                                             /* [456] */
    r_ld_sched_pscan_add_fn_t ld_sched_pscan_add;                                                   /* [457] */
    r_ld_sched_pscan_remove_fn_t ld_sched_pscan_remove;                                             /* [458] */
    r_ld_sched_sscan_add_fn_t ld_sched_sscan_add;                                                   /* [459] */
    r_ld_sched_sscan_remove_fn_t ld_sched_sscan_remove;                                             /* [460] */
    r_ld_sched_inq_add_fn_t ld_sched_inq_add;                                                       /* [461] */
    r_ld_sched_inq_remove_fn_t ld_sched_inq_remove;                                                 /* [462] */
    r_ld_sched_page_add_fn_t ld_sched_page_add;                                                     /* [463] */
    r_ld_sched_page_remove_fn_t ld_sched_page_remove;                                               /* [464] */
    r_ld_sched_acl_add_fn_t ld_sched_acl_add;                                                       /* [465] */
    r_ld_sched_acl_remove_fn_t ld_sched_acl_remove;                                                 /* [466] */
    r_ld_sched_sniff_add_fn_t ld_sched_sniff_add;                                                   /* [467] */
    r_ld_sched_sniff_remove_fn_t ld_sched_sniff_remove;                                             /* [468] */
    r_ld_sched_sco_add_fn_t ld_sched_sco_add;                                                       /* [469] */
    r_ld_sched_sco_remove_fn_t ld_sched_sco_remove;                                                 /* [470] */
    r_ld_sscan_init_fn_t ld_sscan_init;                                                             /* [471] */
    r_ld_sscan_reset_fn_t ld_sscan_reset;                                                           /* [472] */
    r_ld_sscan_activated_fn_t ld_sscan_activated;                                                   /* [473] */
    r_ld_sscan_start_hark_fn_t ld_sscan_start_hark;                                                 /* [474] */
    r_ld_strain_init_fn_t ld_strain_init;                                                           /* [475] */
    r_ld_strain_reset_fn_t ld_strain_reset;                                                         /* [476] */
    r_ld_strain_start_fn_t ld_strain_start;                                                         /* [477] */
    r_ld_strain_stop_fn_t ld_strain_stop;                                                           /* [478] */
    r_ld_util_fhs_unpk_fn_t ld_util_fhs_unpk;                                                       /* [479] */
    r_ld_util_bch_create_fn_t ld_util_bch_create;                                                   /* [480] */
    r_ld_util_fhs_pk_fn_t ld_util_fhs_pk;                                                           /* [481] */
    r_ld_util_active_master_afh_map_set_fn_t ld_util_active_master_afh_map_set;                     /* [482] */
    r_ld_util_active_master_afh_map_get_fn_t ld_util_active_master_afh_map_get;                     /* [483] */
    r_ld_util_stp_unpk_fn_t ld_util_stp_unpk;                                                       /* [484] */
    r_ld_util_stp_pk_fn_t ld_util_stp_pk;                                                           /* [485] */
    r_llc_ch_assess_local_fn_t llc_ch_assess_local;                                                 /* [486] */
    r_llc_ch_assess_get_local_ch_map_fn_t llc_ch_assess_get_local_ch_map;                           /* [487] */
    r_llc_ch_assess_get_current_ch_map_fn_t llc_ch_assess_get_current_ch_map;                       /* [488] */
    r_llc_ch_assess_merge_ch_fn_t llc_ch_assess_merge_ch;                                           /* [489] */
    r_llc_ch_assess_reass_ch_fn_t llc_ch_assess_reass_ch;                                           /* [490] */
    r_llc_llcp_version_ind_pdu_send_fn_t llc_llcp_version_ind_pdu_send;                             /* [491] */
    r_llc_llcp_ch_map_update_pdu_send_fn_t llc_llcp_ch_map_update_pdu_send;                         /* [492] */
    r_llc_llcp_pause_enc_req_pdu_send_fn_t llc_llcp_pause_enc_req_pdu_send;                         /* [493] */
    r_llc_llcp_pause_enc_rsp_pdu_send_fn_t llc_llcp_pause_enc_rsp_pdu_send;                         /* [494] */
    r_llc_llcp_enc_req_pdu_send_fn_t llc_llcp_enc_req_pdu_send;                                     /* [495] */
    r_llc_llcp_enc_rsp_pdu_send_fn_t llc_llcp_enc_rsp_pdu_send;                                     /* [496] */
    r_llc_llcp_start_enc_rsp_pdu_send_fn_t llc_llcp_start_enc_rsp_pdu_send;                         /* [497] */
    r_llc_llcp_reject_ind_pdu_send_fn_t llc_llcp_reject_ind_pdu_send;                               /* [498] */
    r_llc_llcp_con_update_pdu_send_fn_t llc_llcp_con_update_pdu_send;                               /* [499] */
    r_llc_llcp_con_param_req_pdu_send_fn_t llc_llcp_con_param_req_pdu_send;                         /* [500] */
    r_llc_llcp_con_param_rsp_pdu_send_fn_t llc_llcp_con_param_rsp_pdu_send;                         /* [501] */
    r_llc_llcp_feats_req_pdu_send_fn_t llc_llcp_feats_req_pdu_send;                                 /* [502] */
    r_llc_llcp_feats_rsp_pdu_send_fn_t llc_llcp_feats_rsp_pdu_send;                                 /* [503] */
    r_llc_llcp_start_enc_req_pdu_send_fn_t llc_llcp_start_enc_req_pdu_send;                         /* [504] */
    r_llc_llcp_terminate_ind_pdu_send_fn_t llc_llcp_terminate_ind_pdu_send;                         /* [505] */
    r_llc_llcp_unknown_rsp_send_pdu_fn_t llc_llcp_unknown_rsp_send_pdu;                             /* [506] */
    r_llc_llcp_ping_req_pdu_send_fn_t llc_llcp_ping_req_pdu_send;                                   /* [507] */
    r_llc_llcp_ping_rsp_pdu_send_fn_t llc_llcp_ping_rsp_pdu_send;                                   /* [508] */
    r_llc_llcp_length_req_pdu_send_fn_t llc_llcp_length_req_pdu_send;                               /* [509] */
    r_llc_llcp_length_rsp_pdu_send_fn_t llc_llcp_length_rsp_pdu_send;                               /* [510] */
    r_llc_llcp_tester_send_fn_t llc_llcp_tester_send;                                               /* [511] */
    r_llc_llcp_recv_handler_hack_fn_t llc_llcp_recv_handler_hack;                                   /* [512] */
    r_llc_llcp_get_autorize_fn_t llc_llcp_get_autorize;                                             /* [513] */
    r_llc_init_hack_fn_t llc_init_hack;                                                             /* [514] */
    r_llc_reset_hack_fn_t llc_reset_hack;                                                           /* [515] */
    r_llc_stop_hack_fn_t llc_stop_hack;                                                             /* [516] */
    r_llc_start_fn_t llc_start;                                                                     /* [517] */
    r_llc_le_con_cmp_evt_send_fn_t llc_le_con_cmp_evt_send;                                         /* [518] */
    r_llc_discon_event_complete_send_fn_t llc_discon_event_complete_send;                           /* [519] */
    r_llc_con_update_complete_send_fn_t llc_con_update_complete_send;                               /* [520] */
    r_llc_ltk_req_send_fn_t llc_ltk_req_send;                                                       /* [521] */
    r_llc_feats_rd_event_send_fn_t llc_feats_rd_event_send;                                         /* [522] */
    r_llc_version_rd_event_send_fn_t llc_version_rd_event_send;                                     /* [523] */
    r_llc_common_cmd_complete_send_fn_t llc_common_cmd_complete_send;                               /* [524] */
    r_llc_common_cmd_status_send_hack_fn_t llc_common_cmd_status_send_hack;                         /* [525] */
    r_llc_common_flush_occurred_send_fn_t llc_common_flush_occurred_send;                           /* [526] */
    r_llc_common_enc_key_ref_comp_evt_send_fn_t llc_common_enc_key_ref_comp_evt_send;               /* [527] */
    r_llc_common_enc_change_evt_send_fn_t llc_common_enc_change_evt_send;                           /* [528] */
    r_llc_common_nb_of_pkt_comp_evt_send_fn_t llc_common_nb_of_pkt_comp_evt_send;                   /* [529] */
    r_llc_con_update_ind_fn_t llc_con_update_ind;                                                   /* [530] */
    r_llc_lsto_con_update_fn_t llc_lsto_con_update;                                                 /* [531] */
    r_llc_map_update_ind_fn_t llc_map_update_ind;                                                   /* [532] */
    r_llc_con_update_finished_fn_t llc_con_update_finished;                                         /* [533] */
    r_llc_map_update_finished_fn_t llc_map_update_finished;                                         /* [534] */
    r_llc_util_get_free_conhdl_fn_t llc_util_get_free_conhdl;                                       /* [535] */
    r_llc_util_get_nb_active_link_fn_t llc_util_get_nb_active_link;                                 /* [536] */
    r_llc_util_dicon_procedure_fn_t llc_util_dicon_procedure;                                       /* [537] */
    r_llc_util_clear_operation_ptr_fn_t llc_util_clear_operation_ptr;                               /* [538] */
    r_llc_util_update_channel_map_fn_t llc_util_update_channel_map;                                 /* [539] */
    r_llc_util_set_llcp_discard_enable_fn_t llc_util_set_llcp_discard_enable;                       /* [540] */
    r_llc_util_set_auth_payl_to_margin_fn_t llc_util_set_auth_payl_to_margin;                       /* [541] */
    r_llc_util_bw_mgt_fn_t llc_util_bw_mgt;                                                         /* [542] */
    r_llc_end_evt_defer_fn_t llc_end_evt_defer;                                                     /* [543] */
    r_llc_pdu_llcp_tx_ack_defer_fn_t llc_pdu_llcp_tx_ack_defer;                                     /* [544] */
    r_llc_pdu_acl_tx_ack_defer_fn_t llc_pdu_acl_tx_ack_defer;                                       /* [545] */
    r_llc_pdu_defer_fn_t llc_pdu_defer;                                                             /* [546] */
    r_lld_evt_elt_delete_fn_t lld_evt_elt_delete;                                                   /* [547] */
    r_lld_evt_drift_compute_fn_t lld_evt_drift_compute;                                             /* [548] */
    r_lld_evt_delete_elt_push_fn_t lld_evt_delete_elt_push;                                         /* [549] */
    r_lld_evt_channel_next_fn_t lld_evt_channel_next;                                               /* [550] */
    r_lld_evt_deffered_elt_handler_fn_t lld_evt_deffered_elt_handler;                               /* [551] */
    r_lld_evt_delete_elt_handler_fn_t lld_evt_delete_elt_handler;                                   /* [552] */
    r_lld_evt_init_fn_t lld_evt_init;                                                               /* [553] */
    r_lld_evt_init_evt_fn_t lld_evt_init_evt;                                                       /* [554] */
    r_lld_evt_restart_fn_t lld_evt_restart;                                                         /* [555] */
    r_lld_evt_elt_insert_fn_t lld_evt_elt_insert;                                                   /* [556] */
    r_lld_evt_schedule_next_instant_hack_fn_t lld_evt_schedule_next_instant_hack;                   /* [557] */
    r_lld_evt_schedule_next_fn_t lld_evt_schedule_next;                                             /* [558] */
    r_lld_evt_schedule_fn_t lld_evt_schedule;                                                       /* [559] */
    r_lld_evt_prevent_stop_fn_t lld_evt_prevent_stop;                                               /* [560] */
    r_lld_evt_canceled_fn_t lld_evt_canceled;                                                       /* [561] */
    r_lld_evt_scan_create_fn_t lld_evt_scan_create;                                                 /* [562] */
    r_lld_evt_move_to_master_fn_t lld_evt_move_to_master;                                           /* [563] */
    r_lld_evt_update_create_fn_t lld_evt_update_create;                                             /* [564] */
    r_lld_evt_move_to_slave_fn_t lld_evt_move_to_slave;                                             /* [565] */
    r_lld_evt_slave_update_fn_t lld_evt_slave_update;                                               /* [566] */
    r_lld_evt_adv_create_fn_t lld_evt_adv_create;                                                   /* [567] */
    r_lld_evt_end_fn_t lld_evt_end;                                                                 /* [568] */
    r_lld_evt_rx_fn_t lld_evt_rx;                                                                   /* [569] */
    r_lld_evt_end_isr_fn_t lld_evt_end_isr;                                                         /* [570] */
    r_lld_evt_rx_isr_fn_t lld_evt_rx_isr;                                                           /* [571] */
    r_lld_init_fn_t lld_init;                                                                       /* [572] */
    r_lld_core_reset_fn_t lld_core_reset;                                                           /* [573] */
    r_lld_adv_start_fn_t lld_adv_start;                                                             /* [574] */
    r_lld_adv_stop_hack_fn_t lld_adv_stop_hack;                                                     /* [575] */
    r_lld_scan_start_hack_fn_t lld_scan_start_hack;                                                 /* [576] */
    r_lld_scan_stop_hack_fn_t lld_scan_stop_hack;                                                   /* [577] */
    r_lld_con_start_fn_t lld_con_start;                                                             /* [578] */
    r_lld_move_to_master_hack_fn_t lld_move_to_master_hack;                                         /* [579] */
    r_lld_con_update_req_fn_t lld_con_update_req;                                                   /* [580] */
    r_lld_con_update_after_param_req_fn_t lld_con_update_after_param_req;                           /* [581] */
    r_lld_con_param_rsp_fn_t lld_con_param_rsp;                                                     /* [582] */
    r_lld_con_param_req_fn_t lld_con_param_req;                                                     /* [583] */
    r_lld_con_stop_fn_t lld_con_stop;                                                               /* [584] */
    r_lld_get_mode_fn_t lld_get_mode;                                                               /* [585] */
    r_lld_move_to_slave_hack_fn_t lld_move_to_slave_hack;                                           /* [586] */
    r_lld_ch_map_ind_fn_t lld_ch_map_ind;                                                           /* [587] */
    r_lld_con_update_ind_fn_t lld_con_update_ind;                                                   /* [588] */
    r_lld_crypt_isr_fn_t lld_crypt_isr;                                                             /* [589] */
    r_lld_test_mode_tx_fn_t lld_test_mode_tx;                                                       /* [590] */
    r_lld_test_mode_rx_fn_t lld_test_mode_rx;                                                       /* [591] */
    r_lld_test_stop_fn_t lld_test_stop;                                                             /* [592] */
    r_lld_ral_renew_req_fn_t lld_ral_renew_req;                                                     /* [593] */
    r_lld_pdu_check_fn_t lld_pdu_check;                                                             /* [594] */
    r_lld_pdu_tx_loop_fn_t lld_pdu_tx_loop;                                                         /* [595] */
    r_lld_pdu_data_tx_push_fn_t lld_pdu_data_tx_push;                                               /* [596] */
    r_lld_pdu_data_send_fn_t lld_pdu_data_send;                                                     /* [597] */
    r_lld_pdu_tx_push_fn_t lld_pdu_tx_push;                                                         /* [598] */
    r_lld_pdu_tx_prog_fn_t lld_pdu_tx_prog;                                                         /* [599] */
    r_lld_pdu_send_packet_fn_t lld_pdu_send_packet;                                                 /* [600] */
    r_ram_lld_pdu_tx_flush_fn_t ram_lld_pdu_tx_flush;                                               /* [601] */
    r_lld_pdu_adv_pack_fn_t lld_pdu_adv_pack;                                                       /* [602] */
    r_lld_pdu_rx_handler_fn_t lld_pdu_rx_handler;                                                   /* [603] */
    r_lld_util_instant_get_fn_t lld_util_instant_get;                                               /* [604] */
    r_lld_util_get_bd_address_fn_t lld_util_get_bd_address;                                         /* [605] */
    r_lld_util_set_bd_address_fn_t lld_util_set_bd_address;                                         /* [606] */
    r_lld_util_ral_force_rpa_renew_fn_t lld_util_ral_force_rpa_renew;                               /* [607] */
    r_lld_util_freq2chnl_fn_t lld_util_freq2chnl;                                                   /* [608] */
    r_lld_util_get_local_offset_fn_t lld_util_get_local_offset;                                     /* [609] */
    r_lld_util_get_peer_offset_fn_t lld_util_get_peer_offset;                                       /* [610] */
    r_lld_util_connection_param_set_fn_t lld_util_connection_param_set;                             /* [611] */
    r_lld_util_dle_set_cs_fields_fn_t lld_util_dle_set_cs_fields;                                   /* [612] */
    r_lld_util_anchor_point_move_fn_t lld_util_anchor_point_move;                                   /* [613] */
    r_lld_util_flush_list_fn_t lld_util_flush_list;                                                 /* [614] */
    r_lld_util_instant_ongoing_fn_t lld_util_instant_ongoing;                                       /* [615] */
    r_lld_util_compute_ce_max_fn_t lld_util_compute_ce_max;                                         /* [616] */
    r_lld_util_elt_programmed_fn_t lld_util_elt_programmed;                                         /* [617] */
    r_lld_util_priority_set_fn_t lld_util_priority_set;                                             /* [618] */
    r_lld_util_priority_update_fn_t lld_util_priority_update;                                       /* [619] */
    r_lld_util_get_tx_pkt_cnt_fn_t lld_util_get_tx_pkt_cnt;                                         /* [620] */
    r_lld_util_eff_tx_time_set_hack_fn_t lld_util_eff_tx_time_set_hack;                             /* [621] */
    r_lld_wlcoex_set_fn_t lld_wlcoex_set;                                                           /* [622] */
    r_llm_encryption_done_fn_t llm_encryption_done;                                                 /* [623] */
    r_llm_init_hack_fn_t llm_init_hack;                                                             /* [624] */
    r_llm_wl_clr_fn_t llm_wl_clr;                                                                   /* [625] */
    r_llm_ral_clear_fn_t llm_ral_clear;                                                             /* [626] */
    r_llm_ble_ready_fn_t llm_ble_ready;                                                             /* [627] */
    r_llm_common_cmd_complete_send_fn_t llm_common_cmd_complete_send;                               /* [628] */
    r_llm_con_req_ind_fn_t llm_con_req_ind;                                                         /* [629] */
    r_llm_le_adv_report_ind_fn_t llm_le_adv_report_ind;                                             /* [630] */
    r_llm_con_req_tx_cfm_fn_t llm_con_req_tx_cfm;                                                   /* [631] */
    r_llm_common_cmd_status_send_fn_t llm_common_cmd_status_send;                                   /* [632] */
    r_llm_test_mode_start_tx_fn_t llm_test_mode_start_tx;                                           /* [633] */
    r_llm_test_mode_start_rx_fn_t llm_test_mode_start_rx;                                           /* [634] */
    r_llm_set_adv_param_fn_t llm_set_adv_param;                                                     /* [635] */
    r_llm_set_adv_en_fn_t llm_set_adv_en;                                                           /* [636] */
    r_llm_ral_update_fn_t llm_ral_update;                                                           /* [637] */
    r_llm_set_adv_data_fn_t llm_set_adv_data;                                                       /* [638] */
    r_llm_set_scan_rsp_data_fn_t llm_set_scan_rsp_data;                                             /* [639] */
    r_llm_set_scan_param_fn_t llm_set_scan_param;                                                   /* [640] */
    r_llm_set_scan_en_fn_t llm_set_scan_en;                                                         /* [641] */
    r_llm_wl_dev_add_fn_t llm_wl_dev_add;                                                           /* [642] */
    r_llm_wl_dev_rem_fn_t llm_wl_dev_rem;                                                           /* [643] */
    r_llm_wl_dev_add_hdl_fn_t llm_wl_dev_add_hdl;                                                   /* [644] */
    r_llm_wl_dev_rem_hdl_fn_t llm_wl_dev_rem_hdl;                                                   /* [645] */
    r_llm_create_con_fn_t llm_create_con;                                                           /* [646] */
    r_llm_encryption_start_fn_t llm_encryption_start;                                               /* [647] */
    r_llm_ral_dev_add_fn_t llm_ral_dev_add;                                                         /* [648] */
    r_llm_ral_dev_rm_fn_t llm_ral_dev_rm;                                                           /* [649] */
    r_llm_ral_get_rpa_fn_t llm_ral_get_rpa;                                                         /* [650] */
    r_llm_ral_set_timeout_fn_t llm_ral_set_timeout;                                                 /* [651] */
    r_llm_util_bd_addr_wl_position_fn_t llm_util_bd_addr_wl_position;                               /* [652] */
    r_llm_util_bd_addr_in_wl_fn_t llm_util_bd_addr_in_wl;                                           /* [653] */
    r_llm_util_bl_check_fn_t llm_util_bl_check;                                                     /* [654] */
    r_llm_util_check_address_validity_fn_t llm_util_check_address_validity;                         /* [655] */
    r_llm_util_check_map_validity_fn_t llm_util_check_map_validity;                                 /* [656] */
    r_llm_util_apply_bd_addr_fn_t llm_util_apply_bd_addr;                                           /* [657] */
    r_llm_util_set_public_addr_fn_t llm_util_set_public_addr;                                       /* [658] */
    r_llm_util_check_evt_mask_fn_t llm_util_check_evt_mask;                                         /* [659] */
    r_llm_util_get_channel_map_fn_t llm_util_get_channel_map;                                       /* [660] */
    r_llm_util_get_supp_features_fn_t llm_util_get_supp_features;                                   /* [661] */
    r_llm_util_adv_data_update_fn_t llm_util_adv_data_update;                                       /* [662] */
    r_llm_util_bl_add_fn_t llm_util_bl_add;                                                         /* [663] */
    r_llm_util_bd_addr_in_ral_fn_t llm_util_bd_addr_in_ral;                                         /* [664] */
    r_llm_util_bl_rem_fn_t llm_util_bl_rem;                                                         /* [665] */
    r_llm_end_evt_defer_fn_t llm_end_evt_defer;                                                     /* [666] */
    r_llm_pdu_defer_fn_t llm_pdu_defer;                                                             /* [667] */
    r_lm_init_wrap_fn_t lm_init_wrap;                                                               /* [668] */
    r_lm_lt_addr_alloc_fn_t lm_lt_addr_alloc;                                                       /* [669] */
    r_lm_lt_addr_reserve_fn_t lm_lt_addr_reserve;                                                   /* [670] */
    r_lm_lt_addr_free_fn_t lm_lt_addr_free;                                                         /* [671] */
    r_lm_get_nb_acl_fn_t lm_get_nb_acl;                                                             /* [672] */
    r_lm_role_switch_start_fn_t lm_role_switch_start;                                               /* [673] */
    r_lm_role_switch_finished_fn_t lm_role_switch_finished;                                         /* [674] */
    r_lm_read_features_fn_t lm_read_features;                                                       /* [675] */
    r_lm_acl_disc_fn_t lm_acl_disc;                                                                 /* [676] */
    r_lm_get_auth_en_fn_t lm_get_auth_en;                                                           /* [677] */
    r_lm_get_sp_en_fn_t lm_get_sp_en;                                                               /* [678] */
    r_lm_get_sec_con_host_supp_fn_t lm_get_sec_con_host_supp;                                       /* [679] */
    r_LM_GetPINType_fn_t LM_GetPINType;                                                             /* [680] */
    r_LM_GetConnectionAcceptTimeout_fn_t LM_GetConnectionAcceptTimeout;                             /* [681] */
    r_LM_GetLocalNameSeg_fn_t LM_GetLocalNameSeg;                                                   /* [682] */
    r_lm_get_loopback_mode_fn_t lm_get_loopback_mode;                                               /* [683] */
    r_lm_get_pub_key_192_fn_t lm_get_pub_key_192;                                                   /* [684] */
    r_lm_get_priv_key_192_fn_t lm_get_priv_key_192;                                                 /* [685] */
    r_lm_get_pub_key_256_fn_t lm_get_pub_key_256;                                                   /* [686] */
    r_lm_get_priv_key_256_fn_t lm_get_priv_key_256;                                                 /* [687] */
    r_lm_get_oob_local_data_192_fn_t lm_get_oob_local_data_192;                                     /* [688] */
    r_lm_get_oob_local_data_256_fn_t lm_get_oob_local_data_256;                                     /* [689] */
    r_lm_get_oob_local_commit_fn_t lm_get_oob_local_commit;                                         /* [690] */
    r_lm_sp_debug_mode_get_fn_t lm_sp_debug_mode_get;                                               /* [691] */
    r_lm_debug_key_compare_192_fn_t lm_debug_key_compare_192;                                       /* [692] */
    r_lm_debug_key_compare_256_fn_t lm_debug_key_compare_256;                                       /* [693] */
    r_lm_dut_mode_en_get_fn_t lm_dut_mode_en_get;                                                   /* [694] */
    r_lm_afh_ch_ass_en_get_fn_t lm_afh_ch_ass_en_get;                                               /* [695] */
    r_lm_sync_flow_ctrl_en_get_fn_t lm_sync_flow_ctrl_en_get;                                       /* [696] */
    r_lm_afh_host_ch_class_get_fn_t lm_afh_host_ch_class_get;                                       /* [697] */
    r_lm_afh_peer_ch_class_set_fn_t lm_afh_peer_ch_class_set;                                       /* [698] */
    r_lm_afh_master_ch_map_get_fn_t lm_afh_master_ch_map_get;                                       /* [699] */
    r_lm_afh_activate_timer_fn_t lm_afh_activate_timer;                                             /* [700] */
    r_lm_is_acl_con_fn_t lm_is_acl_con;                                                             /* [701] */
    r_lm_is_acl_con_role_fn_t lm_is_acl_con_role;                                                   /* [702] */
    r_lm_is_clk_adj_instant_pending_fn_t lm_is_clk_adj_instant_pending;                             /* [703] */
    r_lm_clk_adj_instant_pending_set_fn_t lm_clk_adj_instant_pending_set;                           /* [704] */
    r_lm_is_clk_adj_ack_pending_fn_t lm_is_clk_adj_ack_pending;                                     /* [705] */
    r_lm_num_clk_adj_ack_pending_set_fn_t lm_num_clk_adj_ack_pending_set;                           /* [706] */
    r_lm_clk_adj_ack_pending_clear_fn_t lm_clk_adj_ack_pending_clear;                               /* [707] */
    r_lm_local_ext_fr_configured_fn_t lm_local_ext_fr_configured;                                   /* [708] */
    r_lm_pca_sscan_link_set_fn_t lm_pca_sscan_link_set;                                             /* [709] */
    r_lm_pca_sscan_link_get_fn_t lm_pca_sscan_link_get;                                             /* [710] */
    r_lm_get_sync_param_fn_t lm_get_sync_param;                                                     /* [711] */
    r_lm_init_sync_fn_t lm_init_sync;                                                               /* [712] */
    r_lm_reset_sync_fn_t lm_reset_sync;                                                             /* [713] */
    r_lm_check_active_sync_fn_t lm_check_active_sync;                                               /* [714] */
    r_lm_add_sync_fn_t lm_add_sync;                                                                 /* [715] */
    r_lm_modif_sync_fn_t lm_modif_sync;                                                             /* [716] */
    r_lm_check_sync_hl_rsp_fn_t lm_check_sync_hl_rsp;                                               /* [717] */
    r_lm_get_synchdl_fn_t lm_get_synchdl;                                                           /* [718] */
    r_lm_look_for_sync_fn_t lm_look_for_sync;                                                       /* [719] */
    r_lm_get_nb_sync_link_fn_t lm_get_nb_sync_link;                                                 /* [720] */
    r_lm_get_min_sync_intv_fn_t lm_get_min_sync_intv;                                               /* [721] */
    r_lm_remove_sync_fn_t lm_remove_sync;                                                           /* [722] */
    r_lm_sco_nego_end_fn_t lm_sco_nego_end;                                                         /* [723] */
    r_lm_master_clk_adj_req_handler_fn_t lm_master_clk_adj_req_handler;                             /* [724] */
    r_rwble_init_fn_t rwble_init;                                                                   /* [725] */
    r_rwble_reset_fn_t rwble_reset;                                                                 /* [726] */
    r_rwble_sleep_check_fn_t rwble_sleep_check;                                                     /* [727] */
    r_rwble_activity_ongoing_check_fn_t rwble_activity_ongoing_check;                               /* [728] */
    r_rwble_version_fn_t rwble_version;                                                             /* [729] */
    r_rwble_isr_fn_t rwble_isr;                                                                     /* [730] */
    r_rwbt_init_fn_t rwbt_init;                                                                     /* [731] */
    r_rwbt_reset_fn_t rwbt_reset;                                                                   /* [732] */
    r_rwbt_version_fn_t rwbt_version;                                                               /* [733] */
    r_rwbt_isr_fn_t rwbt_isr;                                                                       /* [734] */
    r_rwbt_sleep_check_fn_t rwbt_sleep_check;                                                       /* [735] */
    r_rwbt_sleep_enter_fn_t rwbt_sleep_enter;                                                       /* [736] */
    r_rwbt_sleep_wakeup_fn_t rwbt_sleep_wakeup;                                                     /* [737] */
    r_rwbt_sleep_wakeup_end_fn_t rwbt_sleep_wakeup_end;                                             /* [738] */
    r_btdm_lpcycles_2_us_fn_t btdm_lpcycles_2_us;                                                   /* [739] */
    r_btdm_us_2_lpcycles_fn_t btdm_us_2_lpcycles;                                                   /* [740] */
};

/**
 * Pointer to the RivieraWaves IP functions table.
 *
 * The linker resolves the symbol location against libbtdm_app.a with a fallback
 * on esp32.rom.ld to the internal ROM position.
 */
extern struct r_ip_funcs *r_ip_funcs_p;

#endif /* R_IP_FUNCS_H */
