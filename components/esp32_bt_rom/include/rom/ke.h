/*
 * SPDX-FileCopyrightText: 2026 Antonio Vázquez Blanco <antoniovazquezblanco@gmail.com>
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Kernel (ke) message and task type definitions.
 */
#ifndef ROM_KE_H
#define ROM_KE_H

#include <stdint.h>

typedef uint16_t kernel_msg_id_t;
typedef uint16_t kernel_task_id_t;

typedef int (*kernel_msg_func_t)(kernel_msg_id_t const msgid, void const *param,
                                 kernel_task_id_t const dest_id, kernel_task_id_t const src_id);

#endif /* ROM_KE_H */
