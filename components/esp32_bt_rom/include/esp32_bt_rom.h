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
 * Signature of r_E1, slot 13: the BR/EDR E1 authentication function (SAFER+),
 * producing SRES and ACO from the link key, RAND and BD_ADDR. Six pointer args.
 */
typedef void (*r_E1_fn_t)(uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *);

/**
 * Signature of r_E21, slot 14: BR/EDR E21, derives a unit/combination key from a
 * RAND and a BD_ADDR. Six pointer args (inferred).
 */
typedef void (*r_E21_fn_t)(uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *);

/**
 * Signature of r_E22, slot 15: BR/EDR E22, derives an initialisation/PIN key
 * from a RAND, the PIN and a BD_ADDR. Six pointer args (inferred).
 */
typedef void (*r_E22_fn_t)(uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t *);

/**
 * Signature of r_E3, slot 16: BR/EDR E3, derives the ciphering key Kc from the
 * link key, a RAND and the ACO. Four pointer args (inferred).
 */
typedef void (*r_E3_fn_t)(uint8_t *, uint8_t *, uint8_t *, uint8_t *);

/**
 * Signature of r_KPrimC, slot 17: derives the constrained key Kc' from Kc for
 * reduced-key-length encryption. Four pointer args plus a uint8_t (inferred).
 */
typedef void (*r_KPrimC_fn_t)(uint8_t *, uint8_t *, uint8_t *, uint8_t *, uint8_t);

/**
 * Signature of r_XorKey, slot 18: XORs two key/byte buffers into an output
 * buffer. Four pointer args (inferred).
 */
typedef void (*r_XorKey_fn_t)(uint8_t *, uint8_t *, uint8_t *, uint8_t *);

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
    r_bt_util_buf_init_fn_t bt_util_buf_init;                   /* [0] */
    r_bt_util_buf_lmp_tx_alloc_fn_t bt_util_buf_lmp_tx_alloc;   /* [1] */
    r_bt_util_buf_lmp_tx_free_fn_t bt_util_buf_lmp_tx_free;     /* [2] */
    r_bt_util_buf_acl_rx_alloc_fn_t bt_util_buf_acl_rx_alloc;   /* [3] */
    r_bt_util_buf_acl_rx_free_fn_t bt_util_buf_acl_rx_free;     /* [4] */
    r_bt_util_buf_acl_tx_alloc_fn_t bt_util_buf_acl_tx_alloc;   /* [5] */
    r_bt_util_buf_acl_tx_free_fn_t bt_util_buf_acl_tx_free;     /* [6] */
    r_bt_util_buf_sync_init_fn_t bt_util_buf_sync_init;         /* [7] */
    r_bt_util_buf_sync_clear_fn_t bt_util_buf_sync_clear;       /* [8] */
    r_bt_util_buf_sync_tx_alloc_fn_t bt_util_buf_sync_tx_alloc; /* [9] */
    r_bt_util_buf_sync_tx_free_fn_t bt_util_buf_sync_tx_free;   /* [10] */
    r_bt_util_buf_sync_rx_alloc_fn_t bt_util_buf_sync_rx_alloc; /* [11] */
    r_bt_util_buf_sync_rx_free_fn_t bt_util_buf_sync_rx_free;   /* [12] */
    r_E1_fn_t E1;                                               /* [13] */
    r_E21_fn_t E21;                                             /* [14] */
    r_E22_fn_t E22;                                             /* [15] */
    r_E3_fn_t E3;                                               /* [16] */
    r_KPrimC_fn_t KPrimC;                                       /* [17] */
    r_XorKey_fn_t XorKey;                                       /* [18] */
    void *fn[];                                                 /* [19..] not yet typed */
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
