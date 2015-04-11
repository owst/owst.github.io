/*
 * Copyright 2010 Owen Stephens. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *        1. Redistributions of source code must retain the above copyright
 *           notice, this list of conditions and the following disclaimer.
 *
 *        2. Redistributions in binary form must reproduce the above copyright
 *           notice, this list of conditions and the following disclaimer in
 *           the documentation and/or other materials provided with the
 *           distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY Owen Stephens ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL Owen Stephens OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Owen Stephens.
 *
 *
 *          3rd Year Project - Greynet
 *
 *
 *          Provides basic forwarding and configuration of packets that are
 *          deemed to be invalid due to lack of ND/ARP response.
 *
 *          Authors:
 *          Owen Stephens <os1v07@ecs.soton.ac.uk>
 */
#include <linux/kernel.h>
#include <linux/sysctl.h>
#include <linux/inet.h>
#include <asm/uaccess.h>

#include <linux/time.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/skbuff.h>

/* uses format xxx.xxx.xxx.xxx:yyyyy */
#define SYSCTL_NAME "receiver_addr_port"
#define SYSCTL_MAX_SIZE 22

static struct ctl_table_header *sysctl;
static char sysctl_buf[SYSCTL_MAX_SIZE];
static unsigned int curr_addr = 0;
static unsigned short curr_port = 0;

static struct workqueue_struct *wq;

struct wq_wrapper {
        struct work_struct worker;
        struct sk_buff *skb;
};

static int neigh_fwd_wq_init(void)
{
        wq = create_singlethread_workqueue("fwd_wq");

        if (!wq) {
                return -ENOMEM;
        }

        return 0;
}

/* Check if a valid ip:port string has been entered */
int is_valid_addr(char *str)
{
        unsigned int o1,o2,o3,o4,port;

        /* If the input string doesn't contain 5 unsigned ints, fail */
        if (sscanf(str, "%u.%u.%u.%u:%u", &o1, &o2, &o3, &o4, &port) != 5) {
                return -1;
        }

        /* If all components are in range, success */
        if ((o1 < 256) && (o2 < 256) && (o3 < 256) && (o4 < 256)
                        && (port < 65536)) {
                return 0;
        }

        return -2;
}

/* Split a ip:port string into two (properly adjusted) ints */
void extract_addr_port(char *str)
{
        int diff;
        char *port, *pos;

        /* Find the separating ':' */
        pos = strchr(str, ':');

        diff = pos - str;

        /* Set the separator as the byte value 0 (string separator) */
        str[diff] = 0;

        port = (char *)str + diff + 1;

        /* Use string reading functions to read IP addr and port */
        curr_port = htons((short int)simple_strtoul(port, NULL, 10));
        curr_addr = in_aton(str);

        /* Reset the separator, so entire value is returned, when read*/
        str[diff] = ':';
}

/*
 * Handle reading/writing to the proc/sys file. Also
 * check validity and extract ip address on write.
 */
int handle_proc_edit(struct ctl_table *table, int write, struct file *filp,
                     void __user *buffer, size_t *lenp, loff_t *ppos)
{
        int ret = 0;

        /* Use the builtin string reading function */
        ret = proc_dostring(table, write, filp, buffer, lenp, ppos);

        /* If the user wrote a new value to the proc file... */
        if (write) {
                /* Check the new value is valid */
                if (is_valid_addr(sysctl_buf) < 0) {
                        printk(KERN_INFO "ERROR - Invalid IP:port for "
                                         "receiver address: %s\n", sysctl_buf);
                        memset(sysctl_buf, 0, SYSCTL_MAX_SIZE);
                        curr_addr = curr_port = 0;

                        ret = -1;
                } else {
                        extract_addr_port(sysctl_buf);
                }
        }

        return ret;
}

/* create the sysctl array, using an entry with ctl_name of 0 to terminate */
static struct ctl_table sys_table[] =
{
        {
                .procname         = SYSCTL_NAME,
                .data             = &sysctl_buf,
                .maxlen           = SYSCTL_MAX_SIZE,
                .mode             = 0644,
                .proc_handler = handle_proc_edit,
        },
        {0}
};

void unload_sysctl(void)
{
        unregister_sysctl_table(sysctl);
}

/*
 * Set up the /proc/sys fs entry.
 */
int neigh_fwd_procf_init(void)
{
        memset(sysctl_buf, 0, SYSCTL_MAX_SIZE);

        /* Attempt to register the sysctl entry */
        sysctl = register_sysctl_table(sys_table);

        if (sysctl == NULL) {
                unload_sysctl();
                printk(KERN_ALERT "Error: couldn't init /proc/sys/%s!\n",
                            SYSCTL_NAME);
                return -ENOMEM;
        }

        return 0;
}

/*
 * Called by the work-queue, to forward a cloned skb
 */
void neigh_fwd_packet(struct work_struct *data)
{
        struct wq_wrapper *wrapper =
            container_of(data, struct wq_wrapper, worker);
        struct sk_buff *skb = wrapper->skb;
        struct socket * sock;
        struct sockaddr_in addr_in;
        int ret_val;
        unsigned int length = skb->len;
        char *buf = skb->data;
        struct msghdr msg;
        struct iovec iov;
        int len = 0, written = 0, left = length;
        mm_segment_t oldmm;

        printk(KERN_INFO "neigh_fwd: Forwarding sk_buff %p\n", skb);

        if (curr_addr == 0 || curr_port == 0) {
                printk(KERN_INFO "neigh_fwd: Error, badly configured proc "
                                 "file: host: %u port: %hd\n",
                        curr_addr, curr_port);
                goto close;
        }

        /* Attempt to create a UDP socket */
        ret_val = sock_create(PF_INET, SOCK_DGRAM, IPPROTO_UDP, &sock);
        if (ret_val < 0) {
                printk(KERN_ERR "neigh_fwd: Error during creation of socket; "
                                                "error code: %d\n", ret_val);
                goto close;
        }

        /* Create struct to hold destination addr and port */
        memset(&addr_in, 0, sizeof(struct sockaddr_in));
        addr_in.sin_family=AF_INET;
        addr_in.sin_port = curr_port;
        addr_in.sin_addr.s_addr = curr_addr;

        /* Attempt to connect the socket */
        ret_val = sock->ops->connect(sock, (struct sockaddr *)&addr_in,
                                sizeof(struct sockaddr_in), 0);
        if(ret_val < 0) {
                printk(KERN_ERR "neigh_fwd: Error trying to connect socket. "
                                                "Code: %d\n", ret_val);
                goto close;
        }

        /* Set the header values */
        memset(&msg, 0, sizeof(struct msghdr));
        msg.msg_flags = 0;
        msg.msg_name = &addr_in;
        msg.msg_namelen = sizeof(struct sockaddr_in);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = NULL;
        msg.msg_controllen = 0;

/* Write data, until all has been sent */
repeat_send:
        msg.msg_iov->iov_len = left;
        msg.msg_iov->iov_base = (char *)buf + written;

        oldmm = get_fs();
        set_fs(KERNEL_DS);
        len = sock_sendmsg(sock, &msg, left);
        set_fs(oldmm);

        if (len == -ERESTARTSYS)
                goto repeat_send;
        if (len > 0) {
                written += len;
                left -= len;
                if (left)
                        goto repeat_send;
        }

        printk(KERN_INFO "neigh_fwd: written %d bytes\n", written);

close:
        sock_release(sock);
        kfree(wrapper);

        return;
}

/*
 * Clones the passed skb and adds a new work-queue entry, to process it.
 */
static void neigh_fwd_tick(struct sk_buff *skb)
{
        struct timespec time;
        int headroom = 0;
        void *data;
        unsigned long *t;
        struct wq_wrapper *wrapper;

        getnstimeofday(&time);

        /* Ensure there is space in the skb for the timestamp */
        headroom = skb_headroom(skb);
        if (headroom < 8) {
                skb_reserve(skb, 8 - headroom);
        }

        /* Allocates space in the skb for the timestamp */
        data = (void *)skb_push(skb, 8);
        memset(data, 0, sizeof(__kernel_time_t));

        /* Write the timestamp to the start of the skb */
        t = (unsigned long *)data;
        *t = htonl(time.tv_sec);

        /* Recalculate the packet's checksum */
        skb->csum = csum_partial((char *)data, sizeof(long), skb->csum);

        /* Clone the skb and add a new work-queue entry to process it */
        wrapper = (struct wq_wrapper *)kmalloc(sizeof(struct wq_wrapper),
                    GFP_ATOMIC);
        wrapper->skb = skb_clone(skb, GFP_ATOMIC);
        INIT_WORK(&wrapper->worker, neigh_fwd_packet);
        queue_work(wq, &wrapper->worker);
}
