/*
 * Copyright (c) 2019 Huawei Technologies Co., Ltd.
 * MeshAccelerating is licensed under the Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *     http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
 * PURPOSE.
 * See the Mulan PSL v2 for more details.
 * Author: LemmyHuang
 * Create: 2021-09-17
 */

#ifndef _LISTENER_H_
#define _LISTENER_H_

#include "listener.pb-c.h"
#include "filter.h"
#include "cluster.h"
#include "tail_call.h"

bpf_map_t SEC("maps") map_of_listener = {
	.type			= BPF_MAP_TYPE_HASH,
	.key_size		= sizeof(address_t),
	.value_size		= sizeof(listener_t),
	.max_entries	= MAP_SIZE_OF_LISTENER,
	.map_flags		= 0,
};

static inline
listener_t *map_lookup_listener(const address_t *address)
{
	return kmesh_map_lookup_elem(&map_of_listener, address);
}

static inline
int l4_listener_manager(ctx_buff_t *ctx, listener_t *listener)
{
	map_key_t map_key;
	ctx_key_t ctx_key;
	DECLARE_VAR_ADDRESS(ctx, address);

	if (listener->state & LISTENER_STATE_PASSIVE)
		return -EBUSY;
	map_key = listener->map_key;

	ctx_key.address = address;
	ctx_key.tail_call_index = KMESH_TAIL_CALL_CLUSTER;

	if (kmesh_tail_update_ctx(&ctx_key, &map_key) != 0)
		return -ENOSPC;
	kmesh_tail_call(ctx, KMESH_TAIL_CALL_CLUSTER);
	kmesh_tail_delete_ctx(&ctx_key);

	return 0;
}

static inline
int l7_listener_manager(ctx_buff_t *ctx, listener_t *listener)
{
	unsigned i;
	map_key_t map_key;
	ctx_key_t ctx_key;
	filter_chain_t *filter_chain = NULL;

	DECLARE_VAR_ADDRESS(ctx, address);

	if (listener->state & LISTENER_STATE_PASSIVE)
		return -EBUSY;
	map_key = listener->map_key;

	for (i = 0; i < MAP_SIZE_OF_PER_FILTER_CHAIN; i++) {
		map_key.index = i;

		filter_chain = map_lookup_filter_chain(&map_key);
		if (filter_chain == NULL)
			return -ENOENT;

		if (filter_chain_match_check(ctx, &filter_chain->filter_chain_match) == 0)
			break;
	}

	if (i == MAP_SIZE_OF_PER_FILTER_CHAIN)
		return -ENOENT;

	ctx_key.address = address;
	ctx_key.tail_call_index = KMESH_TAIL_CALL_FILTER_CHAIN;

	if (kmesh_tail_update_ctx(&ctx_key, &map_key) != 0)
		return -ENOSPC;
	kmesh_tail_call(ctx, KMESH_TAIL_CALL_FILTER_CHAIN);
	kmesh_tail_delete_ctx(&ctx_key);

	return 0;
}

#endif // _LISTENER_H_
