#ifndef __KMESH_BPF_PROBE_H__
#define __KMESH_BPF_PROBE_H__

#include "access_log.h"
#include "metrics.h"

static inline void observe_on_pre_connect(struct bpf_sock *sk)
{
    struct sock_storage_data *storage = NULL;
    if (!sk)
        return;

    storage = bpf_sk_storage_get(&map_of_sock_storage, sk, 0, BPF_LOCAL_STORAGE_GET_F_CREATE);
    if (!storage) {
        BPF_LOG(ERR, PROBE, "pre_connect bpf_sk_storage_get failed\n");
        return;
    }

    storage->connect_ns = bpf_ktime_get_ns();
    return;
}

static inline void observe_on_connect_established(struct bpf_sock *sk, __u8 direction)
{
    struct bpf_tcp_sock *tcp_sock = NULL;
    struct sock_storage_data *storage = NULL;
    __u64 flags = (direction == OUTBOUND) ? 0 : BPF_LOCAL_STORAGE_GET_F_CREATE;

    if (!sk)
        return;
    tcp_sock = bpf_tcp_sock(sk);
    if (!tcp_sock)
        return;

    storage = bpf_sk_storage_get(&map_of_sock_storage, sk, 0, flags);
    if (!storage) {
        BPF_LOG(ERR, PROBE, "connect bpf_sk_storage_get failed\n");
        return;
    }

    // INBOUND senario
    if (direction == INBOUND)
        storage->connect_ns = bpf_ktime_get_ns();
    storage->direction = direction;
    storage->connect_success = true;

    metric_on_connect(sk, tcp_sock, storage);
}

static inline void observe_on_close(struct bpf_sock *sk)
{
    struct bpf_tcp_sock *tcp_sock = NULL;
    struct sock_storage_data *storage = NULL;
    if (!sk)
        return;
    tcp_sock = bpf_tcp_sock(sk);
    if (!tcp_sock)
        return;

    storage = bpf_sk_storage_get(&map_of_sock_storage, sk, 0, 0);
    if (!storage) {
        BPF_LOG(ERR, PROBE, "close bpf_sk_storage_get failed\n");
        return;
    }

    report_access_log(sk, tcp_sock, storage);
    metric_on_close(sk, tcp_sock, storage);
}
#endif