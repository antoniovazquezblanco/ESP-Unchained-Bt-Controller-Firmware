/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Typed declarations for the RivieraWaves "modules functions" dispatch table
 * (r_modules_funcs) of the ESP32 classic (BR/EDR) Bluetooth controller ROM.
 * Sibling of r_ip_funcs; holds the kernel (r_ke_*) and common (r_co_*) helpers.
 */

#ifndef R_MODULES_FUNCS_H
#define R_MODULES_FUNCS_H

#include <stdbool.h>
#include <stdint.h>

/**
 * r_co_list_init, slot 0 of the r_modules_funcs table.
 * Common: list init.
 */
typedef void (*r_co_list_init_fn_t)(uint32_t *);

/**
 * r_co_list_pool_init, slot 1 of the r_modules_funcs table.
 * Common: list pool init.
 */
typedef void (*r_co_list_pool_init_fn_t)(int32_t *, uint32_t *, uint32_t, int32_t, void *, uint8_t);

/**
 * r_co_list_push_back, slot 2 of the r_modules_funcs table.
 * Common: list push back.
 */
typedef void (*r_co_list_push_back_fn_t)(int32_t *, uint32_t *);

/**
 * r_co_list_push_front, slot 3 of the r_modules_funcs table.
 * Common: list push front.
 */
typedef void (*r_co_list_push_front_fn_t)(int32_t *, int32_t *);

/**
 * r_co_list_pop_front, slot 4 of the r_modules_funcs table.
 * Common: list pop front.
 */
typedef void (*r_co_list_pop_front_fn_t)(int32_t *);

/**
 * r_co_list_extract, slot 5 of the r_modules_funcs table.
 * Common: list extract.
 */
typedef uint32_t (*r_co_list_extract_fn_t)(int32_t *, int32_t *, uint32_t);

/**
 * r_co_list_extract_after, slot 6 of the r_modules_funcs table.
 * Common: list extract after.
 */
typedef void (*r_co_list_extract_after_fn_t)(int32_t *, int32_t *, int32_t *);

/**
 * r_co_list_find, slot 7 of the r_modules_funcs table.
 * Common: list find.
 */
typedef bool (*r_co_list_find_fn_t)(uint32_t *, uint32_t *);

/**
 * r_co_list_merge, slot 8 of the r_modules_funcs table.
 * Common: list merge.
 */
typedef void (*r_co_list_merge_fn_t)(int32_t *, int32_t *);

/**
 * r_co_list_insert_before, slot 9 of the r_modules_funcs table.
 * Common: list insert before.
 */
typedef void (*r_co_list_insert_before_fn_t)(uint32_t *, int32_t *, uint32_t *);

/**
 * r_co_list_insert_after, slot 10 of the r_modules_funcs table.
 * Common: list insert after.
 */
typedef void (*r_co_list_insert_after_fn_t)(uint32_t *, int32_t *, int32_t *);

/**
 * r_co_list_size, slot 11 of the r_modules_funcs table.
 * Common: list size.
 */
typedef int16_t (*r_co_list_size_fn_t)(uint32_t *);

/**
 * r_co_list_check_size_available, slot 12 of the r_modules_funcs table.
 * Common: list check size available.
 */
typedef uint32_t (*r_co_list_check_size_available_fn_t)(uint32_t *, uint16_t);

/**
 * r_co_bytes_to_string, slot 13 of the r_modules_funcs table.
 * Common: bytes to string.
 */
typedef void (*r_co_bytes_to_string_fn_t)(uint8_t *, int32_t, uint32_t);

/**
 * r_co_bdaddr_compare, slot 14 of the r_modules_funcs table.
 * Common: Bluetooth address compare.
 */
typedef uint32_t (*r_co_bdaddr_compare_fn_t)(int32_t, int32_t);

/**
 * r_co_slot_to_duration, slot 15 of the r_modules_funcs table.
 * Common: slot to duration.
 */
typedef uint32_t (*r_co_slot_to_duration_fn_t)(uint32_t);

/**
 * r_co_nb_good_channels, slot 16 of the r_modules_funcs table.
 * Common: number of good channels.
 */
typedef uint8_t (*r_co_nb_good_channels_fn_t)(int32_t);

/**
 * r_dbg_init, slot 17 of the r_modules_funcs table.
 * Debug: init.
 */
typedef void (*r_dbg_init_fn_t)(void);

/**
 * r_dbg_platform_reset_complete, slot 18 of the r_modules_funcs table.
 * Debug: platform reset complete.
 */
typedef void (*r_dbg_platform_reset_complete_fn_t)(int32_t);

/**
 * r_dbg_swdiag_init, slot 19 of the r_modules_funcs table.
 * Debug: software diagnostics init.
 */
typedef void (*r_dbg_swdiag_init_fn_t)(void);

/**
 * r_dbg_swdiag_read, slot 20 of the r_modules_funcs table.
 * Debug: software diagnostics read.
 */
typedef uint32_t (*r_dbg_swdiag_read_fn_t)(void);

/**
 * r_dbg_swdiag_write, slot 21 of the r_modules_funcs table.
 * Debug: software diagnostics write.
 */
typedef void (*r_dbg_swdiag_write_fn_t)(uint32_t);

/**
 * r_ecc_init, slot 22 of the r_modules_funcs table.
 * ECC: init.
 */
typedef void (*r_ecc_init_fn_t)(uint8_t);

/**
 * r_ecc_generate_key256, slot 23 of the r_modules_funcs table.
 * ECC: generate 256-bit key.
 */
typedef void (*r_ecc_generate_key256_fn_t)(int32_t, int32_t, int32_t, uint16_t, uint32_t);

/**
 * r_ecc_abort_key256_generation, slot 24 of the r_modules_funcs table.
 * ECC: abort 256-bit key generation.
 */
typedef void (*r_ecc_abort_key256_generation_fn_t)(int16_t);

/**
 * r_ecc_gen_new_public_key, slot 25 of the r_modules_funcs table.
 * ECC: generate new public key.
 */
typedef void (*r_ecc_gen_new_public_key_fn_t)(uint32_t, uint16_t, uint16_t);

/**
 * r_ecc_gen_new_secret_key, slot 26 of the r_modules_funcs table.
 * ECC: generate new secret key.
 */
typedef void (*r_ecc_gen_new_secret_key_fn_t)(uint8_t *, uint8_t);

/**
 * r_ecc_get_debug_Keys, slot 27 of the r_modules_funcs table.
 * ECC: get debug Keys.
 */
typedef void (*r_ecc_get_debug_Keys_fn_t)(int32_t, int32_t, int32_t);

/**
 * r_h4tl_init, slot 28 of the r_modules_funcs table.
 * H4 transport: init.
 */
typedef void (*r_h4tl_init_fn_t)(uint8_t, int32_t);

/**
 * r_h4tl_write, slot 29 of the r_modules_funcs table.
 * H4 transport: write.
 */
typedef void (*r_h4tl_write_fn_t)(uint8_t, int32_t, uint32_t, uint32_t);

/**
 * r_h4tl_start, slot 30 of the r_modules_funcs table.
 * H4 transport: start.
 */
typedef void (*r_h4tl_start_fn_t)(void);

/**
 * r_h4tl_stop, slot 31 of the r_modules_funcs table.
 * H4 transport: stop.
 */
typedef void (*r_h4tl_stop_fn_t)(void);

/**
 * r_ke_event_init, slot 32 of the r_modules_funcs table.
 * Kernel: event init.
 */
typedef void (*r_ke_event_init_fn_t)(void);

/**
 * r_ke_event_callback_set, slot 33 of the r_modules_funcs table.
 * Kernel: event callback set.
 */
typedef uint32_t (*r_ke_event_callback_set_fn_t)(uint32_t, int32_t);

/**
 * r_ke_event_set, slot 34 of the r_modules_funcs table.
 * Kernel: event set.
 */
typedef void (*r_ke_event_set_fn_t)(uint32_t);

/**
 * r_ke_event_clear, slot 35 of the r_modules_funcs table.
 * Kernel: event clear.
 */
typedef void (*r_ke_event_clear_fn_t)(uint32_t);

/**
 * r_ke_event_get, slot 36 of the r_modules_funcs table.
 * Kernel: event get.
 */
typedef uint32_t (*r_ke_event_get_fn_t)(uint32_t);

/**
 * r_ke_event_get_all, slot 37 of the r_modules_funcs table.
 * Kernel: event get all.
 */
typedef uint32_t (*r_ke_event_get_all_fn_t)(void);

/**
 * r_ke_event_flush, slot 38 of the r_modules_funcs table.
 * Kernel: event flush.
 */
typedef void (*r_ke_event_flush_fn_t)(void);

/**
 * r_ke_event_schedule, slot 39 of the r_modules_funcs table.
 * Kernel: event schedule.
 */
typedef void (*r_ke_event_schedule_fn_t)(void);

/**
 * r_ke_mem_init, slot 40 of the r_modules_funcs table.
 * Kernel: memory init.
 */
typedef void (*r_ke_mem_init_fn_t)(uint32_t, int32_t, int16_t);

/**
 * r_ke_mem_is_empty, slot 41 of the r_modules_funcs table.
 * Kernel: memory is empty.
 */
typedef bool (*r_ke_mem_is_empty_fn_t)(uint32_t);

/**
 * r_ke_check_malloc, slot 42 of the r_modules_funcs table.
 * Kernel: check malloc.
 */
typedef bool (*r_ke_check_malloc_fn_t)(int32_t, uint32_t);

/**
 * r_ke_malloc, slot 43 of the r_modules_funcs table.
 * Kernel: malloc.
 */
typedef int16_t *(*r_ke_malloc_fn_t)(int32_t, uint32_t);

/**
 * r_ke_free, slot 44 of the r_modules_funcs table.
 * Kernel: free.
 */
typedef void (*r_ke_free_fn_t)(int16_t *);

/**
 * r_ke_is_free, slot 45 of the r_modules_funcs table.
 * Kernel: is free.
 */
typedef bool (*r_ke_is_free_fn_t)(int32_t);

/**
 * r_ke_get_mem_usage, slot 46 of the r_modules_funcs table.
 * Kernel: get memory usage.
 */
typedef uint16_t (*r_ke_get_mem_usage_fn_t)(uint32_t);

/**
 * r_ke_get_max_mem_usage, slot 47 of the r_modules_funcs table.
 * Kernel: get max memory usage.
 */
typedef uint32_t (*r_ke_get_max_mem_usage_fn_t)(void);

/**
 * r_ke_msg_alloc, slot 48 of the r_modules_funcs table.
 * Allocates a kernel message from the heap: a 12-byte ke_msg header followed by
 * param_len bytes of parameters. Fills the header (list-next sentinel 0xFFFFFFFF,
 * id, dest_id, src_id, param_len) and zero-fills the parameter area. Returns a
 * pointer to that parameter area (header + 12); the send/free helpers recover the
 * header via negative offsets, so hand this pointer to ke_msg_send() or
 * hci_send_2_host() rather than freeing it directly.
 *
 * @param id        message id (ke_msg_id_t).
 * @param dest_id   destination task id (ke_task_id_t).
 * @param src_id    source task id (ke_task_id_t).
 * @param param_len size in bytes of the parameter area to allocate.
 * @return pointer to the zero-filled parameter area (ke_msg header + 12 bytes).
 */
typedef void *(*r_ke_msg_alloc_fn_t)(uint16_t id, uint16_t dest_id, uint16_t src_id, uint16_t param_len);

/**
 * r_ke_msg_send, slot 49 of the r_modules_funcs table.
 * Kernel: message send.
 */
typedef void (*r_ke_msg_send_fn_t)(int32_t);

/**
 * r_ke_msg_send_basic, slot 50 of the r_modules_funcs table.
 * Kernel: message send basic.
 */
typedef void (*r_ke_msg_send_basic_fn_t)(uint16_t, uint16_t, uint16_t);

/**
 * r_ke_msg_forward, slot 51 of the r_modules_funcs table.
 * Kernel: message forward.
 */
typedef void (*r_ke_msg_forward_fn_t)(int32_t, uint16_t, uint16_t);

/**
 * r_ke_msg_forward_new_id, slot 52 of the r_modules_funcs table.
 * Kernel: message forward new id.
 */
typedef void (*r_ke_msg_forward_new_id_fn_t)(int32_t, uint16_t, uint16_t, uint16_t);

/**
 * r_ke_msg_free, slot 53 of the r_modules_funcs table.
 * Kernel: message free.
 */
typedef void (*r_ke_msg_free_fn_t)(uint32_t);

/**
 * r_ke_msg_dest_id_get, slot 54 of the r_modules_funcs table.
 * Kernel: message dest id get.
 */
typedef uint16_t (*r_ke_msg_dest_id_get_fn_t)(int32_t);

/**
 * r_ke_msg_src_id_get, slot 55 of the r_modules_funcs table.
 * Kernel: message src id get.
 */
typedef uint16_t (*r_ke_msg_src_id_get_fn_t)(int32_t);

/**
 * r_ke_msg_in_queue, slot 56 of the r_modules_funcs table.
 * Kernel: message in queue.
 */
typedef bool (*r_ke_msg_in_queue_fn_t)(int32_t);

/**
 * r_ke_init, slot 57 of the r_modules_funcs table.
 * Kernel: init.
 */
typedef void (*r_ke_init_fn_t)(void);

/**
 * r_ke_flush, slot 58 of the r_modules_funcs table.
 * Kernel: flush.
 */
typedef void (*r_ke_flush_fn_t)(void);

/**
 * r_ke_sleep_check, slot 59 of the r_modules_funcs table.
 * Kernel: sleep check.
 */
typedef bool (*r_ke_sleep_check_fn_t)(void);

/**
 * r_ke_stats_get, slot 60 of the r_modules_funcs table.
 * Kernel: stats get.
 */
typedef uint32_t (*r_ke_stats_get_fn_t)(uint8_t *, uint8_t *, uint8_t *, uint16_t *);

/**
 * r_ke_queue_extract, slot 61 of the r_modules_funcs table.
 * Kernel: queue extract.
 */
typedef void (*r_ke_queue_extract_fn_t)(int32_t *, uint8_t *, uint32_t);

/**
 * r_ke_queue_insert, slot 62 of the r_modules_funcs table.
 * Kernel: queue insert.
 */
typedef void (*r_ke_queue_insert_fn_t)(uint32_t *, uint32_t *, uint8_t *);

/**
 * r_ke_task_init, slot 63 of the r_modules_funcs table.
 * Kernel: task init.
 */
typedef void (*r_ke_task_init_fn_t)(void);

/**
 * r_ke_task_create, slot 64 of the r_modules_funcs table.
 * Kernel: task create.
 */
typedef uint32_t (*r_ke_task_create_fn_t)(uint32_t, int32_t);

/**
 * r_ke_task_delete, slot 65 of the r_modules_funcs table.
 * Kernel: task delete.
 */
typedef uint32_t (*r_ke_task_delete_fn_t)(uint32_t);

/**
 * r_ke_state_set, slot 66 of the r_modules_funcs table.
 * Kernel: state set.
 */
typedef void (*r_ke_state_set_fn_t)(uint32_t, uint8_t);

/**
 * r_ke_state_get, slot 67 of the r_modules_funcs table.
 * Kernel: state get.
 */
typedef uint8_t (*r_ke_state_get_fn_t)(uint32_t);

/**
 * r_ke_msg_discard, slot 68 of the r_modules_funcs table.
 * Kernel: message discard.
 */
typedef uint32_t (*r_ke_msg_discard_fn_t)(void);

/**
 * r_ke_msg_save, slot 69 of the r_modules_funcs table.
 * Kernel: message save.
 */
typedef uint32_t (*r_ke_msg_save_fn_t)(void);

/**
 * r_ke_task_msg_flush, slot 70 of the r_modules_funcs table.
 * Kernel: task message flush.
 */
typedef void (*r_ke_task_msg_flush_fn_t)(uint16_t);

/**
 * r_ke_task_check, slot 71 of the r_modules_funcs table.
 * Kernel: task check.
 */
typedef uint32_t (*r_ke_task_check_fn_t)(uint32_t);

/**
 * r_ke_timer_init_hack, slot 72 of the r_modules_funcs table.
 * Kernel: timer init (Espressif override).
 */
typedef void (*r_ke_timer_init_hack_fn_t)(void);

/**
 * r_ke_timer_set_hack, slot 73 of the r_modules_funcs table.
 * Kernel: timer set (Espressif override).
 */
typedef void (*r_ke_timer_set_hack_fn_t)(int16_t, int16_t, uint32_t);

/**
 * r_ke_timer_clear_hack, slot 74 of the r_modules_funcs table.
 * Kernel: timer clear (Espressif override).
 */
typedef void (*r_ke_timer_clear_hack_fn_t)(uint16_t, uint32_t);

/**
 * r_ke_timer_active, slot 75 of the r_modules_funcs table.
 * Kernel: timer active.
 */
typedef uint32_t (*r_ke_timer_active_fn_t)(int16_t, int16_t);

/**
 * r_ke_timer_adjust_all, slot 76 of the r_modules_funcs table.
 * Kernel: timer adjust all.
 */
typedef void (*r_ke_timer_adjust_all_fn_t)(int32_t);

/**
 * r_ke_timer_sleep_check, slot 77 of the r_modules_funcs table.
 * Kernel: timer sleep check.
 */
typedef uint32_t (*r_ke_timer_sleep_check_fn_t)(uint32_t *, uint32_t);

/**
 * r_nvds_init, slot 78 of the r_modules_funcs table.
 * NVDS: init.
 */
typedef uint32_t (*r_nvds_init_fn_t)(uint32_t, uint32_t);

/**
 * r_nvds_get, slot 79 of the r_modules_funcs table.
 * NVDS: get.
 */
typedef int32_t (*r_nvds_get_fn_t)(uint32_t, uint8_t *, uint32_t);

/**
 * r_nvds_del, slot 80 of the r_modules_funcs table.
 * NVDS: del.
 */
typedef int32_t (*r_nvds_del_fn_t)(uint32_t);

/**
 * r_nvds_lock, slot 81 of the r_modules_funcs table.
 * NVDS: lock.
 */
typedef int32_t (*r_nvds_lock_fn_t)(uint32_t);

/**
 * r_nvds_put, slot 82 of the r_modules_funcs table.
 * NVDS: put.
 */
typedef uint32_t (*r_nvds_put_fn_t)(uint32_t, uint8_t, void *);

/**
 * r_rf_rw_bt_init, slot 83 of the r_modules_funcs table.
 * RF: rw bt init.
 */
typedef void (*r_rf_rw_bt_init_fn_t)(void);

/**
 * r_rf_rw_le_init, slot 84 of the r_modules_funcs table.
 * RF: rw le init.
 */
typedef void (*r_rf_rw_le_init_fn_t)(void);

/**
 * r_rf_rw_init, slot 85 of the r_modules_funcs table.
 * RF: rw init.
 */
typedef void (*r_rf_rw_init_fn_t)(uint32_t *);

/**
 * r_rwip_check_wakeup_boundary, slot 86 of the r_modules_funcs table.
 * RW IP core: check wakeup boundary.
 */
typedef uint32_t (*r_rwip_check_wakeup_boundary_fn_t)(void);

/**
 * r_rwip_init, slot 87 of the r_modules_funcs table.
 * RW IP core: init.
 */
typedef void (*r_rwip_init_fn_t)(int32_t);

/**
 * r_rwip_reset_ext, slot 88 of the r_modules_funcs table.
 * RW IP core: reset ext.
 * Parameters not recovered (no ROM decompilation target).
 */
typedef void (*r_rwip_reset_ext_fn_t)(void);

/**
 * r_rwip_version, slot 89 of the r_modules_funcs table.
 * RW IP core: version.
 */
typedef void (*r_rwip_version_fn_t)(uint32_t, uint32_t);

/**
 * r_rwip_schedule, slot 90 of the r_modules_funcs table.
 * RW IP core: schedule.
 */
typedef void (*r_rwip_schedule_fn_t)(void);

/**
 * r_rwip_sleep, slot 91 of the r_modules_funcs table.
 * RW IP core: sleep.
 */
typedef uint32_t (*r_rwip_sleep_fn_t)(void);

/**
 * r_rwip_wakeup, slot 92 of the r_modules_funcs table.
 * RW IP core: wakeup.
 */
typedef void (*r_rwip_wakeup_fn_t)(void);

/**
 * r_rwip_prevent_sleep_set, slot 93 of the r_modules_funcs table.
 * RW IP core: prevent sleep set.
 */
typedef void (*r_rwip_prevent_sleep_set_fn_t)(uint32_t);

/**
 * r_rwip_wakeup_end, slot 94 of the r_modules_funcs table.
 * RW IP core: wakeup end.
 */
typedef void (*r_rwip_wakeup_end_fn_t)(void);

/**
 * r_rwip_prevent_sleep_clear, slot 95 of the r_modules_funcs table.
 * RW IP core: prevent sleep clear.
 */
typedef void (*r_rwip_prevent_sleep_clear_fn_t)(uint32_t);

/**
 * r_rwip_wakeup_delay_set, slot 96 of the r_modules_funcs table.
 * RW IP core: wakeup delay set.
 */
typedef void (*r_rwip_wakeup_delay_set_fn_t)(uint32_t);

/**
 * r_rwip_sleep_enable, slot 97 of the r_modules_funcs table.
 * RW IP core: sleep enable.
 */
typedef uint8_t (*r_rwip_sleep_enable_fn_t)(void);

/**
 * r_rwip_ext_wakeup_enable, slot 98 of the r_modules_funcs table.
 * RW IP core: ext wakeup enable.
 */
typedef uint8_t (*r_rwip_ext_wakeup_enable_fn_t)(void);

/**
 * r_rwip_pca_clock_dragging_only, slot 99 of the r_modules_funcs table.
 * RW IP core: pca clock dragging only.
 */
typedef uint32_t (*r_rwip_pca_clock_dragging_only_fn_t)(void);

/**
 * r_rwip_wlcoex_set, slot 100 of the r_modules_funcs table.
 * RW IP core: wlcoex set.
 */
typedef void (*r_rwip_wlcoex_set_fn_t)(uint8_t);

/**
 * r_rwip_assert_err, slot 101 of the r_modules_funcs table.
 * RW IP core: assert err.
 */
typedef void (*r_rwip_assert_err_fn_t)(uint8_t *, uint32_t, uint32_t, uint32_t);

/**
 * srand_wrapper, slot 102 of the r_modules_funcs table.
 * Srand (libbtdm wrapper).
 * Parameters not recovered (no ROM decompilation target).
 */
typedef void (*r_srand_fn_t)(void);

/**
 * rand_wrapper, slot 103 of the r_modules_funcs table.
 * Random (libbtdm wrapper).
 * Parameters not recovered (no ROM decompilation target).
 */
typedef void (*r_rand_fn_t)(void);

/**
 * RivieraWaves "modules functions" dispatch table. Same indirection mechanism
 * as r_ip_funcs (see r_ip_funcs.h): the stack calls these via the table so they
 * can be patched at runtime.
 */
struct r_modules_funcs
{
    r_co_list_init_fn_t co_list_init;                                 /* [0] */
    r_co_list_pool_init_fn_t co_list_pool_init;                       /* [1] */
    r_co_list_push_back_fn_t co_list_push_back;                       /* [2] */
    r_co_list_push_front_fn_t co_list_push_front;                     /* [3] */
    r_co_list_pop_front_fn_t co_list_pop_front;                       /* [4] */
    r_co_list_extract_fn_t co_list_extract;                           /* [5] */
    r_co_list_extract_after_fn_t co_list_extract_after;               /* [6] */
    r_co_list_find_fn_t co_list_find;                                 /* [7] */
    r_co_list_merge_fn_t co_list_merge;                               /* [8] */
    r_co_list_insert_before_fn_t co_list_insert_before;               /* [9] */
    r_co_list_insert_after_fn_t co_list_insert_after;                 /* [10] */
    r_co_list_size_fn_t co_list_size;                                 /* [11] */
    r_co_list_check_size_available_fn_t co_list_check_size_available; /* [12] */
    r_co_bytes_to_string_fn_t co_bytes_to_string;                     /* [13] */
    r_co_bdaddr_compare_fn_t co_bdaddr_compare;                       /* [14] */
    r_co_slot_to_duration_fn_t co_slot_to_duration;                   /* [15] */
    r_co_nb_good_channels_fn_t co_nb_good_channels;                   /* [16] */
    r_dbg_init_fn_t dbg_init;                                         /* [17] */
    r_dbg_platform_reset_complete_fn_t dbg_platform_reset_complete;   /* [18] */
    r_dbg_swdiag_init_fn_t dbg_swdiag_init;                           /* [19] */
    r_dbg_swdiag_read_fn_t dbg_swdiag_read;                           /* [20] */
    r_dbg_swdiag_write_fn_t dbg_swdiag_write;                         /* [21] */
    r_ecc_init_fn_t ecc_init;                                         /* [22] */
    r_ecc_generate_key256_fn_t ecc_generate_key256;                   /* [23] */
    r_ecc_abort_key256_generation_fn_t ecc_abort_key256_generation;   /* [24] */
    r_ecc_gen_new_public_key_fn_t ecc_gen_new_public_key;             /* [25] */
    r_ecc_gen_new_secret_key_fn_t ecc_gen_new_secret_key;             /* [26] */
    r_ecc_get_debug_Keys_fn_t ecc_get_debug_Keys;                     /* [27] */
    r_h4tl_init_fn_t h4tl_init;                                       /* [28] */
    r_h4tl_write_fn_t h4tl_write;                                     /* [29] */
    r_h4tl_start_fn_t h4tl_start;                                     /* [30] */
    r_h4tl_stop_fn_t h4tl_stop;                                       /* [31] */
    r_ke_event_init_fn_t ke_event_init;                               /* [32] */
    r_ke_event_callback_set_fn_t ke_event_callback_set;               /* [33] */
    r_ke_event_set_fn_t ke_event_set;                                 /* [34] */
    r_ke_event_clear_fn_t ke_event_clear;                             /* [35] */
    r_ke_event_get_fn_t ke_event_get;                                 /* [36] */
    r_ke_event_get_all_fn_t ke_event_get_all;                         /* [37] */
    r_ke_event_flush_fn_t ke_event_flush;                             /* [38] */
    r_ke_event_schedule_fn_t ke_event_schedule;                       /* [39] */
    r_ke_mem_init_fn_t ke_mem_init;                                   /* [40] */
    r_ke_mem_is_empty_fn_t ke_mem_is_empty;                           /* [41] */
    r_ke_check_malloc_fn_t ke_check_malloc;                           /* [42] */
    r_ke_malloc_fn_t ke_malloc;                                       /* [43] */
    r_ke_free_fn_t ke_free;                                           /* [44] */
    r_ke_is_free_fn_t ke_is_free;                                     /* [45] */
    r_ke_get_mem_usage_fn_t ke_get_mem_usage;                         /* [46] */
    r_ke_get_max_mem_usage_fn_t ke_get_max_mem_usage;                 /* [47] */
    r_ke_msg_alloc_fn_t ke_msg_alloc;                                 /* [48] */
    r_ke_msg_send_fn_t ke_msg_send;                                   /* [49] */
    r_ke_msg_send_basic_fn_t ke_msg_send_basic;                       /* [50] */
    r_ke_msg_forward_fn_t ke_msg_forward;                             /* [51] */
    r_ke_msg_forward_new_id_fn_t ke_msg_forward_new_id;               /* [52] */
    r_ke_msg_free_fn_t ke_msg_free;                                   /* [53] */
    r_ke_msg_dest_id_get_fn_t ke_msg_dest_id_get;                     /* [54] */
    r_ke_msg_src_id_get_fn_t ke_msg_src_id_get;                       /* [55] */
    r_ke_msg_in_queue_fn_t ke_msg_in_queue;                           /* [56] */
    r_ke_init_fn_t ke_init;                                           /* [57] */
    r_ke_flush_fn_t ke_flush;                                         /* [58] */
    r_ke_sleep_check_fn_t ke_sleep_check;                             /* [59] */
    r_ke_stats_get_fn_t ke_stats_get;                                 /* [60] */
    r_ke_queue_extract_fn_t ke_queue_extract;                         /* [61] */
    r_ke_queue_insert_fn_t ke_queue_insert;                           /* [62] */
    r_ke_task_init_fn_t ke_task_init;                                 /* [63] */
    r_ke_task_create_fn_t ke_task_create;                             /* [64] */
    r_ke_task_delete_fn_t ke_task_delete;                             /* [65] */
    r_ke_state_set_fn_t ke_state_set;                                 /* [66] */
    r_ke_state_get_fn_t ke_state_get;                                 /* [67] */
    r_ke_msg_discard_fn_t ke_msg_discard;                             /* [68] */
    r_ke_msg_save_fn_t ke_msg_save;                                   /* [69] */
    r_ke_task_msg_flush_fn_t ke_task_msg_flush;                       /* [70] */
    r_ke_task_check_fn_t ke_task_check;                               /* [71] */
    r_ke_timer_init_hack_fn_t ke_timer_init_hack;                     /* [72] */
    r_ke_timer_set_hack_fn_t ke_timer_set_hack;                       /* [73] */
    r_ke_timer_clear_hack_fn_t ke_timer_clear_hack;                   /* [74] */
    r_ke_timer_active_fn_t ke_timer_active;                           /* [75] */
    r_ke_timer_adjust_all_fn_t ke_timer_adjust_all;                   /* [76] */
    r_ke_timer_sleep_check_fn_t ke_timer_sleep_check;                 /* [77] */
    r_nvds_init_fn_t nvds_init;                                       /* [78] */
    r_nvds_get_fn_t nvds_get;                                         /* [79] */
    r_nvds_del_fn_t nvds_del;                                         /* [80] */
    r_nvds_lock_fn_t nvds_lock;                                       /* [81] */
    r_nvds_put_fn_t nvds_put;                                         /* [82] */
    r_rf_rw_bt_init_fn_t rf_rw_bt_init;                               /* [83] */
    r_rf_rw_le_init_fn_t rf_rw_le_init;                               /* [84] */
    r_rf_rw_init_fn_t rf_rw_init;                                     /* [85] */
    r_rwip_check_wakeup_boundary_fn_t rwip_check_wakeup_boundary;     /* [86] */
    r_rwip_init_fn_t rwip_init;                                       /* [87] */
    r_rwip_reset_ext_fn_t rwip_reset_ext;                             /* [88] */
    r_rwip_version_fn_t rwip_version;                                 /* [89] */
    r_rwip_schedule_fn_t rwip_schedule;                               /* [90] */
    r_rwip_sleep_fn_t rwip_sleep;                                     /* [91] */
    r_rwip_wakeup_fn_t rwip_wakeup;                                   /* [92] */
    r_rwip_prevent_sleep_set_fn_t rwip_prevent_sleep_set;             /* [93] */
    r_rwip_wakeup_end_fn_t rwip_wakeup_end;                           /* [94] */
    r_rwip_prevent_sleep_clear_fn_t rwip_prevent_sleep_clear;         /* [95] */
    r_rwip_wakeup_delay_set_fn_t rwip_wakeup_delay_set;               /* [96] */
    r_rwip_sleep_enable_fn_t rwip_sleep_enable;                       /* [97] */
    r_rwip_ext_wakeup_enable_fn_t rwip_ext_wakeup_enable;             /* [98] */
    r_rwip_pca_clock_dragging_only_fn_t rwip_pca_clock_dragging_only; /* [99] */
    r_rwip_wlcoex_set_fn_t rwip_wlcoex_set;                           /* [100] */
    r_rwip_assert_err_fn_t rwip_assert_err;                           /* [101] */
    r_srand_fn_t srand;                                               /* [102] */
    r_rand_fn_t rand;                                                 /* [103] */
};

/**
 * Pointer to the RivieraWaves modules-functions table (rom.ld: 0x3ffafd68 ->
 * 0x3ffafd6c). Resolved by the linker.
 */
extern struct r_modules_funcs *r_modules_funcs_p;

#endif /* R_MODULES_FUNCS_H */
