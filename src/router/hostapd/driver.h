#ifndef DRIVER_H
#define DRIVER_H

struct driver_ops {
	const char *name;		/* as appears in the config file */

	int (*init)(struct hostapd_data *hapd);
	void (*deinit)(void *priv);

	int (*wireless_event_init)(void *priv);
	void (*wireless_event_deinit)(void *priv);

	/**
	 * set_8021x - enable/disable IEEE 802.1X support
	 * @ifname: Interface name (for multi-SSID/VLAN support)
	 * @priv: driver private data
	 * @enabled: 1 = enable, 0 = disable
	 *
	 * Returns: 0 on success, -1 on failure
	 *
	 * Configure the kernel driver to enable/disable 802.1X support.
	 * This may be an empty function if 802.1X support is always enabled.
	 */
	int (*set_ieee8021x)(const char *ifname, void *priv, int enabled);

	/**
	 * set_privacy - enable/disable privacy
	 * @priv: driver private data
	 * @enabled: 1 = privacy enabled, 0 = disabled
	 *
	 * Return: 0 on success, -1 on failure
	 *
	 * Configure privacy.
	 */
	int (*set_privacy)(void *priv, int enabled);

	int (*set_encryption)(const char *ifname, void *priv, const char *alg,
			      const u8 *addr, int idx,
			      const u8 *key, size_t key_len, int txkey);
	int (*get_seqnum)(const char *ifname, void *priv, const u8 *addr,
			  int idx, u8 *seq);
	int (*flush)(void *priv);
	int (*set_generic_elem)(void *priv, const u8 *elem, size_t elem_len);

	int (*read_sta_data)(void *priv, struct hostap_sta_driver_data *data,
			     const u8 *addr);
	int (*send_eapol)(void *priv, const u8 *addr, const u8 *data,
			  size_t data_len, int encrypt);
	int (*sta_deauth)(void *priv, const u8 *addr, int reason);
	int (*sta_disassoc)(void *priv, const u8 *addr, int reason);
	int (*sta_remove)(void *priv, const u8 *addr);
	int (*get_ssid)(void *priv, u8 *buf, int len);
	int (*set_ssid)(void *priv, const u8 *buf, int len);
	int (*set_countermeasures)(void *priv, int enabled);
	int (*send_mgmt_frame)(void *priv, const void *msg, size_t len,
			       int flags);
	int (*set_assoc_ap)(void *priv, const u8 *addr);
	int (*sta_add)(const char *ifname, void *priv, const u8 *addr, u16 aid,
		       u16 capability, u8 *supp_rates, size_t supp_rates_len,
		       int flags);
	int (*get_inact_sec)(void *priv, const u8 *addr);
	int (*sta_clear_stats)(void *priv, const u8 *addr);

	int (*set_freq)(void *priv, int mode, int freq);
	int (*set_rts)(void *priv, int rts);
	int (*get_rts)(void *priv, int *rts);
	int (*set_frag)(void *priv, int frag);
	int (*get_frag)(void *priv, int *frag);
	int (*set_retry)(void *priv, int short_retry, int long_retry);
	int (*get_retry)(void *priv, int *short_retry, int *long_retry);

	int (*sta_set_flags)(void *priv, const u8 *addr,
			     int flags_or, int flags_and);
	int (*set_rate_sets)(void *priv, int *supp_rates, int *basic_rates,
			     int mode);
	int (*set_channel_flag)(void *priv, int mode, int chan, int flag,
				unsigned char power_level,
				unsigned char antenna_max);
	int (*set_regulatory_domain)(void *priv, unsigned int rd);
	int (*set_beacon)(const char *ifname, void *priv,
			  u8 *head, size_t head_len,
			  u8 *tail, size_t tail_len);
	int (*set_beacon_int)(void *priv, int value);
	int (*set_dtim_period)(void *priv, int value);
	/* Configure broadcast SSID mode:
	 * 0 = include SSID in Beacon frames and reply to Probe Request frames
	 *     that use broadcast SSID
	 * 1 = hide SSID from Beacon frames and ignore Probe Request frames for
	 *     broadcast SSID
	 */
	int (*set_broadcast_ssid)(void *priv, int value);
	int (*set_cts_protect)(void *priv, int value);
	int (*set_key_tx_rx_threshold)(void *priv, int value);
	int (*set_preamble)(void *priv, int value);
	int (*set_short_slot_time)(void *priv, int value);
	int (*set_tx_queue_params)(void *priv, int queue, int aifs, int cw_min,
				   int cw_max, int burst_time);
	int (*passive_scan)(void *priv, int now, int our_mode_only,
			    int interval, int listen, int *channel,
			    int *last_rx);
	struct hostapd_hw_modes * (*get_hw_feature_data)(void *priv,
							 u16 *num_modes,
							 u16 *flags);
	int (*set_sta_vlan)(void *priv, const u8 *addr, const char *ifname,
			    int vlan_id);
};

static inline int
hostapd_driver_init(struct hostapd_data *hapd)
{
	if (hapd->driver == NULL || hapd->driver->init == NULL)
		return -1;
	return hapd->driver->init(hapd);
}

static inline void
hostapd_driver_deinit(struct hostapd_data *hapd)
{
	if (hapd->driver == NULL || hapd->driver->deinit == NULL)
		return;
	hapd->driver->deinit(hapd->driver);
}

static inline int
hostapd_wireless_event_init(struct hostapd_data *hapd)
{
	if (hapd->driver == NULL ||
	    hapd->driver->wireless_event_init == NULL)
		return 0;
	return hapd->driver->wireless_event_init(hapd->driver);
}

static inline void
hostapd_wireless_event_deinit(struct hostapd_data *hapd)
{
	if (hapd->driver == NULL ||
	    hapd->driver->wireless_event_deinit == NULL)
		return;
	hapd->driver->wireless_event_deinit(hapd->driver);
}

static inline int
hostapd_set_ieee8021x(const char *ifname, struct hostapd_data *hapd,
		      int enabled)
{
	if (hapd->driver == NULL || hapd->driver->set_ieee8021x == NULL)
		return 0;
	return hapd->driver->set_ieee8021x(ifname, hapd->driver, enabled);
}

static inline int
hostapd_set_privacy(struct hostapd_data *hapd, int enabled)
{
	if (hapd->driver == NULL || hapd->driver->set_privacy == NULL)
		return 0;
	return hapd->driver->set_privacy(hapd->driver, enabled);
}

static inline int
hostapd_set_encryption(const char *ifname, struct hostapd_data *hapd,
		       const char *alg, const u8 *addr, int idx,
		       u8 *key, size_t key_len, int txkey)
{
	if (hapd->driver == NULL || hapd->driver->set_encryption == NULL)
		return 0;
	return hapd->driver->set_encryption(ifname, hapd->driver, alg, addr,
					    idx, key, key_len, txkey);
}

static inline int
hostapd_get_seqnum(const char *ifname, struct hostapd_data *hapd,
		   const u8 *addr, int idx, u8 *seq)
{
	if (hapd->driver == NULL || hapd->driver->get_seqnum == NULL)
		return 0;
	return hapd->driver->get_seqnum(ifname, hapd->driver, addr, idx, seq);
}

static inline int
hostapd_flush(struct hostapd_data *hapd)
{
	if (hapd->driver == NULL || hapd->driver->flush == NULL)
		return 0;
	return hapd->driver->flush(hapd->driver);
}

static inline int
hostapd_set_generic_elem(struct hostapd_data *hapd, const u8 *elem,
			 size_t elem_len)
{
	if (hapd->driver == NULL || hapd->driver->set_generic_elem == NULL)
		return 0;
	return hapd->driver->set_generic_elem(hapd->driver, elem, elem_len);
}

static inline int
hostapd_read_sta_data(struct hostapd_data *hapd,
		      struct hostap_sta_driver_data *data, const u8 *addr)
{
	if (hapd->driver == NULL || hapd->driver->read_sta_data == NULL)
		return -1;
	return hapd->driver->read_sta_data(hapd->driver, data, addr);
}

static inline int
hostapd_send_eapol(struct hostapd_data *hapd, const u8 *addr, const u8 *data,
		   size_t data_len, int encrypt)
{
	if (hapd->driver == NULL || hapd->driver->send_eapol == NULL)
		return 0;
	return hapd->driver->send_eapol(hapd->driver, addr, data, data_len,
					encrypt);
}

static inline int
hostapd_sta_deauth(struct hostapd_data *hapd, const u8 *addr, int reason)
{
	if (hapd->driver == NULL || hapd->driver->sta_deauth == NULL)
		return 0;
	return hapd->driver->sta_deauth(hapd->driver, addr, reason);
}

static inline int
hostapd_sta_disassoc(struct hostapd_data *hapd, const u8 *addr, int reason)
{
	if (hapd->driver == NULL || hapd->driver->sta_disassoc == NULL)
		return 0;
	return hapd->driver->sta_disassoc(hapd->driver, addr, reason);
}

static inline int
hostapd_sta_remove(struct hostapd_data *hapd, const u8 *addr)
{
	if (hapd->driver == NULL || hapd->driver->sta_remove == NULL)
		return 0;
	return hapd->driver->sta_remove(hapd->driver, addr);
}

static inline int
hostapd_get_ssid(struct hostapd_data *hapd, u8 *buf, size_t len)
{
	if (hapd->driver == NULL || hapd->driver->get_ssid == NULL)
		return 0;
	return hapd->driver->get_ssid(hapd->driver, buf, len);
}

static inline int
hostapd_set_ssid(struct hostapd_data *hapd, const u8 *buf, size_t len)
{
	if (hapd->driver == NULL || hapd->driver->set_ssid == NULL)
		return 0;
	return hapd->driver->set_ssid(hapd->driver, buf, len);
}

static inline int
hostapd_send_mgmt_frame(struct hostapd_data *hapd, const void *msg, size_t len,
			int flags)
{
	if (hapd->driver == NULL || hapd->driver->send_mgmt_frame == NULL)
		return 0;
	return hapd->driver->send_mgmt_frame(hapd->driver, msg, len, flags);
}

static inline int
hostapd_set_assoc_ap(struct hostapd_data *hapd, const u8 *addr)
{
	if (hapd->driver == NULL || hapd->driver->set_assoc_ap == NULL)
		return 0;
	return hapd->driver->set_assoc_ap(hapd->driver, addr);
}

static inline int 
hostapd_set_countermeasures(struct hostapd_data *hapd, int enabled)
{
	if (hapd->driver == NULL || hapd->driver->set_countermeasures == NULL)
		return 0;
	return hapd->driver->set_countermeasures(hapd->driver, enabled);
}

static inline int
hostapd_sta_add(const char *ifname, struct hostapd_data *hapd, const u8 *addr,
		u16 aid, u16 capability, u8 *supp_rates, size_t supp_rates_len,
		int flags)
{
	if (hapd->driver == NULL || hapd->driver->sta_add == NULL)
		return 0;
	return hapd->driver->sta_add(ifname, hapd->driver, addr, aid,
				     capability, supp_rates, supp_rates_len,
				     flags);
}

static inline int
hostapd_get_inact_sec(struct hostapd_data *hapd, const u8 *addr)
{
	if (hapd->driver == NULL || hapd->driver->get_inact_sec == NULL)
		return 0;
	return hapd->driver->get_inact_sec(hapd->driver, addr);
}

static inline int
hostapd_set_freq(struct hostapd_data *hapd, int mode, int freq)
{
	if (hapd->driver == NULL || hapd->driver->set_freq == NULL)
		return 0;
	return hapd->driver->set_freq(hapd->driver, mode, freq);
}

static inline int
hostapd_set_rts(struct hostapd_data *hapd, int rts)
{
	if (hapd->driver == NULL || hapd->driver->set_rts == NULL)
		return 0;
	return hapd->driver->set_rts(hapd->driver, rts);
}

static inline int
hostapd_get_rts(struct hostapd_data *hapd, int *rts)
{
	if (hapd->driver == NULL || hapd->driver->get_rts == NULL)
		return 0;
	return hapd->driver->get_rts(hapd->driver, rts);
}

static inline int
hostapd_set_frag(struct hostapd_data *hapd, int frag)
{
	if (hapd->driver == NULL || hapd->driver->set_frag == NULL)
		return 0;
	return hapd->driver->set_frag(hapd->driver, frag);
}

static inline int
hostapd_get_frag(struct hostapd_data *hapd, int *frag)
{
	if (hapd->driver == NULL || hapd->driver->get_frag == NULL)
		return 0;
	return hapd->driver->get_frag(hapd->driver, frag);
}

static inline int
hostapd_set_retry(struct hostapd_data *hapd, int short_retry, int long_retry)
{
	if (hapd->driver == NULL || hapd->driver->set_retry == NULL)
		return 0;
	return hapd->driver->set_retry(hapd->driver, short_retry, long_retry);
}

static inline int
hostapd_get_retry(struct hostapd_data *hapd, int *short_retry, int *long_retry)
{
	if (hapd->driver == NULL || hapd->driver->get_retry == NULL)
		return 0;
	return hapd->driver->get_retry(hapd->driver, short_retry, long_retry);
}

static inline int
hostapd_sta_set_flags(struct hostapd_data *hapd, u8 *addr,
		      int flags_or, int flags_and)
{
	if (hapd->driver == NULL || hapd->driver->sta_set_flags == NULL)
		return 0;
	return hapd->driver->sta_set_flags(hapd->driver, addr, flags_or,
					   flags_and);
}

static inline int
hostapd_set_rate_sets(struct hostapd_data *hapd, int *supp_rates,
		      int *basic_rates, int mode)
{
	if (hapd->driver == NULL || hapd->driver->set_rate_sets == NULL)
		return 0;
	return hapd->driver->set_rate_sets(hapd->driver, supp_rates,
					   basic_rates, mode);
}

static inline int
hostapd_set_channel_flag(struct hostapd_data *hapd, int mode, int chan,
			 int flag, unsigned char power_level,
			 unsigned char antenna_max)
{
	if (hapd->driver == NULL || hapd->driver->set_channel_flag == NULL)
		return 0;
	return hapd->driver->set_channel_flag(hapd->driver, mode, chan, flag,
					      power_level, antenna_max);
}

static inline int
hostapd_set_regulatory_domain(struct hostapd_data *hapd, unsigned int rd)
{
	if (hapd->driver == NULL ||
	    hapd->driver->set_regulatory_domain == NULL)
		return 0;
	return hapd->driver->set_regulatory_domain(hapd->driver, rd);
}


void driver_register(const char *name, const struct driver_ops *ops);
void driver_unregister(const char *name);
const struct driver_ops *driver_lookup(const char *name);

static inline int
hostapd_sta_clear_stats(struct hostapd_data *hapd, const u8 *addr)
{
	if (hapd->driver == NULL || hapd->driver->sta_clear_stats == NULL)
		return 0;
	return hapd->driver->sta_clear_stats(hapd->driver, addr);
}

static inline int
hostapd_set_beacon(const char *ifname, struct hostapd_data *hapd,
		   u8 *head, size_t head_len,
		   u8 *tail, size_t tail_len)
{
	if (hapd->driver == NULL || hapd->driver->set_beacon == NULL)
		return 0;
	return hapd->driver->set_beacon(ifname, hapd->driver, head, head_len,
					tail, tail_len);
}

static inline int
hostapd_set_beacon_int(struct hostapd_data *hapd, int value)
{
	if (hapd->driver == NULL || hapd->driver->set_beacon_int == NULL)
		return 0;
	return hapd->driver->set_beacon_int(hapd->driver, value);
}

static inline int
hostapd_set_dtim_period(struct hostapd_data *hapd, int value)
{
	if (hapd->driver == NULL || hapd->driver->set_dtim_period == NULL)
		return 0;
	return hapd->driver->set_dtim_period(hapd->driver, value);
}

static inline int
hostapd_set_broadcast_ssid(struct hostapd_data *hapd, int value)
{
	if (hapd->driver == NULL || hapd->driver->set_broadcast_ssid == NULL)
		return 0;
	return hapd->driver->set_broadcast_ssid(hapd->driver, value);
}

static inline int
hostapd_set_cts_protect(struct hostapd_data *hapd, int value)
{
	if (hapd->driver == NULL || hapd->driver->set_cts_protect == NULL)
		return 0;
	return hapd->driver->set_cts_protect(hapd->driver, value);
}

static inline int
hostapd_set_key_tx_rx_threshold(struct hostapd_data *hapd, int value)
{
	if (hapd->driver == NULL ||
	    hapd->driver->set_key_tx_rx_threshold == NULL)
		return 0;
	return hapd->driver->set_key_tx_rx_threshold(hapd->driver, value);
}

static inline int
hostapd_set_preamble(struct hostapd_data *hapd, int value)
{
	if (hapd->driver == NULL || hapd->driver->set_preamble == NULL)
		return 0;
	return hapd->driver->set_preamble(hapd->driver, value);
}

static inline int
hostapd_set_short_slot_time(struct hostapd_data *hapd, int value)
{
	if (hapd->driver == NULL || hapd->driver->set_short_slot_time == NULL)
		return 0;
	return hapd->driver->set_short_slot_time(hapd->driver, value);
}

static inline int
hostapd_set_tx_queue_params(struct hostapd_data *hapd, int queue, int aifs,
			    int cw_min, int cw_max, int burst_time)
{
	if (hapd->driver == NULL || hapd->driver->set_tx_queue_params == NULL)
		return 0;
	return hapd->driver->set_tx_queue_params(hapd->driver, queue, aifs,
						 cw_min, cw_max, burst_time);
}

static inline int
hostapd_passive_scan(struct hostapd_data *hapd, int now, int our_mode_only,
		     int interval, int listen, int *channel,
		     int *last_rx)
{
	if (hapd->driver == NULL || hapd->driver->passive_scan == NULL)
		return -1;
	return hapd->driver->passive_scan(hapd->driver, now, our_mode_only,
					  interval, listen, channel, last_rx);
}

static inline struct hostapd_hw_modes *
hostapd_get_hw_feature_data(struct hostapd_data *hapd, u16 *num_modes,
			    u16 *flags)
{
	if (hapd->driver == NULL || hapd->driver->get_hw_feature_data == NULL)
		return NULL;
	return hapd->driver->get_hw_feature_data(hapd->driver, num_modes,
						 flags);
}

static inline int
hostapd_set_sta_vlan(const char *ifname, struct hostapd_data *hapd,
		     const u8 *addr, int vlan_id)
{
	if (hapd->driver == NULL || hapd->driver->set_sta_vlan == NULL)
		return 0;
	return hapd->driver->set_sta_vlan(hapd->driver, addr, ifname, vlan_id);
}

#endif /* DRIVER_H */
