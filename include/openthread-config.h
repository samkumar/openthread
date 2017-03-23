/*
 * Copyright (C) 2016 José Ignacio Alamos
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    pkg_openthread_cli   OpenThread
 * @ingroup     pkg_openthread
 * @brief       An open source implementation of Thread stack
 * @see         https://github.com/openthread/openthread
 *
 * Thread if a mesh oriented network stack running for IEEE802.15.4 networks.
 * It can be used with the @ref conn.
 * @{
 *
 * @file
 *
 * @author  José Ignacio Alamos <jialamos@uc.cl>
 */

#ifndef OPENTHREAD_CONFIG_H
#define OPENTHREAD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define OPENTHREAD_EXAMPLES RIOT

/* Define to 1 if you want to use DTLS (commisioning) */
#define OPENTHREAD_ENABLE_DTLS 1

/* Define to 1 if you want to use MAC whiltelist */
#define OPENTHREAD_ENABLE_MAC_WHITELIST 1

#ifdef __cplusplus
}
#endif

#endif
/** @} */
